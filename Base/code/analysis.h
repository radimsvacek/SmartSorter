/**********************************/
// This file is part of SmartSorter
// Author: Radim Svaceka
// Created: 2015
/**********************************/

#ifndef analysis_h
#define analysis_h


#include "file.h"
#include <sys/stat.h>
#include <QDirIterator>
#include <QString>
#include <QFileInfo>

#define KILO 1024


/****************************/
// Analysis
class Analysis{
	public:
		Analysis();
		~Analysis();	

        void AddFile(QFileInfo);
        QString PrintResults();
        void NullIt();


    private:
        int allFiles;
        long long allFilesSize;

        int pdfFiles;
        long long pdfFilesSize;
        int htmlFiles;
        long long htmlFilesSize;
        int txtFiles;
        long long txtFilesSize;
        int jpegFiles;
        long long jpegFilesSize;

        int otherFiles;
        int duplicityFiles;

        QString GetSizeInProperUnit(int);

};

#endif
