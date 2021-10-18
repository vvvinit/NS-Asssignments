/**
    @file   constants.h
    @author Vinit Wagh, vvvinit.w@gmail.com, https://github.com/vvvinit
*/

#include "miracl.h"

#define KDC_PORT            8000
#define BOB_PORT            8080
#define BUFFER_SIZE         10240
#define NONCE_SIZE          3
#define ID_SIZE             5
#define KEY_SIZE            7
#define TRACER              0

void anyKey(std::string msg){
    std::cout << msg;
    std::getchar();
    std::cout << "\033[A\33[2K\r";
}

std::string big_to_string(big bignum){
    char *num = new char[BUFFER_SIZE];
	otstr(bignum,num);
    std::string numstr = std::string(num);
    delete[] num;
    return numstr;
}

void string_to_big(std::string num, big a){
    char *noncechar = new char[BUFFER_SIZE];
    std::copy(num.begin(), num.end(), noncechar);
    noncechar[num.length()] = '\0';
    instr(a, noncechar);
    delete[] noncechar;
}