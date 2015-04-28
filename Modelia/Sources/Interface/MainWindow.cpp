#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SceneManager.h"
#include "SceneView.h"
#include "ComponentsLibraryView/ComponentsLibraryView.h"

#include "TypesManagerView/TypeEditor.h"

#include "Model/TypesManager/TypesLibrary.h"
#include "Model/ComponentsLibrary/ComponentsLibrary.h"
#include "Model/Compiler/Compiler.h"

#include "ModelInterface.h"

#include <QFileDialog>
#include <QLabel>
#include <QToolBox>

namespace Interface
{

class MainWindow::Impl
{
	MainWindow* owner;

public:

	ModelInterface modelInterface;

	Model::Compiler compiler;

	Ui::MainWindow* ui;
	SceneManager sceneManager;

	Impl(MainWindow* owner_);

	~Impl();


};

MainWindow::Impl::Impl(MainWindow* owner_) :	owner(owner_), 
												ui(new Ui::MainWindow)
{

	new Model::TypesLibrary();
	new Model::ComponentsLibrary();

	ui->setupUi(owner);
	ui->TypesLibraryView->setModel(App::typesLibrary);
	ui->TypesLibraryView->setTypeEditor(ui->TypeEditor);

	ui->ComponentsLibraryView->setModel(App::componentsLibrary);

	modelInterface.loadModel("Default.model");

	sceneManager.init(ui->ToolBar, ui->NavBar, ui->PropertiesWidget);
	sceneManager.setCurrentComposition(&modelInterface.model()->root());

	ui->SceneView->setScene(&sceneManager);
	ui->SceneView->show();

}

MainWindow::Impl::~Impl()
{
	delete ui;
}

MainWindow::MainWindow(QWidget *parent) : m(new Impl(this)),
											QMainWindow(parent)
{
	connect(m->ui->actionSaveModel, SIGNAL(triggered()), this, SLOT(saveModel()));
	connect(m->ui->actionLoadModel, SIGNAL(triggered()), this, SLOT(loadModel()));
	connect(m->ui->actionLoadLibrary, SIGNAL(triggered()), this, SLOT(loadLibrary()));
	connect(m->ui->actionSaveLibrary, SIGNAL(triggered()), this, SLOT(saveLibrary()));
	connect(m->ui->actionToPython, SIGNAL(triggered()), this, SLOT(compile()));

	showMaximized();
}

MainWindow::~MainWindow()
{

}

void MainWindow::saveModel()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save model"), "", tr("AModeler models (*.model)"));

	if (fileName != "")
		m->modelInterface.saveModel(fileName);
}

void MainWindow::loadModel()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Load model"), "", tr("AModeler models (*.model)"));

	if (fileName != "")
	{
		m->sceneManager.clean();
		App::typesLibraryView->clearSelection();
		m->modelInterface.loadModel(fileName);
		m->sceneManager.setCurrentComposition(&m->modelInterface.model()->root());
	}
}

void MainWindow::loadLibrary()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Load library"), "", tr("AModeler library (*.library)"));

	if (fileName != "")
		App::componentsLibrary->populate(fileName);
}

void MainWindow::saveLibrary()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save library"), "", tr("AModeler library (*.library)"));

	if (fileName != "")
	{
		App::componentsLibrary->save(fileName);
	}
}

void MainWindow::compile()
{
	m->compiler.aModelToPhython(&m->modelInterface.model()->root());
}


}
