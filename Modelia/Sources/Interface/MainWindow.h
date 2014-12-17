#pragma once

#include "Conf.h"

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

namespace Interface
{

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void saveModel();
	void loadModel();
	void loadLibrary();
	void saveLibrary();
	void compile();
	void addToLibrary();

private:
	class Impl;
	UniquePtr<Impl> m;
};

}
 