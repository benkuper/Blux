#include "BluxMpvPlayer.h"
#include "../../../../External/mpv/include/mpv/render.h"

#include <cmath>
#include <memory>

namespace
{
struct BluxMpvApi
{
	DynamicLibrary library;
	decltype(&mpv_create) create = nullptr;
	decltype(&mpv_initialize) initialize = nullptr;
	decltype(&mpv_terminate_destroy) destroy = nullptr;
	decltype(&mpv_set_option_string) setOption = nullptr;
	decltype(&mpv_command_async) command = nullptr;
	decltype(&mpv_set_property_async) setProperty = nullptr;
	decltype(&mpv_get_property) getProperty = nullptr;
	decltype(&mpv_get_property_string) getString = nullptr;
	decltype(&mpv_observe_property) observe = nullptr;
	decltype(&mpv_wait_event) nextEvent = nullptr;
	decltype(&mpv_free) freeString = nullptr;
	decltype(&mpv_render_context_create) createRenderer = nullptr;
	decltype(&mpv_render_context_update) updateRenderer = nullptr;
	decltype(&mpv_render_context_render) render = nullptr;
	decltype(&mpv_render_context_free) freeRenderer = nullptr;

	static BluxMpvApi& get()
	{
		static BluxMpvApi api;
		return api;
	}

	bool open()
	{
		if (library.getNativeHandle() != nullptr) return true;

		const auto exe = File::getSpecialLocation(File::currentExecutableFile);
		const auto overrideDirectory = SystemStats::getEnvironmentVariable("BLUX_MPV_DIR", {}).trim();
		StringArray candidates;
		if (overrideDirectory.isNotEmpty() && File::isAbsolutePath(overrideDirectory))
			candidates.addArray(findLibraries(File(overrideDirectory)));

		candidates.addArray(findLibraries(exe.getParentDirectory()));
		candidates.addArray(findLibraries(exe.getParentDirectory().getChildFile("lib")));
	   #if JUCE_MAC
		const auto app = File::getSpecialLocation(File::currentApplicationFile);
		candidates.addArray(findLibraries(app.getChildFile("Contents/Frameworks")));
		candidates.addArray(findLibraries(File("/opt/homebrew/lib")));
		candidates.addArray(findLibraries(File("/usr/local/lib")));
	   #endif
	   #if !JUCE_WINDOWS
		candidates.addArray(getLibraryNames());
	   #endif

		for (const auto& candidate : candidates)
		{
			if (!library.open(candidate)) continue;
			if (bindFunctions()) return true;
			library.close();
		}
		return false;
	}

private:
	static StringArray getLibraryNames()
	{
	   #if JUCE_WINDOWS
		return { "libmpv-2.dll", "mpv-2.dll" };
	   #elif JUCE_MAC
		return { "libmpv.2.dylib", "libmpv.dylib" };
	   #else
		return { "libmpv.so.2", "libmpv.so" };
	   #endif
	}

	static StringArray findLibraries(const File& directory)
	{
		StringArray paths;
		for (const auto& name : getLibraryNames())
		{
			const auto file = directory.getChildFile(name);
			if (file.existsAsFile()) paths.add(file.getFullPathName());
		}
		return paths;
	}

	template<typename Fn>
	bool bind(Fn& function, const char* name)
	{
		function = reinterpret_cast<Fn>(library.getFunction(name));
		return function != nullptr;
	}

	bool bindFunctions()
	{
		return bind(create, "mpv_create")
			&& bind(initialize, "mpv_initialize")
			&& bind(destroy, "mpv_terminate_destroy")
			&& bind(setOption, "mpv_set_option_string")
			&& bind(command, "mpv_command_async")
			&& bind(setProperty, "mpv_set_property_async")
			&& bind(getProperty, "mpv_get_property")
			&& bind(getString, "mpv_get_property_string")
			&& bind(observe, "mpv_observe_property")
			&& bind(nextEvent, "mpv_wait_event")
			&& bind(freeString, "mpv_free")
			&& bind(createRenderer, "mpv_render_context_create")
			&& bind(updateRenderer, "mpv_render_context_update")
			&& bind(render, "mpv_render_context_render")
			&& bind(freeRenderer, "mpv_render_context_free");
	}
};
}

BluxMpvPlayer::~BluxMpvPlayer()
{
	stopTimer();
	releaseEngine();
}

