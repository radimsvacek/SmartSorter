/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#include "analysis.h"

// Constructor -> initialize all variables to null
Analysis::Analysis(){
    NullIt();
}

// Nothing more to destroy
Analysis::~Analysis(){}

// Clear all variables
void Analysis::NullIt(){
    allFiles=0;
    allFilesSize=0;
    pdfFiles=0;
    pdfFilesSize=0;
    htmlFiles=0;
    htmlFilesSize=0;
    txtFiles=0;
    txtFilesSize=0;
    jpegFiles=0;
    jpegFilesSize=0;
    otherFiles=0;
    duplicityFiles=0;
}

// Analyze another file and save infos
void Analysis::AddFile(QFileInfo F){

    allFiles++;

    int size=F.size();
    allFilesSize += size;

    QString extensionOfFile = F.suffix();

    if(extensionOfFile.compare("txt",Qt::CaseInsensitive) == 0){
        txtFiles++;
        txtFilesSize += size;
	}
    else if(extensionOfFile.compare("html",Qt::CaseInsensitive) == 0){
        htmlFiles++;
        htmlFilesSize += size;
	}
    else if(extensionOfFile.compare("pdf",Qt::CaseInsensitive) == 0){
        pdfFiles++;
        pdfFilesSize += size;
	}
    else if(extensionOfFile.compare("jpeg",Qt::CaseInsensitive) == 0 || extensionOfFile.compare("jpg",Qt::CaseInsensitive) == 0){
        jpegFiles++;
        jpegFilesSize += size;
	}
	else{
        otherFiles++;
	}	
}

// Result of analysis
QString Analysis::PrintResults(){
    QString analysisTextResult = "";

    if(allFiles > 0){
        analysisTextResult = "Files total: " +  QString::number(allFiles) + "\n\t - average size: ";
        analysisTextResult += GetSizeInProperUnit(allFilesSize/(allFiles)) + "\n\n";
        if(pdfFiles>0){
            analysisTextResult += "PDF files total: " + QString::number(pdfFiles) + "\n\t - average size: ";
            analysisTextResult += GetSizeInProperUnit(pdfFilesSize/(pdfFiles))+ " \n\n";;
        }
        if(jpegFiles>0){
            analysisTextResult += "JPEG files total: " + QString::number(jpegFiles) + "\n\t - average size: ";
             analysisTextResult += GetSizeInProperUnit(jpegFilesSize/(jpegFiles))+ " \n\n";;
        }
        if(htmlFiles>0){
            analysisTextResult += "HTML files total: " + QString::number(htmlFiles) + "\n\t - average size: ";
            analysisTextResult += GetSizeInProperUnit(htmlFilesSize/(htmlFiles))+ " \n\n";;
        }
        if(txtFiles>0){
            analysisTextResult += "TXT files total: " + QString::number(txtFiles) + "\n\t - average size: ";
            analysisTextResult += GetSizeInProperUnit(txtFilesSize/(txtFiles))+ " \n\n";;
        }

        analysisTextResult += "Other files total: " + QString::number(otherFiles) + "\n";
    }
    return analysisTextResult;

}

// Convert to proper unit
QString Analysis::GetSizeInProperUnit(int size){
    QString unit = "B";
    if(size>KILO){
        size/=KILO;
        unit = "kB";
    }
    if(size>KILO){
        size/=KILO;
        unit = "MB";
    }
    if(size>KILO){
        size/=KILO;
        unit = "GB";
    }
    return QString::number(size) + " " + unit;
}










