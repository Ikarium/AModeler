#pragma once

#include "Conf.h"

#include <QGraphicsScene>
#include<QMimeData>
#include <QToolBar>

namespace Interface
{

class SceneManager : public QGraphicsScene
{
	Q_OBJECT

public:

	SceneManager(Model::Composition * = nullptr);
	~SceneManager();

	Model::Composition & currentComposition();
	void setCurrentComposition(Model::Composition *);
	void clean();
	void populate();

	void dragEnterEvent(QGraphicsSceneDragDropEvent *);
	void dragMoveEvent(QGraphicsSceneDragDropEvent *);
	void dragLeaveEvent(QGraphicsSceneDragDropEvent *);
	void dropEvent(QGraphicsSceneDragDropEvent *);
	void startDrag(Qt::DropActions);

	void addElem(QMimeData const *, QPointF const &);
	void addLink(SlotView *, ComponentView *);
	void addLink(SlotView *, SlotView *);

	void init(QToolBar*, QToolBar*, QWidget*);

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *) override;
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *) override;
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
	test

protected slots:
	void focusItemChanged(QGraphicsItem *, QGraphicsItem *, Qt::FocusReason);
	void navBarElemTriggered(); 
	 
private:
	class Impl;
	UniquePtr<Impl> m;


};

}