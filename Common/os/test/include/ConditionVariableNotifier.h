
//          Copyright Seth Hendrick 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CONDITION_VARIABLE_NOTIFIER_H_
#define CONDITION_VARIABLE_NOTIFIER_H_

/**
 * \class Condition Varaible Notifier
 * \brief The only purpose of this class is to notify the passed
 *        in condition variable when start is called after 
 *        half a second for testing purposes.
 * \author Seth Hendrick
 */

#include "SConditionVariable.h"
#include "SMutex.h"
#include "SThread.h"

namespace OS {

class ConditionVariableNotifier : public OS::SThread {
    public:
        /**
         * \param notifyAll pass in true to notify all,
         *        false to notify one.
         */
        ConditionVariableNotifier(SConditionVariable *cv,
                                  bool notifyAll);

        ~ConditionVariableNotifier();

        bool getNotified();

    private:
        void run();

        OS::SConditionVariable *m_cv;
        bool m_notified;
        bool m_notifyAll;
        OS::SMutex m_notifiedMutex;
};

}

#endif

