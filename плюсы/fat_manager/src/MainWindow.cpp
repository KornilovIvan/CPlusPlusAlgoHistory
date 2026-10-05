#include "../include/MainWindow.h"

#include "../include/LeftPanel.h"
#include "../include/RightPanel.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFocusEvent>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QShortcut>
#include <QStatusBar>
#include <QToolBar>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	setupUi();
	setupMenu();
	setupToolBar();
	setupConnections();
	setupShortcuts();
	leftPanel->installEventFilter(this);
	rightPanel->installEventFilter(this);
	leftPanel->setFocus();
}

void MainWindow::loadFatImage(const QString &filePath)
{
	if (filePath.isEmpty())
		return;
	QFileInfo fileInfo(filePath);
	if (!fileInfo.exists())
	{
		QMessageBox::warning(this, "Error", "File does not exist: " + filePath);
		return;
	}
	if (!fileInfo.isFile())
	{
		QMessageBox::warning(this, "Error", "Path is not a file: " + filePath);
		return;
	}
	rightPanel->clear();
	if (rightPanel->loadFatImage(filePath))
	{
		leftPanel->selectFile(filePath);
	}
	else
	{
		QMessageBox::critical(this, "Error", QString("Failed to load '%1' as a FAT image.\n\n").arg(fileInfo.fileName()));
		rightPanel->clear();
	}
}

void MainWindow::setupUi()
{
	splitter = new QSplitter(Qt::Horizontal, this);
	leftPanel = new LeftPanel(this);
	rightPanel = new RightPanel(this);
	splitter->addWidget(leftPanel);
	splitter->addWidget(rightPanel);
	splitter->setSizes({ 400, 700 });

	setCentralWidget(splitter);
	setWindowTitle("FAT Manager");
	resize(1200, 800);
}

void MainWindow::setupMenu()
{
	// Меню File
	QMenu *fileMenu = menuBar()->addMenu("File");

	QAction *openAction = fileMenu->addAction("Open FAT Image...");
	openAction->setShortcut(QKeySequence::Open);

	fileMenu->addSeparator();

	QAction *exitAction = fileMenu->addAction("Exit");
	exitAction->setShortcut(QKeySequence::Quit);

	connect(openAction, &QAction::triggered, this, &MainWindow::openFatImage);
	connect(exitAction, &QAction::triggered, this, &MainWindow::exitApplication);

	// Меню View
	QMenu *viewMenu = menuBar()->addMenu("View");

	QAction *homeAction = viewMenu->addAction("Home Directory");
	QAction *rootAction = viewMenu->addAction("Root Directory");
	viewMenu->addSeparator();
	QAction *refreshAction = viewMenu->addAction("Refresh View");

	connect(homeAction, &QAction::triggered, this, &MainWindow::goHome);
	connect(rootAction, &QAction::triggered, this, &MainWindow::goRoot);
	connect(refreshAction, &QAction::triggered, this, &MainWindow::refresh);

	// Меню Help
	QMenu *helpMenu = menuBar()->addMenu("Help");
	aboutAction = helpMenu->addAction("About");
	connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
}

void MainWindow::setupToolBar()
{
	toolBar = new QToolBar("Operations", this);
	toolBar->setMovable(false);
	addToolBar(Qt::BottomToolBarArea, toolBar);
	mountAction = toolBar->addAction("Mount (F2)");
	mountDialogAction = toolBar->addAction("Mount... (F4)");
	copyAction = toolBar->addAction("Copy (F5)");
	toolBar->addSeparator();
	exitAction = toolBar->addAction("Exit (F10)");
	connect(mountAction, &QAction::triggered, this, &MainWindow::mountCurrentFile);
	connect(mountDialogAction, &QAction::triggered, this, &MainWindow::mountDialog);
	connect(copyAction, &QAction::triggered, this, &MainWindow::copyFiles);
	connect(exitAction, &QAction::triggered, this, &MainWindow::exitApplication);
}

void MainWindow::setupConnections()
{
	connect(leftPanel, &LeftPanel::fileSelected, this, &MainWindow::onFileSelectedInLeftPanel);
}

void MainWindow::setupShortcuts()
{
	// Горячие клавиши (работают везде)
	QShortcut *f1Shortcut = new QShortcut(QKeySequence(Qt::Key_F1), this);
	QShortcut *f2Shortcut = new QShortcut(QKeySequence(Qt::Key_F2), this);
	QShortcut *f4Shortcut = new QShortcut(QKeySequence(Qt::Key_F4), this);
	QShortcut *f5Shortcut = new QShortcut(QKeySequence(Qt::Key_F5), this);
	QShortcut *f10Shortcut = new QShortcut(QKeySequence(Qt::Key_F10), this);
	QShortcut *tabShortcut = new QShortcut(QKeySequence(Qt::Key_Tab), this);
	connect(f1Shortcut, &QShortcut::activated, this, &MainWindow::showAbout);
	connect(f2Shortcut, &QShortcut::activated, this, &MainWindow::mountCurrentFile);
	connect(f4Shortcut, &QShortcut::activated, this, &MainWindow::mountDialog);
	connect(f5Shortcut, &QShortcut::activated, this, &MainWindow::copyFiles);
	connect(f10Shortcut, &QShortcut::activated, this, &MainWindow::exitApplication);
	connect(tabShortcut, &QShortcut::activated, this, &MainWindow::switchPanels);
	f1Shortcut->setContext(Qt::ApplicationShortcut);
	f2Shortcut->setContext(Qt::ApplicationShortcut);
	f4Shortcut->setContext(Qt::ApplicationShortcut);
	f5Shortcut->setContext(Qt::ApplicationShortcut);
	f10Shortcut->setContext(Qt::ApplicationShortcut);
	tabShortcut->setContext(Qt::ApplicationShortcut);
}

