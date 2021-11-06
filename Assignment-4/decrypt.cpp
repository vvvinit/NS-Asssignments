#include <bits/stdc++.h>
#include <sys/stat.h>
#include "miracl.h"

void strip(char *name){
    int i;
    for (i=0; name[i]!='\0'; i++)
    {
        if (name[i]!='.') continue;
        name[i]='\0';
        break;
    }
}

string decrypt(vector<string> input){
    string output;
    big N,e,m,c,d,p,q,n;

    FILE *ifile;
    FILE *ofile;

    miracl*mip=mirsys(100,0);

    N=mirvar(0);
    e=mirvar(0);
    m=mirvar(0);
    c=mirvar(0);
    d=mirvar(0);
    p=mirvar(0);
    q=mirvar(0);
    n=mirvar(0);

    ifile = fopen("public_server.key","rt");
    mip->IOBASE=16;
    cinnum(N,ifile);
    cinnum(e,ifile);
    fclose(ifile);
    ifile=fopen("private_server.key","rt");
    cinnum(p,ifile);
    cinnum(q,ifile);
    cinnum(d,ifile);
    cinnum(n,ifile);
    fclose(ifile);

    for(auto st:input)
    {  
        mip->IOBASE=16;
        ofstream nameFileout;
        nameFileout.open("output.enc");
        nameFileout << st;
        nameFileout.close();
        FILE* ofile = fopen("output.enc","rt");
        cinnum(c,ofile);
        fclose(ofile);
        remove("output.enc");
        if (size(c) == 0)
        {
            break;
        }
        powmod(c,d,N,m);
        mip->IOBASE=128;
        ofile = fopen("output.enc","wt");
        cotnum(m, ofile);
        fclose(ofile);
        string item_name;
        ifstream nnameFileout;
        nnameFileout.open("output.enc");
        string li;
        getline(nnameFileout, li);
        output += li;
        nameFileout.close();
        remove("output.enc");
    }
    return output;
}