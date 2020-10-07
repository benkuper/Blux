/*
  ==============================================================================

    LayoutFilterStageViewUI.h
    Created: 7 Oct 2020 9:06:54am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../LayoutFilter.h"

class LayoutFilterStageViewUI :
    public Component,
    public ContainerAsyncListener,
    public Inspectable::InspectableListener
{
public:
    LayoutFilterStageViewUI(LayoutFilter * f);
    ~LayoutFilterStageViewUI();

    LayoutFilter* filter;
    WeakReference<Inspectable> filterRef;

    Vector3D<float> posAtMouseDown;
    Rectangle<float> boundsInView;

    void paint(Graphics& g) override;

    void resized() override;
    Rectangle<int> getBoundsInView(Rectangle<float> r);

    void newMessage(const ContainerAsyncEvent& e) override;

    void inspectableDestroyed(Inspectable* i) override;

    class Handle :
        public Component
    {
    public:
        Handle();
        ~Handle();

        Colour color;

        void paint(Graphics& g) override;
    };

    Handle handle;

    class FilterEvent
    {
    public:
        enum Type { FILTER_REMOVED };

        FilterEvent(Type t, LayoutFilterStageViewUI * ui) : type(t), ui(ui) {}

        Type type;
        LayoutFilterStageViewUI* ui;
    };

    QueuedNotifier<FilterEvent> queuedNotifier;
    typedef QueuedNotifier<FilterEvent>::Listener AsyncListener;


    void addAsyncLayoutFilterListener(AsyncListener* newListener) { queuedNotifier.addListener(newListener); }
    void addAsyncCoalescedLayoutFilterListener(AsyncListener* newListener) { queuedNotifier.addAsyncCoalescedListener(newListener); }
    void removeAsyncLayoutFilterListener(AsyncListener* listener) { queuedNotifier.removeListener(listener); }

};