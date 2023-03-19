/*
  ==============================================================================

    Viz.cpp
    Created: 5 Apr 2022 10:41:47am
    Author:  bkupe

  ==============================================================================
*/

#include "Viz.h"

VizPanel::VizPanel(StringRef name) :
    ShapeShifterContentComponent(name)
#if JUCE_WINDOWS
    , web(WebBrowserComponent::Options().withBackend(WebBrowserComponent::Options::Backend::webview2).withWinWebView2Options(
        WebBrowserComponent::Options::WinWebView2().withUserDataFolder(File::getSpecialLocation(File::windowsLocalAppData).getChildFile(ProjectInfo::projectName)).withStatusBarDisabled()))
#endif
{
    web.goToURL("http://127.0.0.1:6080");
    addAndMakeVisible(web);
}

void VizPanel::resized()
{
    web.setBounds(getLocalBounds());
}
