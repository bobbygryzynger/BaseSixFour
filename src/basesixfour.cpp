#include "basesixfour.h"

const BaseSixFour::Variant BaseSixFour::MIME(
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/",
    '=',
    76,
    "\r\n"
);

BaseSixFour::Variant::Variant() :
    Variant(BaseSixFour::MIME._charset,
            BaseSixFour::MIME._pad,
            BaseSixFour::MIME._maxLn,
            BaseSixFour::MIME._lnTerm){}

BaseSixFour::Variant::Variant(std::string chars, char pad, size_t maxLn, std::string lnTerm){
    this->_charset = chars;
    this->_pad = pad;
    this->_maxLn = maxLn;
    this->_lnTerm = lnTerm;
}

std::string BaseSixFour::Variant::charset() const{
    return _charset;
}

BaseSixFour::BaseSixFour(const Variant &var){
    this->_var = var;
}

std::string BaseSixFour::encodeMIME(const std::vector<uint8_t> &in, bool enforceMaxLen){
    return BaseSixFour::encode(in, MIME._charset, MIME._pad, MIME._maxLn, MIME._lnTerm, enforceMaxLen);
}

std::string BaseSixFour::encode(const std::vector<uint8_t> &in, bool enforceMaxLen) const{
    return encode(in, _var._charset, _var._pad, _var._maxLn, _var._lnTerm, enforceMaxLen);
}

std::string BaseSixFour::encode(const std::vector<uint8_t> &in, std::string charset, char pad,
                                size_t maxLn, std::string lnTerm, bool enforce){

    using namespace std;

    // the Base64-encoded character string to return
    string ret = "";
    ret.reserve(in.size() * (ENCODED_CHARS/DECODED_OCTETS));

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
            encodedChars = encodeOctets(inOcts, charset);
        }
        // there are only two remaining octets
        else if(i+DECODED_OCTETS-2 < in.size()){
            inOcts[0] = in.data()[i];
            inOcts[1] = in.data()[i+1];
            inOcts[2] = 0;
            // there will only be three significant encoded characters
            // add a padding character to ensure length = 4
            encodedChars = encodeOctets(inOcts, charset).substr(0, 3) + pad;
        }
        // there is only one remaining octet
        else{
            inOcts[0] = in.data()[i];
            inOcts[1] = 0;
            inOcts[2] = 0;
            // there will only be two significant encoded characters
            // add two padding characters to ensure length = 4
            encodedChars = encodeOctets(inOcts, charset).substr(0, 2) + pad + pad;
        }

        // if we're enforcing the maximum line length and we have appropriate
        // line termination values
        if(enforce && maxLn > 0 && !lnTerm.empty()){
            appendEncoded(encodedChars, totEncodedChars, maxLn, lnTerm, ret);
        }else{
            ret += encodedChars;
        }

        // update count
        totEncodedChars += ENCODED_CHARS;
    }

    return ret;



}

std::string BaseSixFour::sanitize(const std::string &in, const std::string &charset){

    using namespace std;

    if(charset.length() != BASE){
        throw runtime_error("The character set must have exactly "
                            + to_string(BASE) + " characters");
    }

    regex notB64("[^" + charset + "]");
    return regex_replace(in, notB64, "");

}

std::vector<uint8_t> BaseSixFour::decode(const std::string &in, bool sanitizeInput) const {
    return BaseSixFour::decode(in, _var._charset, _var._pad, sanitizeInput);
}

