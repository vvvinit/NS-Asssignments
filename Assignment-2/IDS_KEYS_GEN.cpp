#include <bits/stdc++.h>
#include "miracl.h"
#include "constants.h"
using namespace std;

int main(){
    freopen("keys.h", "w", stdout);

	miracl *mip=mirsys(5000,10);

	long seed = time(0);
	irand(seed);

    big a = mirvar(0);
    bigdig(ID_SIZE, 10, a);
    char *num = new char[ID_SIZE+1];
	otstr(a,num);
    string x(num);
    cout << "#define ALICE_ID \"" << x << "\"" << endl;
    delete[] num;

    bigdig(ID_SIZE, 10, a);
    num = new char[ID_SIZE+1];
	otstr(a,num);
    x = string(num);
    cout << "#define BOB_ID \"" << x << "\"" << endl
         << endl;
    delete[] num;

    bigdig(KEY_SIZE, 10, a);
    num = new char[KEY_SIZE+1];
	otstr(a,num);
    x = string(num);
    cout << "#define KDC_ALICE_KEY \"" << x << "\"" << endl;
    delete[] num;

    bigdig(KEY_SIZE, 10, a);
    num = new char[KEY_SIZE+1];
	otstr(a,num);
    x = string(num);
    cout << "#define KDC_BOB_KEY \"" << x << "\"" << endl;
    delete[] num;

    return 0;
}

