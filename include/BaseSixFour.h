#ifndef BASESSIXFOUR_H
#define BASESSIXFOUR_H

#include <string>
#include <iostream>
#include <inttypes.h>
#include <vector>

class BaseSixFour
{

    public:

        static const struct CHARSET {
            static const std::string MIME;
        } CHARSET;

        static const char PAD_CHAR;
        static const uint LINE_LEN;
        static const std::string LINE_TERM;

        static std::string encode(const std::vector<uint8_t> &in);
        static std::string encodeOctetSet(uint8_t *in);

        BaseSixFour();

    private:
        static const uint ENCODE_OCTETS = 3;
        static const uint ENCODED_SIZE = 4;
};

#endif // BASESSIXFOUR_H
