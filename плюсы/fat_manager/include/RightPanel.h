#ifndef RIGHTPANEL_H
#define RIGHTPANEL_H

#include "FatParser.h"

#include <QStack>
#include <QWidget>

class QVBoxLayout;
class QHBoxLayout;
class QTableWidget;
class QTextEdit;
class QTabWidget;
class QPushButton;
class QLabel;
class QProgressDialog;

class RightPanel : public QWidget
{
	Q_OBJECT

  public:
	explicit RightPanel(QWidget *parent = nullptr);
	~RightPanel();

	bool loadFatImage(const QString &filePath);
	void clear();
	QString currentImagePath_f() const;
	bool isImageLoaded() const;

	bool hasFocus() const;
	void setFocus();

  signals:
	void statusMessage(const QString &message);

  private slots:
	void onDirectoryDoubleClicked(int row, int column);
	void goBack();
	void goRoot();

  private:
	void displayDirectory(const QVector< FatFileInfo > &files);
	void displayFatInfo();
	void updatePathLabel();

	QString formatFileSize(const FatFileInfo &fileInfo) const;
	QString formatNumberWithSpaces(quint32 number) const;

	void setupUI();
	void setupConnections();

	QVBoxLayout *mainLayout;
	QHBoxLayout *pathLayout;
	QTableWidget *fileTable;
	QLabel *infoLabel;
	QPushButton *backButton;
	QPushButton *rootButton;
	QLabel *pathLabel;

	FatParser fatParser;

	quint32 currentDirectoryCluster;
	QString currentPath;
	QString currentImagePath;
	QStack< quint32 > directoryStack;
};
#endif
