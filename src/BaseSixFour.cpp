#include "BaseSixFour.h"

//const std::string BaseSixFour::_var._charset =  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
//                                                "abcdefghijklmnopqrstuvwxyz"
//                                                "0123456789+/";
//const char BaseSixFour::_var._padchar = '=';
//const unsigned int BaseSixFour::_var._maxlen = 76;
//const std::string BaseSixFour::_var._lineterm = "\r\n";

char BaseSixFour::MIME::_padchar = '=';
std::string BaseSixFour::MIME::_lineterm = "\r\n";
unsigned int BaseSixFour::MIME::_maxlen = 76;
std::string BaseSixFour::MIME::_charset =   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                            "abcdefghijklmnopqrstuvwxyz"
                                            "0123456789+/";


BaseSixFour::BaseSixFour(BaseSixFour::Variant var){
    this->_var = var;
}

std::string BaseSixFour::encode(const std::vector<uint8_t> &in) const{

    using namespace std;

    // the Base64-encoded character string to return
    string ret = "";

    // the total size of the Base64 character string
    size_t totEncodedChars = 0;

    // the container for the three octets to encode
    uint8_t inOcts[] = {0,0,0};

    // the current set of three octets
    // encoded as four Base64 characters
    string encodedChars = "";

    for(size_t i = 0; i < in.size(); i+=ENCODE_OCTETS){

        // the end of the input data has not been reached
        if(i+ENCODE_OCTETS-1 < in.size()){
            inOcts[0] = in.data()[i];
            inOcts[1] = in.data()[i+1];
            inOcts[2] = in.data()[i+2];
            encodedChars = encodeOctets(&inOcts[0]);
        }
        // there are only two remaining octets
        else if(i+ENCODE_OCTETS-2 < in.size()){
            inOcts[0] = in.data()[i];
            inOcts[1] = in.data()[i+1];
            inOcts[2] = 0;
            // there will only be three significant encoded charaters
            // add a padding character to ensure length = 4
            encodedChars = encodeOctets(&inOcts[0]).substr(0, 3) + _var._padchar;
        }
        // there is only one remaining octet
        else{
            inOcts[0] = in.data()[i];
            inOcts[1] = 0;
            inOcts[2] = 0;
            // there will only be two significant encoded charaters
            // add two padding characters to ensure length = 4
            encodedChars = encodeOctets(&inOcts[0]).substr(0, 2) + _var._padchar + _var._padchar;
        }

        // appending the next encoded sequence will exceed the line length
        if((totEncodedChars + ENCODED_SIZE) / _var._maxlen > totEncodedChars / _var._maxlen){
            // look for where the line should end
            for(unsigned int j = 0; j < ENCODED_SIZE; j++){
                //  append single character
                ret += encodedChars.at(j);
                // the end of the line has been reached
                if((totEncodedChars + j+1) % _var._maxlen == 0){
                    // add the line terminating characters
                    ret += _var._lineterm;
                }
            }
        }
        // the maximum line length will not be exceed by the current characters
        // append all of the encoded charaters
        else{
            ret += encodedChars;
        }

        // update count
        totEncodedChars += ENCODED_SIZE;
    }

    return ret;
}
/**
 * @brief BaseSixFour::encodeOctets
 * @param in a pointer to no more than three uint8_t octets,
 *          any additional octets will be ignored. Fewer than
 *          three octets causes undefined behavior
 * @return a string of four Base64 encoded characters
 */

std::string BaseSixFour::encodeOctets(uint8_t *in) const{

    using namespace std;

    string ret = "";

    // byte 0: shift off two LSBs (least significant bit)
    ret += _var._charset.at(in[0] >> 2);
    // byte 0: mask first 2 LSBs and shift off 4 MSBs (most significant bit)
    //  leaving only two original bits
    // byte 1: shift off 4 LSBs
    // add results of operations on byte 0 and byte 1
    ret += _var._charset.at(((in[0] & 0x03) << 4) + (in[1] >> 4));
    // byte 1: shift off 2 MSBs
    // byte 2: shift off 6 LSBs
    // add results of operations on byte 1 and byte 2
    // mask for 6 LSBs
    ret += _var._charset.at(((in[1] << 2) + (in[2] >> 6)) & 0x3f);
    //byte 2: mask for 6 LSBs
    ret += _var._charset.at(in[2] & 0x3f);

    return ret;

}
