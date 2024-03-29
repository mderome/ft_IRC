#ifndef USER_HPP
#define USER_HPP

#pragma once
#include <string>

#include "server.hpp" // for sockaddr_storage

class User
{
    public:
        User(void);
        User(int userFd, struct sockaddr_storage *userAddr);
        User(const User &cpy);
        ~User(void);
        void                        setUsername(std::string username);
        void                        setNickname(std::string nickname);
        void                        setHostname(std::string hostname);
        void                        setRealname(std::string realname);
        void                        setServer(std::string server);
        void                        setRegistered();
        void                        setMessage(std::string message);
        void                        setPassword(bool password);
        void                        setUser(std::string user);
        void                        setModes(std::string modes, bool value);
        void	                    addInvitation(std::string target);

        std::string                 getUsername(void);
        std::string                 getNickname(void);
        std::string                 getHostname(void);
        std::string                 getRealname(void);
        std::string                 getServer(void);
        std::string                 getMessage(void);
        int                         getFd(void);
        bool		                getPassword(void) const;
        std::string                 getUser(void) const;
        std::map<std::string, int>  getOperator() const;
        std::map<std::string, bool> getModes() const;
        std::string                 getprefixe() const;

        void                        welcome(void);
        bool                        hasBeenWelcomed(void) const;
        std::string                 timestamp(void);
        void                        sendReply(std::string reply);
        void                        clearMessage(void);
        std::string                 getUserMode();

    private:
        std::string                 _username;
        std::string                 _nickname;
        std::string                 _hostname;
        std::string                 _realname;
        std::string                 _server;
        int                         _fd;
        struct sockaddr_storage     *_userAddr;
        std::string                 _message;
        bool                        _password;
        bool                        _welcomed;
        std::vector<std::string>    _isInvited;

        std::string                 _user;
        std::map<std::string, int>  _operator;
        std::map<std::string, bool> _modes; // channel modes
};

#endif