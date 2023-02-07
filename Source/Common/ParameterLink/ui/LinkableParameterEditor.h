/*
  ==============================================================================

    LinkableParameterEditor.h
    Created: 21 Dec 2020 11:12:32pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class LinkableParameterEditor :
    public InspectableEditor,
    public Button::Listener,
    public ParameterLink::AsyncListener
{
public:
    LinkableParameterEditor(ParameterLink* pLink);
    ~LinkableParameterEditor();

    static Image linkImage;

    ParameterLink* link;
    std::unique_ptr<ParameterEditor> paramEditor;
    std::unique_ptr<ImageButton> linkBT;

    Rectangle<int> btRect;

    void paint(Graphics& g) override;
    void resized() override;
    void buttonClicked(Button* b) override;
    void childBoundsChanged(Component* c) override;

    String getLinkLabel() const;

    void newMessage(const ParameterLink::ParameterLinkEvent& e) override;

};

class ParamLinkContainerEditor :
    public GenericControllableContainerEditor
{
public:
    ParamLinkContainerEditor(ParamLinkContainer* container, bool isRoot, bool buildAtCreation = true);
    ~ParamLinkContainerEditor();

    ParamLinkContainer* paramLinkContainer;
    bool showLinkEditor;

    virtual InspectableEditor* getEditorUIForControllable(Controllable* c) override;
};
