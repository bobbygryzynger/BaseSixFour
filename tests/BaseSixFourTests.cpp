#include "gtest/gtest.h"
#include "basesixfour.h"
#include <fstream>

#ifndef DATA_DIR
    #define DATA_DIR "data"
#endif

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

TEST_F(BaseSixFourTests, MIME_EncodeText){

    BaseSixFour b64(BaseSixFour::MIME);

    std::string inNoPad =
            "What is a country? A country is a piece of land surrounded on all "
            "sides by boundaries, usually unnatural.";

    std::string expectedNoPad =
            "V2hhdCBpcyBhIGNvdW50cnk/IEEgY291bnRyeSBpcyBhIHBpZWNlIG9mIGxhbmQgc3Vycm91bmRl\r\n"
            "ZCBvbiBhbGwgc2lkZXMgYnkgYm91bmRhcmllcywgdXN1YWxseSB1bm5hdHVyYWwu";

    //from: https://en.wikipedia.org/wiki/Base64#Examples
    std::string inSglPad =
        "Man is distinguished, not only by his reason, but by this singular passion from "
        "other animals, which is a lust of the mind, that by a perseverance of delight "
        "in the continued and indefatigable generation of knowledge, exceeds the short "
        "vehemence of any carnal pleasure.";

    std::string expectedSglPad =
        "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlz\r\n"
        "IHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2Yg\r\n"
        "dGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGlu\r\n"
        "dWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRo\r\n"
        "ZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=";

    // Bowie - "Five Years"
    std::string inDblPad =
            "News guy wept when he told us Earth was really dying / "
            "Cried so much that his face was wet / "
            "Then I knew he was not lying";

    std::string expectedDblPad =
            "TmV3cyBndXkgd2VwdCB3aGVuIGhlIHRvbGQgdXMgRWFydGggd2FzIHJlYWxseSBkeWluZyAvIENy\r\n"
            "aWVkIHNvIG11Y2ggdGhhdCBoaXMgZmFjZSB3YXMgd2V0IC8gVGhlbiBJIGtuZXcgaGUgd2FzIG5v\r\n"
            "dCBseWluZw==";


    // object tests
    ASSERT_STREQ(expectedNoPad.c_str(), b64.encode(std::vector<uint8_t>(inNoPad.begin(), inNoPad.end()), true).c_str());
    ASSERT_STREQ(expectedSglPad.c_str(), b64.encode(std::vector<uint8_t>(inSglPad.begin(), inSglPad.end()), true).c_str());
    ASSERT_STREQ(expectedDblPad.c_str(), b64.encode(std::vector<uint8_t>(inDblPad.begin(), inDblPad.end()), true).c_str());

    // static tests
    ASSERT_STREQ(expectedNoPad.c_str(), BaseSixFour::encodeMIME(std::vector<uint8_t>(inNoPad.begin(), inNoPad.end()), true).c_str());
    ASSERT_STREQ(expectedSglPad.c_str(), BaseSixFour::encodeMIME(std::vector<uint8_t>(inSglPad.begin(), inSglPad.end()), true).c_str());
    ASSERT_STREQ(expectedDblPad.c_str(), BaseSixFour::encodeMIME(std::vector<uint8_t>(inDblPad.begin(), inDblPad.end()), true).c_str());
}


TEST_F(BaseSixFourTests, MIME_DecodeText){

    BaseSixFour b64(BaseSixFour::MIME);


    std::string inNoPad =
            "V2hhdCBpcyBhIGNvdW50cnk/IEEgY291bnRyeSBpcyBhIHBpZWNlIG9mIGxhbmQgc3Vycm91bmRl\r\n"
            "ZCBvbiBhbGwgc2lkZXMgYnkgYm91bmRhcmllcywgdXN1YWxseSB1bm5hdHVyYWwu";

    std::string expectedNoPad =
            "What is a country? A country is a piece of land surrounded on all "
            "sides by boundaries, usually unnatural.";

    //from: https://en.wikipedia.org/wiki/Base64#Examples
    std::string inSglPad =
        "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlz\r\n"
        "IHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2Yg\r\n"
        "dGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGlu\r\n"
        "dWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRo\r\n"
        "ZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=";

    std::string expectedSglPad =
        "Man is distinguished, not only by his reason, but by this singular passion from "
        "other animals, which is a lust of the mind, that by a perseverance of delight "
        "in the continued and indefatigable generation of knowledge, exceeds the short "
        "vehemence of any carnal pleasure.";

    // Bowie - "Five Years"
    std::string inDblPad =
            "TmV3cyBndXkgd2VwdCB3aGVuIGhlIHRvbGQgdXMgRWFydGggd2FzIHJlYWxseSBkeWluZyAvIENy\r\n"
            "aWVkIHNvIG11Y2ggdGhhdCBoaXMgZmFjZSB3YXMgd2V0IC8gVGhlbiBJIGtuZXcgaGUgd2FzIG5v\r\n"
            "dCBseWluZw==";

    std::string expectedDblPad =
            "News guy wept when he told us Earth was really dying / "
            "Cried so much that his face was wet / "
            "Then I knew he was not lying";

    // object tests
    ASSERT_EQ(std::vector<uint8_t> (expectedNoPad.begin(), expectedNoPad.end()), b64.decode(inNoPad));
    ASSERT_EQ(std::vector<uint8_t> (expectedSglPad.begin(), expectedSglPad.end()), b64.decode(inSglPad));
    ASSERT_EQ(std::vector<uint8_t> (expectedDblPad.begin(), expectedDblPad.end()), b64.decode(inDblPad));

    // static tests
    ASSERT_EQ(std::vector<uint8_t> (expectedNoPad.begin(), expectedNoPad.end()), BaseSixFour::decodeMIME(inNoPad));
    ASSERT_EQ(std::vector<uint8_t> (expectedSglPad.begin(), expectedSglPad.end()),BaseSixFour::decodeMIME(inSglPad));
    ASSERT_EQ(std::vector<uint8_t> (expectedDblPad.begin(), expectedDblPad.end()), BaseSixFour::decodeMIME(inDblPad));

}

