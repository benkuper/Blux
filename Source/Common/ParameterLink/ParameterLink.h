/*
  ==============================================================================

    ParameterLink.h
    Created: 21 Dec 2020 11:12:38pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Object;

class ParameterLink :
    public Inspectable::InspectableListener,
    public SpatManager::ManagerListener
{
public:
    enum LinkType { NONE, CUSTOM_PARAM, OBJECT_ID, OBJECT_POSX, OBJECT_POSY, OBJECT_POSZ, OBJECT_POSXZ, OBJECT_POSXYZ, SPAT_X, SPAT_Z, SPAT_XZ };
    
    ParameterLink(WeakReference<Parameter> p);
    ~ParameterLink();

    LinkType linkType;
    WeakReference<Parameter> parameter;

    SpatItem* spatializer;
    WeakReference<Inspectable> spatRef;
    String spatGhostName;

    bool isLinkable;

    //links
    WeakReference<Parameter> linkedCustomParam; //from ObjectManger custom params

    //bool replacementHasMappingInputToken;
    //String replacementString;

    void setLinkType(LinkType type);
    void setSpatLink(LinkType type, SpatItem* spat);

    void setLinkedCustomParam(Parameter * p);
    var getLinkedValue(Object * o, int id);

    

    //For target parameters
    WeakReference<Controllable> getLinkedTarget(Object * o);
    WeakReference<ControllableContainer> getLinkedTargetContainer(Object *o);


    void itemAdded(SpatItem* item) override;
    void itemsAdded(Array<SpatItem*> items) override;
    void itemRemoved(SpatItem* item) override;
    void itemsRemoved(Array<SpatItem*> items) override;
    
    //String getReplacementString(int multiplexIndex);

    void notifyLinkUpdated();

    var getJSONData(bool includeNonOverriden = false);
    void loadJSONData(var data);

    class ParameterLinkListener
    {
    public:
        virtual ~ParameterLinkListener() {}
        virtual void linkUpdated(ParameterLink * pLink) {}
    };

    ListenerList<ParameterLinkListener> parameterLinkListeners;
    void addParameterLinkListener(ParameterLinkListener* newListener) { parameterLinkListeners.add(newListener); }
    void removeParameterLinkListener(ParameterLinkListener* listener) { if (isLinkBeingDestroyed) return; parameterLinkListeners.remove(listener); }

    DECLARE_ASYNC_EVENT(ParameterLink, ParameterLink, paramLink, ENUM_LIST(LINK_UPDATED, PREVIEW_UPDATED), EVENT_NO_CHECK)

private:
    bool isLinkBeingDestroyed;
};

class ParamLinkContainer :
    public ControllableContainer,
    public ParameterLink::ParameterLinkListener
{
public:
    ParamLinkContainer(const String& name);
    virtual ~ParamLinkContainer();

    bool paramsCanBeLinked;

    OwnedArray<ParameterLink> paramLinks;
    HashMap<Parameter*, ParameterLink*> paramLinkMap;
    HashMap<ParameterLink*, Parameter*> linkParamMap;

    var ghostData;

    virtual void onControllableAdded(Controllable* c) override;
    virtual void onControllableRemoved(Controllable* c) override;

    virtual void parameterControlModeChanged(Parameter* p) override;

    virtual ParameterLink* getLinkedParam(Parameter* p);
    virtual var getLinkedValue(Parameter* p, Object * o, int id, float time = 0);

    var getParamValue(Parameter* p, float time = 0);

    virtual void linkUpdated(ParameterLink* p) override;

    template<class T>
    T* getLinkedTargetAs(TargetParameter* target, Object * o)
    {
        if (target == nullptr) return nullptr;

        if (paramsCanBeLinked)
        {
            if (ParameterLink* pl = getLinkedParam(target)) return dynamic_cast<T*>(pl->getLinkedTarget(o).get());
        }

        return dynamic_cast<T*>(target->target.get());
    }

    template<class T>
    T* getLinkedTargetContainerAs(TargetParameter * target, Object *o)
    {
        if (paramsCanBeLinked)
        {
            if (ParameterLink* pl = getLinkedParam(target)) return dynamic_cast<T*>(pl->getLinkedTargetContainer(o).get());
        }

        return dynamic_cast<T*>(target->targetContainer.get());
    }

    class ParamLinkContainerListener
    {
    public:
        virtual ~ParamLinkContainerListener() {}
        virtual void linkUpdated(ParamLinkContainer* container, ParameterLink* pLink) {}
        virtual void paramControlModeChanged(ParamLinkContainer * container, ParameterLink* pLink) {}
    };


    ListenerList<ParamLinkContainerListener> paramLinkContainerListeners;
    void addParamLinkContainerListener(ParamLinkContainerListener* newListener) { paramLinkContainerListeners.add(newListener); }
    void removeParamLinkContainerListener(ParamLinkContainerListener* listener) { paramLinkContainerListeners.remove(listener); }

    virtual var getJSONData(bool includeNonOverriden = false) override;
    virtual void loadJSONDataInternal(var data) override;

    virtual InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = {}) override;
};