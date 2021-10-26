/**
    @file   connection.cpp
    @author Vinit Wagh, vvvinit.w@gmail.com, https://github.com/vvvinit
*/

#include "connection.h"

tcp::Server::Server(int port, int buffer_size){
    _buffer_size = buffer_size;
    _port = port;
    _p_buffer = new char[_buffer_size];

    if ((_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        _printError("ERROR : Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR ,&_socket_option, sizeof(_socket_option))){
        _printError("ERROR : Socket attachment failed");
        exit(EXIT_FAILURE);
    }
    
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(port);
    
    if (::bind(_server_fd, (struct sockaddr *)&_address, sizeof(_address)) < 0){
        _printError("ERROR : Socket binding failed");
        exit(EXIT_FAILURE);
    }
    _client_connected = CLIENT_DISCONNECTED;
}

tcp::Server::~Server(){
    delete[] _p_buffer;
}

void tcp::Server::setBufferSize(int size){
    delete[] _p_buffer;
    _p_buffer = new char[size + 1];
}

void tcp::Server::listen(){
    if (::listen(_server_fd, 3) < 0){
        _printError("ERROR : Failed to listen");
        exit(EXIT_FAILURE);
    }
}

void tcp::Server::accept(){
    if ((_new_socket = ::accept(_server_fd, (struct sockaddr *)&_address,(socklen_t*)&_address_length))<0){
        _printError("ERROR : Failed to accept");
        exit(EXIT_FAILURE);
    }
    _client_connected = CLIENT_CONNECTED;
}

int tcp::Server::send(std::string msg){
    if(_client_connected==CLIENT_DISCONNECTED){
        _printError("ERROR : No end-point");
        return -1;
    }
    
    bzero(_p_buffer, _buffer_size);
    std::copy(msg.begin(), msg.end(), _p_buffer);
    _p_buffer[msg.length()] = '\0';
    return ::send(_new_socket, _p_buffer, _buffer_size, 0);
}

int tcp::Server::receive(std::string &msg){
    if(_client_connected==CLIENT_DISCONNECTED){
        _printError("ERROR : No end-point");
        return -1;
    }
    
    bzero(_p_buffer, _buffer_size);
    int code = read(_new_socket, _p_buffer, _buffer_size);
    if (code == CLIENT_DISCONNECTED){
        close(_new_socket);
        _client_connected = false;
        return -1;
    }

    msg = std::string(_p_buffer);
    return code;
}

int tcp::Server::send(std::string msg, int size){
    if(_client_connected==CLIENT_DISCONNECTED){
        _printError("ERROR : No end-point");
        return -1;
    }
    
    bzero(_p_buffer, _buffer_size);
    std::copy(msg.begin(), msg.end(), _p_buffer);
    _p_buffer[msg.length()] = '\0';
    return ::send(_new_socket, _p_buffer, size, 0);
}

int tcp::Server::receive(std::string &msg, int size){
    if(_client_connected==CLIENT_DISCONNECTED){
        _printError("ERROR : No end-point");
        return -1;
    }
    
    bzero(_p_buffer, _buffer_size);
    int code = read(_new_socket, _p_buffer, size);
    if (code == CLIENT_DISCONNECTED){
        close(_new_socket);
        _client_connected = false;
        return -1;
    }

    msg = std::string(_p_buffer);
    return code;
}

int tcp::Server::send(char* msg, int size){
    if(_client_connected==CLIENT_DISCONNECTED){
        _printError("ERROR : No end-point");
        return -1;
    }
    return ::send(_new_socket, msg, size, 0);
}

int tcp::Server::receive(char* msg, int size){
    if(_client_connected==CLIENT_DISCONNECTED){
        _printError("ERROR : No end-point");
        return -1;
    }
    
    bzero(msg, _buffer_size);
    int code = read(_new_socket , msg, size);
    if(code==CLIENT_DISCONNECTED){
        close(_new_socket);
        _client_connected = false;
        return -1;
    }
    return code;
}

tcp::Client::Client(int port, int buffer_size){
    _buffer_size = buffer_size;
    _port = port;
    _p_buffer = new char[_buffer_size];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        _printError("ERROR : Socket creation failed");
        exit(EXIT_FAILURE);
    }

    _serv_addr.sin_family = AF_INET;
    _serv_addr.sin_port = htons(_port);
    _server_connected = false;
}

tcp::Client::~Client(){
    delete[] _p_buffer;
}

void tcp::Client::setBufferSize(int size){
    delete[] _p_buffer;
    _p_buffer = new char[size + 1];
}

void tcp::Client::connect(){
    if(inet_pton(AF_INET, "127.0.0.1", &_serv_addr.sin_addr)<=0){
        _printError("ERROR : Address conversion failed");
        exit(EXIT_FAILURE);
    }

    if (::connect(sock, (struct sockaddr *)&_serv_addr, sizeof(_serv_addr)) < 0){
        _printError("ERROR : Socket connection failed");
        exit(EXIT_FAILURE);
    }
    _server_connected = true;
}

int tcp::Client::send(std::string msg){     
    if(_server_connected==SERVER_DISCONNECTED){
        _printError("ERROR : No end-point");
        return -1;
    }

    bzero(_p_buffer, _buffer_size);
    std::copy(msg.begin(), msg.end(), _p_buffer);
    _p_buffer[msg.length()] = '\0';
    return ::send(sock, _p_buffer, _buffer_size, 0);
}

int tcp::Client::receive(std::string &msg){
    if(_server_connected==SERVER_DISCONNECTED){
        _printError("ERROR : No end-point");
        return 1;
    }
    
    bzero(_p_buffer, _buffer_size);
    int code = read(sock, _p_buffer, _buffer_size);
    if(code==SERVER_DISCONNECTED){
        close(sock);
        _server_connected = false;
        return -1;
    }
    
    msg = std::string(_p_buffer);
    return code;
}

int tcp::Client::send(std::string msg, int size){     
    if(_server_connected==SERVER_DISCONNECTED){
        _printError("ERROR : No end-point");
        return -1;
    }

    bzero(_p_buffer, _buffer_size);
    std::copy(msg.begin(), msg.end(), _p_buffer);
    _p_buffer[msg.length()] = '\0';
    return ::send(sock, _p_buffer, size, 0);
}

int tcp::Client::receive(std::string &msg, int size){
    if(_server_connected==SERVER_DISCONNECTED){
        _printError("ERROR : No end-point");
        return 1;
    }
    
    bzero(_p_buffer, _buffer_size);
    int code = read(sock, _p_buffer, size);
    if(code==SERVER_DISCONNECTED){
        close(sock);
        _server_connected = false;
        return -1;
    }
    
    msg = std::string(_p_buffer);
    return code;
}

int tcp::Client::send(char* msg, int size){     
    if(_server_connected==SERVER_DISCONNECTED){
        _printError("ERROR : No end-point");
        return -1;
    }
    return ::send(sock, msg, size, 0);
}

int tcp::Client::receive(char* msg){
    if(_server_connected==SERVER_DISCONNECTED){
        _printError("ERROR : No end-point");
        return 1;
    }
    
    bzero(msg, _buffer_size);
    int code = read(sock, msg, _buffer_size);
    if(code==SERVER_DISCONNECTED){
        close(sock);
        _server_connected = false;
        return -1;
    }
    return code;
}


void tcp::_printError(std::string msg){
    std::cerr << msg << std::endl;
}