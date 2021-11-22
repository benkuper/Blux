/*
  ==============================================================================

    SceneHelpers.h
    Created: 9 Oct 2020 10:10:15am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class SceneHelpers
{
public:
    static var getParamsSceneData(ControllableContainer * i, Array<Parameter *> excludeParams = Array<Parameter *>(), bool recursive = false);
    static void lerpSceneParams(ControllableContainer * i, var startData, var endData, float weight, bool recursive = false);
    static void lerpSceneParam(Parameter * p, var startData, var endData, float weight);
};