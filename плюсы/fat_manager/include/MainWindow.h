#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>
#include <QToolBar>

class LeftPanel;
class RightPanel;
class QLineEdit;
class QLabel;

class MainWindow : public QMainWindow
{
	Q_OBJECT

  public:
	MainWindow(QWidget *parent = nullptr);
	void loadFatImage(const QString &filePath);

  protected:
	void keyPressEvent(QKeyEvent *event) override;
	bool eventFilter(QObject *obj, QEvent *event) override;

  private:
	void setupUi();
	void setupMenu();
	void setupToolBar();
	void setupConnections();
	void setupShortcuts();
	void showAbout();			// F1 - About
	void mountCurrentFile();	// F2 - Mount
	void mountDialog();			// F4 - Mount...
	void copyFiles();			// F5 - Copy
	void exitApplication();		// F10 - Exit
	void switchPanels();		// Tab - переключение панелей

	QSplitter *splitter;
	LeftPanel *leftPanel;
	RightPanel *rightPanel;

	QToolBar *toolBar;

	QAction *aboutAction;
	QAction *mountAction;
	QAction *mountDialogAction;
	QAction *copyAction;
	QAction *exitAction;

  private slots:
	void onFileSelectedInLeftPanel(const QString &filePath);
	void goHome();
	void goRoot();
	void refresh();
	void openFatImage();
};
#endif
