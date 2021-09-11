/*
  ==============================================================================

	ChainViz.h
	Created: 30 Oct 2020 4:34:50pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "ChainVizTarget.h"
#include "Common/CommonIncludes.h"

class Object;

class ChainVizComponent :
	public Component,
	public Inspectable::InspectableListener
{
public:
	ChainVizComponent(ChainVizTarget * item, Object * o, ChainVizTarget::ChainVizType type);
	virtual ~ChainVizComponent();

	static const Colour typeColors[ChainVizTarget::CHAINVIZ_TYPE_MAX];

	bool transparentBG;

	ChainVizTarget * item;
	Object* object;
	ChainVizTarget::ChainVizType type;

	virtual bool isReallyAffecting() { return true; }

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
	bool showItemName;
	std::unique_ptr<BoolToggleUI> enableUI;

	void mouseDown(const MouseEvent& e) override;
	virtual void paint(Graphics& g) override;
	virtual void resized() override;

	virtual String getVizLabel() const { return baseItem->niceName; }

	void inspectableSelectionChanged(Inspectable* i) override;
	virtual void inspectableDestroyed(Inspectable* i) override;
};

class ChainViz :
	public ShapeShifterContentComponent,
	public Inspectable::InspectableListener,
	public ContainerAsyncListener,
	public AsyncSceneListener,
	public ComponentListener,
	public Parameter::AsyncListener
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
	BoolParameter showOnlyActives;
	std::unique_ptr<BoolButtonToggleUI> showOnlyActivesUI;

	Rectangle<int> objectEffectsRect;
	Rectangle<int> sceneRect;
	Rectangle<int> sequenceRect;
	Rectangle<int> groupRect;
	Rectangle<int> globalEffectsRect;

	bool resizing;

	void clear();
	void setCurrentObject(Object* o);
	void buildChain();
	void rebuildTargetVizComponents(Array<ChainVizTarget *> effectsToAdd, OwnedArray<ChainVizComponent>* arrayToAdd, ChainVizTarget::ChainVizType type);
	void removeVizComponent(ChainVizComponent* c);

	void paint(Graphics& g) override;
	void resized() override;
	Rectangle<int> placeVizComponents(OwnedArray<ChainVizComponent>* components, Rectangle<int>& r);


	void newMessage(const SceneManagerEvent& e) override;
	void newMessage(const ContainerAsyncEvent& e) override;
	void newMessage(const Parameter::ParameterEvent& e) override;
	void inspectableDestroyed(Inspectable*) override;

	void componentMovedOrResized(Component & c, bool wasMoved, bool wasResized) override;

	static ChainViz* create(const String& name) { return new ChainViz(name); }
	static const String panelName;
};