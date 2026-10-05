#include "../include/FatParser.h"

#include <QDebug>
#include <QMessageBox>

FatParser::FatParser() :
	fatType(UNKNOWN), fatStartSector(0), rootDirSector(0), rootDirCluster(0), dataStartSector(0), sectorsPerFat(0),
	totalSectors(0), bytesPerSector(512), sectorsPerCluster(1)
{
}

FatParser::~FatParser()
{
	close();
}

bool FatParser::loadImage(const QString &filePath)
{
	close();

	fatFile.setFileName(filePath);
	if (!fatFile.open(QIODevice::ReadOnly))
	{
		lastError = "Cannot open FAT image: " + filePath;
		return false;
	}

	if (!parseBootSector())
	{
		lastError = "Invalid boot sector";
		return false;
	}

	if (!determineFatType())
	{
		lastError = "Unsupported FAT type or invalid image";
		return false;
	}

	return true;
}

void FatParser::close()
{
	if (fatFile.isOpen())
	{
		fatFile.close();
	}
	fatType = UNKNOWN;
	lastError.clear();
}

bool FatParser::isLoaded() const
{
	return fatFile.isOpen() && fatType != UNKNOWN;
}

QString FatParser::lastErrorf() const
{
	return lastError;
}

FatParser::FatType FatParser::getFatType() const
{
	return fatType;
}

quint32 FatParser::getRootDirectoryCluster() const
{
	return rootDirCluster;
}

bool FatParser::parseBootSector()
{
	if (fatFile.size() < 512)
	{
		lastError = "File too small to be a FAT image";
		return false;
	}
	if (!fatFile.seek(0))
	{
		lastError = "Cannot seek to boot sector";
		return false;
	}
	if (fatFile.read((char *)&bootSectorCommon, sizeof(FatBootSectorCommon)) != sizeof(FatBootSectorCommon))
	{
		lastError = "Cannot read boot sector";
		return false;
	}
	bytesPerSector = bootSectorCommon.bytesPerSector;
	sectorsPerCluster = bootSectorCommon.sectorsPerCluster;
	return true;
}

bool FatParser::determineFatType()
{
	totalSectors = bootSectorCommon.totalSectors16 ? bootSectorCommon.totalSectors16 : bootSectorCommon.totalSectors32;
	quint32 dataSectors =
		totalSectors - bootSectorCommon.reservedSectors - (bootSectorCommon.fatCopies * bootSectorCommon.sectorsPerFat16);
	if (bootSectorCommon.rootEntries > 0)
	{
		dataSectors -= ((bootSectorCommon.rootEntries * 32) + bytesPerSector - 1) / bytesPerSector;
	}
	quint32 totalClusters = dataSectors / sectorsPerCluster;
	if (totalClusters < 4085)
	{
		lastError = "FAT12 detected but not supported";
		return false;
	}
	else if (totalClusters < 65525)
	{
		fatType = FAT16;
		sectorsPerFat = bootSectorCommon.sectorsPerFat16;
		rootDirSector = bootSectorCommon.reservedSectors + (bootSectorCommon.fatCopies * sectorsPerFat);
		dataStartSector = rootDirSector + ((bootSectorCommon.rootEntries * 32) / bytesPerSector);
		rootDirCluster = 0;
	}
	else
	{
		fatType = FAT32;
		FatBootSector32 fat32Boot;
		if (!fatFile.seek(0))
		{
			lastError = "Cannot seek to FAT32 boot sector";
			return false;
		}
		if (fatFile.read((char *)&fat32Boot, sizeof(FatBootSector32)) != sizeof(FatBootSector32))
		{
			lastError = "Cannot read FAT32 boot sector";
			return false;
		}
		sectorsPerFat = fat32Boot.sectorsPerFat32;
		rootDirCluster = fat32Boot.rootCluster;
		rootDirSector = 0;
		dataStartSector = bootSectorCommon.reservedSectors + (bootSectorCommon.fatCopies * sectorsPerFat);
	}
	fatStartSector = bootSectorCommon.reservedSectors;
	return true;
}

QVector< FatFileInfo > FatParser::readDirectory(quint32 cluster)
{
	QVector< FatFileInfo > result;
	quint64 directoryOffset = getDirectoryOffset(cluster);
	if (directoryOffset == 0)
	{
		return result;
	}
	quint32 directorySize;
	if (fatType == FAT16 && cluster == 0)
	{
		directorySize = bootSectorCommon.rootEntries * 32;
	}
	else
	{
		directorySize = sectorsPerCluster * bytesPerSector;
	}
	QVector< FatDirectoryEntry > entries = readRawDirectoryEntries(directoryOffset, directorySize);
	for (int i = 0; i < entries.size(); ++i)
	{
		const FatDirectoryEntry &entry = entries[i];
		if (isDeletedEntry(entry))
			continue;
		if (isVolumeLabel(entry))
			continue;
		if (static_cast< uint8_t >(entry.filename[0]) == 0x00)
			break;
		QString longName;
		if (i > 0 && isLongFileNameEntry(entries[i - 1]) && !isLongFileNameEntry(entry))
		{
			longName = readLongFileName(entries, i - 1);
		}
		if (isLongFileNameEntry(entry))
			continue;
		QString name = getEntryName(entry, longName);
		if (name.isEmpty() || name == "." || name == "..")
			continue;
		FatFileInfo fileInfo;
		fileInfo.name = name;
		fileInfo.longName = longName;
		fileInfo.isDirectory = isDirectory(entry);
		fileInfo.size = entry.fileSize;
		fileInfo.firstCluster = getFirstCluster(entry);
		fileInfo.attributes = entry.attributes;
		fileInfo.modifiedTime = formatFatDate(entry.lastWriteDate) + " " + formatFatTime(entry.lastWriteTime);
		fileInfo.createdTime = formatFatDate(entry.creationDate) + " " + formatFatTime(entry.creationTime);
		result.append(fileInfo);
	}
	return result;
}

