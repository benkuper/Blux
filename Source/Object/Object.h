/*
  ==============================================================================

    Object.h
    Created: 26 Sep 2020 10:02:32am
    Author:  bkupe

  ==============================================================================
*/

#pragma once


class ChainVizComponent;
class SubObjectManager;
class EffectManager;
class Effect;

class ObjectManagerCustomParams;

class Object :
    public BaseItem,
    public ChainVizTarget
{
public:
    Object(var params = var());
    virtual ~Object();


    String objectType;
    var objectData;

    TargetParameter* targetInterface;
    std::unique_ptr<ControllableContainer> interfaceParameters;
    var interfaceGhostData;

    std::unique_ptr<ComponentManager> componentManager;
    std::unique_ptr<EffectManager> effectManager;
    std::unique_ptr<SubObjectManager> objectManager;

    IntParameter* globalID;
    int previousID;
    
    BoolParameter* excludeFromScenes;

    Point3DParameter* stagePosition;

    //ui
    EnumParameter* icon;
    Parameter* slideManipParameter;
    float slideManipValueRef;

    //chainviz
    HashMap<Effect*, float, DefaultHashFunctions, CriticalSection> effectIntensityOutMap;

    std::unique_ptr<ObjectManagerCustomParams> customParams;

    virtual void clearItem() override;


    void rebuildInterfaceParams();

    template<class T>
    T* getComponent();

    void onContainerParameterChangedInternal(Parameter* p) override;
    void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

    void checkAndComputeComponentValuesIfNeeded();
    void computeComponentValues(ObjectComponent* c);

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);


    Array<ChainVizTarget *> getEffectChain();

    ChainVizComponent* createVizComponent(Object * o, ChainVizTarget::ChainVizType type) override;

    //Listener
    class  ObjectListener
    {
    public:
        /** Destructor. */
        virtual ~ObjectListener() {}
        virtual void objectIDChanged(Object * o, int previousID) {}
    };

    ListenerList<ObjectListener> objectListeners;
    void addObjectListener(ObjectListener* newListener) { objectListeners.add(newListener); }
    void removeObjectListener(ObjectListener* listener) { objectListeners.remove(listener); }

    String getTypeString() const override { return objectType; }
    static Object* create(var params) { return new Object(params); }
};

template<class T>
T* Object::getComponent()
{
    for (auto& c : componentManager->items)
    {
        if (T* result = dynamic_cast<T*>(c)) return result;
    }
    return nullptr;
}