#include <bits/stdc++.h>
#include "connection.h"
#include "miracl.h"
#include "constants.h"
#include "keys.h"
#include "encryption.cpp"

int main(){
    cout << endl;
    cout << "===============================" << endl;
    cout << "              KDC              " << endl;
    cout << "===============================" << endl
         << endl;
    
    tcp::Server kdc(KDC_PORT, BUFFER_SIZE);
    kdc.listen();
	cout << "KDC Initialized\nListening for clients...\n";
	kdc.accept();
    cout << "Client connected" << endl
         << endl;
    string s;
    kdc.receive(s);
    
    if(TRACER)
        cout << "Received data : " << s << endl;
    
    if(s.length()!=(NONCE_SIZE+2*ID_SIZE)){
        cout << "Invalid Data" << endl;
        exit(EXIT_FAILURE);
    }

    string nonce = s.substr(0, NONCE_SIZE);
    string client_id = s.substr(NONCE_SIZE, ID_SIZE);

    if(client_id!=ALICE_ID){
        cout << "Unknown client" << endl;
        exit(EXIT_FAILURE);
    }

    string server_id = s.substr(NONCE_SIZE+ID_SIZE, ID_SIZE);

    if(TRACER){
		cout << "Nonce received : " << nonce << endl;
		cout << "Client ID : " << client_id << endl;
        cout << "Server ID : " << server_id << endl
             << endl;
    }

    

    miracl *mip=mirsys(5000,10);

	long seed = time(0);
	irand(seed);

    big session_key = mirvar(0);
    bigdig(KEY_SIZE, 10, session_key);
    char *num = new char[KEY_SIZE+1];
	otstr(session_key,num);
    string key(num);
    delete[] num;

    string server_ticket = key + ALICE_ID;
    string encrypted_server_ticket = ns::encrypt(server_ticket, KDC_BOB_KEY);

    string client_ticket = nonce + BOB_ID + key + encrypted_server_ticket;
    string encrypted_client_ticket = ns::encrypt(client_ticket, KDC_ALICE_KEY);


    if(TRACER){
		cout << "Session key : " << key << endl;
		cout << "Server ticket : " << server_ticket << endl;
		cout << "Server ticket (Encrypted) : " << encrypted_server_ticket << endl;
		cout << "Client ticket : " << client_ticket << endl;
        cout << "Client ticket (Encrypted) : " << encrypted_client_ticket << endl
             << endl;
    }

		cout << "Server ticket length: " << server_ticket.length() << endl;
		cout << "Server ticket (Encrypted) length: " << encrypted_server_ticket.length() << endl;
    kdc.send(encrypted_client_ticket);

    cout << "Send ticket to client" << endl
         << "KDC Exiting..." << endl;

    return 0;
}