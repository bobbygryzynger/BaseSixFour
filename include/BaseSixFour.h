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
        static const unsigned int LINE_LEN;
        static const std::string LINE_TERM;

        static std::string encode(const std::vector<uint8_t> &in);

        BaseSixFour();

    private:
        static const unsigned int ENCODE_OCTETS = 3;
        static const unsigned int ENCODED_SIZE = 4;

        static std::string encodeOctetSet(uint8_t *in);
};

#endif // BASESSIXFOUR_H
