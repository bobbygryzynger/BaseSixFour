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
    string encoded = "";
    size_t totEncodedChars = 0;
    size_t encodedLines = 0;
    uint8_t* inPtr = new uint8_t[ENCODE_OCTETS];

    for(size_t i = 0; i < in.size(); i+=ENCODE_OCTETS){

        if(i+ENCODE_OCTETS-1 < in.size()){
            inPtr[0] = in.data()[i];
            inPtr[1] = in.data()[i+1];
            inPtr[2] = in.data()[i+2];
            encoded = encodeOctets(inPtr, 3);
        }else if(i+ENCODE_OCTETS-2 < in.size()){
            inPtr[0] = in.data()[i];
            inPtr[1] = in.data()[i+1];
            inPtr[2] = 0;
            encoded = encodeOctets(inPtr, 2) + PAD_CHAR;
        }else{
            inPtr[0] = in.data()[i];
            inPtr[1] = 0;
            inPtr[2] = 0;
            encoded = encodeOctets(inPtr, 1) + PAD_CHAR + PAD_CHAR;
        }

        ret += encoded;
        totEncodedChars += ENCODED_SIZE;
    }

    for(size_t i = 1; i < totEncodedChars; i++){
        if(i % LINE_LEN == 0){
            ret.insert(i + (encodedLines * LINE_TERM.length()), LINE_TERM);
            encodedLines++;
        }
    }

    inPtr = NULL;
    delete inPtr;

    return ret;
}

std::string BaseSixFour::encodeOctets(uint8_t *in, uint len){

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

        //end of input
        if((len == 1 && i == 1) || (len == 2 && i == 2)){
            break;
        }


    }

    return ret;

}

BaseSixFour::BaseSixFour(){

}
