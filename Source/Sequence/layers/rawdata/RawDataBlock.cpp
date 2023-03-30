/*
  ==============================================================================

	RawDataBlock.cpp
	Created: 12 Dec 2022 5:38:15pm
	Author:  bkupe

  ==============================================================================
*/

#include "Sequence/SequenceIncludes.h"

RawDataBlock::RawDataBlock() :
	LayerBlock(getTypeString()),
	lastReadFrameIndex(0),
	rawDataNotifier(5)
{
	blendMode = addEnumParameter("Blend Mode", "Data blending");
	blendMode->addOption("Alpha", ALPHA)->addOption("Add", ADD)->addOption("Multiply", MULTIPLY)->addOption("Max", MAX)->addOption("Min", MIN);

	fadeIn = addFloatParameter("Fade In", "Fade in time", 0, 0, getTotalLength(), false);
	fadeIn->defaultUI = FloatParameter::TIME;
	fadeIn->canBeDisabledByUser = true;
	fadeOut = addFloatParameter("Fade Out", "Fade out time", 0, 0, getTotalLength(), false);
	fadeOut->defaultUI = FloatParameter::TIME;
	fadeOut->canBeDisabledByUser = true;

	fileParam = addFileParameter("File", "The Raw data file to use");

}

RawDataBlock::~RawDataBlock()
{
}

void RawDataBlock::onContainerParameterChangedInternal(Parameter* p)
{
	LayerBlock::onContainerParameterChangedInternal(p);
	if (p == fileParam)
	{
		file = fileParam->getFile();
		readInfos();
	}
}

inline void RawDataBlock::controllableStateChanged(Controllable* c)
{
	LayerBlock::controllableStateChanged(c);
	if (c == fadeIn || c == fadeOut)
	{
		//effectBlockListeners.call(&EffectBlockListener::effectBlockFadesChanged, this);
		rawDataNotifier.addMessage(new RawDataBlockEvent(RawDataBlockEvent::FADES_CHANGED, this));
	}
}

void RawDataBlock::readInfos()
{
	universeFrameMap.clear();
	universeFrameMap.set(-1, Array<FrameData*>());
	frames.clear();
	fs.reset();

	if (!file.existsAsFile()) return;

	fs.reset(new FileInputStream(file));

	float totalTime = fs->readFloat();
	int numUniverses = fs->readInt();
	int numWrittenFrames = fs->readInt();

	LOG("Read data, total time : " << totalTime << ", num universes : " << numUniverses << ", num written frames : " << numWrittenFrames);


	while (fs->getNumBytesRemaining() > 0)
	{
		int frameSize = fs->readInt();
		float frameTime = fs->readFloat();
		int numUniverses = fs->readInt();
		int dataPos = fs->getPosition();

		FrameData* frame = new FrameData(frameTime, numUniverses, dataPos);

		for (int i = 0; i < numUniverses; i++)
		{
			int univ = fs->readInt();

			frame->universePosMap.set(univ, fs->getPosition());

			if (!universeFrameMap.contains(univ)) universeFrameMap.set(univ, Array<FrameData*>());
			universeFrameMap.getReference(univ).add(frame);

			recordedUniverseIndices.addIfNotAlreadyThere(univ);

			fs->setPosition(fs->getPosition() + DMX_NUM_CHANNELS);
		}

		frames.add(frame);
		universeFrameMap.getReference(-1).add(frame);

		fs->setPosition(dataPos + frameSize);
	}

	LOG("Check, num stored frames " << frames.size());

	setCoreLength(totalTime, false);
}

Array<DMXUniverse*> RawDataBlock::readFrameAtTime(float time)
{
	Array<DMXUniverse*> result;

	if (fs == nullptr) return result;

	FrameData* f = getFrameDataAtTime(time);
	if (f == nullptr) return result;

	fs->setPosition(f->filePos);
	for (int i = 0; i < f->numUniverses; i++)
	{
		int universeIndex = fs->readInt();
		DMXUniverse* u = new DMXUniverse(universeIndex);
		fs->read(u->values.getRawDataPointer(), DMX_NUM_CHANNELS);
		result.add(u);
	}

	return result;
}

Array<DMXUniverse*> RawDataBlock::readAllUniversesAtTime(float time)
{
	Array<DMXUniverse*> result;

	for (auto& univ : recordedUniverseIndices)
	{
		DMXUniverse* u = readUniverseAtTime(time, univ);
		if (u == nullptr) continue;
		result.add(u);
	}

	return result;
}

DMXUniverse* RawDataBlock::readUniverseAtTime(float time, int universeIndex)
{
	FrameData* f = getFrameDataAtTime(time, universeIndex);
	if (f == nullptr) return nullptr;

	fs->setPosition(f->universePosMap[universeIndex]);

	DMXUniverse* u = new DMXUniverse(universeIndex);
	fs->read(u->values.getRawDataPointer(), DMX_NUM_CHANNELS);

	return u;
}


RawDataBlock::FrameData* RawDataBlock::getFrameDataAtTime(float time, int universeIndex)
{
	float t = getRelativeTime(time, true);
	if (time < 0) return nullptr;

	Array<FrameData*> univFrames = universeFrameMap.getReference(universeIndex); //-1 holds all the frames

	if (univFrames.size() == 0) return nullptr;
	if (univFrames.size() == 1) return univFrames[0];

	int frameIndex = jlimit(0, univFrames.size() - 1, lastReadFrameIndex + 1);
	if (univFrames[frameIndex] != nullptr && univFrames[frameIndex]->time < t)
	{
		while (univFrames[frameIndex]->time <= t && frameIndex < univFrames.size() - 1)
		{
			frameIndex++;
		}

	}

	if (univFrames[frameIndex] != nullptr && univFrames[frameIndex]->time > t)
	{
		while (univFrames[frameIndex]->time > t && frameIndex > 0)
		{
			frameIndex--;
		}
	}

	if (frameIndex < 0) return nullptr;

	return univFrames[frameIndex];
}

float RawDataBlock::getFadeFactorAtTime(float t)
{
	float factor = 1;
	double relTimeTotal = t - time->floatValue();
	if (fadeIn->enabled && fadeIn->floatValue() > 0) factor *= jmin<double>(relTimeTotal / fadeIn->floatValue(), 1.f);
	if (fadeOut->enabled && fadeOut->floatValue() > 0) factor *= jmin<double>((getTotalLength() - relTimeTotal) / fadeOut->floatValue(), 1.f);
	factor = jmax(factor, 0.f);
	return factor;
}
