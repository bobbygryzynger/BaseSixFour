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

        static std::string encode(const std::vector<uint8_t> &in);
        static std::string encodeOctets(uint8_t *in, uint len);

        BaseSixFour();

    private:
        static const uint ENCODE_OCTETS = 3;
        static const uint ENCODED_SIZE = 4;
};

#endif // BASESSIXFOUR_H