bool BluxMpvPlayer::openEngine()
{
	if (handle != nullptr) return true;

	auto& api = BluxMpvApi::get();
	if (!api.open()) return false;

	handle = api.create();
	if (handle == nullptr) return false;

	api.setOption(handle, "config", "no");
	api.setOption(handle, "terminal", "no");
	api.setOption(handle, "load-scripts", "no");
	api.setOption(handle, "ytdl", "no");
	api.setOption(handle, "vo", "libmpv");
	api.setOption(handle, "pause", "yes");
	api.setOption(handle, "keep-open", "yes");
	api.setOption(handle, "image-display-duration", "inf");
	api.setOption(handle, "osd-level", "0");
	api.setOption(handle, "volume-max", "1000");

	char software[] = MPV_RENDER_API_TYPE_SW;
	mpv_render_param setup[] = { { MPV_RENDER_PARAM_API_TYPE, software }, {} };
	if (api.initialize(handle) < 0 || api.createRenderer(&renderer, handle, setup) < 0)
	{
		releaseEngine();
		return false;
	}

	api.observe(handle, 0, "time-pos", MPV_FORMAT_DOUBLE);
	api.observe(handle, 0, "eof-reached", MPV_FORMAT_FLAG);
	return true;
}

void BluxMpvPlayer::releaseEngine()
{
	auto& api = BluxMpvApi::get();
	if (renderer != nullptr) api.freeRenderer(std::exchange(renderer, nullptr));
	if (handle != nullptr) api.destroy(std::exchange(handle, nullptr));
}

bool BluxMpvPlayer::load(const String& filePath)
{
	loadErrorPath = filePath;
	loadError.clear();

	const File file = File::isAbsolutePath(filePath) ? File(filePath) : File();
	if (!file.existsAsFile())
	{
		loadError = "The video file could not be found.";
		return false;
	}
	if (!openEngine())
	{
		loadError = "Video playback is unavailable because libmpv could not start.";
		Logger::writeToLog("Video playback requires libmpv with software rendering support. Set BLUX_MPV_DIR or place libmpv beside Blux.");
		return false;
	}

	path = filePath;
	if (loaded && sourceFile == file)
	{
		shouldNotifyLoaded = true;
		startTimerHz(60);
		return true;
	}

	sourceFile = file;
	loaded = false;
	shouldNotifyLoaded = false;
	framePending = true;
	frame = {};
	position = duration = 0;
	videoWidth = videoHeight = 0;
	lastSeek = -1;
	setPaused(true);

	const auto fullPath = file.getFullPathName();
	const char* args[] = { "loadfile", fullPath.toRawUTF8(), "replace", nullptr };
	if (BluxMpvApi::get().command(handle, ++loadRequest, args) < 0)
	{
		loadError = "The video file could not be opened.";
		return false;
	}

	startTimerHz(60);
	return true;
}

void BluxMpvPlayer::unload()
{
	if (path.isEmpty()) return;

	stopTimer();
	loaded = playing = shouldNotifyLoaded = framePending = false;
	path.clear();
	sourceFile = File();
	frame = {};
	position = duration = 0;
	lastSeek = -1;
	videoWidth = videoHeight = 0;
	const char* args[] = { "stop", nullptr };
	BluxMpvApi::get().command(handle, 0, args);
}

void BluxMpvPlayer::play()
{
	if (loaded) setPaused(false);
}

void BluxMpvPlayer::pause()
{
	if (loaded) setPaused(true);
}

void BluxMpvPlayer::stop()
{
	pause();
	lastSeek = -1;
	setPosition(0);
}

void BluxMpvPlayer::setPaused(bool paused)
{
	playing = !paused;
	int flag = paused ? 1 : 0;
	BluxMpvApi::get().setProperty(handle, 0, "pause", MPV_FORMAT_FLAG, &flag);
}

void BluxMpvPlayer::setPosition(double seconds)
{
	if (!loaded) return;
	seconds = jmax(0.0, seconds);
	if (std::abs(seconds - lastSeek) < 0.001 && std::abs(seconds - position) < 0.05) return;
	lastSeek = seconds;
	const auto target = String(seconds, 6);
	const char* args[] = { "seek", target.toRawUTF8(), "absolute+exact", nullptr };
	BluxMpvApi::get().command(handle, 0, args);
}

void BluxMpvPlayer::setPlaySpeed(float rate)
{
	if (speed == rate) return;
	const bool changeDirection = (speed < 0) != (rate < 0);
	speed = rate;
	if (handle == nullptr) return;

	auto& api = BluxMpvApi::get();
	if (changeDirection)
	{
		const char* direction = speed < 0 ? "backward" : "forward";
		api.setProperty(handle, 0, "play-direction", MPV_FORMAT_STRING, &direction);
	}
	double magnitude = jlimit(0.01, 100.0, std::abs((double) speed));
	api.setProperty(handle, 0, "speed", MPV_FORMAT_DOUBLE, &magnitude);
}

void BluxMpvPlayer::setVolume(float gain)
{
	volume = gain;
	if (handle == nullptr) return;
	double percent = jlimit(0.0, 1000.0, (double) volume * 100.0);
	BluxMpvApi::get().setProperty(handle, 0, "volume", MPV_FORMAT_DOUBLE, &percent);
}