TEST_F(BaseSixFourTests, MIME_EncodeFile){

    BaseSixFour b64(BaseSixFour::MIME);

    std::string encoded;
    std::string expected =
        "/9j/4AAQSkZJRgABAQAASABIAAD/4QBYRXhpZgAATU0AKgAAAAgAAgESAAMAAAABAAEAAIdpAAQA\r\n"
        "AAABAAAAJgAAAAAAA6ABAAMAAAABAAEAAKACAAQAAAABAAAAEKADAAQAAAABAAAAEAAAAAD/7QA4\r\n"
        "UGhvdG9zaG9wIDMuMAA4QklNBAQAAAAAAAA4QklNBCUAAAAAABDUHYzZjwCyBOmACZjs+EJ+/8AA\r\n"
        "EQgAEAAQAwERAAIRAQMRAf/EAB8AAAEFAQEBAQEBAAAAAAAAAAABAgMEBQYHCAkKC//EALUQAAIB\r\n"
        "AwMCBAMFBQQEAAABfQECAwAEEQUSITFBBhNRYQcicRQygZGhCCNCscEVUtHwJDNicoIJChYXGBka\r\n"
        "JSYnKCkqNDU2Nzg5OkNERUZHSElKU1RVVldYWVpjZGVmZ2hpanN0dXZ3eHl6g4SFhoeIiYqSk5SV\r\n"
        "lpeYmZqio6Slpqeoqaqys7S1tre4ubrCw8TFxsfIycrS09TV1tfY2drh4uPk5ebn6Onq8fLz9PX2\r\n"
        "9/j5+v/EAB8BAAMBAQEBAQEBAQEAAAAAAAABAgMEBQYHCAkKC//EALURAAIBAgQEAwQHBQQEAAEC\r\n"
        "dwABAgMRBAUhMQYSQVEHYXETIjKBCBRCkaGxwQkjM1LwFWJy0QoWJDThJfEXGBkaJicoKSo1Njc4\r\n"
        "OTpDREVGR0hJSlNUVVZXWFlaY2RlZmdoaWpzdHV2d3h5eoKDhIWGh4iJipKTlJWWl5iZmqKjpKWm\r\n"
        "p6ipqrKztLW2t7i5usLDxMXGx8jJytLT1NXW19jZ2uLj5OXm5+jp6vLz9PX29/j5+v/bAEMAAQEB\r\n"
        "AQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEB\r\n"
        "AQEBAf/bAEMBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEB\r\n"
        "AQEBAQEBAQEBAQEBAQEBAf/dAAQAAv/aAAwDAQACEQMRAD8A/mW+AX7OvwZn+DvhnUNT8KaB4v1P\r\n"
        "xx4S0+/1/XNTiN/dpPqttdXM1lpE73Vz/wAI1eaINTm0WS98OSaPqMlxpdvd3jpqVrE8H8M+I/id\r\n"
        "x1T44zXDYTOMyyXCZBnWJw+W5fhJ/VqEqeEq0aUMRjacaVNZrRzD6rDHxoZpHGYWNLF1KNGLwlac\r\n"
        "J/8AVP8AQu+gx9FXGfRa8P8AO+IfDfgzxO4g8XvDPJM5414vz/DyznM6WL4iwOY4/FZTwzjKuY49\r\n"
        "8A5nwks/xPClbNeBKvC+e1sbw/gszzSvHiDLsPVwH5Z/GLwdo/w++J3jPwb4f1hdc0fQdYktLK+D\r\n"
        "xSSxq8MNxLpd5JAzRS6joc88uianIiweZqOn3Tm1s2LWkH9ecD55juJOE8izzMsE8vxuY4KNavh+\r\n"
        "WcYSaqVKUMXQjU9+GFzCnThj8JCUqnLhsVSiq1ZJVp/86v0pPCzhbwT+kH4q+FfBXE8OL+GODOKK\r\n"
        "2WZVnCq4aviKNOphMJjsRw9mdfCVJ4evnvCGMxeJ4S4gr04YRV88yTMKssuyuc5Zbhf/0P4VfCPx\r\n"
        "x+LXgTQb7wx4U8d65pGg6hbS2h05JYbqLTop2vpJ38PyX0F1P4YuZ59Su7qa98OzaVeS3jx3sk7X\r\n"
        "dtazwfH514f8GcQ5jh82zjh7L8bmOHqwrLFShUozxM6aw8aazKOHqUaebUqdPC0KVOhmkcZQhQjP\r\n"
        "DxpKjVqwn/R/hn9L36S/g7wZnHh74b+MfF/DPBudYDE5bLIqWIweY4fI8PjZ5xWxlTgqtnGEzHGe\r\n"
        "H2YYvF57meY4rNeBcTw5mmIzWrQzatjJ5lgcvxeH8rd3kd5JHaSR2Z3d2LO7sSWd2YlmZiSWYkkk\r\n"
        "kknOa+vjGMYqMUoxilGMYpKMYpWSSVkklZJJWS0Vj+dKtWrXq1K9epUrVq1SdWtWqzlUq1atSTnU\r\n"
        "qVKkm5TqTm3Kc5NylJtttts//9k=";

    //read image from file
    std::ifstream ifs(DATA_DIR"/icon.jpg", std::ios::binary);
    ifs.seekg(0, std::ios::end);
    std::vector<char> charVec(ifs.tellg());
    ifs.seekg(0, std::ios::beg);
    ifs.read(&charVec[0], charVec.size());

    encoded =  b64.encode(std::vector<uint8_t>(charVec.begin(), charVec.end()), true);

    ASSERT_STREQ(expected.c_str(), encoded.c_str());
}

