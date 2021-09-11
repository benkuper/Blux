/*
  ==============================================================================

    ObjectIncludes.cpp
    Created: 9 Sep 2021 7:02:53pm
    Author:  bkupe

  ==============================================================================
*/

#include "ObjectIncludes.h"

#include "MainIncludes.h"
#include "Interface/InterfaceIncludes.h"

#include "Effect/EffectIncludes.h"
#include "Scene/SceneIncludes.h"
#include "Sequence/SequenceIncludes.h"

#include "actions/ObjectAction.cpp"

#include "Component/ObjectComponent.cpp"
#include "Component/ComponentManager.cpp"
#include "Component/components/color/ColorComponent.cpp"
#include "Component/components/color/ui/ColorComponentEditor.cpp"
#include "Component/components/gobo/GoboComponent.cpp"
#include "Component/components/intensity/IntensityComponent.cpp"
#include "Component/components/laser/LaserComponent.cpp"
#include "Component/components/laser/LaserPattern/LaserPattern.cpp"
#include "Component/components/pantilt/PanTiltComponent.cpp"
#include "Component/components/script/ScriptComponent.cpp"
#include "Component/components/script/ui/ScriptComponentEditor.cpp"
#include "Component/components/servo/ServoComponent.cpp"
#include "Component/components/stepper/StepperComponent.cpp"
#include "Component/components/strobe/StrobeComponent.cpp"
#include "Component/ui/ComponentManagerEditor.cpp"
#include "Component/ui/ObjectComponentEditor.cpp"

#include "Group/Group.cpp"
#include "Group/GroupManager.cpp"
#include "Group/groups/object/ObjectGroup.cpp"
#include "Group/groups/object/ui/ObjectGroupUI.cpp"
#include "Group/groups/virtual/VirtualGroup.cpp"
#include "Group/ui/GroupManagerUI.cpp"
#include "Group/ui/GroupUI.cpp"

#include "Layout/StageLayout.cpp"
#include "Layout/StageLayoutManager.cpp"
#include "Layout/ui/Object2DView.cpp"
#include "Layout/ui/StageLayout2DView.cpp"
#include "Layout/ui/StageLayoutManagerUI.cpp"
#include "Layout/ui/StageLayoutUI.cpp"

#include "Object.cpp"
#include "ObjectManager.cpp"
#include "ui/ObjectChainVizUI.cpp"
#include "ui/ObjectGridUI.cpp"
#include "ui/ObjectManagerGridUI.cpp"
#include "ui/ObjectManagerUI.cpp"
#include "ui/ObjectUI.cpp"