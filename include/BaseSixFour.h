#ifndef BASESIXFOUR_H
#define BASESIXFOUR_H

#include <string>
#include <iostream>
#include <inttypes.h>
#include <vector>
#include <regex>

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
        std::string sanitize(const std::string &in) const;
        std::vector<uint8_t> decode(const std::string &in, bool sanitizeInput = true) const;

    private:

        static const unsigned int DECODED_OCTETS = 3;
        static const unsigned int ENCODED_CHARS = 4;
        static const unsigned int BASE = 64;

        std::string _charset;
        char _padChar;
        size_t _maxLineLen;
        std::string _lineTerminus;
        bool _enforceMaxLen;

        std::string encodeOctets(const uint8_t (&in)[3]) const;
        bool decodeCharacters(const char (&in)[4], uint8_t (&ret)[3]) const;
};

#endif // BASESIXFOUR_H
