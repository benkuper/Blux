/*
  ==============================================================================

    AssetManager.h
    Created: 26 Sep 2020 2:38:52pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class BluxAssetManager
{
public:
    static Image getImage(StringRef name, StringRef extension = "png")
    {
        int size = 0;
        const char* data = BinaryData::getNamedResource((name + "_" + extension).getCharPointer(), size);
        return ImageCache::getFromMemory(data, size);
    }
};
