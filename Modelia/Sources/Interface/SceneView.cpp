#include "Conf.h"

#include <QWheelEvent>

#include "SceneView.h"

namespace Interface
{

class SceneView::Impl
{
	SceneView* owner;


public:

	Impl(SceneView* owner_) : owner(owner_)
	{
		
	}

};

SceneView::SceneView(QWidget * parent) : QGraphicsView(parent), impl(new Impl(this))
{
	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	setAcceptDrops(true);
	setInteractive(true);
	setDragMode(RubberBandDrag);
	setViewportUpdateMode(SmartViewportUpdate);
	setResizeAnchor(NoAnchor);
}

void SceneView::wheelEvent(QWheelEvent* event) 
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
 
    double scaleFactor = 1.15;
    if(event->delta() > 0) 
	{
        scale(scaleFactor, scaleFactor);
    }
	else 
	{
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}



}