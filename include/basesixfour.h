/**
* \file
* \author Bobby Gryzynger <bobby.gryzynger@gmail.com>
* \version 0.0.1
*
* \section LICENSE
* Copyright (c) 2016 Bobby Gryzynger
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the Software
* is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies
* or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*
*/

#ifndef BASESIXFOUR_H
#define BASESIXFOUR_H

#include <string>
#include <inttypes.h>
#include <vector>
#include <regex>

/**
 * \brief The BaseSixFour class
 */

class BaseSixFour{

    public:

        /**
         * \brief Variant
         * \brief Class for defining Base64 Variants.
         *
         * \var Variant::_charset
         * The character set to use for encoding and decoding
         *      operations.
         *
         * \var Variant::_pad
         * The character to use for padding when necessary.
         *
         * \var Variant::_maxLn
         * The maximum length of an encoded line.
         *
         * \var Variant::_lnTerm
         * The character string to use when terminating encoded
         *      lines.
         */
        class Variant{

            friend class BaseSixFour;

            public:

                /**
                 * \brief Default constructor.
                 *
                 * Constructs the variant as BaseSixFour::MIME.
                 */
                Variant();

                /**
                 * \brief Constructor which allow for custom Variant creation.
                 *
                 * \param chars
                 * The character set to use.
                 *
                 * \param pad
                 * The padding chracter to use.
                 *
                 * \param maxLn
                 * The maximum length of an encoded line.
                 *
                 * \param lnTerm
                 * The character string to use when terminating encoded
                 *      lines.
                 */
                Variant(std::string chars, char pad, size_t maxLn, std::string lnTerm);

            private:
                std::string _charset;
                char _pad;
                size_t _maxLn;
                std::string _lnTerm;
        };

        /**
         * \brief Constructor that sets the Base64 Variant to use.
         *
         * \param var
         * A BaseSixFour Variant, defaults to BaseSixFour::MIME.
         *
         */
        BaseSixFour(const Variant &var = BaseSixFour::MIME);

        /**
         * \brief MIME The Base64 MIME Variant
         *
         * For more information see this
         * <a href="https://en.wikipedia.org/wiki/Base64#Variants_summary_table">variant table</a>.
         */
        static const Variant MIME;

        /**
         * \brief Encodes a vector of data as a Base64 string.
         *
         * \param in
         * A vector of data to Base64 encode.
         *
         * \param enforceLineLen
         * A boolean for whether or not to enforce
         *      a Variant's max line length, defaults to <b>true</b>.
         *
         * \return The data encoded as a Base64 string.
         */
        std::string encode(const std::vector<uint8_t> &in, bool enforceMaxLen = true) const;

        /**
         * \brief Decodes an input string into its original data.
         *
         * If \p sanitizeInput is <b>false</b>, the caller is responsible
         *      for ensuring that \p in doesn't contain any characters outside
         *      of the current character set.
         *
         * \param in
         * The input string to decode.
         *
         * \param sanitizeInput
         * Whether or not the input string should be sanitized, defaults to <b>true</b>.
         *
         * \return A vector of unencoded data.
         *
         * \throws std::runtime_error if any of the input characters
         *      cannot be found in the current character set.
         */
        std::vector<uint8_t> decode(const std::string &in, bool sanitizeInput = true) const;

        /**
         * \brief Sanitizes a string for all non-Base64 characters, including
         *      padding characters, making it safe to use in a decoding
         *      operation.
         *
         * \param in
         * The input string to sanitize.
         *
         * \return A string stripped of all non-Base64 characters
         *      according to the current character set.
         */
        std::string sanitize(const std::string &in) const;

    private:

        /**
         * \brief The number of input octets to encode as
         *      Base64 characters.
         */
        static const unsigned int DECODED_OCTETS = 3;

        /**
         * \brief The number of encoded characters resulting
         *      from Base64 encoding a set of octets with a
         *      length of BaseSixFour::DECODED_OCTETS.
         */
        static const unsigned int ENCODED_CHARS = 4;

        /**
         * \brief The length of a Base64 character set.
         */
        static const unsigned int BASE = 64;

        Variant _var;

        /**
         * \brief Encodes three octets as four Base64 characters using
         *          using the current character set.
         *
         * \param in
         * An array of three octets to encode.
         *
         * \return A string of four Base64 encoded characters in the
         *          current character set.
         */
        std::string encodeOctets(const uint8_t (&in)[3]) const;

        /**
         * \brief Decodes four Basae64 input characters as their
         *      original data values.
         *
         * \param in
         * An array of four characters to decode.
         *
         * \param ret
         * A reference to an array where the decoded output
         *      will be stored.
         *
         * \throws std::runtime_error if any of the input characters
         *      cannot be found in the current character set.
         */
        void decodeCharacters(const char (&in)[4], uint8_t (&ret)[3]) const;
};

#endif // BASESIXFOUR_H
