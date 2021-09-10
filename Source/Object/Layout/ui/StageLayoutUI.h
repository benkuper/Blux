/*
  ==============================================================================

    StageLayoutUI.h
    Created: 9 Oct 2020 8:53:10pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class StageLayoutUI :
    public BaseItemUI<StageLayout>
{
public:
    StageLayoutUI(StageLayout* item);
    ~StageLayoutUI();

    void mouseDown(const MouseEvent& e) override;

};