void BluxMpvPlayer::setLoop(bool shouldLoop)
{
	loop = shouldLoop;
	if (handle == nullptr) return;
	const char* value = loop ? "inf" : "no";
	BluxMpvApi::get().setProperty(handle, 0, "loop-file", MPV_FORMAT_STRING, &value);
}

void BluxMpvPlayer::readVideoSize()
{
	auto& api = BluxMpvApi::get();
	int64_t width = 0, height = 0;
	api.getProperty(handle, "dwidth", MPV_FORMAT_INT64, &width);
	api.getProperty(handle, "dheight", MPV_FORMAT_INT64, &height);
	videoWidth = (int) width;
	videoHeight = (int) height;
}

void BluxMpvPlayer::timerCallback()
{
	auto& api = BluxMpvApi::get();
	bool failed = false;
	bool ended = false;
	for (auto* event = api.nextEvent(handle, 0); event->event_id != MPV_EVENT_NONE; event = api.nextEvent(handle, 0))
	{
		switch (event->event_id)
		{
		case MPV_EVENT_FILE_LOADED:
		{
			auto* currentPath = api.getString(handle, "path");
			const bool isCurrent = currentPath != nullptr && File(String::fromUTF8(currentPath)) == sourceFile;
			api.freeString(currentPath);
			if (!isCurrent) break;

			loaded = shouldNotifyLoaded = true;
			duration = 0;
			api.getProperty(handle, "duration", MPV_FORMAT_DOUBLE, &duration);
			readVideoSize();
			setVolume(volume);
			setLoop(loop);
			break;
		}
		case MPV_EVENT_VIDEO_RECONFIG:
			readVideoSize();
			break;
		case MPV_EVENT_COMMAND_REPLY:
			if (event->reply_userdata == loadRequest && event->error < 0) failed = true;
			break;
		case MPV_EVENT_END_FILE:
			if (const auto* end = static_cast<const mpv_event_end_file*>(event->data))
				if (end->reason == MPV_END_FILE_REASON_ERROR) failed = true;
			break;
		case MPV_EVENT_PROPERTY_CHANGE:
		{
			const auto* property = static_cast<const mpv_event_property*>(event->data);
			if (property->format == MPV_FORMAT_DOUBLE)
				position = *static_cast<const double*>(property->data);
			else if (property->format == MPV_FORMAT_FLAG && *static_cast<const int*>(property->data) != 0)
				ended = true;
			break;
		}
		default:
			break;
		}
	}

	if (failed && !loaded)
	{
		loadError = "The video file could not be opened.";
		unload();
		notifyFileEnd();
		return;
	}

	framePending |= (api.updateRenderer(renderer) & MPV_RENDER_UPDATE_FRAME) != 0;
	if (loaded && framePending && videoWidth > 0 && videoHeight > 0)
	{
		framePending = false;
		renderFrame();
	}

	if (std::exchange(shouldNotifyLoaded, false))
		notifyFileLoaded();
	else if (ended && std::exchange(playing, false))
		notifyFileEnd();
}

void BluxMpvPlayer::renderFrame()
{
	const auto largestDimension = jmax(videoWidth, videoHeight);
	const double scale = jmin(1.0, 1280.0 / largestDimension);
	int size[] = { jmax(1, roundToInt(videoWidth * scale)), jmax(1, roundToInt(videoHeight * scale)) };
	size_t stride = (size[0] * 4u + 63u) & ~size_t(63);
	const size_t bytes = stride * size[1];
	pixels.resize(bytes + 63u);
	void* data = pixels.data();
	auto available = pixels.size();
	if (std::align(64, bytes, data, available) == nullptr) return;

	char pixelFormat[] = "bgr0";
	int blockForTime = 0;
	mpv_render_param params[] {
		{ MPV_RENDER_PARAM_SW_SIZE, size },
		{ MPV_RENDER_PARAM_SW_FORMAT, pixelFormat },
		{ MPV_RENDER_PARAM_SW_STRIDE, &stride },
		{ MPV_RENDER_PARAM_SW_POINTER, data },
		{ MPV_RENDER_PARAM_BLOCK_FOR_TARGET_TIME, &blockForTime },
		{}
	};
	if (BluxMpvApi::get().render(renderer, params) < 0) return;

	if (frame.getWidth() != size[0] || frame.getHeight() != size[1])
		frame = Image(Image::ARGB, size[0], size[1], false, SoftwareImageType());

	frame.duplicateIfShared();
	{
		Image::BitmapData bitmap(frame, Image::BitmapData::writeOnly);
		const auto* source = static_cast<const uint8*>(data);
		for (int y = 0; y < size[1]; ++y)
		{
			auto* line = reinterpret_cast<PixelARGB*>(bitmap.getLinePointer(y));
			for (int x = 0; x < size[0]; ++x)
			{
				const auto* pixel = source + x * 4;
				line[x] = PixelARGB(255, pixel[2], pixel[1], pixel[0]);
			}
			source += stride;
		}
	}
	notifyFrameUpdate();
}
