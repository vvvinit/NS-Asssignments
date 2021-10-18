#include <bits/stdc++.h>
#include "connection.h"
#include "miracl.h"
#include "constants.h"
#include "keys.h"
#include "encryption.cpp"

int main(){
    cout << endl;
    cout << "===============================" << endl;
    cout << "              Bob              " << endl;
    cout << "===============================" << endl
         << endl;
    
    tcp::Server bob(BOB_PORT, BUFFER_SIZE);
    bob.listen();
	cout << "Bob Initialized\nListening for clients...\n";
	bob.accept();
    cout << "Client connected" << endl
         << endl;
    string s;
    bob.receive(s);
    string data = ns::decrypt(s, KDC_BOB_KEY);
    string session_key = data.substr(0, KEY_SIZE);
    string client_id = data.substr(KEY_SIZE, ID_SIZE);

    if(client_id!=ALICE_ID){
        cout << "Invalid client" << endl;
        exit(EXIT_FAILURE);
    }

    string encrypted_nonce;
    bob.receive(encrypted_nonce);
    string noncestr = ns::decrypt(encrypted_nonce, session_key);

    if(TRACER){
        cout << "Received ticket (Encrypted): " << s << endl;
        cout << "Received ticket : " << data << endl;
        cout << "Client ID : " << client_id << endl;
        cout << "Session key : " << session_key << endl;
        cout << "Received nonce (Encrypted) : " << encrypted_nonce << endl;
        cout << "Received nonce : " << noncestr << endl;
    }

	miracl *mip=mirsys(5000,10);
    big nonce = mirvar(0);
    char *noncechar = new char[NONCE_SIZE + 1];
    copy(noncestr.begin(), noncestr.end(), noncechar);
    noncechar[noncestr.length()] = '\0';
    instr(nonce, noncechar);
    delete[] noncechar;

    otnum(nonce,stdout);

    subtract(nonce,mirvar(1),nonce);
    otnum(nonce,stdout);

    int remaining = SESSION_LENGTH;


    return 0;
}