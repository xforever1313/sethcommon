//In your namespaced DateVersion source, please #include <string> before the namespace

const std::string &DateVersion::getBuildTime(){
    static const std::string buildTime(std::string(__DATE__)  + std::string(" ") + std::string(__TIME__));
    return buildTime;
}

const std::string &DateVersion::getVersionNumber(){
    #ifndef VERSION
    #define VERSION ""
    #endif

    static const std::string versionString(VERSION);
    return versionString;
}

