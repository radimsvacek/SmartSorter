#ifndef operation_h
#define operation_h

#include "file.h"
#include <QString>


/****************************/
// Operation
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
