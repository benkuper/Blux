/*
  ==============================================================================

	RawDataBlock.h
	Created: 12 Dec 2022 5:38:15pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class RawDataBlock :
	public LayerBlock
{
public:
	RawDataBlock();
	~RawDataBlock();

	FileParameter* fileParam;
	File file;

	FloatParameter* fadeIn;
	FloatParameter* fadeOut;

	enum BlendMode { ALPHA, ADD, MULTIPLY, MAX, MIN };
	EnumParameter* blendMode;
	std::unique_ptr<FileInputStream> fs;

	struct FrameData
	{
		FrameData(float time, float numUniverses, float filePos) : time(time), numUniverses(numUniverses), filePos(filePos) {}
		float time = -1;
		int numUniverses = 0;
		int filePos = 0;
		HashMap<int, int> universePosMap;
	};

	OwnedArray<FrameData> frames;
	HashMap<int, Array<FrameData*>> universeFrameMap;
	Array<int> recordedUniverseIndices;;

	int lastReadFrameIndex;

	void onContainerParameterChangedInternal(Parameter* p) override;
	void controllableStateChanged(Controllable* c);

	void readInfos();
	Array<DMXUniverse*> readFrameAtTime(float time);
	Array<DMXUniverse*> readAllUniversesAtTime(float time);
	DMXUniverse* readUniverseAtTime(float time, int universeIndex);

	FrameData* getFrameDataAtTime(float time, int universeIndex = -1);
	
	float getLastFrameTime();

	float getFadeFactorAtTime(float time);

	DECLARE_ASYNC_EVENT(RawDataBlock, RawDataBlock, rawData, ENUM_LIST(LOADED, FADES_CHANGED));
	DECLARE_TYPE("Raw Data Block");
};