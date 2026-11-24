/*
  ==============================================================================

	BluxSequence.cpp
	Created: 5 Oct 2020 2:42:07pm
	Author:  bkupe

  ==============================================================================
*/

#include "Sequence/SequenceIncludes.h"
#include "Audio/AudioManager.h"
#include "Scene/Scene.h"
#include "BluxSequence.h"
#include "layers/video/BluxMpvPlayer.h"

namespace
{
class BluxVideoLayer final : public VideoLayer
{
public:
	BluxVideoLayer(Sequence* sequence, var params) : VideoLayer(sequence, params)
	{
		for (auto* slot : { &moviePlayer, &overlapPlayer })
		{
			auto player = std::make_unique<BluxMpvPlayer>();
			player->addListener(this);
			*slot = std::move(player);
		}
	}

	static VideoLayer* create(Sequence* sequence, var params)
	{
		return new BluxVideoLayer(sequence, params);
	}

	void playerFileLoaded(VideoPlayerEngine* player) override
	{
		VideoLayer::playerFileLoaded(player);
		cacheFrame(player, static_cast<BluxMpvPlayer*>(player)->getFrame());
	}

	void playerFrameUpdate() override
	{
		cacheFrame(moviePlayer.get(), static_cast<BluxMpvPlayer&>(*moviePlayer).getFrame());
		cacheFrame(overlapPlayer.get(), static_cast<BluxMpvPlayer&>(*overlapPlayer).getFrame());
	}

private:
	void cacheFrame(VideoPlayerEngine* player, const Image& frame)
	{
		auto* clip = player == moviePlayer.get() ? loadedClip.get() : loadedOverlapClip.get();
		if (clip == nullptr || !frame.isValid()) return;
		const double time = player->getPosition();
		if (clip->needsThumbnail(time)) clip->cacheThumbnail(time, frame);
	}
};

class BluxVideoPreview final : public ShapeShifterContentComponent, private Timer
{
public:
	explicit BluxVideoPreview(const String& contentName) : ShapeShifterContentComponent(contentName)
	{
		contentIsFlexible = true;
		setOpaque(true);
	}

	void visibilityChanged() override
	{
		if (isShowing()) startTimerHz(30);
		else stopTimer();
	}

	void parentHierarchyChanged() override
	{
		visibilityChanged();
	}

	void paint(Graphics& g) override
	{
		g.fillAll(Colours::black);

		auto* sequence = getSequence();
		if (sequence == nullptr)
		{
			drawStatus(g, "Open a sequence to preview its video.");
			return;
		}

		bool hasVideoLayer = false;
		bool hasFrame = false;
		String clipStatus;
		for (int index = sequence->layerManager->items.size(); --index >= 0;)
		{
			auto* layer = dynamic_cast<BluxVideoLayer*>(sequence->layerManager->items[index]);
			if (layer == nullptr) continue;
			hasVideoLayer = true;
			if (!layer->enabled->boolValue()) continue;

			hasFrame |= drawClip(g, *layer, layer->loadedClip.get(), layer->currentClip.get(), layer->moviePlayer.get(), clipStatus);
			hasFrame |= drawClip(g, *layer, layer->loadedOverlapClip.get(), layer->overlapClip.get(), layer->overlapPlayer.get(), clipStatus);
		}

		if (!hasVideoLayer) drawStatus(g, "Add a Video layer to the sequence.");
		else if (!hasFrame) drawStatus(g, clipStatus.isEmpty() ? "Move the playhead over a video clip." : clipStatus);
	}

private:
	void timerCallback() override { repaint(); }

	static Sequence* getSequence()
	{
		if (auto* panels = ShapeShifterManager::getInstanceWithoutCreating())
		{
			if (auto* timeMachine = panels->getContentForType<TimeMachineView>())
			{
				if (timeMachine->editor != nullptr && timeMachine->editor->sequenceRef != nullptr)
					return timeMachine->editor->sequence;
			}
		}
		auto* selection = InspectableSelectionManager::mainSelectionManager;
		if (selection == nullptr) return nullptr;
		if (auto* sequence = selection->getInspectableAs<Sequence>()) return sequence;
		if (auto* container = selection->getInspectableAs<ControllableContainer>())
			return ControllableUtil::findParentAs<Sequence>(container);
		return ControllableUtil::findParentAs<Sequence>(selection->getInspectableAs<Controllable>());
	}

	void drawStatus(Graphics& g, const String& message)
	{
		g.setColour(Colours::white.withAlpha(0.6f));
		g.drawFittedText(message, getLocalBounds().reduced(12), Justification::centred, 2);
	}

