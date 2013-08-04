BOOST_AUTO_TEST_CASE(SkyvoOSDateVersion_printDate){
    std::cout << "Date built: " << DateVersion::getBuildTime() << std::endl;
    BOOST_CHECK(DateVersion::getBuildTime() != "");
}

BOOST_AUTO_TEST_CASE(SkyvoOSDateVersion_printVersion){
    std::cout << "Version: " << DateVersion::getVersionNumber() << std::endl;
    BOOST_CHECK(DateVersion::getVersionNumber() != "");
}
