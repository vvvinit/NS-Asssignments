#include <bits/stdc++.h>
#include <sys/stat.h>
#include "miracl.h"

vector<string> encrypt(string input){
    vector<string> output;

    big N,e,m,c;

    FILE *ifile;
    FILE *ofile;

    static char line[1000];
    static char buff[256];

    int i,mlen,klen;

    miracl *mip=mirsys(100,0);

    N=mirvar(0);
    e=mirvar(0);
    m=mirvar(0);
    c=mirvar(0);

    ifile=fopen("public_server.key","rt");
    mip->IOBASE=16;
    cinnum(N,ifile);
    cinnum(e,ifile);
    fclose(ifile);
    klen = logb2(N)/7 - 1;    



    std::copy(input.begin(), input.end(), line);
    mlen = input.length();
    while(TRUE)
    {
        if(mlen < klen)
        {
            break;
        }
        for(int i=0; i<klen; i++)
            buff[i] = line[i];            
        buff[klen] = '\0';

        for(int i=klen; i<=mlen; i++)
            line[i-klen] = line[i];
        mlen = strlen(line);

        mip->IOBASE=128;
        cinstr(m,buff);
        powmod(m,e,N,c);
        mip->IOBASE=16;
        ofile = fopen("output.enc","wt");
        cotnum(c,ofile);
        fclose(ofile);
        string item_name;
        ifstream nameFileout;
        nameFileout.open("output.enc");
        string li;
        getline(nameFileout, li);
        output.push_back(li);
        nameFileout.close();
        remove("output.enc");
    }

    if (mlen < klen && mlen != 0)
    {
        for (int i = 0; i < mlen; ++i)
            buff[i] = line[i];
        buff[mlen] = '\0';

        mip->IOBASE=128;
        cinstr(m,buff);
        powmod(m,e,N,c);
        mip->IOBASE=16;
        ofile = fopen("output.enc","wt");
        cotnum(c,ofile);
        fclose(ofile);
        string item_name;
        ifstream nameFileout;
        nameFileout.open("output.enc");
        string li;
        getline(nameFileout, li);
        output.push_back(li);
        nameFileout.close();
        remove("output.enc");
    }
    fclose(ofile);

    return output;
}
