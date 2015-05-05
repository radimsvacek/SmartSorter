#include "control.h"

// Constructor
Control::Control(){
	// New instance of file lookup engine
/*	LU = new LookUp();

	// Instance of decision tree
	DecisionTree = new DecTree();
*/}

// Delete instances of lookup and decision tree
Control::~Control(){
//	delete LU;
//	delete DecisionTree;
}

void Control::Ready(QString folder){}
/*	// stuff with GUI

	// Path to folder which will be organized
	string path = folder;

	// Do the analysis of folder
	LU->DoTheAnalysis(path);

	// Build decision tree based on user's wish
	DecisionTree->BuildTree();
	// then work
	Work(path);
	// And remove tree
	DecisionTree->RemoveTree();
	// there will be non-end cycle 
}


void Control::Work(string path){
	// Start look up in selected folder
	LU->SetPath(path);

	// Pointer to instance of file object
	File *FileToClassify;

	// Infinity loop to look-up all files 
	while((FileToClassify = LU->GetNextFile()) != NULL){
		DecisionTree->DoTheMagic(FileToClassify);
	}	


}*/
