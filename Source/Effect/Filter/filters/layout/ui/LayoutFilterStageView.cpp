/*
  ==============================================================================

    LayoutFilterStageView.cpp
    Created: 7 Oct 2020 9:09:00am
    Author:  bkupe

  ==============================================================================
*/

LayoutFilterStageView::LayoutFilterStageView() :
    shouldRepaint(false)
{
    LayoutFilter::broadcaster.addChangeListener(this);
    for (auto& f : LayoutFilter::instances)
    {
        addUIForFilter(f);
    }

    addMouseListener(this, true);
}

LayoutFilterStageView::~LayoutFilterStageView()
{
    LayoutFilter::broadcaster.removeChangeListener(this);
    while (filterUIs.size()) removeUIForFilter(filterUIs[0]->filter);
}

void LayoutFilterStageView::setBoundsInView(Rectangle<float> r)
{
    boundsInView = r;
    placeItems();
}

void LayoutFilterStageView::addUIForFilter(LayoutFilter* f)
{
    LayoutFilterStageViewUI* ui = new LayoutFilterStageViewUI(f);
    ui->addAsyncLayoutFilterListener(this);
    f->addAsyncContainerListener(this);
    addChildComponent(ui);
    filterUIs.add(ui);

    bool locked = StageLayoutManager::getInstance()->lockFilterUIs->boolValue();
    ui->setInterceptsMouseClicks(!locked, !locked);

    if (ui->isVisible()) placeItem(ui);
}

void LayoutFilterStageView::removeUIForFilter(LayoutFilter* f)
{
    LayoutFilterStageViewUI* ui = getUIForFilter(f);
    if (ui != nullptr)  removeFilterUI(ui);    
}

void LayoutFilterStageView::removeFilterUI(LayoutFilterStageViewUI* ui)
{
    if (ui->filter != nullptr) ui->filter->removeAsyncContainerListener(this);
   
    removeChildComponent(ui);
    filterUIs.removeObject(ui);

}

LayoutFilterStageViewUI* LayoutFilterStageView::getUIForFilter(LayoutFilter* f)
{
    for (auto& ui : filterUIs) if (f == ui->filter) return ui;
    return nullptr;
}

void LayoutFilterStageView::paint(Graphics& g)
{
    //g.fillAll(Colours::purple.withAlpha(.1f));
    for (auto& ui : filterUIs)
    {
        //Colour c = ui->filter->colorInView->getColor();
        LayoutFilter::LayoutMode m = ui->filter->mode->getValueDataAsEnum<LayoutFilter::LayoutMode>();
        switch (m)
        {
            case LayoutFilter::RADIUS:
                //Rectangle<int> r = getBounds();
            break;
                
            default:
                break;
        }
    }
}

void LayoutFilterStageView::placeItems()
{
    for (auto& ui : filterUIs) placeItem(ui);
}

void LayoutFilterStageView::placeItem(LayoutFilterStageViewUI* ui)
{
    if (ui == nullptr) return;
    if (!ui->isVisible()) return;

    Vector3D<float> pos = ui->filter->position->getVector();
    float size = ui->filter->size->floatValue() * 2;
    Rectangle<float> filterBounds = Rectangle<float>().withSize(size , size).withCentre(Point<float>(pos.x, pos.z));
    
    Rectangle<float> relBounds = (filterBounds - boundsInView.getTopLeft()) / Point<float>(boundsInView.getWidth(), boundsInView.getHeight());

    Rectangle<int> viewBounds =  getLocalBounds().getProportion(relBounds);

    LayoutFilter::LayoutMode m = ui->filter->mode->getValueDataAsEnum<LayoutFilter::LayoutMode>();

    switch (m)
    {
    case LayoutFilter::AXIS_X: viewBounds = viewBounds.withTop(0).withBottom(getHeight()); break;
    case LayoutFilter::AXIS_Z: viewBounds = viewBounds.withLeft(0).withRight(getWidth()); break;
    case LayoutFilter::AXIS_Y: 
    case LayoutFilter::RADIUS:
        break;
    }

    Point<float> viewSize = Point<float>(getWidth(), getHeight());
    Rectangle<float> relViewBounds = Rectangle<float>(viewBounds.getTopLeft().toFloat() / viewSize, viewBounds.getBottomRight().toFloat() / viewSize);
    Rectangle<float> filterBoundsInView(boundsInView.getProportion(relViewBounds));

    ui->boundsInView = filterBoundsInView;
    ui->setBounds(viewBounds);
    ui->resized();
}

void LayoutFilterStageView::mouseDown(const MouseEvent& e)
{
    if (LayoutFilterStageViewUI* ui = dynamic_cast<LayoutFilterStageViewUI*>(e.eventComponent->getParentComponent()))
    {
        if(e.eventComponent == &ui->handle) ui->posAtMouseDown = ui->filter->position->getVector();
    }
}

void LayoutFilterStageView::mouseDrag(const MouseEvent& e)
{
    if (LayoutFilterStageViewUI* ui = dynamic_cast<LayoutFilterStageViewUI*>(e.eventComponent->getParentComponent()))
    {
        if (e.eventComponent == &ui->handle)
        {

            Point<float> viewPoint(e.getOffsetFromDragStart().x * boundsInView.getWidth() / getWidth(), e.getOffsetFromDragStart().y * boundsInView.getHeight() / getHeight());
            ui->filter->position->setVector(ui->posAtMouseDown.x + viewPoint.x, ui->filter->position->y, ui->posAtMouseDown.z + viewPoint.y);
        }
    }
}

void LayoutFilterStageView::mouseUp(const MouseEvent& e)
{
    if (LayoutFilterStageViewUI* ui = dynamic_cast<LayoutFilterStageViewUI*>(e.eventComponent->getParentComponent()))
    {
        if (e.eventComponent == &ui->handle) ui->filter->position->setUndoableVector(ui->filter->position->getVector());
    }
}

bool LayoutFilterStageView::hitTest(int x, int y)
{
    for (auto& ui : filterUIs)
    {
        Rectangle<int> b = getLocalArea(&ui->handle, ui->handle.getLocalBounds());
        if (b.contains(x,y)) return true;
    }

    return false;
}

void LayoutFilterStageView::newMessage(const ContainerAsyncEvent& e)
{
    if (Engine::mainEngine->isLoadingFile) return;

    switch (e.type)
    {
    case ContainerAsyncEvent::ControllableFeedbackUpdate:
        if (LayoutFilter* f = e.targetControllable->getParentAs<LayoutFilter>())
        {
            if (e.targetControllable == f->position || e.targetControllable == f->size || e.targetControllable == f->mode) placeItem(getUIForFilter(f));
        }
        default:
            break;
    }
}

void LayoutFilterStageView::newMessage(const LayoutFilterStageViewUI::FilterEvent& e)
{
    if(e.type == LayoutFilterStageViewUI::FilterEvent::FILTER_REMOVED) removeFilterUI(e.ui);
}

void LayoutFilterStageView::changeListenerCallback(ChangeBroadcaster* source)
{
    for (auto& f : LayoutFilter::instances)
    {
        if (getUIForFilter(f) == nullptr) addUIForFilter(f);
    }
}

void LayoutFilterStageView::timerCallback()
{
    if (shouldRepaint)
    {
        repaint();
        shouldRepaint = false;
    }
}
