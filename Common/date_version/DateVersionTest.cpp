
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


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

