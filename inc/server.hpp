/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achane-l <achane-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 13:53:37 by esafar            #+#    #+#             */
/*   Updated: 2022/12/03 16:41:10 by achane-l         ###   ########.fr       */
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

class Server
{
    typedef std::vector<pollfd>::iterator pollfd_iterator;
    typedef void (Server::*func)(User *, std::string);

    public:
        Server();
        Server(std::string port, std::string password);
        ~Server();
        std::map<std::string, func> _indexCmd;
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

        void    _indexingCmd();
        void	Server::chooseCmd(User *user);
        void	pass_cmd(User *user, std::string param);
        void	nick_cmd(User *user, std::string param);
        void	ping_cmd(User *user, std::string param);
        void	oper_cmd(User *user, std::string param);
        void	quit_cmd(User *user, std::string param);

    private:
        std::string _port;
        std::string _password;
        std::string _hostname;
        int     _listener;
        std::vector<struct pollfd> _pollfds;
        std::map<int, User *>   _users;
        
};

#endif