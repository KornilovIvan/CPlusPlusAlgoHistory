#ifndef LEFTPANEL_H
#define LEFTPANEL_H

#include <QFileSystemModel>
#include <QTreeView>
#include <QWidget>

class LeftPanel : public QWidget
{
	Q_OBJECT

  public:
	explicit LeftPanel(QWidget *parent = nullptr);
	~LeftPanel() = default;

	QString currentPath() const;
	QString selectedFilePath() const;
	void setRootPath(const QString &path);
	void selectFile(const QString &filePath);
	bool hasFocus() const;
	void setFocus();
	bool hasFileSelected() const;

  signals:
	void fileSelected(const QString &filePath);

  private slots:
	void onItemDoubleClicked(const QModelIndex &index);

  private:
	QFileSystemModel *model;
	QTreeView *treeView;
};
#endif
