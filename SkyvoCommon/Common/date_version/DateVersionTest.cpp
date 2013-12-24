TEST_GROUP(DateVersionTest){
};

TEST(DateVersionTest, printDate){
    std::cout << "\nDate built: " << DateVersion::getBuildTime() << std::endl;
    CHECK(DateVersion::getBuildTime() != "");
}

TEST(DateVersionTest, printVersion){
    std::cout << "\nVersion: " << DateVersion::getVersionNumber() << std::endl;
    CHECK(DateVersion::getVersionNumber() != "");
}

