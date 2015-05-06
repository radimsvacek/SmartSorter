/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#ifndef node_h
#define node_h

class Node;

#include "operation.h"
#include "treebranch.h"
#include "rule.h"

#include <QDebug>
#include <vector>
#include <QString>

using namespace std;


/****************************/
// Node
class Node{
	public:
		Node();
		~Node();	

		void SetParent(Node *);
		void DeleteTree();

        void SetOperation(int,QString,QString);
        Operation * GetOperation();
        void SetPriority(int);
        void SetAttributeToCompare(QString);

        void SetNewBranch(Node *,QString,int);
        void SetNewBranch(Node *,QDate,int);
        void SetNewBranch(Node *,int,int);
        void SetNewBranch(Node *);

        std::vector<Node *> GetSubTreesByFile(QString, QString);
        Node * GetFileFormatNode(QString);
        QString GetComparingAttribute();
        Node * GetBranchNodeWithSameCond(Condition *);
        Node * GetWalkThroughBranchNode(QString);
        TreeBranch * GetBranchAtPosition(int);
        int GetBranchesCount();
        bool IsLeaf();


	private:
        // For DEBUG
        void PrintTree();

		// Pointer to parent node in tree
        Node *parentNode;

		// String containing property to compare, another string contains format of file and vector of possible results
        QString stuffToCompare;
        vector <TreeBranch *> branches;

		// In case of leaf node, there will be pointer to operation
        Operation *operationInNode;
};


#endif
