/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esafar <esafar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 13:53:37 by esafar            #+#    #+#             */
/*   Updated: 2022/12/13 13:41:50 by esafar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <cstdlib>
# include <sstream> // std::stringstream
# include <unistd.h>
# include <signal.h>

# include <poll.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>

# include <vector>
# include <map>

# include "Channel.hpp"
# include "user.hpp"
# include "returncode.hpp"
# include "parsing.hpp"

# include "color.hpp"

class User;
class Channel;

class Server : public parsing
{
    typedef std::vector<pollfd>::iterator pollfd_iterator;
	typedef std::map<int, User *>::iterator users_iterator;
    typedef std::map<std::string, Channel>::iterator channel_iterator;
    typedef void (Server::*func)(User *, std::string);

    public:
        Server();
        Server(std::string port, std::string password);
        ~Server();

    private:
        std::string                         _port;
        std::string                         _password;
        std::string                         _hostname;
        int                                 _listener;
        std::vector<struct pollfd>          _pollfds;
        std::map<int, User *>               _users;
        std::map<std::string, func>         _indexCmd;
        std::map<std::string, Channel>      _channel;
        
        std::vector<struct pollfd>      _getPollFd(void)const;
        std::string                     _getPort(void)const;
        std::string                     _getPassword(void)const;
        int                             _getListener(void)const;
        
        void    _createListener(void);
        void    _serverStart(void);
        void    _acceptNewConnection(void);
        void    _addUserToPollFd(int clientFd, struct sockaddr_storage clientAddr);
        void    _printUserData(int clientFd, struct sockaddr_storage clientAddr);
        void    _receiveData(pollfd_iterator &it);
        int     _getMessage(User *user);
        void    _deleteUser(pollfd_iterator &it);

        void    _indexingCmd();
        void	_chooseCmd(User *user);
        
        void    _closeConnection(User *user);

		void	_userCmd(User *user, std::string param);
		void	_passCmd(User *user, std::string param);
		void	_nickCmd(User *user, std::string param);
        void	_caplsCmd(User *user, std::string param);
		void	_quitCmd(User *user, std::string param);
        void	_pingCmd(User *user, std::string param);
        void	_whoCmd(User *user, std::string param);
        // void    _topicCmd(User *user, std::string param);
        void    _privmsgCmd(User *user, std::string param);
        void    _joinCmd(User *user, std::string param);
        void    _listCmd(User *user, std::string param);
        void    _noticeCmd(User *user, std::string param);
        void	_kickCmd(User *user, std::string param);
        void	_partCmd(User *user, std::string param);
        // void    _killCmd(User *user, std::string param);
        void	_inviteCmd(User *user, std::string param);

        bool    checkChannelExistOnNetwork(std::string channel);
        bool    checkUserExistOnNetwork(std::string user);
        void	changeModes(User *user, std::string target, std::string mode, bool value, bool isChannel, std::vector<std::string> *modearg);
        void	_modeCmd(User *user, std::string param);

        void    _clean(void);
};

#endif