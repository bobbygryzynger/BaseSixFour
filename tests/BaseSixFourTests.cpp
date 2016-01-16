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

TEST_F(BaseSixFourTests, EncodeText){

    //from: https://en.wikipedia.org/wiki/Base64#Examples
    std::string in =    "Man is distinguished, not only by his reason, but by this singular passion from "
                        "other animals, which is a lust of the mind, that by a perseverance of delight "
                        "in the continued and indefatigable generation of knowledge, exceeds the short "
                        "vehemence of any carnal pleasure.";

    std::string out =   "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlz\r\n"
                        "IHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2Yg\r\n"
                        "dGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGlu\r\n"
                        "dWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRo\r\n"
                        "ZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=";


    std::vector<uint8_t> inVec;
    for(size_t i = 0; i < in.length(); i++){
        inVec.push_back(in.at(i));
    }

    ASSERT_STREQ(out.c_str(), BaseSixFour::encode(inVec).c_str());
}
