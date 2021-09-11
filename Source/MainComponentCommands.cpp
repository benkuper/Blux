/*
 ==============================================================================

 MainComponentMenuBar.cpp
 Created: 25 Mar 2016 6:02:02pm
 Author:  Martin Hermant

 ==============================================================================
 */


namespace BluxCommandIDs
{
	static const int showAbout = 0x60000;
	static const int gotoWebsite = 0x60001;
	static const int gotoForum = 0x60002;
	static const int gotoDocs = 0x60003;
	static const int postGithubIssue = 0x60004;
	static const int donate = 0x60005;
	static const int showWelcome = 0x60006;
	static const int gotoChangelog = 0x60007;

	static const int guideStart = 0x300; //up to 0x300 +100
	static const int exitGuide = 0x399; 
	static const int reloadObjectDefinitions = 0x501;
	static const int downloadObjectsDefinitions = 0x502;

	static const int flashSelected = 0x700;
	static const int loadNextScene = 0x800;
	static const int loadPreviousScene = 0x801;
	static const int saveCurrentScene = 0x802;

}

void MainComponent::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) 
{
	if (commandID >= BluxCommandIDs::guideStart && commandID < BluxCommandIDs::guideStart + 99)
	{
		//result.setInfo(Guider::getInstance()->getGuideName(commandID - BluxCommandIDs::guideStart), "", "Guides", result.readOnlyInKeyEditor);
		return;
	}

	switch (commandID)
	{
	case BluxCommandIDs::showAbout:
		result.setInfo("About...", "", "General", result.readOnlyInKeyEditor);
		break;

	case BluxCommandIDs::showWelcome:
		result.setInfo("Show Welcome Screen...", "", "General", result.readOnlyInKeyEditor);
		break;

	case BluxCommandIDs::donate:
		result.setInfo("Be cool and donate", "", "General", result.readOnlyInKeyEditor);
		break;

	case BluxCommandIDs::gotoWebsite:
		result.setInfo("Go to website", "", "Help", result.readOnlyInKeyEditor);
		break;
	case BluxCommandIDs::gotoForum:
		result.setInfo("Go to forum", "", "Help", result.readOnlyInKeyEditor);
		break;

	case BluxCommandIDs::gotoDocs:
		result.setInfo("Go to the Amazing Documentation", "", "Help", result.readOnlyInKeyEditor);
		break;

	case BluxCommandIDs::gotoChangelog:
		result.setInfo("See the changelog", "", "Help", result.readOnlyInKeyEditor);
		break;


	case BluxCommandIDs::postGithubIssue:
		result.setInfo("Post an issue on github", "", "Help", result.readOnlyInKeyEditor);
		break;

	
	case BluxCommandIDs::reloadObjectDefinitions:
		result.setInfo("Reload Object Definitions", "", "General", result.readOnlyInKeyEditor);
		break;

	case BluxCommandIDs::downloadObjectsDefinitions:
		result.setInfo("Download Object Definitions", "", "General", result.readOnlyInKeyEditor);
		break;

	case BluxCommandIDs::flashSelected:
		result.setInfo("Flash selected Objects", "", "Blux", 0);
		result.addDefaultKeypress(KeyPress::createFromDescription("f").getKeyCode(), ModifierKeys::noModifiers);
		break;

	case BluxCommandIDs::loadNextScene:
		result.setInfo("Load Next Scene", "", "Blux", 0);
		result.addDefaultKeypress(KeyPress::createFromDescription(" ").getKeyCode(), ModifierKeys::commandModifier);
		break;

	case BluxCommandIDs::loadPreviousScene:
		result.setInfo("Load Previous Scene", "", "Blux", 0);
		result.addDefaultKeypress(KeyPress::createFromDescription(" ").getKeyCode(), ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
		break;

	case BluxCommandIDs::saveCurrentScene:
		result.setInfo("Save Current Scene", "", "Blux", 0);
		result.addDefaultKeypress(KeyPress::createFromDescription("m").getKeyCode(), ModifierKeys::commandModifier);
		break;


	case BluxCommandIDs::exitGuide:
		result.setInfo("Exit current guide", "", "Guides", result.readOnlyInKeyEditor);
		result.addDefaultKeypress(KeyPress::escapeKey, ModifierKeys::noModifiers);
		result.setActive(false);// Guider::getInstance()->guide != nullptr);
		break;
		

	default:
		OrganicMainContentComponent::getCommandInfo(commandID, result);
		break;
	}
}



void MainComponent::getAllCommands(Array<CommandID>& commands) {

	OrganicMainContentComponent::getAllCommands(commands);

	const CommandID ids[] = {

		BluxCommandIDs::showAbout,
		BluxCommandIDs::showWelcome,
		BluxCommandIDs::donate,
		BluxCommandIDs::gotoWebsite,
		BluxCommandIDs::gotoForum,
		BluxCommandIDs::gotoDocs,
		BluxCommandIDs::gotoChangelog,
		BluxCommandIDs::postGithubIssue,
		//BluxCommandIDs::goToCommunityModules,
		BluxCommandIDs::reloadObjectDefinitions,
		BluxCommandIDs::downloadObjectsDefinitions,
		BluxCommandIDs::exitGuide,
		BluxCommandIDs::flashSelected,
		BluxCommandIDs::loadNextScene,
		BluxCommandIDs::loadPreviousScene,
		BluxCommandIDs::saveCurrentScene
	};

	commands.addArray(ids, numElementsInArray(ids));
	//for (int i = 0; i < Guider::getInstance()->factory.defs.size(); ++i) commands.add(BluxCommandIDs::guideStart + i);
}


PopupMenu MainComponent::getMenuForIndex(int topLevelMenuIndex, const String& menuName) 
{
	PopupMenu menu = OrganicMainContentComponent::getMenuForIndex(topLevelMenuIndex, menuName);

	if (menuName == "Help")
	{
		menu.addCommandItem(&getCommandManager(), BluxCommandIDs::showAbout);
		menu.addCommandItem(&getCommandManager(), BluxCommandIDs::showWelcome);
		menu.addCommandItem(&getCommandManager(), BluxCommandIDs::donate);
		menu.addSeparator();
		menu.addCommandItem(&getCommandManager(), BluxCommandIDs::gotoWebsite);
		menu.addCommandItem(&getCommandManager(), BluxCommandIDs::gotoForum);
		menu.addCommandItem(&getCommandManager(), BluxCommandIDs::gotoDocs);
		menu.addCommandItem(&getCommandManager(), BluxCommandIDs::gotoChangelog);
		menu.addCommandItem(&getCommandManager(), BluxCommandIDs::postGithubIssue);

	}else if (menuName == "Guides")
	{
	/*	for (int i = 0; i < Guider::getInstance()->factory.defs.size(); ++i)
		{
			menu.addCommandItem(&getCommandManager(), BluxCommandIDs::guideStart + i);
		}

		menu.addSeparator();
		menu.addCommandItem(&getCommandManager(), BluxCommandIDs::exitGuide);
		*/
	}
	else if (menuName == "Object")
	{
		menu.addCommandItem(&getCommandManager(), BluxCommandIDs::flashSelected);
	}
	else if (menuName == "Scene")
	{
		menu.addCommandItem(&getCommandManager(), BluxCommandIDs::loadPreviousScene);
		menu.addCommandItem(&getCommandManager(), BluxCommandIDs::loadNextScene);
		menu.addSeparator();
		menu.addCommandItem(&getCommandManager(), BluxCommandIDs::saveCurrentScene);
	}

	return menu;
}

void MainComponent::fillFileMenuInternal(PopupMenu & menu)
{
	//menu.addCommandItem(&getCommandManager(), BluxCommandIDs::goToCommunityModules);
	menu.addCommandItem(&getCommandManager(), BluxCommandIDs::reloadObjectDefinitions);
	menu.addCommandItem(&getCommandManager(), BluxCommandIDs::downloadObjectsDefinitions);
}

bool MainComponent::perform(const InvocationInfo& info)
{

	//if (info.commandID >= BluxCommandIDs::guideStart && info.commandID < BluxCommandIDs::guideStart + 100)
	//{
	//	Guider::getInstance()->launchGuideAtIndex(info.commandID - BluxCommandIDs::guideStart);
	//	return true;
	//}

	switch (info.commandID)
	{


	//case BluxCommandIDs::showAbout:
	//{
	//	AboutWindow w;
	//	DialogWindow::showModalDialog("About", &w, getTopLevelComponent(), Colours::transparentBlack, true);
	//}
	//break;

	//case BluxCommandIDs::showWelcome:
	//{
	//	WelcomeScreen w;
	//	DialogWindow::showModalDialog("Welcome", &w, getTopLevelComponent(), Colours::black, true);
	//}
	//break;

	case BluxCommandIDs::donate:
		URL("https://www.paypal.me/benkuper").launchInDefaultBrowser();
		break;

	case BluxCommandIDs::gotoWebsite:
		URL("http://benjamin.kuperberg.fr/blux").launchInDefaultBrowser();
		break;

	case BluxCommandIDs::gotoForum:
		URL("http://benjamin.kuperberg.fr/blux/forum").launchInDefaultBrowser();
		break;

	case BluxCommandIDs::gotoDocs:
		URL("https://benjamin.kuperberg.fr/blux/docs").launchInDefaultBrowser();
		break;

	case BluxCommandIDs::gotoChangelog:
		URL("https://benjamin.kuperberg.fr/blux/releases/changelog.html").launchInDefaultBrowser();
		break;

	case BluxCommandIDs::postGithubIssue:
		URL("http://github.com/benkuper/Blux/issues").launchInDefaultBrowser();
		break;

	case BluxCommandIDs::flashSelected:

		break;

	case BluxCommandIDs::reloadObjectDefinitions:
		ObjectManager::getInstance()->updateFactoryDefinitions();
		break;

	case BluxCommandIDs::downloadObjectsDefinitions:
		ObjectManager::getInstance()->downloadObjects();
		break;

	case BluxCommandIDs::loadNextScene:
		SceneManager::getInstance()->loadNextSceneTrigger->trigger();
		break;

	case BluxCommandIDs::loadPreviousScene:
		SceneManager::getInstance()->loadPreviousSceneTrigger->trigger();
		break;

	case BluxCommandIDs::saveCurrentScene:
		if (SceneManager::getInstance()->currentScene != nullptr)
		{
			SceneManager::getInstance()->currentScene->saveScene();
		}
		break;

	//case BluxCommandIDs::exitGuide:
	//	Guider::getInstance()->setCurrentGuide(nullptr);
	//	break;

	default:
		return OrganicMainContentComponent::perform(info);
	}

	return true;
}

StringArray MainComponent::getMenuBarNames()
{
	StringArray names = OrganicMainContentComponent::getMenuBarNames();
	names.add("Object");
	names.add("Scene");
	//names.add("Guides");
	names.add("Help");
	return names;
}
