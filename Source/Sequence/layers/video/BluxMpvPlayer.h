#pragma once

#include "JuceHeader.h"
#include <vector>

struct mpv_handle;
struct mpv_render_context;

class BluxMpvPlayer final : public VideoPlayerEngine, private juce::Timer
{
public:
	BluxMpvPlayer() = default;
	~BluxMpvPlayer() override;

	bool load(const juce::String& filePath) override;
	void unload() override;
	bool isFileLoaded() const override { return loaded; }
	juce::String getFilePath() const override { return path; }

	void play() override;
	void pause() override;
	void stop() override;
	bool isPlaying() const override { return loaded && playing; }

	void setPosition(double seconds) override;
	double getPosition() const override { return position; }
	double getDuration() const override { return duration; }
	void setPlaySpeed(float rate) override;
	float getPlaySpeed() const override { return speed; }
	void setVolume(float gain) override;
	float getVolume() const override { return volume; }
	void setLoop(bool shouldLoop) override;
	bool getLoop() const override { return loop; }

	int getVideoWidth() const override { return videoWidth; }
	int getVideoHeight() const override { return videoHeight; }
	int getNumChannels() const override { return 0; }
	void setupGL() override {}
	void renderGL(juce::OpenGLFrameBuffer&) override {}
	juce::AudioProcessor* getAudioProcessor() override { return nullptr; }

	juce::Image getFrame() const { return frame; }

private:
	bool openEngine();
	void releaseEngine();
	void timerCallback() override;
	void readVideoSize();
	void renderFrame();
	void setPaused(bool paused);

	mpv_handle* handle = nullptr;
	mpv_render_context* renderer = nullptr;
	juce::String path;
	juce::File sourceFile;
	juce::Image frame;
	std::vector<unsigned char> pixels;
	uint64_t loadRequest = 0;
	double duration = 0;
	double position = 0;
	double lastSeek = -1;
	int videoWidth = 0;
	int videoHeight = 0;
	float speed = 1;
	float volume = 1;
	bool loop = false;
	bool loaded = false;
	bool playing = false;
	bool shouldNotifyLoaded = false;
	bool framePending = false;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BluxMpvPlayer)
};
