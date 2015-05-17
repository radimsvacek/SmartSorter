/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#ifndef file_h
#define file_h

#include <iterator>
#include <algorithm>
#include <QString>
#include <QFileInfo>
#include <QDate>


/****************************/
// File
/*! \brief Represents file
 *
 * Each instance of this class stands for one file, contains info about this file
 */
class File{
	public:
		File();
		~File();	

        void SetFileInfo(QFileInfo);

        QFileInfo GetFileInfo();
        QString GetName();
        QString GetBaseName();
        QString GetPath();
		int GetSize();
        QString GetExtension();
        QDate GetCreationDate();
        QDate GetLastModificationDateDate();
        int GetCreationMonth();
        int GetCreationYear();
        int GetCreationDay();
	private:
        QString path;
        QFileInfo fileInfo;
};

#endif
