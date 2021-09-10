/*
  ==============================================================================

	LayoutFilterStageViewUI.cpp
	Created: 7 Oct 2020 9:06:54am
	Author:  bkupe

  ==============================================================================
*/

LayoutFilterStageViewUI::LayoutFilterStageViewUI(LayoutFilter* f) :
	filter(f),
	filterRef(f),
	queuedNotifier(5)
{
	setSize(20, 20);

	handle.color = filter->colorInView->getColor();
	addAndMakeVisible(&handle);

	setVisible(filter->enableInView->boolValue());

	filter->addInspectableListener(this);
	filter->addAsyncContainerListener(this);
}

LayoutFilterStageViewUI::~LayoutFilterStageViewUI()
{
	if (!filterRef.wasObjectDeleted())
	{
		filter->removeInspectableListener(this);
		filter->removeAsyncContainerListener(this);
	}
}

void LayoutFilterStageViewUI::paint(Graphics& g)
{
	if (filterRef.wasObjectDeleted()) return;

	Colour c = filter->colorInView->getColor();


	ColourGradient gradient;
	for (int i = 0; i <= 10; i++) gradient.addColour(i / 10.f, c.withAlpha(jlimit<float>(0,1,filter->fadeCurve.getValueAtNormalizedPosition(i / 10.f))));

	LayoutFilter::LayoutMode m = filter->mode->getValueDataAsEnum<LayoutFilter::LayoutMode>();

	switch (m)
	{
	case LayoutFilter::AXIS_X: 
		gradient.point1 = Point<float>(0, 0);
		gradient.point2 = Point<float>(getWidth(), 0);
	break;
	case LayoutFilter::AXIS_Z:
		gradient.point1 = Point<float>(0, 0);
		gradient.point2 = Point<float>(0, getHeight());
	case LayoutFilter::AXIS_Y: break;
	case LayoutFilter::RADIUS:
		gradient.isRadial = true;
		gradient.point1 = getLocalBounds().getCentre().toFloat();
		gradient.point2 = getLocalBounds().getCentre().withX(getWidth()).toFloat();
		break;
	}

	gradient.multiplyOpacity(.3f);
	g.setGradientFill(gradient);
	g.fillRect(getLocalBounds());
}

void LayoutFilterStageViewUI::resized()
{
	Vector3D<float> pos = filter->position->getVector();
	Rectangle<float> r = Rectangle<float>().withSize(.2f, .2f).withCentre(Point<float>(pos.x, pos.z));
	handle.setBounds(getBoundsInView(r));
}

Rectangle<int> LayoutFilterStageViewUI::getBoundsInView(Rectangle<float> r)
{
	Rectangle<float> relBounds = (r - boundsInView.getTopLeft()) / Point<float>(boundsInView.getWidth(), boundsInView.getHeight());
	return getLocalBounds().getProportion(relBounds);
}

void LayoutFilterStageViewUI::newMessage(const ContainerAsyncEvent& e)
{
	switch (e.type)
	{
	case ContainerAsyncEvent::ControllableFeedbackUpdate:
	{
		if (e.targetControllable == filter->enableInView)
		{
			setVisible(filter->enableInView->boolValue());
		}
		else if (e.targetControllable == filter->colorInView)
		{
			handle.color = filter->colorInView->getColor();
			handle.repaint();
		}
    default:
        break;
	}
	break;
	}
}

void LayoutFilterStageViewUI::inspectableDestroyed(Inspectable* i)
{
	queuedNotifier.addMessage(new FilterEvent(FilterEvent::FILTER_REMOVED, this));
}

LayoutFilterStageViewUI::Handle::Handle()
{
}

LayoutFilterStageViewUI::Handle::~Handle()
{
}

void LayoutFilterStageViewUI::Handle::paint(Graphics& g)
{
	g.setColour(color.withAlpha(.2f));
	g.fillEllipse(getLocalBounds().toFloat());
	g.setColour(color.withAlpha(.6f));
	g.drawEllipse(getLocalBounds().toFloat(), 1);
}
