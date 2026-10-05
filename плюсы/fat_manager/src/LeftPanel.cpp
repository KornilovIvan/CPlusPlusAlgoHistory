#include "../include/LeftPanel.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QHeaderView>
#include <QVBoxLayout>

LeftPanel::LeftPanel(QWidget *parent) : QWidget(parent)
{
	model = new QFileSystemModel(this);
	model->setRootPath("");
	treeView = new QTreeView(this);
	treeView->setModel(model);
	treeView->setRootIndex(model->index(QDir::currentPath()));

	treeView->setColumnHidden(1, false);	// Size
	treeView->setColumnHidden(2, false);	// Type
	treeView->setColumnHidden(3, false);	// Time

	treeView->setFocusPolicy(Qt::StrongFocus);
	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(2, 2, 2, 2);
	layout->addWidget(treeView);

	connect(treeView, &QTreeView::doubleClicked, this, &LeftPanel::onItemDoubleClicked);
}

QString LeftPanel::currentPath() const
{
	QModelIndex index = treeView->currentIndex();
	if (index.isValid())
	{
		return model->filePath(index);
	}
	return QDir::currentPath();
}

QString LeftPanel::selectedFilePath() const
{
	QModelIndex index = treeView->currentIndex();
	if (index.isValid() && !model->isDir(index))
	{
		return model->filePath(index);
	}
	return QString();
}

void LeftPanel::setRootPath(const QString &path)
{
	if (QDir(path).exists())
	{
		treeView->setRootIndex(model->index(path));
	}
}

void LeftPanel::selectFile(const QString &filePath)
{
	QFileInfo fileInfo(filePath);
	if (fileInfo.exists())
	{
		setRootPath(fileInfo.path());
		QModelIndex fileIndex = model->index(filePath);
		if (fileIndex.isValid())
		{
			treeView->setCurrentIndex(fileIndex);
			treeView->scrollTo(fileIndex);
		}
	}
}

bool LeftPanel::hasFocus() const
{
	return treeView->hasFocus();
}

void LeftPanel::setFocus()
{
	treeView->setFocus();
}

bool LeftPanel::hasFileSelected() const
{
	QModelIndex index = treeView->currentIndex();
	return index.isValid() && !model->isDir(index);
}

void LeftPanel::onItemDoubleClicked(const QModelIndex &index)
{
	if (model->isDir(index))
	{
		if (treeView->isExpanded(index))
		{
			treeView->collapse(index);
		}
		else
		{
			treeView->expand(index);
		}
	}
	else
	{
		QString filePath = model->filePath(index);
		emit fileSelected(filePath);
	}
}
