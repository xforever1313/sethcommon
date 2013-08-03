#ifndef DATE_VERSION_TEST_CPP
#define DATE_VERSION_TEST_CPP

BOOST_AUTO_TEST_CASE(SkyvoOSDateVersion_printDate){
    std::cout << "Date built: " << DateVersion::getBuildTime() << std::endl;
}

BOOST_AUTO_TEST_CASE(SkyvoOSDateVersion_printVersion){
    std::cout << "Version: " << DateVersion::getVersionNumber() << std::endl;
}

#endif // DATE_VERSION_TEST_CPP
