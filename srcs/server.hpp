/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mderome <mderome@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 13:53:37 by esafar            #+#    #+#             */
/*   Updated: 2022/12/02 21:23:39 by mderome          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <string>
# include <iostream>
# include <cstdlib>

#include <vector>
#include <poll.h>
#include <map>

#include "user.hpp"
#include "color.hpp"

class User;

class Server
{
    typedef std::vector<pollfd>::iterator pollfd_iterator;

    public:
        Server();
        Server(std::string port, std::string password);
        ~Server();

        std::vector<struct pollfd>  getPollFd(void)const;
        std::string getPort(void)const;
        std::string getPassword(void)const;
        int getListener(void)const;

        void    createListener(void);
        void    serverStart(void);
        void    acceptNewConnection(void);
        void    addUserToPollFd(int clientFd, struct sockaddr_storage clientAddr);
        void    printUserData(int clientFd, struct sockaddr_storage clientAddr);
        void    receiveData(pollfd_iterator &it);
        int     getMessage(User *user);

    private:
        std::string _port;
        std::string _password;
        int     _listener;
        std::vector<struct pollfd> _pollfds;
        std::map<int, User *>   _users;
        
};

#endif