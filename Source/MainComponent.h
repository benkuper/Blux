#pragma once

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

ApplicationProperties& getAppProperties();
ApplicationCommandManager& getCommandManager();

class MainComponent  : public OrganicMainContentComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    void init() override;

    static Inspector* createBluxInspector(InspectableSelectionManager* s);

private:
    //==============================================================================
    // Your private member variables go here...
    void getAllCommands(Array<CommandID>& commands) override;
    virtual void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
    virtual bool perform(const InvocationInfo& info) override;
    StringArray getMenuBarNames() override;
    virtual PopupMenu getMenuForIndex(int topLevelMenuIndex, const String& menuName) override;
    void fillFileMenuInternal(PopupMenu& menu) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
