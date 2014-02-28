
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOX_H
#define BOX_H

namespace Math{

struct Box{
    public:
        Box();
        Box(double x, double y, double z, double width, double height);
        virtual ~Box();

        bool isInBox(double x, double y);

        double x, y, z, width, height;
};

}

#endif // BOX_H
