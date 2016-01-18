#include "BaseSixFour.h"

const BaseSixFour::Variant BaseSixFour::MIME = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/",
    '=',
    76,
    "\r\n"
};


BaseSixFour::BaseSixFour(const Variant &var, bool enforceLineLen){

    this->_charset = var._charset;
    this->_padChar = var._padChar;
    this->_maxLineLen = var._maxLineLen;
    this->_lineTerminus = var._lineTerminus;
    this->_enforceMaxLen = enforceLineLen;
}

std::string BaseSixFour::encode(const std::vector<uint8_t> &in) const{

    using namespace std;

    // the Base64-encoded character string to return
    string ret = "";

    // the total size of the Base64 character string
    size_t totEncodedChars = 0;

    // the container for the three octets to encode
    uint8_t inOcts[] = {0, 0, 0};

    // the current set of three octets
    // encoded as four Base64 characters
    string encodedChars = "";

    for(size_t i = 0; i < in.size(); i+=DECODED_OCTETS){

        // the end of the input data has not been reached
        if(i+DECODED_OCTETS-1 < in.size()){
            inOcts[0] = in.data()[i];
            inOcts[1] = in.data()[i+1];
            inOcts[2] = in.data()[i+2];
            encodedChars = encodeOctets(&inOcts[0]);
        }
        // there are only two remaining octets
        else if(i+DECODED_OCTETS-2 < in.size()){
            inOcts[0] = in.data()[i];
            inOcts[1] = in.data()[i+1];
            inOcts[2] = 0;
            // there will only be three significant encoded characters
            // add a padding character to ensure length = 4
            encodedChars = encodeOctets(&inOcts[0]).substr(0, 3) + _padChar;
        }
        // there is only one remaining octet
        else{
            inOcts[0] = in.data()[i];
            inOcts[1] = 0;
            inOcts[2] = 0;
            // there will only be two significant encoded characters
            // add two padding characters to ensure length = 4
            encodedChars = encodeOctets(&inOcts[0]).substr(0, 2) + _padChar + _padChar;
        }

        // if we're enforcing the maximum line length and
        // appending the next encoded sequence will exceed the line length
        if(_enforceMaxLen &&
           (totEncodedChars + ENCODED_CHARS) / _maxLineLen > totEncodedChars / _maxLineLen){
            // look for where the line should end
            for(unsigned int j = 0; j < ENCODED_CHARS; j++){
                //  append single character
                ret += encodedChars.at(j);
                // the end of the line has been reached
                if((totEncodedChars + j+1) % _maxLineLen == 0){
                    // add the line terminating characters
                    ret += _lineTerminus;
                }
            }
        }
        // maximium line length is not enfored or
        // the maximum line length will not be exceed by the current characters
        // append all of the encoded charaters
        else{
            ret += encodedChars;
        }

        // update count
        totEncodedChars += ENCODED_CHARS;
    }

    return ret;
}

std::string BaseSixFour::sanitize(std::string &in) const{

    using namespace std;

    regex b64Exp("[^" + _charset + _padChar + "]");
    return regex_replace(in, b64Exp, "");

}

std::vector<uint8_t> BaseSixFour::decode(const std::string &in){

    using namespace std;

    // the decoded data to return
    vector<uint8_t> ret;

    return ret;

}

/**
 * @brief BaseSixFour::encodeOctets
 * @param in a pointer to no more than three uint8_t octets,
 *          any additional octets will be ignored. Fewer than
 *          three octets causes undefined behavior
 * @return a string of four Base64 encoded characters
 */

std::string BaseSixFour::encodeOctets(const uint8_t *in) const{

    using namespace std;

    string ret = "";

    // byte 0: shift off two LSBs (least significant bit)
    ret += _charset.at(in[0] >> 2);
    // byte 0: shift off 4 MSBs (most significant bit)
    //  and mask for 6 LSBs
    // byte 1: shift off 4 LSBs
    // add results of operations on byte 0 and byte 1
    ret += _charset.at(((in[0] << 4) & 0x3f) + (in[1] >> 4));
    // byte 1: shift off 2 MSBs
    // byte 2: shift off 6 LSBs
    // add results of operations on byte 1 and byte 2
    //  and mask for 6 LSBs
    ret += _charset.at(((in[1] << 2) + (in[2] >> 6)) & 0x3f);
    // byte 2: mask for 6 LSBs
    ret += _charset.at(in[2] & 0x3f);

    return ret;

}

uint8_t* BaseSixFour::decodeCharacters(const std::string &in) const{

    // the decoded data
    uint8_t* ret = new uint8_t[DECODED_OCTETS];

    // with this approach, in must be sanitized for non-base64 chars
    char encoded[] = {
        _charset.data()[_charset.find(in.at(0))],
        _charset.data()[_charset.find(in.at(1))],
        _charset.data()[_charset.find(in.at(2))],
        _charset.data()[_charset.find(in.at(3))]
    };

    ret[0] = (encoded[0] << 2) + (encoded[1] & 0x03);
    ret[1] = (encoded[1] << 6) + (encoded[2] >> 6);
    ret[2] = (encoded[2] << 6) + (encoded[3]); //unnecessary to mask if this is a base64 char

    return ret;

}
