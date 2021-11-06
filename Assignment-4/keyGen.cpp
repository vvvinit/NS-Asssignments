#include <bits/stdc++.h>
#include <sys/stat.h>
#include "miracl.h"
#include "constants.h"

static miracl *mip;

static big pd,pl,ph;

void strongp(big p,int n){
    srand(time(0));
    long seed1 = lrand48(), seed2 = lrand48();

    int r,r1,r2;
    
    irand(seed1);
    bigbits(2*n/3,pd);
    nxprime(pd,pd);
    expb2(n-1,ph);
    divide(ph,pd,ph);
    expb2(n-2,pl);
    divide(pl,pd,pl);
    subtract(ph,pl,ph);

    irand(seed2);
    bigrand(ph,ph);
    add(ph,pl,ph);
    r1=subdiv(pd,12,pl);
    r2=subdiv(ph,12,pl);

    r=0;
    while ((r1*(r2+r))%12!=5) 
        r++;

    incr(ph,r,ph);
    do {
        multiply(ph,pd,p);
        premult(p,2,p);
        incr(p,1,p);
        incr(ph,12,ph);
    } while (!isprime(p));
}

void server_keys(){
    int i;
    long seed[2*NP];

    big p[NP],N,a,b,n,e,d;

    big z,o,t;

    FILE *outfile;
    
    mip=mirsys(100,0);

    gprime(15000);

    for (i=0;i<NP;i++) 
        p[i]=mirvar(0);

    N=mirvar(0);
    pd=mirvar(0);
    pl=mirvar(0);
    ph=mirvar(0);
    a=mirvar(0);
    b=mirvar(0);
    n=mirvar(0);
    e=mirvar(0);
    z=mirvar(0);
    o=mirvar(0);
    t=mirvar(0);
    d=mirvar(0);

    convert(1,N);
    for (i=0;i<NP;i++) {
        strongp(p[i],PRIME_BITS);
        multiply(N,p[i],N);
    }

    decr(p[0],1,a);
    decr(p[1],1,b);
    multiply(a,b,n);

    convert(0,z);
    convert(1,o);
    do {
        bigrand(n,e);
        egcd(e,n,t);
    } while(mr_compare(e,z) == 0 || mr_compare(e,o) == 0 || mr_compare(t,o) != 0);

    multi_inverse(1,&e,n,&d);

    mip->IOBASE=16;

    outfile=fopen("public_server.key","wt");

    cotnum(N,outfile);

    cotnum(e,outfile);

    fclose(outfile);


    outfile=fopen("private_server.key","wt");

    cotnum(p[0],outfile);
    
    cotnum(p[1],outfile);

    cotnum(d,outfile);

    cotnum(n,outfile);

    fclose(outfile);
}

void client_keys(){
    int i;
    long seed[2*NP];

    big p[NP],N,a,b,n,e,d;

    big z,o,t;

    FILE *outfile;
    
    mip=mirsys(100,0);

    gprime(15000);

    for (i=0;i<NP;i++) 
        p[i]=mirvar(0);

    N=mirvar(0);
    pd=mirvar(0);
    pl=mirvar(0);
    ph=mirvar(0);
    a=mirvar(0);
    b=mirvar(0);
    n=mirvar(0);
    e=mirvar(0);
    z=mirvar(0);
    o=mirvar(0);
    t=mirvar(0);
    d=mirvar(0);

    convert(1,N);
    for (i=0;i<NP;i++) {
        strongp(p[i],PRIME_BITS);
        multiply(N,p[i],N);
    }

    decr(p[0],1,a);
    decr(p[1],1,b);
    multiply(a,b,n);

    convert(0,z);
    convert(1,o);
    do {
        bigrand(n,e);
        egcd(e,n,t);
    } while(mr_compare(e,z) == 0 || mr_compare(e,o) == 0 || mr_compare(t,o) != 0);

    multi_inverse(1,&e,n,&d);

    mip->IOBASE=16;

    outfile=fopen("public_client.key","wt");

    cotnum(N,outfile);

    cotnum(e,outfile);

    fclose(outfile);


    outfile=fopen("private_client.key","wt");

    cotnum(p[0],outfile);
    
    cotnum(p[1],outfile);

    cotnum(d,outfile);

    cotnum(n,outfile);

    fclose(outfile);
}

int main(){
    server_keys();
    client_keys();
    
    return 0;
}