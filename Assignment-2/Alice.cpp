#include <bits/stdc++.h>
#include "connection.h"
#include "miracl.h"
#include "constants.h"
#include "keys.h"
#include "encryption.cpp"

int main(){
    cout << endl;
    cout << "===============================" << endl;
    cout << "              Alice            " << endl;
    cout << "===============================" << endl
         << endl;

	tcp::Client kdc(KDC_PORT, BUFFER_SIZE);
	kdc.connect();
	cout << "Alice connected to KDC" << endl
		 << endl;

	miracl *mip=mirsys(5000,10);
	irand(time(0));

    big nonce1 = mirvar(0);
    bigdig(NONCE_SIZE, 10, nonce1);
	string s_nonce1 = big_to_string(nonce1);

	string to_kdc = s_nonce1 + ALICE_ID + BOB_ID;

	if(TRACER){
		cout << "Nonce generated : " << s_nonce1 << endl;
		cout << "Self ID : " << ALICE_ID << endl;
		cout << "Bob's ID : " << BOB_ID << endl;
		cout << "To KDC : " << to_kdc << endl
			 << endl;
	}

	anyKey("Press return to send request to KDC");

	kdc.send(to_kdc);

	cout << "Sent request to KDC" << endl
		 << endl;

	string e_kdc_ticket;
	kdc.receive(e_kdc_ticket);

	string kdc_ticket = ns::decrypt(e_kdc_ticket, KDC_ALICE_KEY);

	cout << "Received ticket from KDC" << endl
		 << endl;

	string v_nonce1 = kdc_ticket.substr(0, NONCE_SIZE);

	if(v_nonce1!=s_nonce1){
        cout << "Received nonce does not match" << endl;
        exit(EXIT_FAILURE);
    }


    string v_server_id = kdc_ticket.substr(NONCE_SIZE, ID_SIZE);

	if(v_server_id!=BOB_ID){
        cout << "Received server ID does not match" << endl;
        exit(EXIT_FAILURE);
    }

    string session_key = kdc_ticket.substr(NONCE_SIZE+ID_SIZE, KEY_SIZE);
    string kdc_bob_ticket = kdc_ticket.substr(NONCE_SIZE+ID_SIZE+KEY_SIZE, kdc_ticket.length()-NONCE_SIZE+ID_SIZE+KEY_SIZE);

    if(TRACER){
		cout << "Received ticket (Encrypted): " << e_kdc_ticket << endl;
		cout << "Received ticket: " << kdc_ticket << endl;
		cout << "Session key : " << session_key << endl;
		cout << "Received nonce : " << v_nonce1 << endl;
		cout << "Bob's ticket from KDC : " << kdc_bob_ticket << endl
			 << endl;
	}

	anyKey("Press return to generate nonce for Bob");

	big nonce2 = mirvar(0);
    bigdig(NONCE_SIZE, 10, nonce2);
	string s_nonce2 = big_to_string(nonce2);

	string e_nonce2 = ns::encrypt(s_nonce2, session_key);

	if(TRACER){
		cout << "Generated Nonce : " << s_nonce2 << endl;
		cout << "Generated Nonce (Encrypted) : " << e_nonce2 << endl
			 << endl;
	}

	anyKey("Press return to connect to Bob");

	tcp::Client alice(BOB_PORT, BUFFER_SIZE);
	alice.connect();
	cout << "Alice connected to Bob" << endl
		 << endl;

	anyKey("Press return to send ticket to Bob");

	alice.send(kdc_bob_ticket);
	alice.send(e_nonce2);

	cout << "Sent ticket to Bob" << endl
		 << endl;

	string e_bob_response;
	alice.receive(e_bob_response);

	cout << "Received response from Bob" << endl
		 << endl;

	string bob_response = ns::decrypt(e_bob_response, session_key);



	string s_v_nonce2 = bob_response.substr(0, NONCE_SIZE);
	string s_nonce3 = bob_response.substr(NONCE_SIZE, NONCE_SIZE);

	big v_nonce2 = mirvar(0);
    string_to_big(s_v_nonce2, v_nonce2);
    subtract(v_nonce2,mirvar(-1),v_nonce2);






    if(TRACER){
		cout << "Received response (Encrypted): " << e_bob_response << endl;
		cout << "Received response: " << bob_response << endl;
		cout << "Nonce response : " << s_v_nonce2 << endl;
		cout << "New nonce : " << s_nonce3 << endl
			 << endl;
	}

	if(mr_compare(v_nonce2,nonce2)!=0){
        cout << "Bob could not be verified" << endl;
        exit(EXIT_FAILURE);
    }

	cout << "Bob verified" << endl
		 << endl;

	anyKey("Press return to send response");

	big nonce3 = mirvar(0);
	string_to_big(s_nonce3, nonce3);

	subtract(nonce3,mirvar(1),nonce3);

    s_nonce3 = big_to_string(nonce3);

	s_nonce3 = ns::encrypt(s_nonce3, session_key);

	alice.send(s_nonce3);

	cout << "Response sent" << endl
		 << endl;

	cout << "Exiting..." << endl;
	return 0;
}

