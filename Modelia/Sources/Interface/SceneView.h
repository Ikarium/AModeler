#pragma once

#include <QGraphicsView>

namespace Interface
{

class SceneView : public QGraphicsView
{
	Q_OBJECT
public:

	SceneView(QWidget * = 0);

	void wheelEvent(QWheelEvent*);

private:
	class Impl;
	Impl* impl;


};

}