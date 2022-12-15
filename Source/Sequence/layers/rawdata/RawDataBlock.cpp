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

void RawDataBlock::readInfos()
{
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

		LOG(" > " << frameTime << " : " << numUniverses);

		frames.add({ frameTime,numUniverses, dataPos });

		fs->setPosition(dataPos + frameSize);
	}

	LOG("Check, num stored frames " << frames.size());

	setCoreLength(totalTime, false);
}

Array<DMXUniverse*> RawDataBlock::readAtTime(float time)
{
	Array<DMXUniverse*> result;

	if (fs == nullptr) return result;

	FrameData f = getFrameDataAtTime(time);
	if (f.time == -1) return result;

	fs->setPosition(f.filePos);
	for (int i = 0; i < f.numUniverses; i++)
	{
		int universeIndex = fs->readInt();
		DMXUniverse* u = new DMXUniverse(universeIndex);
		fs->read(u->values.getRawDataPointer(), DMX_NUM_CHANNELS);
		result.add(u);
	}

	return result;
}

RawDataBlock::FrameData RawDataBlock::getFrameDataAtTime(float time)
{
	float t = getRelativeTime(time, true);
	if (time < 0) return FrameData();
	
	int frameIndex = lastReadFrameIndex;
	while (frames[frameIndex].time < t && frameIndex < frames.size())
	{
		frameIndex++;
	}

	if (frameIndex >= frames.size()) return FrameData();

	while (frames[frameIndex].time > t && frameIndex > 0)
	{
		frameIndex--;
	}

	return frames[frameIndex];
}