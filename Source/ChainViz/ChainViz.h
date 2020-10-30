/*
  ==============================================================================

	ChainViz.h
	Created: 30 Oct 2020 4:34:50pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
class Object;

class ChainVizTarget
{
public:
	
	virtual float getChainVizIntensity() { return 0; }
	virtual Colour getChainVizColor() { return Colours::black;  }

};

class ChainVizComponent :
	public Component
{
public:
	ChainVizComponent(ChainVizTarget * item);
	~ChainVizComponent();

	ChainVizTarget * item;
	std::unique_ptr<BoolToggleUI> enableUI;

	void paint(Graphics& g) override;
};

class ChainViz :
	public ShapeShifterContentComponent
{
public:
	ChainViz(const String& name);
	~ChainViz();

	Viewport viewport;
	Component container;
	OwnedArray<ChainVizComponent> vizComponents;

	Object* currentObject;
	WeakReference<Inspectable> objectRef;

	void clear();
	void setCurrentObject(Object* o);
	void buildChain();

	void resized() override;

	static ChainViz* create(const String& name) { return new ChainViz(name); }
	static const String panelName;
};