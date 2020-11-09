/*
  ==============================================================================

    PixelShapeChooser.h
    Created: 8 Nov 2020 10:06:49am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../PixelShape.h"

class PixelShapeChooser :
    public TextButton
{
public:
    PixelShapeChooser();
    ~PixelShapeChooser();

    void clicked() override;

    class ChooserListener
    {
    public:
        /** Destructor. */
        virtual ~ChooserListener() {}
        virtual void shapeChosen(const String &type) {}
    };

    ListenerList<ChooserListener> chooserListeners;
    void addChooserListener(ChooserListener* newListener) { chooserListeners.add(newListener); }
    void removeChooserListener(ChooserListener* listener) { chooserListeners.remove(listener); }
};
