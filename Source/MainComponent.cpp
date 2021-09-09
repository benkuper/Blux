#include "MainComponent.h"

#include "Object/ui/ObjectManagerUI.h"
#include "Object/ui/ObjectManagerGridUI.h"
#include "Interface/ui/InterfaceManagerUI.h"
#include "Object/Group/ui/GroupManagerUI.h"
#include "Scene/ui/SceneManagerUI.h"
#include "Effect/ui/GlobalEffectManagerUI.h"
#include "Sequence/ui/GlobalSequenceManagerUI.h"
#include "Object/Layout/ui/StageLayoutManagerUI.h"
#include "Object/Layout/ui/StageLayout2DView.h"
#include "Interface/interfaces/dmx/ui/DMXChannelView.h"
#include "ChainViz/ChainViz.h"
#include "Color/ColorSource/ui/ColorSourceLibraryGridUI.h"
#include "UI/BluxInspector.h"

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
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition(InterfaceManager::getInstance()->niceName, &InterfaceManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition(GroupManager::getInstance()->niceName, &GroupManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition(SceneManager::getInstance()->niceName, &SceneManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition(GlobalEffectManager::getInstance()->niceName, &GlobalEffectManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition(GlobalSequenceManager::getInstance()->niceName, &GlobalSequenceManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Sequence Editor", &TimeMachineView::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition(StageLayoutManager::getInstance()->niceName, &StageLayoutManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Stage 2D View", &StageLayout2DView::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition(ColorSourceLibrary::getInstance()->niceName, &ColorSourceLibraryGridUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("DMX Channel Tester", &DMXChannelView::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition(ChainViz::panelName, &ChainViz::create));

	InspectorUI::customCreateInspectorFunc = &MainComponent::createBluxInspector;


	OrganicMainContentComponent::init();

	String lastVersion = getAppProperties().getUserSettings()->getValue("lastVersion", "");

	if (lastVersion != getAppVersion())
	{
		//WelcomeScreen w;
		//DialogWindow::showModalDialog("Welcome", &w, getTopLevelComponent(), Colours::black, true);
	}
}

Inspector* MainComponent::createBluxInspector(InspectableSelectionManager* s)
{
	return new BluxInspector(s);
}
