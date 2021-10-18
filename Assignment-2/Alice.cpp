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

	long seed = time(0);
	irand(seed);

    big nonce1 = mirvar(0);
    bigdig(NONCE_SIZE, 10, nonce1);
    char *num = new char[NONCE_SIZE+1];
	otstr(nonce1,num);
    string nonce_kdc(num);
    delete[] num;

	string to_kdc = nonce_kdc + ALICE_ID + BOB_ID;

	if(TRACER){
		cout << "Nonce generated : " << nonce_kdc << endl;
		cout << "SELF ID : " << ALICE_ID << endl;
		cout << "BOB's ID : " << BOB_ID << endl;
		cout << "To KDC : " << to_kdc << endl;
	}

	kdc.send(to_kdc);

	cout << "Sent request to KDC" << endl;

	string encrypted_kdc_ticket;
	kdc.receive(encrypted_kdc_ticket);

	string kdc_ticket = ns::decrypt(encrypted_kdc_ticket, KDC_ALICE_KEY);

	cout << "Received ticket from KDC" << endl;


	string nonce_kdc_received = kdc_ticket.substr(0, NONCE_SIZE);

	if(nonce_kdc_received!=nonce_kdc){
        cout << "Received nonce does not match" << endl;
        exit(EXIT_FAILURE);
    }


    string server_id_received = kdc_ticket.substr(NONCE_SIZE, ID_SIZE);

	if(server_id_received!=BOB_ID){
        cout << "Received server ID does not match" << endl;
        exit(EXIT_FAILURE);
    }

    string session_key = kdc_ticket.substr(NONCE_SIZE+ID_SIZE, KEY_SIZE);
    string kdc_bob_ticket = kdc_ticket.substr(NONCE_SIZE+ID_SIZE+KEY_SIZE, kdc_ticket.length()-NONCE_SIZE+ID_SIZE+KEY_SIZE);

    if(TRACER){
		cout << "Session key : " << session_key << endl;
		cout << "Bob's ticket from KDC : " << kdc_bob_ticket << endl;
	}

	big nonce2 = mirvar(0);
    bigdig(NONCE_SIZE, 10, nonce2);
    num = new char[NONCE_SIZE+1];
	otstr(nonce2,num);
    string nonce_bob(num);
    delete[] num;

	string encrypted_nonce = ns::encrypt(nonce_bob, session_key);

	if(TRACER){
		cout << "Nonce : " << nonce_bob << endl;
		cout << "Nonce (Encrypted) : " << encrypted_nonce << endl;
	}

	tcp::Client alice(BOB_PORT, BUFFER_SIZE);
	alice.connect();
	cout << "Alice connected to BOB" << endl
		 << endl;

	alice.send(kdc_bob_ticket);
	alice.send(encrypted_nonce);

	cout << "Sent ticket to Bob" << endl;

	return 0;
}
