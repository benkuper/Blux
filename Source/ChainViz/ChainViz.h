/*
  ==============================================================================

	ChainViz.h
	Created: 30 Oct 2020 4:34:50pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "Scene/SceneManager.h"
#include "ChainVizTarget.h"

class Object;

class ChainVizComponent :
	public Component,
	public Inspectable::InspectableListener
{
public:
	ChainVizComponent(ChainVizTarget * item, Object * o, ChainVizTarget::ChainVizType type);
	virtual ~ChainVizComponent();

	static const Colour typeColors[ChainVizTarget::CHAINVIZ_TYPE_MAX];

	ChainVizTarget * item;
	Object* object;
	ChainVizTarget::ChainVizType type;

	virtual void paint(Graphics& g) override;
	virtual void inspectableDestroyed(Inspectable* i) override;
};


class BaseItemChainVizComponent :
	public ChainVizComponent
{
public:
	BaseItemChainVizComponent(BaseItem* i, Object * o, ChainVizTarget::ChainVizType type);
	virtual ~BaseItemChainVizComponent();

	BaseItem* baseItem;
	std::unique_ptr<BoolImageToggleUI> enableUI;

	void mouseDown(const MouseEvent& e) override;
	virtual void paint(Graphics& g) override;
	virtual void resized() override;

	void inspectableSelectionChanged(Inspectable* i) override;
	virtual void inspectableDestroyed(Inspectable* i) override;
};

class ChainViz :
	public ShapeShifterContentComponent,
	public Inspectable::InspectableListener,
	public ContainerAsyncListener,
	public SceneManager::AsyncSceneListener
{
public:
	ChainViz(const String& name);
	~ChainViz();

	
	Viewport viewport;
	Component container;
	std::unique_ptr<ChainVizComponent> objectStartVizComponent;
	std::unique_ptr<ChainVizComponent> objectEndVizComponent;
	OwnedArray<ChainVizComponent> objectEffectsVizComponents;
	OwnedArray<ChainVizComponent> sceneVizComponents;
	OwnedArray<ChainVizComponent> sequenceVizComponents;
	OwnedArray<ChainVizComponent> groupVizComponents;
	OwnedArray<ChainVizComponent> globalEffectsVizComponents;

	Object* currentObject;
	WeakReference<Inspectable> objectRef;

	Rectangle<int> objectEffectsRect;
	Rectangle<int> sceneRect;
	Rectangle<int> sequenceRect;
	Rectangle<int> groupRect;
	Rectangle<int> globalEffectsRect;

	void clear();
	void setCurrentObject(Object* o);
	void buildChain();
	void rebuildTargetVizComponents(Array<ChainVizTarget *> effectsToAdd, OwnedArray<ChainVizComponent>* arrayToAdd, ChainVizTarget::ChainVizType type);
	
	void paint(Graphics& g) override;
	void resized() override;
	Rectangle<int> placeVizComponents(OwnedArray<ChainVizComponent>* components, Rectangle<int>& r);


	void newMessage(const SceneManager::SceneManagerEvent& e) override;
	void newMessage(const ContainerAsyncEvent& e) override;
	void inspectableDestroyed(Inspectable*) override;

	static ChainViz* create(const String& name) { return new ChainViz(name); }
	static const String panelName;
};