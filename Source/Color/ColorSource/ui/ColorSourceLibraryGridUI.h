/*
  ==============================================================================

    ColorSourceLibraryGridUI.h
    Created: 3 Nov 2020 11:21:01am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ColorSourceLibraryGridUI :
    public BaseManagerShapeShifterUI<ColorSourceLibrary, ColorSource, ColorSourceGridUI>,
    public ContainerAsyncListener
{
public:
    ColorSourceLibraryGridUI(const String& name);
    ~ColorSourceLibraryGridUI();

    std::unique_ptr<IntSliderUI> thumbSizeUI;

    void resizedInternalHeader(Rectangle<int>& r) override;
    void resizedInternalContent(Rectangle<int> &r) override;

    void newMessage(const ContainerAsyncEvent& e) override;


    static ColorSourceLibraryGridUI* create(const String& name) { return new ColorSourceLibraryGridUI(name); }
};