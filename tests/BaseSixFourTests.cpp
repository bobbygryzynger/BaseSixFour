#include "gtest/gtest.h"
#include "include/BaseSixFour.h"

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

TEST_F(BaseSixFourTests, TestEncode){

    //from: https://en.wikipedia.org/wiki/Base64#Examples
    std::string in =    "Man is distinguished, not only by his reason, but by this singular passion from\n"
                        "other animals, which is a lust of the mind, that by a perseverance of delight\n"
                        "in the continued and indefatigable generation of knowledge, exceeds the short\n"
                        "vehemence of any carnal pleasure.";

    std::string out =   "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlz\n"
                        "IHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2Yg\n"
                        "dGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGlu\n"
                        "dWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRo\n"
                        "ZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=";

    ASSERT_STREQ(BaseSixFour::encode(in).c_str(), out.c_str());
}
