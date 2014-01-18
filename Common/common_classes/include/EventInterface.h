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
