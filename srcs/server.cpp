/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achane-l <achane-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 14:01:27 by esafar            #+#    #+#             */
/*   Updated: 2022/12/03 15:46:10 by achane-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/server.hpp"
#include <stdio.h>
// #include <string.h>
#include <cstring>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>

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

std::vector<struct pollfd>  Server::getPollFd(void)const
{
    return (this->_pollfds);
}

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
    std::memset(&hint, 0, sizeof(hint));

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
    
    // Create a communication endpoint
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
    
    // Listen for incoming connections. 1000 represents the maximum length of the queue of pending connections.
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
    pollfd serverFd;

    serverFd.fd = this->_listener; // File descriptor to poll (to listen)
    serverFd.events = POLLIN; // POLLIN : There is data to read. Manage events expected.
    serverFd.revents = 0; // Manage event detection

    this->_pollfds.push_back(serverFd); // Add serverFd to pollfds vector 

    std::cout << CYAN "Waiting for clients..." END << std::endl;

    while (1)
    {
        for (pollfd_iterator it = this->_pollfds.begin(); it != this->_pollfds.end(); it++)
        {
            // int pollResult = poll(&serverFd, 1, -1); // initially
            // get vector array and throw it to poll
            int pollResult = poll(this->_pollfds.data(), this->_pollfds.size(), -1); // poll() waits for an event concerning fd. "-1" is to wait indefinitely
            if (pollResult == -1)
            {
                std::cerr << "Error: poll()" << std::endl;
                exit(1);
            }
            // std::cout << GREEN "=== poll() success" END << std::endl;
            
            if (it->revents & POLLIN)
            {
                if (it->fd == this->_listener)
                {
                    // Accept a new connection
                    std::cout << "listener: " << this->_listener << std::endl;
                    acceptNewConnection();
                }
                else
                {
                    std::cout << "receiveData() it->fd: " << it->fd << std::endl;
                    User	*user = this->_users.find(it->fd)->second;
                    std::cout << "user->getFd() = " << user->getFd() << std::endl;
		            int		nbytes = 0;
                    nbytes = getMessage(user);
                    std::cout << "test\n";
                    if (nbytes <= 0)
                    {
                        if (nbytes == 0) // disconnect
                             std::cout << "pollserver : socket " << it->fd << " hung up" << std::endl;
                        else // error
                            std::cerr << "Error : recv" << std::endl;
                        //_delUser(it);
                        std::cout << "delUser" << std::endl;
                    }
                    else
                    {
                        std::cout << "handleCmd" << std::endl;
                        // print message received
                        std::cout << "pollserver : socket " << it->fd << " sent " << nbytes << " bytes : " << user->getMessage() << std::endl;
                        // _handleCmd(user);
                        std::string lol = "001 : lolilol\n";
                    }
                    std::cout << GREEN "=== send() success" END << std::endl;
                    // send(it->fd, lol.c_str(), lol.size(), 0);
                }
            }   
        }
    }
}

void    Server::acceptNewConnection(void)
{
    struct sockaddr_storage userAddr;
    socklen_t userAddrSize = sizeof(userAddr);
    
    // Accept a new connection on a specified socket
    std::cout << "avant accept\n"; 
    int userFd = accept(this->_listener, (struct sockaddr *)&userAddr, &userAddrSize);
    std::cout << "apres accept\n"; 
    if (userFd == -1)
    {
        std::cerr << RED "Error: accept()" END << std::endl;
        exit(1);
    }
    std::cout << GREEN "=== accept() success" END << std::endl;

    // Add userFd to pollfds vector
    addUserToPollFd(userFd, userAddr);

    // Print data about the client
    printUserData(userFd, userAddr);
}


void    Server::addUserToPollFd(int userFd, struct sockaddr_storage userAddr)
{
    pollfd userFdPoll;
    
    userFdPoll.fd = userFd; // File descriptor to poll (to listen)
    userFdPoll.events = POLLIN; // POLLIN : There is data to read. Manage events expected.
    userFdPoll.revents = 0; // Manage event detection
    
    std::cout << "===== userFd: " << userFd << std::endl;
    this->_pollfds.push_back(userFdPoll); // Add userFd to pollfds vector
    this->_users.insert(std::pair<int, User *>(userFd, new User(userFd, &userAddr))); // Add userFd to users map
    // this->_users.insert(std::make_pair(userFd, new User(userFd, &userAddr))); // Add userFd to users map
}

void    Server::printUserData(int userFd, struct sockaddr_storage userAddr)
{
    char host[NI_MAXHOST]; // NI_MAXHOST : Maximum size of a fully-qualified domain name
    char port[NI_MAXSERV]; // NI_MAXSERV : Maximum size of a decimal port number

    // Get the user's IP address and port number
    int result = getnameinfo((struct sockaddr *)&userAddr, sizeof(userAddr), host, NI_MAXHOST, port, NI_MAXSERV, 0);
    if (result)
    {
        std::cerr << RED "Error: getnameinfo()" END << std::endl;
        exit(1);
    }
    std::cout << GREEN "=== getnameinfo() success" END << std::endl;

    std::cout << CYAN "Client connected: " MAGENTA << host << ":" << port << CYAN ", on socket " MAGENTA << userFd << END << std::endl;
}

// void    Server::receiveData(pollfd_iterator &it)
// {
//     User   *user = _users.at(it->fd);
    
//     // Receive data from a client
//     char buf[4096]; // 4096 : Maximum size of a message
//     std::memset(buf, 0, sizeof(buf));
//     int bytesReceived = recv(it->fd, buf, sizeof(buf), 0); // recv() returns the number of bytes received
//     if (bytesReceived <= 0)
//     {
//         // std::cerr << RED "Error: recv()" END << std::endl;
//         close(this->_listener);
//         // exit(1);
//     }
//     // std::cout << GREEN "=== recv() success" END << std::endl;

//     if (std::string(buf, 0, bytesReceived) != "")
//         std::cout << WHITE "Received: " << std::string(buf, 0, bytesReceived) << END << std::endl;
// }

void	Server::receiveData(pollfd_iterator &it)
{
	// try
	// {
        std::cout << "receiveData() it->fd: " << it->fd << std::endl;
		User	*user = this->_users.find(it->fd)->second;
        std::cout << "user->getFd() = " << user->getFd() << std::endl;


        
	// catch (const std::out_of_range &e)
	// {
	// 	std::cout << "Out of range from receiveData" << std::endl;
	// }
}

int	Server::getMessage(User *user)
{
	int			fd = user->getFd();
	int			nbytes = 0;
	char		buf[1024];
	std::string	str;

	user->clearMessage();
    // std::cout << "clear msg" << std::endl;
	while (str.rfind("\r\n") != str.length() - 2 || str.length() < 2)
	{
		memset(buf, 0, sizeof(buf));
		nbytes = recv(fd, buf, sizeof(buf), 0);
        std::cout << nbytes << " buf = " << buf <<std::endl;
		if (nbytes <= 0)
			break ;
		str.append(buf);
	}
    std::cout << MAGENTA "Message received = " << str << END << std::endl;
	user->setMessage(str);    
	return (nbytes);
}