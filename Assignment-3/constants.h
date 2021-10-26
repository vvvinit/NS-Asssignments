/**
    @file   constants.h
    @author Vinit Wagh, vvvinit.w@gmail.com, https://github.com/vvvinit
    @date	2021-10-26, 03:11:34
*/

#include "miracl.h"
#include <bits/stdc++.h>

#define RECEIVER_PORT         8000
#define BUFFER_SIZE         1024
#define ASYMMETRIC_KEY_SIZE 20
#define SESSION_KEY_SIZE    20
#define TRACER              1

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