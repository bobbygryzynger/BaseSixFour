#include "BaseSixFour.h"

const std::string BaseSixFour::CHARSET::MIME =  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                                "abcdefghijklmnopqrstuvwxyz"
                                                "0123456789"
                                                "/+=";

std::string BaseSixFour::encode(const std::vector<uint8_t> &in){

    using namespace std;
    string ret = "";
    uint8_t* inPtr = new uint8_t[ENCODE_OCTETS];

    for(size_t i = 0; i < in.size(); i+=ENCODE_OCTETS){

        if(i+ENCODE_OCTETS < in.size()){
            inPtr[0] = in.data()[i];
            inPtr[1] = in.data()[i+1];
            inPtr[2] = in.data()[i+2];
            ret += encodeOctets(inPtr, 3);
        }else if(i+ENCODE_OCTETS-1 < in.size()){
            inPtr[0] = in.data()[i];
            inPtr[1] = in.data()[i+1];
            inPtr[2] = 0;
            ret += encodeOctets(inPtr, 2);
        }else if(i+ENCODE_OCTETS-2 < in.size()){
            inPtr[0] = in.data()[i];
            inPtr[1] = 0;
            inPtr[2] = 0;
            ret += encodeOctets(inPtr, 1);
        }
    }

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

        if((len == 1 && i == 1) || (len == 2 && i == 2)){
            break;
        }


    }

    return ret;

}

BaseSixFour::BaseSixFour(){

}
