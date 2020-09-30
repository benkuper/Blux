/*
  ==============================================================================

    SceneManager.h
    Created: 26 Sep 2020 1:49:54pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Scene.h"

class SceneManager :
    public BaseManager<Scene>
{
public:
    juce_DeclareSingleton(SceneManager, true);

    SceneManager();
    ~SceneManager();

    Factory<Scene> factory;
};