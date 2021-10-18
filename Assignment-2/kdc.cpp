/**
    @file   kdc.cpp
    @author Vinit Wagh, vvvinit.w@gmail.com, https://github.com/vvvinit
*/

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
    
    string request;
    kdc.receive(request);
    
    if(TRACER)
        cout << "Received data : " << request << endl;
    
    if(request.length()!=(NONCE_SIZE+2*ID_SIZE)){
        cout << "Invalid Data" << endl;
        exit(EXIT_FAILURE);
    }

    string nonce = request.substr(0, NONCE_SIZE);
    string client_id = request.substr(NONCE_SIZE, ID_SIZE);

    if(client_id!=ALICE_ID){
        cout << "Unknown client" << endl;
        exit(EXIT_FAILURE);
    }

    string server_id = request.substr(NONCE_SIZE+ID_SIZE, ID_SIZE);

    if(server_id!=BOB_ID){
        cout << "Unknown server" << endl;
        exit(EXIT_FAILURE);
    }

    if(TRACER){
		cout << "Nonce received : " << nonce << endl;
		cout << "Client ID : " << client_id << endl;
        cout << "Server ID : " << server_id << endl
             << endl;
    }

    anyKey("Press return key to generate session key and ticket");

    miracl *mip=mirsys(5000,10);
	irand(time(0));

    big session_key = mirvar(0);
    bigdig(KEY_SIZE, 10, session_key);
    string s_session_key = big_to_string(session_key);

    string server_ticket = s_session_key + ALICE_ID;
    string encrypted_server_ticket = ns::encrypt(server_ticket, KDC_BOB_KEY);

    string client_ticket = nonce + BOB_ID + s_session_key + encrypted_server_ticket;
    string encrypted_client_ticket = ns::encrypt(client_ticket, KDC_ALICE_KEY);

    cout << "Session key and ticket generated" << endl << endl;

    if(TRACER){
		cout << "Session key : " << s_session_key << endl;
		cout << "Server ticket : " << server_ticket << endl;
		cout << "Server ticket (Encrypted) : " << encrypted_server_ticket << endl;
		cout << "Client ticket : " << client_ticket << endl;
        cout << "Client ticket (Encrypted) : " << encrypted_client_ticket << endl
             << endl;
    }

    anyKey("Press return to send ticket to client");

    kdc.send(encrypted_client_ticket);

    cout << "Sent ticket to client" << endl << endl
         << "KDC Exiting..." << endl;

    return 0;
}