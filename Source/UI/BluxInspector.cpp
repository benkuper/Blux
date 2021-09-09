/*
  ==============================================================================

    BluxInspector.cpp
    Created: 9 Nov 2020 8:12:06am
    Author:  bkupe

  ==============================================================================
*/

#include "BluxInspector.h"
#include "Object/Object.h"
#include "Color/ColorIncludes.h"
#include "Object/Component/components/color/ColorComponent.h"

BluxInspector::BluxInspector(InspectableSelectionManager* s) :
    Inspector(s)
{
}

BluxInspector::~BluxInspector()
{
}

void BluxInspector::setCurrentInspectable(WeakReference<Inspectable> inspectable, bool setInspectableSelection)
{
    if (colorViz != nullptr)
    {
        removeChildComponent(colorViz.get());
        colorViz.reset();
    }

    Inspector::setCurrentInspectable(inspectable, setInspectableSelection);

    if (Object* o = dynamic_cast<Object*>(inspectable.get()))
    {
        if (ColorComponent* c = o->getComponent<ColorComponent>())
        {
            colorViz.reset(new ColorViz(c));
            addAndMakeVisible(colorViz.get());
            resized();
        }
    }
}

void BluxInspector::paint(Graphics& g)
{
    Inspector::paint(g);

    if (colorViz != nullptr)
    {
        g.setColour(BG_COLOR.brighter(.25f));
        Rectangle<float> vr = getLocalBounds().removeFromBottom(200).toFloat();
        g.drawLine(0, vr.getY() - 1, getWidth(), vr.getY() - 1, 1);
    }
   
}

void BluxInspector::resizedInternal(Rectangle<int>& r)
{
    if (colorViz != nullptr)
    {
        Rectangle<int> vr = r.removeFromBottom(200);
        colorViz->setBounds(vr.reduced(8));
    }

    Inspector::resizedInternal(r);
}
