#include "../include/MainWindow.h"

#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDebug>
#include <QException>
#include <QFileInfo>
#include <QMessageBox>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QApplication::setApplicationName("FAT Manager");
	QCommandLineParser parser;
	try
	{
		parser.process(app);
	} catch (const QException &e)
	{
		QMessageBox::critical(nullptr, "Command Line Error", QString("Failed to parse command line: %1").arg(e.what()));
		return 1;
	}
	MainWindow window;
	window.show();

	QString filePath = parser.positionalArguments().value(0);
	if (!filePath.isEmpty())
	{
		QFileInfo fileInfo(filePath);
		if (fileInfo.exists() && fileInfo.isFile())
		{
			window.loadFatImage(filePath);
		}
		else
		{
			qWarning() << "Invalid file path:" << filePath;
			// Ошибка будет обработана в MainWindow::loadFatImage
		}
	}
	return app.exec();
}
