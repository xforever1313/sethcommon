#ifndef TASK_INTERFACE_H
#define TASK_INTERFACE_H

#include "ReturnObjectInterface.h"

class TaskInterface
{
    public:

        virtual ~TaskInterface(){}

        virtual ReturnObjectInterface *execute() = 0;
};

#endif // TASK_INTERFACE_H
