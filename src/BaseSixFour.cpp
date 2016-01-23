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
    uint8_t inOcts[3];

    // the current set of three octets
    // encoded as four Base64 characters
    string encodedChars = "";

    for(size_t i = 0; i < in.size(); i+=DECODED_OCTETS){

        // the end of the input data has not been reached
        if(i+DECODED_OCTETS-1 < in.size()){
            inOcts[0] = in.data()[i];
            inOcts[1] = in.data()[i+1];
            inOcts[2] = in.data()[i+2];
            encodedChars = encodeOctets(inOcts);
        }
        // there are only two remaining octets
        else if(i+DECODED_OCTETS-2 < in.size()){
            inOcts[0] = in.data()[i];
            inOcts[1] = in.data()[i+1];
            inOcts[2] = 0;
            // there will only be three significant encoded characters
            // add a padding character to ensure length = 4
            encodedChars = encodeOctets(inOcts).substr(0, 3) + _padChar;
        }
        // there is only one remaining octet
        else{
            inOcts[0] = in.data()[i];
            inOcts[1] = 0;
            inOcts[2] = 0;
            // there will only be two significant encoded characters
            // add two padding characters to ensure length = 4
            encodedChars = encodeOctets(inOcts).substr(0, 2) + _padChar + _padChar;
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
        // append all of the encoded characters
        else{
            ret += encodedChars;
        }

        // update count
        totEncodedChars += ENCODED_CHARS;
    }

    return ret;
}

std::string BaseSixFour::sanitize(const std::string &in) const{

    using namespace std;

    regex notB64("[^" + _charset + "]");
    return regex_replace(in, notB64, "");

}

std::vector<uint8_t> BaseSixFour::decode(const std::string &in, bool sanitizeInput) const {

    using namespace std;

    string encoded = sanitizeInput ? sanitize(in) : in;

    // the decoded data to return
    vector<uint8_t> ret;
    ret.reserve(encoded.length() * (DECODED_OCTETS/ENCODED_CHARS));

    size_t padPos = encoded.find(_padChar, encoded.length()-2);
    size_t encodedLen = padPos != string::npos ? padPos : encoded.length();

    char encodedChs[4];
    uint8_t decodedBytes[3];

    for(size_t i = 0; i < encodedLen; i+=ENCODED_CHARS){

        if(i+ENCODED_CHARS-1 < encodedLen){
            encodedChs[0] = encoded.data()[i];
            encodedChs[1] = encoded.data()[i+1];
            encodedChs[2] = encoded.data()[i+2];
            encodedChs[3] = encoded.data()[i+3];
            decodeCharacters(encodedChs, decodedBytes);
            ret.insert(ret.end(), decodedBytes, decodedBytes+3);
        }else if(i+ENCODED_CHARS-2 < encodedLen){
            encodedChs[0] = encoded.data()[i];
            encodedChs[1] = encoded.data()[i+1];
            encodedChs[2] = encoded.data()[i+2];
            decodeCharacters(encodedChs, decodedBytes);
            ret.insert(ret.end(), decodedBytes, decodedBytes+2);
        }else{
            encodedChs[0] = encoded.data()[i];
            encodedChs[1] = encoded.data()[i+1];
            decodeCharacters(encodedChs, decodedBytes);
            ret.insert(ret.end(), decodedBytes, decodedBytes+1);
        }

    }

    return ret;

}

std::string BaseSixFour::encodeOctets(const uint8_t (&in)[3]) const{

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

void BaseSixFour::decodeCharacters(const char (&in)[4], uint8_t (&ret)[3]) const{

    using namespace std;

    size_t encodedIdx[] = {
        _charset.find(in[0]),
        _charset.find(in[1]),
        _charset.find(in[2]),
        _charset.find(in[3])
    };

    // Base64 characters were found for all
    // passed characters
    if(encodedIdx[0] != string::npos &&
       encodedIdx[1] != string::npos &&
       encodedIdx[2] != string::npos &&
       encodedIdx[3] != string::npos){
        // all operations assume bits 7 & 8 are not significant
        // and are not present
        ret[0] = (encodedIdx[0] << 2) + (encodedIdx[1] >> 4);
        ret[1] = (encodedIdx[1] << 4) + (encodedIdx[2] >> 2);
        ret[2] = (encodedIdx[2] << 6) + (encodedIdx[3]);
        return;
    }


    throw std::runtime_error("The input data [" + string(in) + "] "
                            "contains characters outside of the current character set");
}
