#ifndef FATSTRUCTURES_H
#define FATSTRUCTURES_H

#include <QString>
#include <QtGlobal>

#pragma pack(push, 1)
struct FatBootSectorCommon
{
	uint8_t jumpCode[3];
	char oemName[8];
	uint16_t bytesPerSector;
	uint8_t sectorsPerCluster;
	uint16_t reservedSectors;
	uint8_t fatCopies;
	uint16_t rootEntries;
	uint16_t totalSectors16;
	uint8_t mediaDescriptor;
	uint16_t sectorsPerFat16;
	uint16_t sectorsPerTrack;
	uint16_t numberOfHeads;
	uint32_t hiddenSectors;
	uint32_t totalSectors32;
};

struct FatBootSector16
{
	FatBootSectorCommon common;
	uint8_t driveNumber;
	uint8_t reserved;
	uint8_t bootSignature;
	uint32_t volumeId;
	char volumeLabel[11];
	char fileSystemType[8];
};

struct FatBootSector32
{
	FatBootSectorCommon common;
	uint32_t sectorsPerFat32;
	uint16_t extendedFlags;
	uint16_t fatVersion;
	uint32_t rootCluster;
	uint16_t fsInfoSector;
	uint16_t backupBootSector;
	uint8_t reserved[12];
	uint8_t driveNumber;
	uint8_t reserved1;
	uint8_t bootSignature;
	uint32_t volumeId;
	char volumeLabel[11];
	char fileSystemType[8];
};

struct FatDirectoryEntry
{
	unsigned char filename[8];
	unsigned char extension[3];
	uint8_t attributes;
	uint8_t reserved;
	uint8_t creationTimeTenths;
	uint16_t creationTime;
	uint16_t creationDate;
	uint16_t lastAccessDate;
	uint16_t firstClusterHigh;
	uint16_t lastWriteTime;
	uint16_t lastWriteDate;
	uint16_t firstClusterLow;
	uint32_t fileSize;
};

struct FatLongFileNameEntry
{
	uint8_t sequenceNumber;
	uint16_t name1[5];
	uint8_t attributes;
	uint8_t type;
	uint8_t checksum;
	uint16_t name2[6];
	uint16_t firstClusterLow;
	uint16_t name3[2];
};

struct FatFileInfo
{
	QString name;
	QString longName;
	bool isDirectory;
	quint32 size;
	QString createdTime;
	QString modifiedTime;
	quint32 firstCluster;
	quint8 attributes;
};
#pragma pack(pop)
#endif
