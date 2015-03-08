
//          Copyright Seth Hendrick 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CONDITION_VARIABLE_WAITER_H
#define CONDITION_VARIABLE_WAITER_H

#include "SConditionVariable.h"
#include "SThread.h"

namespace OS {

/**
* \class ConditionVariableWaiter
* \brief The only point of this class is to call wait() on a condition variable
*        in the run method.  Used for testing purposes
*/
class ConditionVariableWaiter : public OS::Runnable<ConditionVariableWaiter> {
    public:
        ConditionVariableWaiter(OS::SConditionVariable *cv);
        ~ConditionVariableWaiter();

    private:
        void run();

        ConditionVariableWaiter();
        OS::SConditionVariable *m_cv;
};

}

#endif