QVector< FatDirectoryEntry > FatParser::readRawDirectoryEntries(quint64 offset, quint32 size)
{
	QVector< FatDirectoryEntry > entries;
	if (!fatFile.seek(offset))
		return entries;
	int maxEntries = size / 32;
	for (int i = 0; i < maxEntries; ++i)
	{
		FatDirectoryEntry entry;
		if (fatFile.read((char *)&entry, 32) != 32)
			break;

		if (static_cast< uint8_t >(entry.filename[0]) == 0x00)
			break;

		entries.append(entry);
	}
	return entries;
}

QString FatParser::readLongFileName(const QVector< FatDirectoryEntry > &entries, int startIndex)
{
	QString longName;
	QVector< quint16 > nameChars;
	for (int i = startIndex; i >= 0; --i)
	{
		if (!isLongFileNameEntry(entries[i]))
			break;
		FatLongFileNameEntry *lfn = (FatLongFileNameEntry *)&entries[i];
		quint8 sequence = lfn->sequenceNumber & 0x1F;
		for (int j = 0; j < 5; ++j)
			if (lfn->name1[j] != 0xFFFF && lfn->name1[j] != 0x0000)
				nameChars.append(lfn->name1[j]);
		for (int j = 0; j < 6; ++j)
			if (lfn->name2[j] != 0xFFFF && lfn->name2[j] != 0x0000)
				nameChars.append(lfn->name2[j]);
		for (int j = 0; j < 2; ++j)
			if (lfn->name3[j] != 0xFFFF && lfn->name3[j] != 0x0000)
				nameChars.append(lfn->name3[j]);
		if (sequence == 1)
			break;
	}
	for (quint16 ch : nameChars)
	{
		if (ch == 0)
			break;
		longName.append(QChar(ch));
	}
	return longName.trimmed();
}

QString FatParser::getEntryName(const FatDirectoryEntry &entry, const QString &longName)
{
	if (!longName.isEmpty())
	{
		return longName;
	}
	QString name = QString::fromLatin1(reinterpret_cast< const char * >(entry.filename), 8).trimmed();
	QString ext = QString::fromLatin1(reinterpret_cast< const char * >(entry.extension), 3).trimmed();
	name = name.trimmed();
	ext = ext.trimmed();
	if (ext.isEmpty())
		return name;
	else
		return name + "." + ext;
}

bool FatParser::isDirectory(const FatDirectoryEntry &entry) const
{
	return (static_cast< uint8_t >(entry.attributes) & 0x10) != 0;
}

bool FatParser::isLongFileNameEntry(const FatDirectoryEntry &entry) const
{
	return static_cast< uint8_t >(entry.attributes) == 0x0F;
}

bool FatParser::isDeletedEntry(const FatDirectoryEntry &entry) const
{
	return static_cast< uint8_t >(entry.filename[0]) == 0xE5;
}

bool FatParser::isVolumeLabel(const FatDirectoryEntry &entry) const
{
	return (static_cast< uint8_t >(entry.attributes) & 0x08) != 0;
}

quint32 FatParser::getFirstCluster(const FatDirectoryEntry &entry) const
{
	if (fatType == FAT16)
	{
		return entry.firstClusterLow;
	}
	else
	{
		return (entry.firstClusterHigh << 16) | entry.firstClusterLow;
	}
}

quint64 FatParser::getDirectoryOffset(quint32 cluster) const
{
	if (fatType == FAT16 && cluster == 0)
	{
		return rootDirSector * bytesPerSector;
	}
	else
	{
		return clusterToSector(cluster) * bytesPerSector;
	}
}

quint32 FatParser::clusterToSector(quint32 cluster) const
{
	return dataStartSector + (cluster - 2) * sectorsPerCluster;
}

QString FatParser::formatFatTime(quint16 time) const
{
	int hour = (time >> 11) & 0x1F;
	int minute = (time >> 5) & 0x3F;
	int second = (time & 0x1F) * 2;
	return QString("%1:%2:%3").arg(hour, 2, 10, QChar('0')).arg(minute, 2, 10, QChar('0')).arg(second, 2, 10, QChar('0'));
}

QString FatParser::formatFatDate(quint16 date) const
{
	int year = ((date >> 9) & 0x7F) + 1980;
	int month = (date >> 5) & 0x0F;
	int day = date & 0x1F;
	return QString("%1-%2-%3").arg(year).arg(month, 2, 10, QChar('0')).arg(day, 2, 10, QChar('0'));
}

QString FatParser::getFileSystemInfo() const
{
	QString info;
	info += "FAT File System Information\n";
	info += "===========================\n";
	info += QString("File System Type: %1\n").arg(fatType == FAT16 ? "FAT16" : "FAT32");
	info += QString("Bytes per Sector: %1\n").arg(bytesPerSector);
	info += QString("Sectors per Cluster: %1\n").arg(sectorsPerCluster);
	info += QString("Total Sectors: %1\n").arg(totalSectors);
	if (fatType == FAT32)
	{
		info += QString("Root Directory Cluster: %1\n").arg(rootDirCluster);
	}
	else
	{
		info += QString("Root Directory Sector: %1\n").arg(rootDirSector);
	}
	info += QString("Data Start Sector: %1\n").arg(dataStartSector);
	return info;
}
