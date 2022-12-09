/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esafar <esafar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 14:01:27 by esafar            #+#    #+#             */
/*   Updated: 2022/12/09 13:52:49 by esafar           ###   ########.fr       */
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
#include "../inc/returncode.hpp"

bool    run;

Server::Server() {}

Server::Server(std::string port, std::string password) : _hostname("localhost")
{
    _port = port;
    _password = password;

    createListener();
    std::cout << WHITE "* Server created *" END << std::endl;
    _indexingCmd();
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
    int    pollResult;
    serverFd.fd = this->_listener; // File descriptor to poll (to listen)
    serverFd.events = POLLIN; // POLLIN : There is data to read. Manage events expected.
    serverFd.revents = 0; // Manage event detection

    this->_pollfds.push_back(serverFd); // Add serverFd to pollfds vector 

    std::cout << CYAN "Waiting for clients..." END << std::endl;
    while (run)
    {
        // int pollResult = poll(&serverFd, 1, -1); // initially
        // get vector array and throw it to poll
        pollResult = poll(this->_pollfds.data(), this->_pollfds.size(), -1); // poll() waits for an event concerning fd. "-1" is to wait indefinitely
        if (pollResult != -1){
            for (pollfd_iterator it = this->_pollfds.begin(); it != this->_pollfds.end(); it++)
            {
                // std::cout << GREEN "=== poll() success" END << std::endl;
                
                if (it->revents & POLLIN)
                {
                    if (it->fd == this->_listener)
                    {
                        // Accept a new connection
                        std::cout << "listener: " << this->_listener << std::endl;
                        acceptNewConnection(); 
                        break;
                    }
                    // Receive data from client
                    receiveData(it);
                } 
                else if (it->revents & POLLHUP)
                {
                    // Client disconnected
                    std::cout << "Client disconnected" << std::endl;
                    close(it->fd);
                    this->_pollfds.erase(it);
                    break;
                }
                else if (it->revents & POLLERR)
                {
                    // Error
                    std::cerr << "Error: poll()" << std::endl;
                    close(it->fd);
                    this->_pollfds.erase(it);
                    break;
                }
                if (it == this->_pollfds.end())
                    break;
            }
        }
    }
    std::cout << GREEN "Server stopped successfully" END << std::endl;
    std::cout << "Rune equals : " << run << std::endl;
    clean();
}

void    Server::acceptNewConnection(void)
{
    struct sockaddr_storage userAddr;
    socklen_t userAddrSize = sizeof(userAddr);
    
    // Accept a new connection on a specified socket
    int userFd = accept(this->_listener, (struct sockaddr *)&userAddr, &userAddrSize);
    if (userFd == -1)
    {
        std::cerr << RED "Error: accept()" END << std::endl;
        // exit(1);
    }
    else
    {
        std::cout << GREEN "=== accept() success" END << std::endl;

        // Add userFd to pollfds vector
        addUserToPollFd(userFd, userAddr);

        // Print data about the client
        printUserData(userFd, userAddr);
    }
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

void	Server::receiveData(pollfd_iterator &it)
{
	try
	{
		User	*user = this->_users.at(it->fd);
		int		nbytes = 0;

		nbytes = getMessage(user);        
        std::cout << YELLOW "bytes sent: " << nbytes << END << std::endl;
		if (nbytes <= 0)
		{
			if (nbytes == 0) // disconnect
				std::cout << "pollserver : socket " << it->fd << " hung up" << std::endl;
			else // error
				std::cerr << "Error : recv" << std::endl;
			deleteUser(it);
		}
		else
			chooseCmd(user);
	}
	catch (const std::out_of_range &e)
	{
		std::cout << "Out of range from receiveData" << std::endl;
	}
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
		if (nbytes <= 0)
			break ;
		str.append(buf);
	}
    std::cout << std::endl << GREEN "Full message received from client: " MAGENTA << str << END << "";
	user->setMessage(str);    
	return (nbytes);
}

void	Server::closeConnection(User *user)
{
	try
	{
		user->sendReply("Please, enter PASS first. Disconnecting.");
		int	fd = user->getFd();
		close(fd);
		for (pollfd_iterator it = _pollfds.begin(); it != _pollfds.end(); ++it)
		{
			if (fd == it->fd)
			{
				_pollfds.erase(it);
				break;
			}
		}
		_users.erase(fd);
		delete user;
		std::cout << "Disconnecting user on socket " << fd << std::endl;
	}
	catch (const std::out_of_range &e) {}
}

void	Server::deleteUser(pollfd_iterator &it)
{
    try
    {
        User	*user = this->_users.at(it->fd);
        int		fd = user->getFd();

        if (fd != -1)
            close(fd);
        _pollfds.erase(it);
        _users.erase(fd);
        delete user;
        std::cout << "Disconnecting user on socket " << fd << std::endl;
    }
    catch (const std::out_of_range &e) {
        std::cout << "Out of range from deleteUser" << std::endl;
    }
}

// la special pour mderome
// std::map<int, User *> &Server::splitStrToMap(std::string str, std::string sep)
// {
//     std::map<int, User *> map;
//     std::string::size_type pos1, pos2;
//     int i = 0;

//     pos2 = str.find(sep);
//     pos1 = 0;
//     while (std::string::npos != pos2)
//     {
//         map.insert(std::pair<int, User *>(i, new User(str.substr(pos1, pos2 - pos1))));
//         pos1 = pos2 + sep.size();
//         pos2 = str.find(sep, pos1);
//         i++;
//     }
//     if (pos1 != str.length())
//         map.insert(std::pair<int, User *>(i, new User(str.substr(pos1))));
//     return (map);
// }

void    Server::clean(void)
{
    // for (channel_iterator it = _channel.begin(); it != _channel.end(); ++it)
    // {
    //     delete it->second;
    // }
    for (std::map<int, User *>::iterator it = _users.begin(); it != _users.end(); ++it)
    {
        // if (it->first != _sockfd)
        // {
            // close(it->first);
            // _users.erase(it);
            delete it->second;
        // }
    }
    for (pollfd_iterator it = _pollfds.begin(); it != _pollfds.end(); ++it)
    {
        // if (it->fd != _sockfd)
        // {
            close(it->fd);
            // _pollfds.erase(it);
        // }
    }
}