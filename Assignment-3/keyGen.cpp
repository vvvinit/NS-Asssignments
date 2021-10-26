/**
    @file   keyGen.cpp
    @author Vinit Wagh, vvvinit.w@gmail.com, https://github.com/vvvinit
    @date	2021-10-26, 03:32:13
*/

#include <bits/stdc++.h>
#include "miracl.h"
#include "constants.h"
using namespace std;

int main(){

    freopen("keys.h", "w", stdout);
	miracl *mip=mirsys(5000,10);
	irand(time(0));

    string numstr;
    
    big a = mirvar(0);
    bigdig(ASYMMETRIC_KEY_SIZE, 10, a);
    numstr = big_to_string(a);
    cout << "#define SENDER_PUBLIC_KEY \"" << numstr << "\"" << endl;
    cout << "#define SENDER_PRIVATE_KEY \"" << numstr << "\"" << endl;

    a = mirvar(0);
    bigdig(ASYMMETRIC_KEY_SIZE, 10, a);
    numstr = big_to_string(a);
    cout << "#define RECEIVER_PUBLIC_KEY \"" << numstr << "\"" << endl;
    cout << "#define RECEIVER_PRIVATE_KEY \"" << numstr << "\"" << endl;

    return 0;
}