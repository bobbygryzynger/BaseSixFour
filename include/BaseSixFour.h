#ifndef BASESSIXFOUR_H
#define BASESSIXFOUR_H

#include <string>

class BaseSixFour
{

    public:

        static const struct CHARSET {
            static const std::string MIME;
        } CHARSET;

        static std::string encode(std::string in);

        BaseSixFour();
};

#endif // BASESSIXFOUR_H
