//Include guards are missing on purpose.  The purpose of this file is to be
//#included in to several namespaces.  Therefore, no include guards needed.

//In your namespaced DateVersion header, please #include <string> before the namespace

class DateVersion{
    public:
        //Both these methods have a static string in the backend, making returning a reference safe.
        static const std::string &getBuildTime();
        static const std::string &getVersionNumber();
    private:
        DateVersion();
        ~DateVersion();
};
