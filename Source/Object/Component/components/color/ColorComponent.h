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

class ColorComponent :
    public ObjectComponent
{
public:
    ColorComponent(Object * o, var params);
    ~ColorComponent();

    IntParameter* resolution;

    Array<Colour, CriticalSection> sourceColors;
    Array<Colour, CriticalSection> outColors;

    std::unique_ptr<ColorSource> prevColorSource; //for transitionning
    std::unique_ptr<ColorSource> colorSource;

    std::unique_ptr<PixelShape> pixelShape;

    void setupSource(const String &type, ColorSource * templateRef = nullptr);
    void setupShape(const String &type);
    void setupFromJSONDefinition(var definition) override;

    void update() override;
    virtual var getOriginalComputedValues() override;
    virtual void fillOutValueMap(HashMap<int, float>& channelValueMap, int startChannel, bool ignoreChannelOffset = false) override;

    void onContainerParameterChangedInternal(Parameter* p) override;

    var getJSONData() override;
    void loadJSONDataItemInternal(var data) override;

    class ColorComponentEvent
    {
    public:
        enum Type { SOURCE_CHANGED, SHAPE_CHANGED };

        ColorComponentEvent(Type t, ColorComponent * comp) : type(t), component(comp) {}
        Type type;
        ColorComponent* component;
    };

    QueuedNotifier<ColorComponentEvent> colorComponentNotifier;
    typedef QueuedNotifier<ColorComponentEvent>::Listener AsyncColorComponentListener;

    void addAsyncColorComponentListener(AsyncColorComponentListener* newListener) { colorComponentNotifier.addListener(newListener); }
    void addAsyncCoalescedColorComponentListener(AsyncColorComponentListener* newListener) { colorComponentNotifier.addAsyncCoalescedListener(newListener); }
    void removeAsyncColorComponentListener(AsyncColorComponentListener* listener) { colorComponentNotifier.removeListener(listener); }

    String getTypeString() const override { return "Color"; }
    static ColorComponent* create(Object * o, var params) { return new ColorComponent(o, params); }

    InspectableEditor* getEditorInternal(bool isRoot) override;
};