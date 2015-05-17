/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#ifndef operation_h
#define operation_h

#include "file.h"
#include <QString>


/****************************/
// Operation
/*! \brief Operation class
 *
 * Class represents one operation with file, contains destination & distribution path
 */
class Operation{
	public:
		Operation();
		~Operation();	

        void SetOperation(int);
        void SetDistribution(QString);
        void SetDestination(QString);
        void SetPriority(int);

        int GetPriority();
        int GetOperation();
        QString GetDistribution();
        QString GetDestination();
	private:
        int action;
        QString destinatin;
        QString distribution;

        int priority;
};

#endif