	bool drawClip(Graphics& g, VideoLayer& layer, VideoLayerClip* loaded, VideoLayerClip* active, VideoPlayerEngine* engine, String& clipStatus)
	{
		if (active == nullptr || engine == nullptr) return false;
		auto& player = static_cast<BluxMpvPlayer&>(*engine);
		const auto frame = loaded == active ? player.getFrame() : Image();
		if (!frame.isValid())
		{
			const auto path = active->filePath->getAbsolutePath();
			const auto error = player.getLoadError(path);
			if (error.isNotEmpty()) clipStatus = error;
			else if (clipStatus.isEmpty())
				clipStatus = path.isEmpty() ? "Choose a media file for the video clip." : "Waiting for a video frame...";
			return false;
		}

		const auto area = getLocalBounds();
		const auto fit = RectanglePlacement(RectanglePlacement::centred)
			.appliedTo(frame.getBounds(), area).toFloat();
		auto destination = fit.withSizeKeepingCentre(fit.getWidth() * loaded->getRenderScaleX(),
			fit.getHeight() * loaded->getRenderScaleY());
		destination.translate(area.getWidth() * loaded->getRenderXPercent() / 100.0f,
			area.getHeight() * loaded->getRenderYPercent() / 100.0f);

		g.setOpacity(jlimit(0.0f, 1.0f, loaded->getRenderOpacity() * layer.getClipFadeFactor(loaded)));
		g.drawImage(frame, destination, RectanglePlacement::stretchToFit);
		g.setOpacity(1.0f);
		return true;
	}
};
}

ShapeShifterContent* BluxSequence::createVideoPreview(const String& contentName)
{
	return new BluxVideoPreview(contentName);
}

BluxSequence::BluxSequence() :
	manualStartAtLoad(false)
{
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Effect", &EffectLayer::create, this));
	//layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Automation", &AutomationLayer::create, this));
	//layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Color Source", &ColorSourceLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Actions", &ActionLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", RawDataLayer::getTypeStringStatic(), &RawDataLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Audio", &AudioLayer::create, this, true));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Video", &BluxVideoLayer::create, this, false, true));

	layerManager->addManagerListener(this);

	setAudioDeviceManager(&AudioManager::getInstance()->am);
}

BluxSequence::~BluxSequence()
{
	layerManager->removeManagerListener(this);
}

bool BluxSequence::isAffectingObject(Object* o)
{
	for (int i = layerManager->items.size() - 1; i >= 0; --i)
	{
		if (!layerManager->items[i]->enabled->boolValue()) continue;
		if (EffectLayer* e = dynamic_cast<EffectLayer*>(layerManager->items[i]))
		{
			if (e->filterManager->isAffectingObject(o)) return true;
		}
	}

	return false;
}

Array<ChainVizTarget*> BluxSequence::getChainVizTargetsForObjectAndComponent(Object* o, ComponentType c)
{
	Array<ChainVizTarget*> result;
	for (int i = layerManager->items.size() - 1; i >= 0; --i)
	{
		if (EffectLayer* e = dynamic_cast<EffectLayer*>(layerManager->items[i]))
		{
			result.addArray(e->getChainVizTargetsForObjectAndComponent(o, c));
		}
	}
	return result;
}

void BluxSequence::processComponent(Object* o, ObjectComponent* c, HashMap<Parameter*, var>& values, float weightMultiplier)
{
	for (int i = layerManager->items.size() - 1; i >= 0; --i)
	{
		if (!layerManager->items[i]->enabled->boolValue()) continue;
		if (EffectLayer* e = dynamic_cast<EffectLayer*>(layerManager->items[i]))
		{
			e->processComponent(o, c, values, weightMultiplier);
		}
	}
}

void BluxSequence::processRawData()
{
	for (int i = layerManager->items.size() - 1; i >= 0; --i)
	{
		if (!layerManager->items[i]->enabled->boolValue()) continue;
		if (RawDataLayer* ri = dynamic_cast<RawDataLayer*>(layerManager->items[i])) ri->processRawData();
	}
}

void BluxSequence::itemAdded(SequenceLayer* layer)
{
	if (AudioLayer* al = dynamic_cast<AudioLayer*>(layer))
	{
		al->setAudioProcessorGraph(&AudioManager::getInstance()->graph, AUDIO_OUTPUT_GRAPH_ID);
	}
}

void BluxSequence::itemsAdded(Array<SequenceLayer*> layers)
{
	for (auto& layer : layers)
	{
		if (AudioLayer* al = dynamic_cast<AudioLayer*>(layer))
		{
			al->setAudioProcessorGraph(&AudioManager::getInstance()->graph, AUDIO_OUTPUT_GRAPH_ID);
		}
	}
}

void BluxSequence::handleStartAtLoad()
{
	//do nothing for start at load, handled by scene manager
}

String BluxSequence::getPanelName() const
{
	if (Scene* s = ControllableUtil::findParentAs<Scene>(parentContainer)) return s->niceName;
	return niceName;
}

ChainVizComponent* BluxSequence::createVizComponent(Object* o, ComponentType ct, ChainVizTarget::ChainVizType type)
{
	return new BluxSequenceChainVizUI(this, o, ct, type);
}

InspectableEditor* BluxSequence::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new BluxSequenceEditor(this, isRoot);
}
