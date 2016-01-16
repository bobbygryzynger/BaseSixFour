#include "BaseSixFour.h"

const std::string BaseSixFour::CHARSET::MIME =  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                                "abcdefghijklmnopqrstuvwxyz"
                                                "0123456789"
                                                "/+";
const char BaseSixFour::PAD_CHAR = '=';
const uint BaseSixFour::LINE_LEN = 76;
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
            for(uint j = 0; j < ENCODED_SIZE; j++){
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

std::string BaseSixFour::encodeOctetSet(uint8_t *in){

    using namespace std;

    string ret = "";
    int charsetIdx = 0;

    for(size_t i = 0; i < ENCODED_SIZE; i++){

        if(i == 0){
            charsetIdx = (in[i] >> 2) & 0x3f;
        }else if(i == 1){
            charsetIdx = ((in[i-1] & 0x03) << 4) + (in[i] >> 4);
        }else if(i == 2){
            charsetIdx = ((in[i-1] << 2) + (in[i] >> 6)) & 0x3f;
        }else{
            charsetIdx = in[i-1] & 0x3f;
        }

        ret += CHARSET::MIME.at(charsetIdx);
    }

    return ret;

}

BaseSixFour::BaseSixFour(){

}
