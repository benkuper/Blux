#include "MainComponent.h"

#include "Object/ui/ObjectManagerUI.h"
#include "Object/ui/ObjectManagerGridUI.h"
#include "Interface/ui/InterfaceManagerUI.h"
#include "Group/ui/GroupManagerUI.h"
#include "Scene/ui/SceneManagerUI.h"
#include "Effect/ui/GlobalEffectManagerUI.h"
#include "Sequence/ui/GlobalSequenceManagerUI.h"
#include "Layout/ui/StageLayoutManagerUI.h"
#include "Layout/ui/StageLayout2DView.h"
#include "Interface/interfaces/dmx/ui/DMXChannelView.h"
#include "ChainViz/ChainViz.h"
#include "Color/ColorSource/ui/ColorSourceLibraryGridUI.h"

String getAppVersion();
ApplicationProperties& getAppProperties();
ApplicationCommandManager& getCommandManager();


//==============================================================================
MainComponent::MainComponent()
{
    setSize (800,600);
	getCommandManager().registerAllCommandsForTarget(this);
}

MainComponent::~MainComponent()
{
}

void MainComponent::init()
{
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition(ObjectManager::getInstance()->niceName, &ObjectManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Objects Grid View", &ObjectManagerGridUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Interfaces", &InterfaceManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Groups", &GroupManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Scenes", &SceneManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Global Effects", &GlobalEffectManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Global Sequences", &GlobalSequenceManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Sequence Editor", &TimeMachineView::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Stage Layouts", &StageLayoutManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Stage 2D View", &StageLayout2DView::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Color Source Library", &ColorSourceLibraryGridUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("DMX Channel Tester", &DMXChannelView::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition(ChainViz::panelName, &ChainViz::create));

	//ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Stage 3D View", &Stage3DView::create));
	
	OrganicMainContentComponent::init();

	String lastVersion = getAppProperties().getUserSettings()->getValue("lastVersion", "");

	if (lastVersion != getAppVersion())
	{
		//WelcomeScreen w;
		//DialogWindow::showModalDialog("Welcome", &w, getTopLevelComponent(), Colours::black, true);
	}
}
