#include "../include/RightPanel.h"

#include <QApplication>
#include <QDebug>
#include <QFont>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QProgressBar>
#include <QProgressDialog>
#include <QPushButton>
#include <QStack>
#include <QTableWidget>
#include <QTextEdit>
#include <QVBoxLayout>

RightPanel::RightPanel(QWidget *parent) : QWidget(parent)
{
	currentDirectoryCluster = 0;
	currentPath = "/";
	currentImagePath = "";

	setupUI();
	setupConnections();
}

RightPanel::~RightPanel()
{
	clear();
}

void RightPanel::setupUI()
{
	mainLayout = new QVBoxLayout(this);
	pathLayout = new QHBoxLayout();
	backButton = new QPushButton("← Back", this);
	rootButton = new QPushButton("Root", this);
	pathLabel = new QLabel("/", this);
	pathLayout->addWidget(backButton);
	pathLayout->addWidget(rootButton);
	pathLayout->addWidget(pathLabel);
	pathLayout->addStretch();
	mainLayout->addLayout(pathLayout);
	infoLabel = new QLabel("No FAT image loaded", this);
	infoLabel->setAlignment(Qt::AlignCenter);
	infoLabel->setStyleSheet("color: gray; padding: 10px;");
	mainLayout->addWidget(infoLabel);
	fileTable = new QTableWidget(this);
	fileTable->setColumnCount(3);
	fileTable->setHorizontalHeaderLabels({ "Name", "Size", "Time" });
	fileTable->horizontalHeader()->setStretchLastSection(false);
	fileTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	fileTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	fileTable->setFocusPolicy(Qt::StrongFocus);
	fileTable->setColumnWidth(0, 300);	  // Name
	fileTable->setColumnWidth(1, 150);	  // Size
	fileTable->setColumnWidth(2, 150);	  // Time
	mainLayout->addWidget(fileTable);
	backButton->setEnabled(false);
	fileTable->setVisible(false);
	infoLabel->setVisible(true);
}

void RightPanel::setupConnections()
{
	connect(fileTable, &QTableWidget::cellDoubleClicked, this, &RightPanel::onDirectoryDoubleClicked);
	connect(backButton, &QPushButton::clicked, this, &RightPanel::goBack);
	connect(rootButton, &QPushButton::clicked, this, &RightPanel::goRoot);
}

bool RightPanel::loadFatImage(const QString &filePath)
{
	clear();
	if (!fatParser.loadImage(filePath))
	{
		QMessageBox::warning(this, "Error", fatParser.lastErrorf());
		return false;
	}
	currentImagePath = filePath;
	if (fatParser.getFatType() == FatParser::FAT16)
	{
		currentDirectoryCluster = 0;
	}
	else
	{
		currentDirectoryCluster = fatParser.getRootDirectoryCluster();
	}

	QVector< FatFileInfo > files = fatParser.readDirectory(currentDirectoryCluster);
	if (files.isEmpty())
	{
		QMessageBox::warning(this, "Error", "Cannot read root directory");
		clear();
		return false;
	}

	displayDirectory(files);

	infoLabel->setText(fatParser.getFileSystemInfo());
	fileTable->setVisible(true);

	currentPath = "/";
	pathLabel->setText("/");
	backButton->setEnabled(false);
	rootButton->setEnabled(true);

	emit statusMessage(QString("FAT image loaded: %1").arg(filePath));
	return true;
}

void RightPanel::clear()
{
	fileTable->setRowCount(0);
	pathLabel->setText("/");
	currentPath = "/";
	directoryStack.clear();
	currentDirectoryCluster = 0;
	currentImagePath = "";
	fatParser.close();
	infoLabel->setText("No FAT image loaded");
	fileTable->setVisible(false);
	backButton->setEnabled(false);
	rootButton->setEnabled(false);
}

QString RightPanel::currentImagePath_f() const
{
	return currentImagePath;
}

bool RightPanel::isImageLoaded() const
{
	return fatParser.isLoaded();
}

bool RightPanel::hasFocus() const
{
	return fileTable->hasFocus();
}

void RightPanel::setFocus()
{
	fileTable->setFocus();
}

