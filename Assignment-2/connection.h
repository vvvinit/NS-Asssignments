/**
    @file   connection.h
    @author Vinit Wagh, vvvinit.w@gmail.com, https://github.com/vvvinit
*/

#ifndef CONNECTION_H
#define CONNECTION_H

#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

namespace tcp {
    void _printError(std::string msg);

    class Server {
        const int CLIENT_DISCONNECTED = 0;
        const int CLIENT_CONNECTED = 1;

    private:
        int _port;
        char *_p_buffer;
        int _buffer_size;
        bool _client_connected;

        int _server_fd, _new_socket;
        struct sockaddr_in _address;
        int _socket_option = 1;
        int _address_length = sizeof(_address);

    public:
        Server(int PORT, int _buffer_size);
        ~Server();

        void listen();
        void accept();
        int send(std::string msg);
        int receive(std::string &msg);
    };

    class Client {
        const int SERVER_DISCONNECTED = 0;
        const int SERVER_CONNECTED = 1;

    private:    
        int _port;
        char *_p_buffer;
        int _buffer_size;
        bool _server_connected;


        struct sockaddr_in _serv_addr;
        int sock;

    public:
        Client(int PORT, int _buffer_size);
        ~Client();

        void connect();
        int send(std::string msg);
        int receive(std::string &msg);
    };
}

#endif