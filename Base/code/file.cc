/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#include "file.h"

File::File(){}

File::~File(){}


void File::SetFileInfo(QFileInfo f){
    fileInfo = f;
}


QString File::GetPath(){
    return fileInfo.filePath();
}

QString File::GetName(){
    return fileInfo.fileName();
}

QString File::GetBaseName(){
    return fileInfo.baseName();
}

int File::GetSize(){
    return fileInfo.size();
}

QString File::GetExtension(){
    return fileInfo.suffix();
}

QDate File::GetLastModificationDateDate(){
    return fileInfo.lastModified().date();
}

QDate File::GetCreationDate(){
    return fileInfo.created().date();
}

int File::GetCreationYear(){
    return fileInfo.lastModified().date().year();
}
int File::GetCreationMonth(){
    return fileInfo.lastModified().date().month();
}
int File::GetCreationDay(){
    return fileInfo.lastModified().date().day();
}

QFileInfo File::GetFileInfo(){
    return fileInfo;
}









