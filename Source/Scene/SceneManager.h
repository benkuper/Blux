/*
  ==============================================================================

	SceneManager.h
	Created: 26 Sep 2020 1:49:54pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class Object;
class ObjectComponent;

class SceneManager :
	public BaseManager<Scene>,
	public Inspectable::InspectableListener,
	public SceneListener,
	public OSCRemoteControl::RemoteControlListener,
	public Thread
{
public:
	juce_DeclareSingleton(SceneManager, true);

	SceneManager();
	~SceneManager();

	Factory<Scene> factory;

	float loadTime;


	BoolParameter* addLoadToUndo;
	FloatParameter* forceLoadTime;
	

	Scene* previousScene;
	Scene* currentScene;

	StringParameter* previousSceneName;
	StringParameter* currentSceneName;
	StringParameter* nextSceneName;

	Trigger* loadNextSceneTrigger;
	Trigger* loadPreviousSceneTrigger;


	enum PreviewMode { NONE, NEXT, HOVER, SELECTED };
	EnumParameter* previewMode;

	BoolParameter* lockUI;

	void addItemInternal(Scene* s, var data) override;
	void removeItemInternal(Scene* s) override;

	void loadScene(Scene* s, float loadTime = -1, bool setUndoIfNeeded = true);

	Scene* getNextScene();
	Scene* getPreviousScene();

	void run() override;
	//void lerpSceneParams(float weight);

	void askForLoadScene(Scene* s, float time) override;


	Array<ChainVizTarget*> getChainVizTargetsForObjectAndComponent(Object* o, ComponentType t);
	void processComponent(Object* o, ObjectComponent* c, HashMap<Parameter*, var>& values);

	void onContainerTriggerTriggered(Trigger* t) override;
	void onContainerParameterChanged(Parameter* p) override;

	void processMessage(const OSCMessage& m) override;

	void inspectableDestroyed(Inspectable* i) override;

	QueuedNotifier<SceneManagerEvent> sceneManagerNotifier;

	void addAsyncSceneManagerListener(AsyncSceneListener* newListener) { sceneManagerNotifier.addListener(newListener); }
	void addAsyncCoalescedSceneManagerListener(AsyncSceneListener* newListener) { sceneManagerNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncSceneManagerListener(AsyncSceneListener* listener) { sceneManagerNotifier.removeListener(listener); }



	class SceneLoadAction :
		public UndoableAction
	{
	public:
		SceneLoadAction(Scene* prevScene, float prevTime, Scene* s, float time);
		~SceneLoadAction() {}

		Scene* prevScene;
		WeakReference<Inspectable> prevSceneRef;
		float prevTime;

		Scene* scene;
		WeakReference<Inspectable> sceneRef;
		float time;



		bool perform() override;
		bool undo() override;
	};
};