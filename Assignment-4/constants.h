/**
    @file   constants.h
    @author Vinit Wagh, vvvinit.w@gmail.com, https://github.com/vvvinit
    @date	2021-11-07, 04:04:18
*/

#ifndef CONSTANT_H
#define CONSTANT_H

#include "miracl.h"
#include <bits/stdc++.h>

#define PORT                    8000
#define RANDOM_SIZE             20
#define BUFFER_SIZE             4096
#define SESSION_ID_SIZE         20
#define PREMASTER_KEY_SIZE      20
#define MASTER_KEY_SIZE         20
#define NP                      2 
#define PRIME_BITS              40
#define TRACER                  1
string master_secret_string  = "010010101001";


void anyKey(std::string msg){
    std::cout << msg;
    std::getchar();
    std::cout << "\033[A\33[2K\r"
              << endl
              << "\033[A\33[2K\r";
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

int prf_seed_gen(string s){
    int seed = 0;
    for(auto i:s)
        seed += (int)i;
    return seed;
}
#endif