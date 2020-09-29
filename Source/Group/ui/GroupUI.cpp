/*
  ==============================================================================

    GroupUI.cpp
    Created: 29 Sep 2020 9:46:46pm
    Author:  bkupe

  ==============================================================================
*/

#include "GroupUI.h"
#include "Object/Object.h"
#include "../groups/object/ObjectGroup.h"

GroupUI::GroupUI(Group* item) :
    BaseItemUI(item)
{
    acceptedDropTypes.add("Object");
}

GroupUI::~GroupUI()
{
}

void GroupUI::itemDropped(const DragAndDropTarget::SourceDetails& details)
{

	String dataType = details.description.getProperty("dataType", "");
	if (dataType == "Object")
	{
		
		if (BaseItemMinimalUI<Object> * bi = static_cast<BaseItemMinimalUI<Object> *>(details.sourceComponent.get()))
		{
			if (Object* o = dynamic_cast<Object*>(bi->item))
			{
				if (ObjectGroup* og = dynamic_cast<ObjectGroup*>(item))
				{
					og->addObject(o);
				}
			}
		}
	
	}
	else
	{
		LOG("Drop not supported for type : " << dataType);
	}
}
