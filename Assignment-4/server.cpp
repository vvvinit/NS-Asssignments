/**
    @file   server.cpp
    @author Vinit Wagh, vvvinit.w@gmail.com, https://github.com/vvvinit
    @date	2021-11-07, 00:06:08
*/

#include <bits/stdc++.h>
#include <sys/stat.h>
#include "connection.h"
#include "miracl.h"
#include "constants.h"
#include "decrypt.cpp"

void decrypt(char* image, int n, string key);
    
int main(){
    cout << endl;
    cout << "===============================" << endl;
    cout << "             Server            " << endl;
    cout << "===============================" << endl
         << endl;

    tcp::Server server(PORT, BUFFER_SIZE);
	server.listen();
    server.accept();
    cout << "Client Connected" << endl
         << endl;

    string clientHello;
    server.receive(clientHello);

    cout << "Client Hello received!" << endl
         << endl;

    miracl *mip=mirsys(5000,10);
	irand(time(0));

    big client_random = mirvar(0);
    string s_client_random = clientHello.substr(0, RANDOM_SIZE);
    string_to_big(s_client_random, client_random);

    big session_id = mirvar(0);
    string s_session_id = clientHello.substr(RANDOM_SIZE, SESSION_ID_SIZE);
    string_to_big(s_session_id, session_id);

    string cipher_suite = clientHello.substr(RANDOM_SIZE + SESSION_ID_SIZE, clientHello.length() - RANDOM_SIZE - SESSION_ID_SIZE);

    if(TRACER){
        cout << "<------ clientHello ------>" << endl;
        cout << "Client Random: " << s_client_random << endl;
        cout << "Session ID: " << s_session_id << endl;
        cout << "Cipher Suite: " << cipher_suite << endl
             << endl;
    }

    anyKey("Press return to generate serverHello!");

    big server_random = mirvar(0);
    bigdig(RANDOM_SIZE, 10, server_random);
	string s_server_random = big_to_string(server_random);

    string serverHello = s_server_random + s_session_id + cipher_suite;

    if(TRACER){
        cout << "Server Random: " << s_server_random << endl;
        cout << "Session ID: " << s_session_id << endl;
        cout << "Cipher Suite: " << cipher_suite << endl
             << endl;
    }

    anyKey("Press return to send serverHello!");

    server.send(serverHello);

    string s_len;
    server.receive(s_len);

    vector<string> e_premaster_key;

    for (int i = 0; i < stoi(s_len); i++){
        string temp;
        server.receive(temp);
        e_premaster_key.push_back(temp);
    }

    big premaster_key = mirvar(0);
    string s_premaster_key = decrypt(e_premaster_key);
    mip=mirsys(5000,10);
    string_to_big(s_premaster_key, premaster_key);

    if(TRACER){
        cout << "Pre-master Key (Encrypted): ";
        for(auto st:e_premaster_key)
            cout << st;
        cout << endl;
        cout << "Pre-master Key: " << s_premaster_key << endl
             << endl;
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