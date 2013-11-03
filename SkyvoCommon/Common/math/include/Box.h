#ifndef BOX_H
#define BOX_H

namespace Common{

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
