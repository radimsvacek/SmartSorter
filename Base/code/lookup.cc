/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#include "lookup.h"


LookUp::LookUp(){
}

LookUp::~LookUp(){
}

// Set the path to folder where are files to organize
void LookUp::SetPath(QString where){
    path=where;
    directoryIterator = new QDirIterator(path, QDir::Files, QDirIterator::Subdirectories);
}

// Returns next file in the folder
File* LookUp::GetNextFile(){

    File *nextFile;

    if (directoryIterator->hasNext()) {
        nextFile=new File();
        directoryIterator->next();
        nextFile->SetFileInfo(directoryIterator->fileInfo());
        return nextFile;
    }
    else{
        delete directoryIterator;
        return NULL;
    }

}

