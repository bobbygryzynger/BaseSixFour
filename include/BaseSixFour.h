#ifndef BASESSIXFOUR_H
#define BASESSIXFOUR_H

#include <string>
#include <iostream>
#include <inttypes.h>
#include <vector>

typedef struct Variant{
        std::string _charset;
        char _padchar;
        unsigned int _maxlen;
        std::string _lineterm;
}Variant;

class BaseSixFour{

    public:

//        static const struct CHARSET {
//            static const std::string MIME;
//        } CHARSET;

        static Variant MIME;

//        static const char PAD_CHAR;
//        static const unsigned int MAX_LINE_LEN;
//        static const std::string LINE_TERM;

        std::string encode(const std::vector<uint8_t> &in) const;

        BaseSixFour(Variant var = BaseSixFour::MIME);

    private:

        friend class Variant;
        Variant _var;

        static const unsigned int ENCODE_OCTETS = 3;
        static const unsigned int ENCODED_SIZE = 4;

        std::string encodeOctets(uint8_t *in) const;
};

#endif // BASESSIXFOUR_H
