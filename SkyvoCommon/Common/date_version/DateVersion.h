#ifndef DATEVERSION_H
#define DATEVERSION_H

//In your namespaced DateVersion header, please #include <string> before the namespace

class DateVersion{
    public:
        static const std::string getBuildTime();
        static const std::string getVersionNumber();
    private:
        DateVersion();
        ~DateVersion();
};

#endif // DATEVERSION_H
