#include "BaseSixFour.h"

const std::string BaseSixFour::CHARSET::MIME =  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                                "abcdefghijklmnopqrstuvwxyz"
                                                "0123456789+/";
const char BaseSixFour::PAD_CHAR = '=';
const unsigned int BaseSixFour::LINE_LEN = 76;
const std::string BaseSixFour::LINE_TERM = "\r\n";

std::string BaseSixFour::encode(const std::vector<uint8_t> &in){

    using namespace std;

    string ret = "";
    string encodedChars = "";
    size_t totEncodedChars = 0;
    uint8_t* inPtr = new uint8_t[ENCODE_OCTETS];

    for(size_t i = 0; i < in.size(); i+=ENCODE_OCTETS){

        if(i+ENCODE_OCTETS-1 < in.size()){
            inPtr[0] = in.data()[i];
            inPtr[1] = in.data()[i+1];
            inPtr[2] = in.data()[i+2];
            encodedChars = encodeOctetSet(inPtr);
        }else if(i+ENCODE_OCTETS-2 < in.size()){
            inPtr[0] = in.data()[i];
            inPtr[1] = in.data()[i+1];
            inPtr[2] = 0;
            encodedChars = encodeOctetSet(inPtr).substr(0, 3) + PAD_CHAR;
        }else{
            inPtr[0] = in.data()[i];
            inPtr[1] = 0;
            inPtr[2] = 0;
            encodedChars = encodeOctetSet(inPtr).substr(0, 2) + PAD_CHAR + PAD_CHAR;
        }

        //the next encoded sequence will exceed the line length
        if((totEncodedChars + ENCODED_SIZE) / LINE_LEN > totEncodedChars / LINE_LEN){
            for(unsigned int j = 0; j < ENCODED_SIZE; j++){
                ret += encodedChars.at(j);
                //the end of the line has been reached
                if((totEncodedChars + j+1) % LINE_LEN == 0){
                    ret += LINE_TERM;
                }
            }
        }else{
            ret += encodedChars;
        }

        totEncodedChars += ENCODED_SIZE;
    }

    inPtr = NULL;
    delete inPtr;

    return ret;
}
/**
 * @brief BaseSixFour::encodeOctetSet
 * @param in a pointer to no more than three uint8_t bytes,
 *          any additional bytes will be ignored. Fewer than
 *          three bytes will throw an out-of-range expection
 * @return a string of four Base64 encoded characters
 */

std::string BaseSixFour::encodeOctetSet(uint8_t *in){

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
