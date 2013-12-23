TEST_GROUP(DateVersionTest){
};

TEST(DateVersionTest, printDate){
    std::cout << "Date built: " << DateVersion::getBuildTime() << std::endl;
    CHECK(DateVersion::getBuildTime() != "");
}

TEST(DateVersionTest, printVersion){
    std::cout << "Version: " << DateVersion::getVersionNumber() << std::endl;
    CHECK(DateVersion::getVersionNumber() != "");
}