std::vector<uint8_t> BaseSixFour::decode(const std::string &in, const std::string &charset, char pad, bool sanitizeInput){

    using namespace std;

    string encoded = sanitizeInput ? sanitize(in, charset) : in;

    // the decoded data to return
    vector<uint8_t> ret;
    ret.reserve(encoded.length() * (DECODED_OCTETS/ENCODED_CHARS));

    size_t padPos = encoded.find(pad, encoded.length()-2);
    size_t encodedLen = padPos != string::npos ? padPos : encoded.length();

    char encodedChs[4];
    uint8_t decodedBytes[3];

    for(size_t i = 0; i < encodedLen; i+=ENCODED_CHARS){

        if(i+ENCODED_CHARS-1 < encodedLen){
            encodedChs[0] = encoded.data()[i];
            encodedChs[1] = encoded.data()[i+1];
            encodedChs[2] = encoded.data()[i+2];
            encodedChs[3] = encoded.data()[i+3];
            decodeCharacters(encodedChs, decodedBytes, charset);
            ret.insert(ret.end(), decodedBytes, decodedBytes+3);
        }else if(i+ENCODED_CHARS-2 < encodedLen){
            encodedChs[0] = encoded.data()[i];
            encodedChs[1] = encoded.data()[i+1];
            encodedChs[2] = encoded.data()[i+2];
            decodeCharacters(encodedChs, decodedBytes, charset);
            ret.insert(ret.end(), decodedBytes, decodedBytes+2);
        }else{
            encodedChs[0] = encoded.data()[i];
            encodedChs[1] = encoded.data()[i+1];
            decodeCharacters(encodedChs, decodedBytes, charset);
            ret.insert(ret.end(), decodedBytes, decodedBytes+1);
        }

    }

    return ret;


}

std::vector<uint8_t> BaseSixFour::decodeMIME(const std::string &in, bool sanitizeInput){
    return BaseSixFour::decode(in, MIME._charset, MIME._pad, sanitizeInput);
}

std::string BaseSixFour::encodeOctets(const uint8_t (&in)[3], const std::string &charset){

    using namespace std;

    if(charset.length() != BASE){
        throw runtime_error("The character set must have exactly "
                            + to_string(BASE) + " characters");
    }

    string ret = "";

    // byte 0: shift off two LSBs (least significant bit)
    ret += charset.at(in[0] >> 2);
    // byte 0: shift off 4 MSBs (most significant bit)
    //  and mask for 6 LSBs
    // byte 1: shift off 4 LSBs
    // add results of operations on byte 0 and byte 1
    ret += charset.at(((in[0] << 4) & 0x3f) + (in[1] >> 4));
    // byte 1: shift off 2 MSBs
    // byte 2: shift off 6 LSBs
    // add results of operations on byte 1 and byte 2
    //  and mask for 6 LSBs
    ret += charset.at(((in[1] << 2) + (in[2] >> 6)) & 0x3f);
    // byte 2: mask for 6 LSBs
    ret += charset.at(in[2] & 0x3f);

    return ret;

}

void BaseSixFour::decodeCharacters(const char (&in)[4], uint8_t (&ret)[3], const std::string &charset){

    using namespace std;

    if(charset.length() != BASE){
        throw runtime_error("The character set must have exactly "
                            + to_string(BASE) + " characters");
    }

    size_t encodedIdx[] = {
        charset.find(in[0]),
        charset.find(in[1]),
        charset.find(in[2]),
        charset.find(in[3])
    };

    // Base64 characters were found for all
    // passed characters
    if(encodedIdx[0] < BASE && encodedIdx[1] < BASE &&
       encodedIdx[2] < BASE && encodedIdx[3] < BASE){
        // all operations assume bits 7 & 8 are not significant
        // and are not present
        ret[0] = (encodedIdx[0] << 2) + (encodedIdx[1] >> 4);
        ret[1] = (encodedIdx[1] << 4) + (encodedIdx[2] >> 2);
        ret[2] = (encodedIdx[2] << 6) + (encodedIdx[3]);
        return;
    }


    throw runtime_error("The input data [" + string(in) + "] "
                        "contains characters outside of the "
                        "current character set");
}

void BaseSixFour::appendEncoded(const std::string &encodedChars, const size_t &encodedCt,
                                size_t maxLn, std::string lnTerm, std::string &ret){

    // if the next encoded sequence will exceed the line length
    if((encodedCt + ENCODED_CHARS) / maxLn > encodedCt / maxLn){
        // look for where the line should end
        for(unsigned int j = 0; j < ENCODED_CHARS; j++){
            //  append single character
            ret += encodedChars.at(j);
            // the end of the line has been reached
            if((encodedCt + j+1) % maxLn == 0){
                // add the line terminating characters
                ret += lnTerm;
            }
        }
    }
    // the maximum line length will not be exceed by the current characters
    // append all of the encoded characters
    else{
        ret += encodedChars;
    }

}