TEST_F(BaseSixFourTests, MIME_SanitizeDecodeInput){

    BaseSixFour b64(BaseSixFour::MIME);

    std::string input =
            "WcYSaqVKUMXQjU9+GFzCnThj8JCUqnLhsVSiq1ZJVp/86v0pPCzhbwT+kH4q+FfBXE8OL+GODOKK\r\n"
            "2WZVnCq4aviKNOphMJjsRw9mdfCVJ4evnvCGMxeJ4S4gr04YRV88yTMKssuyuc5Zbhf/0P4VfCPx\r\n"
            "x+LXgTQb7wx4U8d65pGg6hbS2h05JYbqLTop2vpJ38PyX0F1P4YuZ59Su7qa98OzaVeS3jx3sk7X\r\n"
            "dtazwfH514f8GcQ5jh82zjh7L8bmOHqwrLFShUozxM6aw8aazKOHqUaebUqdPC0KVOhmkcZQhQjP\r\n"
            "DxpKjVqwn/R/hn9L36S/g7wZnHh74b+MfF/DPBudYDE5bLIqWIweY4fI8PjZ5xWxlTgqtnGEzHGe\r\n"
            "H2YYvF57meY4rNeBcTw5mmIzWrQzatjJ5lgcvxeH8rd3kd5JHaSR2Z3d2LO7sSWd2YlmZiSWYkkk\r\n"
            "kknOa+vjGMYqMUoxilGMYpKMYpWSSVkklZJJWS0Vj+dKtWrXq1K9epUrVq1SdWtWqzlUq1atSTnU\r\n"
            "qVKkm5TqTm3Kc5NylJtttts//9k=";

    std::string expected =
            "WcYSaqVKUMXQjU9+GFzCnThj8JCUqnLhsVSiq1ZJVp/86v0pPCzhbwT+kH4q+FfBXE8OL+GODOKK"
            "2WZVnCq4aviKNOphMJjsRw9mdfCVJ4evnvCGMxeJ4S4gr04YRV88yTMKssuyuc5Zbhf/0P4VfCPx"
            "x+LXgTQb7wx4U8d65pGg6hbS2h05JYbqLTop2vpJ38PyX0F1P4YuZ59Su7qa98OzaVeS3jx3sk7X"
            "dtazwfH514f8GcQ5jh82zjh7L8bmOHqwrLFShUozxM6aw8aazKOHqUaebUqdPC0KVOhmkcZQhQjP"
            "DxpKjVqwn/R/hn9L36S/g7wZnHh74b+MfF/DPBudYDE5bLIqWIweY4fI8PjZ5xWxlTgqtnGEzHGe"
            "H2YYvF57meY4rNeBcTw5mmIzWrQzatjJ5lgcvxeH8rd3kd5JHaSR2Z3d2LO7sSWd2YlmZiSWYkkk"
            "kknOa+vjGMYqMUoxilGMYpKMYpWSSVkklZJJWS0Vj+dKtWrXq1K9epUrVq1SdWtWqzlUq1atSTnU"
            "qVKkm5TqTm3Kc5NylJtttts//9k";

   ASSERT_STREQ(expected.c_str(), b64.sanitize(input, BaseSixFour::MIME.charset()).c_str());

}