void MainWindow::showAbout()
{
	QMessageBox::about(
		this,
		"About FAT Manager",
		"<h3>FAT Manager</h3>"
		"<p><b>Version 1.0</b></p>"
		"<p>Advanced FAT16/FAT32 File System Viewer</p>"
		"<p><b>Hotkeys:</b></p>"
		"<ul>"
		"<li><b>F1</b> - Show this help</li>"
		"<li><b>F2</b> - Mount selected file from left panel</li>"
		"<li><b>F4</b> - Open file dialog to mount image</li>"
		"<li><b>F5</b> - Copy files from FAT to local (planned)</li>"
		"<li><b>F10</b> - Exit application</li>"
		"<li><b>Tab</b> - Switch between panels</li>"
		"</ul>"
		"<p><b>Features:</b></p>"
		"<ul>"
		"<li>Full FAT16 and FAT32 support</li>"
		"<li>Long file name (LFN) support</li>"
		"<li>Manual binary parsing without external libraries</li>"
		"<li>Dual-panel file management</li>"
		"<li>Cross-platform compatibility</li>"
		"</ul>"
		"<p>Developed with Qt Framework</p>");
}

void MainWindow::mountCurrentFile()
{
	// F2 - Mount
	QString selectedFile = leftPanel->selectedFilePath();
	if (!selectedFile.isEmpty())
	{
		QFileInfo fileInfo(selectedFile);
		if (fileInfo.isFile())
		{
			loadFatImage(selectedFile);
		}
		else
		{
			QMessageBox::information(this, "Mount", "Please select a file, not a directory.");
		}
	}
	else
	{
		QMessageBox::information(this, "Mount", "Please select a file in the left panel first.");
	}
}

void MainWindow::mountDialog()
{
	// F4 - Mount...
	QString filePath = QFileDialog::getOpenFileName(this, "Open FAT Image");
	if (!filePath.isEmpty())
	{
		loadFatImage(filePath);
	}
}

void MainWindow::copyFiles()
{
	// F5 - Copy
	QMessageBox::information(this, "Copy Files", "Copy functionality is planned for the next version.\n\n");
	// нужно реализовать
}

void MainWindow::exitApplication()
{
	// F10 - Exit
	QMessageBox::StandardButton reply =
		QMessageBox::question(this, "Exit FAT Manager", "Are you sure you want to exit?", QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes)
	{
		QApplication::quit();
	}
}

void MainWindow::switchPanels()
{
	// Tab - переключение между панелями
	if (leftPanel->hasFocus())
	{
		rightPanel->setFocus();
	}
	else
	{
		leftPanel->setFocus();
	}
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_F1:
		showAbout();
		event->accept();
		break;
	case Qt::Key_F2:
		mountCurrentFile();
		event->accept();
		break;
	case Qt::Key_F4:
		mountDialog();
		event->accept();
		break;
	case Qt::Key_F5:
		copyFiles();
		event->accept();
		break;
	case Qt::Key_F10:
		exitApplication();
		event->accept();
		break;
	case Qt::Key_Tab:
		if (event->modifiers() == Qt::NoModifier)
		{
			switchPanels();
			event->accept();
		}
		else
		{
			QMainWindow::keyPressEvent(event);
		}
		break;
	default:
		QMainWindow::keyPressEvent(event);
	}
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast< QKeyEvent * >(event);

		if (obj == leftPanel || obj == rightPanel)
		{
			switch (keyEvent->key())
			{
			case Qt::Key_F1:
				showAbout();
				return true;
			case Qt::Key_F2:
				mountCurrentFile();
				return true;
			case Qt::Key_F4:
				mountDialog();
				return true;
			case Qt::Key_F5:
				copyFiles();
				return true;
			case Qt::Key_F10:
				exitApplication();
				return true;
			case Qt::Key_Tab:
				if (keyEvent->modifiers() == Qt::NoModifier)
				{
					switchPanels();
					return true;
				}
				break;
			default:
				break;
			}
		}
	}
	return QMainWindow::eventFilter(obj, event);
}

void MainWindow::onFileSelectedInLeftPanel(const QString &filePath)
{
	loadFatImage(filePath);
}

void MainWindow::goHome()
{
	leftPanel->setRootPath(QDir::homePath());
}

void MainWindow::goRoot()
{
	leftPanel->setRootPath("/");
}

void MainWindow::refresh()
{
	QString currentPath = leftPanel->currentPath();
	leftPanel->setRootPath(currentPath);
}

void MainWindow::openFatImage()
{
	QString filePath = QFileDialog::getOpenFileName(this, "Open FAT Image", QDir::homePath(), "FAT Images (*.img *.dmg *.iso *.bin *.vhd *.vdi);;All Files (*.*)");

	if (!filePath.isEmpty())
	{
		loadFatImage(filePath);
	}
}
