/**
    @file   client.cpp
    @author Vinit Wagh, vvvinit.w@gmail.com, https://github.com/vvvinit
    @date	2021-11-07, 00:06:26
*/

#include <bits/stdc++.h>
#include <sys/stat.h>
#include "connection.h"
#include "miracl.h"
#include "constants.h"
#include "encrypt.cpp"

void encrypt(char *image, int n, string key);

int main(){
    cout << endl;
    cout << "===============================" << endl;
    cout << "             Client            " << endl;
    cout << "===============================" << endl
         << endl;

	tcp::Client client(PORT, BUFFER_SIZE);
	client.connect();
	cout << "Connected to Server" << endl
		 << endl;

    anyKey("Press return to generate clientHello!");

    miracl *mip=mirsys(5000,10);
	irand(time(0));

    big client_random = mirvar(0);
    bigdig(RANDOM_SIZE, 10, client_random);
	string s_client_random = big_to_string(client_random);

    big session_id = mirvar(0);
    bigdig(SESSION_ID_SIZE, 10, session_id);
	string s_session_id = big_to_string(session_id);

    string cipher_suite = "TLS_RSA_WITH_RC4_128_SHA";

    string clientHello = s_client_random + s_session_id + cipher_suite;

    if(TRACER){
        cout << "Client Random: " << s_client_random << endl;
        cout << "Session ID: " << s_session_id << endl;
        cout << "Cipher Suite: " << cipher_suite << endl
             << endl;
    }

    anyKey("Press return to send clientHello!");
    
    client.send(clientHello);

    string serverHello;
    client.receive(serverHello);

    big server_random = mirvar(0);
    string s_server_random = serverHello.substr(0, RANDOM_SIZE);
    string_to_big(s_server_random, server_random);

    if(TRACER){
        cout << "<------ serverHello ------>" << endl;
        cout << "Server Random: " << s_server_random << endl;
        cout << "Session ID: " << s_session_id << endl;
        cout << "Cipher Suite: " << cipher_suite << endl
             << endl;
    }

    anyKey("Press return to generate Premaster Key!");

    big premaster_key = mirvar(0);
    bigdig(RANDOM_SIZE, 10, premaster_key);
	string s_premaster_key = big_to_string(premaster_key);
    vector<string> e_premaster_key = encrypt(s_premaster_key);
        mip=mirsys(5000,10);

    if(TRACER){
        cout << "Pre-master Key: " << s_premaster_key << endl;
        cout << "Pre-master Key (Encrypted): ";
        for(auto st:e_premaster_key)
            cout << st;
        cout << endl
             << endl;
    }

    anyKey("Press return to send Premaster Key!");

    client.send(to_string(e_premaster_key.size()));

    for(auto st:e_premaster_key){
        client.send(st);
    }

    // master_secret = PRF(pre_master_secret, "master secret",
    //                       ClientHello.random + ServerHello.random)
    //                       [0..47];

    anyKey("Press return to generate master key!");

    string s_prf_seed = s_premaster_key + master_secret_string + s_client_random + s_server_random;
    big prf_seed = mirvar(0);
    string_to_big(s_prf_seed, prf_seed);



    if(TRACER){
        cout << "Prf-seed: " << s_prf_seed << endl
             << endl;
    }

    irand(prf_seed_gen(s_prf_seed));
    big master_key = mirvar(0);
    bigdig(MASTER_KEY_SIZE, 10, master_key);
	string s_master_key = big_to_string(master_key);

    if(TRACER){
        cout << "Master Secret Generated: " << s_master_key << endl
             << endl;
    }

    anyKey("Press return to exit!");

    return 0;
}