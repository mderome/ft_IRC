#ifndef USER_H
#define USER_H

#pragma once
#include <string>

class User
{
    public:
        User();
        ~User();
        void    setUsername(std::string username);
        void    setNickname(std::string nickname);
        void    setHostname(std::string hostname);
        void    setRealname(std::string realname);
        void    setServer(std::string server);
        void    setRegister();
        std::string getUsername();
        std::string getNickname();
        std::string getHostname();
        std::string getRealname();
        std::string getServer();
        bool        getRegistered();
    private:
        std::string _username;
        std::string _nickname;
        std::string _hostname;
        std::string _realname;
        std::string _server;
        bool        _registered;
};

#endif