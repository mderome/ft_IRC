/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esafar <esafar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 14:01:27 by esafar            #+#    #+#             */
/*   Updated: 2022/12/01 16:49:19 by esafar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

Server::Server() {}

Server::Server(std::string port, std::string password)
{
    _port = port;
    _password = password;

    createListener();
    std::cout << WHITE "* Server created *" END << std::endl;
    serverStart();
}

Server::~Server() {}

std::string Server::getPort(void)const
{
    return (this->_port);
}

std::string Server::getPassword(void)const
{
    return (this->_password);
}

int Server::getListener(void)const
{
    return (this->_listener);
}

void    Server::createListener(void)
{
    struct addrinfo     hint;
    struct addrinfo     *res = NULL;

    /*
        Set up hints for getaddrinfo call
    */
    memset(&hint, 0, sizeof(hint));

    /*
        Family of address desired:
        AF_UNSPEC : Allow IPv4 or IPv6
    */
    hint.ai_family = AF_UNSPEC;
    /*
        TCP (SOCK_STREAM) or UDP (SOCK_DGRAM)
    
        TCP is a connection based protocol. The connection is established and the two parties
        have a conversation until the connection is terminated by one of the parties or by a network error.
        
        UDP is a datagram based protocol. You send one datagram and get one reply and then the connection terminates.
    */
    hint.ai_socktype = SOCK_STREAM;
    /*
        AI_PASSIVE : Socket address returned will be used in bind() call to bind a socket to all available interfaces.
        and to accept connections from any hosts.
    */
    hint.ai_flags = AI_PASSIVE;
    
    if (getaddrinfo(NULL, this->_port.c_str(), &hint, &res) != 0)
    {
        std::cerr << "Error: getaddrinfo()" << std::endl;
        exit(1);
    }

    std::cout << GREEN "=== getaddrinfo() success" END << std::endl;
    /*
        // DEBUG
    std::cout << CYAN "res->ai_family: " << res->ai_family << std::endl;
    std::cout << "res->ai_socktype: " << res->ai_socktype << std::endl;
    std::cout << "res->ai_protocol: " << res->ai_protocol << std::endl;
    std::cout << "res->ai_addrlen: " << res->ai_addrlen << std::endl;
    std::cout << "res->ai_addr: " << res->ai_addr << std::endl;
    std::cout << "res->ai_canonname: " << res->ai_canonname << std::endl;
    std::cout << "res->ai_next: " << res->ai_next << END << std::endl;
    */
    
    int     listenerFd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (listenerFd == -1)
    {
        std::cerr << "Error: socket()" << std::endl;
        exit(1);
    }
    std::cout << GREEN "=== socket() success" END << std::endl;

    // Connect the socket to the address returned by getaddrinfo()
    if (bind(listenerFd, res->ai_addr, res->ai_addrlen) == -1)
    {
        std::cerr << "Error: bind()" << std::endl;
        freeaddrinfo(res);
        close(listenerFd);
        exit(1);
    }
    freeaddrinfo(res);

    std::cout << GREEN "=== bind() success" END << std::endl;
    
    // Listen for incoming connections
    if (listen(listenerFd, 1000) == -1)
    {
        std::cerr << "Error: listen()" << std::endl;
        close(listenerFd);
        exit(1);
    }
    std::cout << GREEN "=== listen() success" END << std::endl;

    this->_listener = listenerFd;
}

void    Server::serverStart(void)
{
    int     clientFd;
    struct sockaddr_storage clientAddr;
    socklen_t   addrSize = sizeof(clientAddr);
    char    host[NI_MAXHOST];
    char    service[NI_MAXSERV];

    std::cout << CYAN "Waiting for connection..." END << std::endl;

    while (1)
    {
        clientFd = accept(this->_listener, (struct sockaddr *)&clientAddr, &addrSize);
        if (clientFd == -1)
        {
            std::cerr << "Error: accept()" << std::endl;
            continue;
        }
        std::cout << GREEN "=== accept() success" END << std::endl;

        if (getnameinfo((struct sockaddr *)&clientAddr, addrSize, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
        {
            std::cout << CYAN "=== Connection from " << host << " on port " << service << END << std::endl;
        }
        // else
        // {
        //     inet_ntop(clientAddr.ss_family, get_in_addr((struct sockaddr *)&clientAddr), host, NI_MAXHOST);
        //     std::cout << CYAN "=== Connection from " << host << " on port " << ntohs(get_in_port((struct sockaddr *)&clientAddr)) << END << std::endl;
        // }
        close(clientFd);
    }
}