/*
  ==============================================================================

    ObjectEditor.cpp
    Created: 8 Nov 2020 11:17:00pm
    Author:  bkupe

  ==============================================================================
*/

#include "ObjectEditor.h"
#include "Color/ColorSource/ui/ColorSourceViz.h"
#include "../Component/components/color/ColorComponent.h"

ObjectEditor::ObjectEditor(Object* o, bool isRoot) :
    BaseItemEditor(o, isRoot),
    object(o)
{
   /*
   if (isRoot)
    {
        if (ColorComponent* c = o->getComponent<ColorComponent>())
        {
            viz.reset(new ColorSourceViz(c));
            addAndMakeVisible(viz.get());
        }
    }
    */
}

ObjectEditor::~ObjectEditor()
{
}

/*
void ObjectEditor::resizedInternalContent(Rectangle<int>& r)
{
    if (viz != nullptr)
    {
        viz->setBounds(r.removeFromBottom(150));
        r.removeFromBottom(8);
    }
    

    BaseItemEditor::resizedInternalContent(r);
}
*/