/**
    @file   bob.cpp
    @author Vinit Wagh, vvvinit.w@gmail.com, https://github.com/vvvinit
*/

#include <bits/stdc++.h>
#include "connection.h"
#include "miracl.h"
#include "constants.h"
#include "keys.h"
#include "encryption.cpp"b+

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
    
    string e_ticket;
    bob.receive(e_ticket);
    string ticket = ns::decrypt(e_ticket, KDC_BOB_KEY);
    string session_key = ticket.substr(0, KEY_SIZE);
    string client_id = ticket.substr(KEY_SIZE, ID_SIZE);

    if(client_id!=ALICE_ID){
        cout << "Invalid client" << endl;
        exit(EXIT_FAILURE);
    }

    string e_nonce;
    bob.receive(e_nonce);
    string s_nonce = ns::decrypt(e_nonce, session_key);

    if(TRACER){
        cout << "Received ticket (Encrypted): " << e_ticket << endl;
        cout << "Received ticket : " << ticket << endl;
        cout << "Client ID : " << client_id << endl;
        cout << "Session key : " << session_key << endl;
        cout << "Received nonce (Encrypted) : " << e_nonce << endl;
        cout << "Received nonce : " << s_nonce << endl
             << endl;
    }

    miracl *mip=mirsys(5000,10);
    big nonce = mirvar(0);
    string_to_big(s_nonce, nonce);
    subtract(nonce,mirvar(1),nonce);
    s_nonce = big_to_string(nonce);

    big nonce3 = mirvar(0);
    bigdig(NONCE_SIZE, 10, nonce3);
	string s_nonce3 = big_to_string(nonce3);

    s_nonce += s_nonce3;
    s_nonce = ns::encrypt(s_nonce, session_key);

    anyKey("Press return to send response");

    bob.send(s_nonce);

    cout << "Response sent" << endl
         << endl;

    string e_alice_response;
	bob.receive(e_alice_response);

	cout << "Received response from Alice" << endl
		 << endl;

	string alice_response = ns::decrypt(e_alice_response, session_key);
	string s_v_nonce = alice_response.substr(0, NONCE_SIZE);

	big v_nonce = mirvar(0);
    string_to_big(s_v_nonce, v_nonce);
    subtract(v_nonce,mirvar(-1),v_nonce);

    if(TRACER){
		cout << "Received response (Encrypted): " << e_alice_response << endl;
        cout << "Received response: " << alice_response << endl
             << endl;
    }

    if(mr_compare(v_nonce,nonce3)!=0){
        cout << "Alice could not be verified" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Alice verified" << endl
         << endl;

    cout << "Exiting..." << endl;

    return 0;
}