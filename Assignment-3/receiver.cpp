/**
    @file   receiver.cpp
    @author Vinit Wagh, vvvinit.w@gmail.com, https://github.com/vvvinit
    @date	2021-10-26, 05:30:42
*/

#include <bits/stdc++.h>
#include <sys/stat.h>
#include "connection.h"
#include "miracl.h"
#include "encryption.cpp"
#include "constants.h"
#include "keys.h"

void decrypt(char* image, int n, string key);
    
int main(){
    cout << endl;
    cout << "===============================" << endl;
    cout << "            Receiver           " << endl;
    cout << "===============================" << endl
         << endl;

    tcp::Server receiver(RECEIVER_PORT, BUFFER_SIZE);
	receiver.listen();
    receiver.accept();
    cout << "Sender connected" << endl
         << endl;

    string session_key;
    receiver.receive(session_key);

    cout << "Session key received" << endl;

    if(TRACER)
        cout << "Session key (encrypted): " << session_key << endl;
    session_key = ns::decrypt(session_key, RECEIVER_PRIVATE_KEY);
    if(TRACER)
        cout << "Session key: " << session_key << endl
             << endl;

    string email_header;
    receiver.receive(email_header);
    if (TRACER)
        cout << "Email header (encrypted): " << email_header << endl;
    email_header = ns::decrypt(email_header, session_key);
    if(TRACER)
        cout << "Email header: " << email_header << endl
             << endl;
    
    if(email_header.length()!=3){
        cerr << "Error : Invalid email." << endl;
        exit(EXIT_FAILURE);
    }


    string email_final;
    receiver.receive(email_final);
    if (TRACER)
        cout << "Email final (encrypted): " << email_final << endl;
    email_final = ns::decrypt(email_final, session_key);
    if(TRACER)
        cout << "Email final: " << email_final << endl
             << endl;

    bool isSubjectPresent = (email_header[0] == '1') ? true : false;
    bool isMessagePresent = (email_header[1] == '1') ? true : false;
    bool isFilePresent = (email_header[2] == '1') ? true : false;

    string email_subject, email_message, filename;
    int file_size;

    char *c_email = new char[10000];
    copy(email_final.begin(), email_final.end(), c_email);
    char *token = strtok(c_email, ",");

    if(isSubjectPresent)
        email_subject = string(token);
    token = strtok(NULL, ",");

    if(isMessagePresent)
        email_message = string(token);
    token = strtok(NULL, ",");

    if(isFilePresent){
        file_size = stoi(string(string(token)));
        token = strtok(NULL, ",");
        filename = string(token);
    }

    if(isFilePresent){
        receiver.setBufferSize(file_size);
        FILE *attachment;
        attachment = fopen(("attachments/" + filename).c_str(), "wb");
        char buffer[BUFFER_SIZE];
        do {
            int num = min(file_size, BUFFER_SIZE);
            receiver.receive(buffer, num);
            decrypt(buffer, num, session_key);
            int offset = 0;
            do {
                size_t written = fwrite(&buffer[offset], 1, num-offset, attachment);
                if (written < 1)
                    return 1;
                offset += written;
            }
            while (offset < num);
            file_size -= num;
        }
        while (file_size > 0);

        cout << "Attachment received and decrypted" << endl << endl;
    }


    if (TRACER&&isSubjectPresent)
        cout << "Email subject: " << email_subject << endl;
    if(TRACER&&isMessagePresent)
        cout << "Email message: " << email_message << endl;

    if (TRACER && isFilePresent)
        cout << "Attachment is saved at /attachments/" << filename << endl
        << endl;

    return 0;
}

void decrypt(char* image, int n, string key){
    int key_t = (int)(key[0]) % 4;
    for (int i = 0; i < n; i++)
        image[i] = (char)((int)image[i] + key_t);
}