/*
  ==============================================================================

    ChainViz.cpp
    Created: 30 Oct 2020 4:34:50pm
    Author:  bkupe

  ==============================================================================
*/

#include "ChainViz.h"
#include "Effect/Effect.h"
#include "Object/Object.h"

const String ChainViz::panelName = "Channel Viz";

ChainViz::ChainViz(const String& name) :
    ShapeShifterContentComponent(name)
{
    viewport.setViewedComponent(&container, false);
    viewport.setScrollBarsShown(false, true, false, false);
    viewport.setScrollBarThickness(16);
    addAndMakeVisible(&viewport);
}

ChainViz::~ChainViz()
{
    setCurrentObject(nullptr);
}

void ChainViz::clear()
{
    for (auto& c : vizComponents) removeChildComponent(c);
    vizComponents.clear();
}

void ChainViz::setCurrentObject(Object* o)
{
    if (o == currentObject) return;

    if (currentObject != nullptr && !objectRef.wasObjectDeleted())
    {
    }
    
    clear();

    currentObject = o;
    objectRef = (Inspectable *)o;

    buildChain();
    resized();
}

void ChainViz::buildChain()
{
    if (currentObject == nullptr || objectRef.wasObjectDeleted()) return;

    ChainVizComponent* oc = new ChainVizComponent(currentObject);
    container.addAndMakeVisible(oc);
    vizComponents.add(oc);

    Array<Effect *> effects = currentObject->getEffectChain();

    for (auto& e : effects)
    {
        ChainVizComponent* ec = new ChainVizComponent(e);
        container.addAndMakeVisible(ec);
        vizComponents.add(ec);
    }

}

void ChainViz::resized()
{
    Rectangle<int> r = getLocalBounds().reduced(10);
    for (auto& c : vizComponents)
    {
        r.setWidth(c->getWidth());
        c->setBounds(r);
        r.setX(r.getRight() + 10);
    }
    container.setSize(r.getRight() + 20, getHeight() - viewport.getScrollBarThickness());
    viewport.setBounds(getLocalBounds());
}


ChainVizComponent::ChainVizComponent(ChainVizTarget* item) :
    item(item)
{
    setSize(200, 100);
}

ChainVizComponent::~ChainVizComponent()
{
}

void ChainVizComponent::paint(Graphics& g)
{
    if (item == nullptr) return;

    g.setColour(PANEL_COLOR);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 4);

    if (BaseItem* i = dynamic_cast<BaseItem*>(item))
    {
        g.setColour(TEXT_COLOR);
        g.setFont(16);
        g.drawText(i->niceName, getLocalBounds().toFloat(), Justification::centred);
    }


}
