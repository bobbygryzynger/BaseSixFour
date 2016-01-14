#include "BaseSixFour.h"

const std::string BaseSixFour::CHARSET::MIME =  "abcdefghijklmnopqrstuvwxyz"
                                                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                                "/+=";

std::string BaseSixFour::encode(std::vector<uint8_t> in){

    using namespace std;
    string ret = "";

    for(size_t i = 0; i < in.size(); i++){
        std::cout << (int)in.at(i);
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
