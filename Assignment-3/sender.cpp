/**
    @file   sender.cpp
    @author Vinit Wagh, vvvinit.w@gmail.com, https://github.com/vvvinit
    @date	2021-10-26, 03:09:05
*/

#include <bits/stdc++.h>
#include <sys/stat.h>
#include "connection.h"
#include "miracl.h"
#include "encryption.cpp"
#include "constants.h"
#include "keys.h"

void encrypt(char *image, int n, string key);

int main(){
    cout << endl;
    cout << "===============================" << endl;
    cout << "              Sender           " << endl;
    cout << "===============================" << endl
         << endl;

	tcp::Client sender(RECEIVER_PORT, BUFFER_SIZE);
	sender.connect();
	cout << "Sender connected to Receiver" << endl
		 << endl;

    anyKey("Press return to generate session key!");

    miracl *mip=mirsys(5000,10);
	irand(time(0));

    big session_key = mirvar(0);
    bigdig(SESSION_KEY_SIZE, 10, session_key);
	string s_session_key = big_to_string(session_key);

    cout << "Session key generated!" << endl
         << endl;
    
    string encrypted_session_key = ns::encrypt(s_session_key, RECEIVER_PUBLIC_KEY);


    if(TRACER)
        cout << "Session key: " << s_session_key << endl;
    if(TRACER)
        cout << "Session key (encrypted): " << encrypted_session_key << endl
             << endl;

    string email_subject;
    cout << "Subject: ";
    getline(cin, email_subject);

    string email_message;
    cout << "Message: ";
    getline(cin, email_message);

    string filename;
    cout << "Paste the file to be attached in the current directory and enter the filename (Press enter if you don't want to attach any file): ";
    getline(cin, filename);

    bool isFileAttached = (filename.length() == 0)? false : true;

    FILE *attachment;
    long file_size;
    if(isFileAttached){
        char *c_filename = new char[100];
        copy(filename.begin(), filename.end(), c_filename);
        cout << endl
             << "Checking attachment..." << endl;
        struct stat  buffer;  
        if ((stat (c_filename, &buffer) != 0)){
            fclose(attachment);
            cerr << "Error : Invalid file" << endl;
            exit(EXIT_FAILURE);
        }
        attachment = fopen(c_filename, "rb");
        fseek(attachment, 0, SEEK_END);
        file_size = ftell(attachment);
        cout << "Success" << endl
             << endl;
    }

    string email_header;
    email_header += (email_subject.length() > 0) ? "1" : "0";
    email_header += (email_message.length() > 0) ? "1" : "0";
    email_header += (isFileAttached) ? "1" : "0";

    string email_final;
    email_final += (email_subject.length() > 0) ? email_subject + "," : "";
    email_final += (email_message.length() > 0) ? email_message + "," : "";
    email_final += (isFileAttached) ? to_string(file_size) + "," + filename : "";

    if(email_final.length()==0){
        cerr << "Error : email empty." << endl;
        exit(EXIT_FAILURE);
    }

    sender.send(encrypted_session_key);

    string encrypted_email_header;
    if (TRACER)
        cout << "Email header: " << email_header << endl;
    encrypted_email_header = ns::encrypt(email_header, s_session_key);
    if(TRACER)
        cout << "Email header (encrypted): " << encrypted_email_header << endl
             << endl;

    sender.send(encrypted_email_header);

    string encrypted_email_final;
    if (TRACER)
        cout << "Email: " << email_final << endl;
    encrypted_email_final = ns::encrypt(email_final, s_session_key);
    if(TRACER)
        cout << "Email (encrypted): " << encrypted_email_final << endl
             << endl;

    sender.send(encrypted_email_final);

    if(isFileAttached){
        cout << "Attaching encrypted file with email" << endl;
        sender.setBufferSize(file_size);

        fseek(attachment, 0, SEEK_END);
        file_size = ftell(attachment);
        fseek(attachment, 0, SEEK_SET);

        char buffer[BUFFER_SIZE];
        do {
            size_t num = min(file_size, (long)sizeof(buffer));
            num = fread(buffer, sizeof(char), num, attachment);
            if (num < 1)
                return false;
            encrypt(buffer, num, s_session_key);
            sender.send(buffer, num);
            file_size -= num;
        } while (file_size > 0);
    }

    cout << endl << "Email sent!" << endl
         << endl;

    return 0;
}

void encrypt(char* image, int n, string key){
    int key_t = (int)(key[0]) % 4;
    for (int i = 0; i < n; i++)
        image[i] = (char)((int)image[i] - key_t);
}