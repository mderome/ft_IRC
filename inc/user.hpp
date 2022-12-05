#ifndef USER_HPP
#define USER_HPP

#pragma once
#include <string>

#include "server.hpp" // for sockaddr_storage
#include "./returncode.hpp"

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
        std::string getUsername(void);
        std::string getNickname(void);
        std::string getHostname(void);
        std::string getRealname(void);
        std::string getServer(void);
        std::string getMessage(void);
        int         getFd(void);
        bool		getPassword(void) const;

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
        int         _fd;
        struct sockaddr_storage *_userAddr;
        std::string _message;
        bool        _password;
        bool       _welcomed;
};

#endif