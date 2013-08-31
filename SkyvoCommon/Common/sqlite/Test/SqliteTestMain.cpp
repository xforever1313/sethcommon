#define BOOST_TEST_MODULE sqlite_Unit_Tests
#include <boost/test/unit_test.hpp>
#include <gmock/gmock.h>

///\struct InitGMock
///\brief This is needed to cause gmock to throw exceptions if an expect call fails.
struct InitGMock {
    InitGMock() {
        ::testing::GTEST_FLAG(throw_on_failure) = true;
        ::testing::InitGoogleMock(&boost::unit_test::framework::master_test_suite().argc,
                                  boost::unit_test::framework::master_test_suite().argv);
    }
    ~InitGMock() { }
};
BOOST_GLOBAL_FIXTURE(InitGMock)