void RightPanel::displayDirectory(const QVector< FatFileInfo > &files)
{
	fileTable->setRowCount(0);

	int row = 0;
	for (const FatFileInfo &fileInfo : files)
	{
		fileTable->insertRow(row);
		QString nameColumn = fileInfo.longName.isEmpty() ? fileInfo.name : fileInfo.longName;
		QString sizeColumn = formatFileSize(fileInfo);
		QString timeColumn = fileInfo.modifiedTime;
		fileTable->setItem(row, 0, new QTableWidgetItem(nameColumn));
		fileTable->setItem(row, 1, new QTableWidgetItem(sizeColumn));
		fileTable->setItem(row, 2, new QTableWidgetItem(timeColumn));
		fileTable->item(row, 0)->setData(Qt::UserRole, fileInfo.firstCluster);
		fileTable->item(row, 0)->setData(Qt::UserRole + 1, fileInfo.isDirectory);
		fileTable->item(row, 0)->setData(Qt::UserRole + 2, fileInfo.size);
		row++;
	}
}

QString RightPanel::formatFileSize(const FatFileInfo &fileInfo) const
{
	if (fileInfo.isDirectory)
	{
		return "Dir";
	}
	else
	{
		return formatNumberWithSpaces(fileInfo.size) + " bytes";
	}
}

QString RightPanel::formatNumberWithSpaces(quint32 number) const
{
	QString str = QString::number(number);
	QString result;
	int counter = 0;
	for (int i = str.length() - 1; i >= 0; --i)
	{
		if (counter > 0 && counter % 3 == 0)
		{
			result.prepend(' ');
		}
		result.prepend(str[i]);
		counter++;
	}
	return result;
}

void RightPanel::updatePathLabel()
{
	pathLabel->setText(currentPath);
	backButton->setEnabled(!directoryStack.isEmpty());
}

void RightPanel::onDirectoryDoubleClicked(int row, int column)
{
	Q_UNUSED(column);
	if (row < 0 || row >= fileTable->rowCount())
		return;
	QTableWidgetItem *item = fileTable->item(row, 0);
	if (!item)
		return;
	bool isDirectory = item->data(Qt::UserRole + 1).toBool();
	if (!isDirectory)
		return;
	directoryStack.push(currentDirectoryCluster);
	quint32 newCluster = item->data(Qt::UserRole).toUInt();
	QString fileName = item->text();
	QString newPath = currentPath + fileName + "/";
	currentDirectoryCluster = newCluster;
	QVector< FatFileInfo > files = fatParser.readDirectory(newCluster);
	if (!files.isEmpty())
	{
		currentPath = newPath;
		displayDirectory(files);
		updatePathLabel();
		emit statusMessage(QString("Entered directory: %1").arg(newPath));
	}
	else
	{
		directoryStack.pop();
		QMessageBox::warning(this, "Error", "Cannot read directory: " + fileName);
	}
}

void RightPanel::goBack()
{
	if (directoryStack.isEmpty())
		return;
	quint32 prevCluster = directoryStack.pop();
	QString path = currentPath.left(currentPath.lastIndexOf('/'));
	if (path.isEmpty())
		path = "/";
	QVector< FatFileInfo > files = fatParser.readDirectory(prevCluster);
	if (!files.isEmpty())
	{
		currentDirectoryCluster = prevCluster;
		currentPath = path;
		displayDirectory(files);
		updatePathLabel();
		emit statusMessage(QString("Back to: %1").arg(path));
	}
}

void RightPanel::goRoot()
{
	directoryStack.clear();
	quint32 rootCluster;
	if (fatParser.getFatType() == FatParser::FAT16)
	{
		rootCluster = 0;
	}
	else
	{
		rootCluster = fatParser.getRootDirectoryCluster();
	}
	QVector< FatFileInfo > files = fatParser.readDirectory(rootCluster);
	if (!files.isEmpty())
	{
		currentDirectoryCluster = rootCluster;
		currentPath = "/";
		displayDirectory(files);
		updatePathLabel();
		emit statusMessage("Returned to root directory");
	}
}
