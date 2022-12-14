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
	if (!file.existsAsFile()) return;
	FileInputStream fs(file);

	float totalTime = fs.readFloat();
	int numUniverses = fs.readInt();
	int numWrittenFrames = fs.readInt();

	LOG("Read data, total time : " << totalTime << ", num universes : " << numUniverses << ", num written frames : " << numWrittenFrames);

	setCoreLength(totalTime, false);
}

void RawDataBlock::readAtTime(float time)
{
}
