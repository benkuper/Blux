/*
  ==============================================================================

    ColorSourceChooser.h
    Created: 8 Nov 2020 10:04:28am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ColorSourceChooser :
    public TextButton
{
public:
    ColorSourceChooser();
    ~ColorSourceChooser();

    void clicked() override;
    
    class ChooserListener
    {
    public:
        /** Destructor. */
        virtual ~ChooserListener() {}
        virtual void sourceChosen(const String &type, ColorSource * templateRef) {}
    };

    ListenerList<ChooserListener> chooserListeners;
    void addChooserListener(ChooserListener* newListener) { chooserListeners.add(newListener); }
    void removeChooserListener(ChooserListener* listener) { chooserListeners.remove(listener); }

};


class ColorSourceMenu :
    public PopupMenu
{
public:
    ColorSourceMenu();
    ~ColorSourceMenu() {}
};