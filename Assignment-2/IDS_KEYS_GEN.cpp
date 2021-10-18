/**
    @file   IDS_KEYS_GEN.cpp
    @author Vinit Wagh, vvvinit.w@gmail.com, https://github.com/vvvinit
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
    bigdig(ID_SIZE, 10, a);
    numstr = big_to_string(a);
    cout << "#define ALICE_ID \"" << numstr << "\"" << endl;

    a = mirvar(0);
    bigdig(ID_SIZE, 10, a);
    numstr = big_to_string(a);
    cout << "#define BOB_ID \"" << numstr << "\"" << endl
         << endl;

    a = mirvar(0);
    bigdig(KEY_SIZE, 10, a);
    numstr = big_to_string(a);
    cout << "#define KDC_ALICE_KEY \"" << numstr << "\"" << endl;

    a = mirvar(0);
    bigdig(KEY_SIZE, 10, a);
    numstr = big_to_string(a);
    cout << "#define KDC_BOB_KEY \"" << numstr << "\"" << endl;

    return 0;
}