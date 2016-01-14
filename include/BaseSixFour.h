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

        static std::string encode(std::vector<uint8_t> in);
        static std::string encodeOctets(uint8_t *in, uint len);

        BaseSixFour();
};

#endif // BASESSIXFOUR_H
