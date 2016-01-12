#include "gtest/gtest.h"
#include "include/BasesSixFour.h"

using namespace std;


class BaseSixFourTests : public ::testing::Test {

    protected:

        BaseSixFourTests() {
        }

        virtual ~BaseSixFourTests() {
        }

        virtual void SetUp() {
        }

        virtual void TearDown() {
        }
};

TEST_F(BaseSixFourTests, Test){

    ASSERT_EQ(1, 1);
}
