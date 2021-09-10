/*
  ==============================================================================

    StageLayout.h
    Created: 6 Oct 2020 9:37:41pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Object;

class StageLayout :
    public BaseItem
{
public:
    StageLayout();
    ~StageLayout();

    var layoutData;

    void loadLayout();
    void saveLayout();

    Vector3D<float> getObjectPosition(Object * o);

    var getJSONData() override;
    void loadJSONDataItemInternal(var data) override;
};