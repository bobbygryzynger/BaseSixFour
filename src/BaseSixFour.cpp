#include "BaseSixFour.h"

const std::string BaseSixFour::CHARSET::MIME =  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                                "abcdefghijklmnopqrstuvwxyz"
                                                "0123456789+/";
const char BaseSixFour::PAD_CHAR = '=';
const unsigned int BaseSixFour::LINE_LEN = 76;
const std::string BaseSixFour::LINE_TERM = "\r\n";

std::string BaseSixFour::encode(const std::vector<uint8_t> &in){

    using namespace std;

    // the Base64-encoded character string to return
    string ret = "";

    // the total size of the Base64 character string
    size_t totEncodedChars = 0;

    // the container for the three octets to encode
    uint8_t* inPtr = new uint8_t[ENCODE_OCTETS];

    // the current set of three octets
    // encoded as four Base64 characters
    string encodedChars = "";

    for(size_t i = 0; i < in.size(); i+=ENCODE_OCTETS){

        // the end of the input data has not been reached
        if(i+ENCODE_OCTETS-1 < in.size()){
            inPtr[0] = in.data()[i];
            inPtr[1] = in.data()[i+1];
            inPtr[2] = in.data()[i+2];
            encodedChars = encodeOctets(inPtr);
        }
        // there are only two remaining octets
        else if(i+ENCODE_OCTETS-2 < in.size()){
            inPtr[0] = in.data()[i];
            inPtr[1] = in.data()[i+1];
            inPtr[2] = 0;
            // there will only be three significant encoded charaters
            // add a padding character to ensure length = 4
            encodedChars = encodeOctets(inPtr).substr(0, 3) + PAD_CHAR;
        }
        // there is only one remaining octet
        else{
            inPtr[0] = in.data()[i];
            inPtr[1] = 0;
            inPtr[2] = 0;
            // there will only be two significant encoded charaters
            // add two padding characters to ensure length = 4
            encodedChars = encodeOctets(inPtr).substr(0, 2) + PAD_CHAR + PAD_CHAR;
        }

        // appending the next encoded sequence will exceed the line length
        if((totEncodedChars + ENCODED_SIZE) / LINE_LEN > totEncodedChars / LINE_LEN){
            // look for where the line should end
            for(unsigned int j = 0; j < ENCODED_SIZE; j++){
                //  append single character
                ret += encodedChars.at(j);
                // the end of the line has been reached
                if((totEncodedChars + j+1) % LINE_LEN == 0){
                    // add the line terminating characters
                    ret += LINE_TERM;
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

    //clean up
    inPtr = NULL;
    delete inPtr;

    return ret;
}
/**
 * @brief BaseSixFour::encodeOctets
 * @param in a pointer to no more than three uint8_t octets,
 *          any additional octets will be ignored. Fewer than
 *          three octets will throw an out-of-range expection
 * @return a string of four Base64 encoded characters
 */

std::string BaseSixFour::encodeOctets(uint8_t *in){

    using namespace std;

    string ret = "";

    // byte 0: shift off two LSBs (least significant bit)
    ret += CHARSET::MIME.at(in[0] >> 2);
    // byte 0: mask first 2 LSBs and shift off 4 MSBs (most significant bit)
    //  leaving only two original bits
    // byte 1: shift off 4 LSBs
    // add results of operations on byte 0 and byte 1
    ret += CHARSET::MIME.at(((in[0] & 0x03) << 4) + (in[1] >> 4));
    // byte 1: shift off 2 MSBs
    // byte 2: shift off 6 LSBs
    // add results of operations on byte 1 and byte 2
    // mask for 6 LSBs
    ret += CHARSET::MIME.at(((in[1] << 2) + (in[2] >> 6)) & 0x3f);
    //byte 2: mask for 6 LSBs
    ret += CHARSET::MIME.at(in[2] & 0x3f);

    return ret;

}

BaseSixFour::BaseSixFour(){

}
