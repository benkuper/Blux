/*
  ==============================================================================

    ComponentManager.h
    Created: 26 Sep 2020 10:02:54am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Object;

class ComponentFactory :
    public Factory<ObjectComponent>
{
public:
    ComponentFactory(Object * o);
    ~ComponentFactory() {}


    typedef std::function<ObjectComponent* (Object*, var)> CreateCompFunc;

    class ComponentDefinition :
        public FactoryParametricDefinition<ObjectComponent, CreateCompFunc>
    {
    public:
        ComponentDefinition(String menuPath, String type, Object* o, CreateCompFunc func, var params = new DynamicObject()) :
            FactoryParametricDefinition(menuPath, type, func, params),
            object(o) 
        {
        }

        virtual ~ComponentDefinition() {}

        Object* object;

        virtual ObjectComponent * create() override {
            return this->createFunc(object, this->params);
        }

        static ComponentDefinition* createDef(StringRef menu, StringRef type, CreateCompFunc createFunc, Object * o)
        {
            ComponentDefinition* d = new ComponentDefinition(menu, type, o, createFunc);
            return d;
        }
    };
};

class ComponentManager :
    public Manager<ObjectComponent>
{
public:
    ComponentManager(Object * o);
    ~ComponentManager();


    ComponentFactory factory;

    void addComponentFromDefinition(StringRef type, var definition, bool canBeRemoved = false);

    var getSceneData();
    void updateSceneData(var& sceneData);
    void lerpFromSceneData(var startData, var endData, float weight);

    virtual var getVizData();
    //var getJSONData(bool includeNonOverriden = false) override;
    //void loadJSONDataInternal(var data) override;
};