/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#ifndef lookup_h
#define lookup_h

#include "file.h"
#include "analysis.h"

#include <QString>
#include <QDirIterator>
#include <QDir>
#include <QMessageBox>
#include <QDebug>


/****************************/
// LookUp
/*! \brief Look inside folder for files
 *
 * This class is needed for looking up in folders for files
 */
class LookUp{
	public:
		// Constuctor & Destructor
		LookUp();
		~LookUp();
		// Setting path to folder
        void SetPath(QString);
		// Function returning instance of next file object
		File* GetNextFile();


	private:
		// Instance of object to analyze files in folder
		Analysis *analyze;
        QString path;
        QStringList files;

        QDirIterator *directoryIterator;
};
#endif
