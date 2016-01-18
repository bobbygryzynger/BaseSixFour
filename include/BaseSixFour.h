#ifndef BASESIXFOUR_H
#define BASESIXFOUR_H

#include <string>
#include <iostream>
#include <inttypes.h>
#include <vector>

class BaseSixFour{

    public:

        typedef const struct{
            public:
                std::string _charset;
                char _padChar;
                size_t _maxLineLen;
                std::string _lineTerminus;
        }Variant;

        static const Variant MIME;

        BaseSixFour(const Variant &var = BaseSixFour::MIME, bool enforceLineLen = true);
        std::string encode(const std::vector<uint8_t> &in) const;

    private:

        static const unsigned int ENCODE_OCTETS = 3;
        static const unsigned int ENCODED_SIZE = 4;

        std::string _charset;
        char _padChar;
        size_t _maxLineLen;
        std::string _lineTerminus;
        bool _enforceMaxLen;

        std::string encodeOctets(const uint8_t *in) const;
};

#endif // BASESIXFOUR_H
