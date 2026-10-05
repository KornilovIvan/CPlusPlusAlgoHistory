#ifndef FATPARSER_H
#define FATPARSER_H

#include "FatStructures.h"

#include <QByteArray>
#include <QFile>
#include <QString>
#include <QVector>

class FatParser
{
  public:
	enum FatType
	{
		FAT16,
		FAT32,
		UNKNOWN
	};

	FatParser();
	~FatParser();

	bool loadImage(const QString &filePath);
	void close();
	bool isLoaded() const;
	QString lastErrorf() const;
	FatType getFatType() const;
	QString getFileSystemInfo() const;
	QVector< FatFileInfo > readDirectory(quint32 cluster = 0);
	quint32 getRootDirectoryCluster() const;

  private:
	bool parseBootSector();
	bool determineFatType();
	QVector< FatDirectoryEntry > readRawDirectoryEntries(quint64 offset, quint32 size);
	QString readLongFileName(const QVector< FatDirectoryEntry > &entries, int startIndex);
	QString getEntryName(const FatDirectoryEntry &entry, const QString &longName = "");
	bool isDirectory(const FatDirectoryEntry &entry) const;
	bool isLongFileNameEntry(const FatDirectoryEntry &entry) const;
	bool isDeletedEntry(const FatDirectoryEntry &entry) const;
	bool isVolumeLabel(const FatDirectoryEntry &entry) const;
	quint32 getFirstCluster(const FatDirectoryEntry &entry) const;
	quint32 clusterToSector(quint32 cluster) const;
	quint64 getDirectoryOffset(quint32 cluster) const;
	QString formatFatTime(quint16 time) const;
	QString formatFatDate(quint16 date) const;
	QFile fatFile;
	FatType fatType;
	FatBootSectorCommon bootSectorCommon;
	quint32 fatStartSector;
	quint32 rootDirSector;
	quint32 rootDirCluster;
	quint32 dataStartSector;
	quint32 sectorsPerFat;
	quint32 totalSectors;
	quint16 bytesPerSector;
	quint8 sectorsPerCluster;
	QString lastError;
};

#endif
