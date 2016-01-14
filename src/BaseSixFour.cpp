#include "BaseSixFour.h"

const std::string BaseSixFour::CHARSET::MIME =  "abcdefghijklmnopqrstuvwxyz"
                                                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                                "/+=";

std::string BaseSixFour::encode(std::vector<uint8_t> in){

    using namespace std;
    string ret = "";

    for(size_t i = 0; i < 4; i++){

        uint index = 0;

        if(i == 0){
            index =  (in.at(i) >> 2) & 0x3f;

            std::cout << (char)in.at(i) << ": " << index << std::endl;
        }else if(i == 1){
            index = ((in.at(i-1) & 0x03) << 4) + (in.at(i) >> 4);
            std::cout << (char)in.at(i) << ": " << index << std::endl;
        }else if(i == 2){
            index = ((in.at(i-1) << 2) + (in.at(i) >> 6)) & 0x3f;
            std::cout << (char)in.at(i) << ": " << index << std::endl;
        }else{
            index = in.at(i) & 0x3f;
            std::cout << (char)in.at(i-1) << ": " << index << std::endl;
        }
    }

    std::cout << std::endl;

    return ret;
}

std::string BaseSixFour::encodeOctets(uint8_t *in, uint len){

    using namespace std;
    return "";

}

BaseSixFour::BaseSixFour(){

}
