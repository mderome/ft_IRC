/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esafar <esafar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 13:53:37 by esafar            #+#    #+#             */
/*   Updated: 2022/12/08 12:53:12 by esafar           ###   ########.fr       */
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
# include <unistd.h>

#include <vector>
#include <poll.h>
#include <map>

#include "user.hpp"
#include "../inc/Channel.hpp"
#include "color.hpp"
#include "./returncode.hpp"

class User;
class Channel;

class Server
{
    typedef std::vector<pollfd>::iterator pollfd_iterator;
	typedef std::map<int, User *>::iterator users_iterator;
    typedef void (Server::*func)(User *, std::string);

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
        void    deleteUser(pollfd_iterator &it);

        void    _indexingCmd();
        void	chooseCmd(User *user);
        
        void    closeConnection(User *user);

		void	_userCmd(User *user, std::string buf);
		void	_passCmd(User *user, std::string buf);
		void	_nickCmd(User *user, std::string buf);
        void	_caplsCmd(User *user, std::string buf);
		void	_quitCmd(User *user, std::string buf);
        void	_pingCmd(User *user, std::string buf);
        void    _privmsgCmd(User *user, std::string param);
        void    _joinCmd(User *user, std::string buf);

    private:
        std::string                         _port;
        std::string                         _password;
        std::string                         _hostname;
        int                                 _listener;
        std::vector<struct pollfd>          _pollfds;
        std::map<int, User *>               _users;
        std::map<std::string, func>         _indexCmd;
        std::map<std::string, Channel>      _channel;
};

#endif