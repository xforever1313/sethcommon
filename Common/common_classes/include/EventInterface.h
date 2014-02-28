
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef EVENTINTERFACE_H
#define EVENTINTERFACE_H

namespace Common{

class EventInterface
{
    public:
        virtual ~EventInterface(){};

        virtual void execute() = 0;
};

}
#endif // EVENTINTERFACE_H
