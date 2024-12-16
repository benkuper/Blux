/*
  ==============================================================================

	ColorComponent.h
	Created: 26 Sep 2020 2:20:01pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once


class ColorSource;
class PixelShape;
class DimmerComponent;

class ColorComponent :
	public ObjectComponent
{
public:
	ColorComponent(Object* o, var params);
	~ColorComponent();

	IntParameter* resolution;
	BoolParameter* useDimmerForOpacity;

	Array<Colour, CriticalSection> sourceColors;
	Array<Colour, CriticalSection> outColors;

	std::unique_ptr<ColorSource> prevColorSource; //for transitionning
	std::unique_ptr<ColorSource> colorSource;

	std::unique_ptr<PixelShape> pixelShape;

	enum ColorMode { RGB, RGBW, WRGB, RGBAW, RGBWA, CMY, HS, MODES_MAX };
	const int colorModeIndices[MODES_MAX][5] = {
		{ 0, 1, 2, -1, -1},
		{ 0, 1, 2, 3, -1 },
		{ 3, 0, 1, 2, -1 },
		{ 0, 1, 2, 4, 3 },
		{ 0, 1, 2, 3, 4 },
		{ 0, 1, 2, -1, -1},
		{ 0, 1, -1, -1, -1}
	};

	enum FineMode { None, Alternate, Follow };
	EnumParameter* fineMode;

	EnumParameter* colorMode;
	FloatParameter* whiteTemperature;

	ColorParameter* mainColor; //fake computed parameter for viz

	DimmerComponent* dimmerComponent; //if useDimmerForOpacity is checked


	void setupSource(const String& type, ColorSource* templateRef = nullptr);
	void setupShape(const String& type);
	void setupFromJSONDefinition(var definition) override;


	void update() override;
	void fillComputedValueMap(HashMap<Parameter*, var>& values) override;
	void updateComputedValues(HashMap<Parameter*, var>& values) override;

	virtual void fillInterfaceDataInternal(Interface* i, var data, var params) override;// (HashMap<int, float>& channelValueMap, int startChannel, bool 

	//virtual void fillOutValueMap(HashMap<int, float>& channelValueMap, int startChannel, bool ignoreChannelOffset = false) override;

	void onContainerParameterChangedInternal(Parameter* p) override;


	var getJSONData(bool includeNonOverriden = false) override;
	void loadJSONDataItemInternal(var data) override;

	class ColorComponentEvent
	{
	public:
		enum Type { SOURCE_CHANGED, SHAPE_CHANGED };

		ColorComponentEvent(Type t, ColorComponent* comp) : type(t), component(comp) {}
		Type type;
		ColorComponent* component;
	};

	QueuedNotifier<ColorComponentEvent> colorComponentNotifier;
	typedef QueuedNotifier<ColorComponentEvent>::Listener AsyncColorComponentListener;

	void addAsyncColorComponentListener(AsyncColorComponentListener* newListener) { colorComponentNotifier.addListener(newListener); }
	void addAsyncCoalescedColorComponentListener(AsyncColorComponentListener* newListener) { colorComponentNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncColorComponentListener(AsyncColorComponentListener* listener) { colorComponentNotifier.removeListener(listener); }

	String getTypeString() const override { return "Color"; }
	static ColorComponent* create(Object* o, var params) { return new ColorComponent(o, params); }

	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = {}) override;
};