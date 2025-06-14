/*
  ==============================================================================

    ObjectManagerUI.h
    Created: 26 Sep 2020 7:51:25pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ObjectManagerUI :
    public ManagerShapeShifterUI<ObjectManager, Object, ObjectUI>
{
public:
    ObjectManagerUI(const String & name);
    ~ObjectManagerUI();

    static ObjectManagerUI* create(const String& name) { return new ObjectManagerUI(name); }
};
