#ifndef USER_HPP
#define USER_HPP

#pragma once
#include <string>

#include "server.hpp" // for sockaddr_storage

#define	RPL_WELCOME(nick, server, user, hostname)	":" + server + " 001 " + nick + " :Welcome to the " + server + " network, " + nick + "[" + user + "@" + hostname + "]"
	;
#define	RPL_YOURHOST(nick, server)					":" + server + " 002 " + nick + " :Your host is " + server + ", running version 1.2.3"
#define	RPL_CREATED(nick, server, timestamp)		":" + server + " 003 " + nick + " :This server was created " + timestamp
#define	RPL_MYINFO(nick, server)					":" + server + " 004 " + nick + " " + server + " irssi 1.2.3 (20210409 0011)"

class User
{
    public:
        User(void);
        User(int userFd, struct sockaddr_storage *userAddr);
        ~User(void);
        void    setUsername(std::string username);
        void    setNickname(std::string nickname);
        void    setHostname(std::string hostname);
        void    setRealname(std::string realname);
        void    setServer(std::string server);
        void    setRegistered();
        void    setMessage(std::string message);
        void    setPassword(bool password);
        void    setUser(std::string user);
        void    setModes(std::string modes, bool value);
        std::string getUsername(void);
        std::string getNickname(void);
        std::string getHostname(void);
        std::string getRealname(void);
        std::string getServer(void);
        bool        getRegistered(void);
        std::string getMessage(void);
        int         getFd(void);
        bool		getPassword(void) const;
        std::string getUser(void) const;
        std::map<std::string, int> getOperator() const;
        void        clearMessage(void);

        void    welcome(void);
        bool    hasBeenWelcomed(void) const;
        std::string timestamp(void);
        void    sendReply(std::string reply);

    private:
        std::string _username;
        std::string _nickname;
        std::string _hostname;
        std::string _realname;
        std::string _server;
        bool        _registered;
        int         _fd;
        struct sockaddr_storage *_userAddr;
        std::string _message;
        bool        _password;
        bool       _welcomed;
        std::string _user;
        std::map<std::string, int>  _operator;
        std::map<std::string, bool> _modes; // channel modes
};

#endif