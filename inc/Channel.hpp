#ifndef CHANEL_HPP
#define CHANEL_HPP

#include "user.hpp"
#include <vector>
#include <string>
#include <map>
#include "server.hpp"

class Channel
{
    private:
        std::string                 _name; // channel name
        std::string                 _pwd; // channel password
        std::map<std::string, int>  _users; // users in channel with fd's
        std::vector<std::string>    _bans; // banned users
        std::vector<std::string>    _old_messages; // all messages in channel
        std::map<std::string, bool> _modes; // channel modes
        std::map<std::string, int>  _operator; // channel operators
        int                         _limit; // channel limit if mode +l is set

    public:
        Channel(User user, std::string name);
        Channel(User user, std::string name, std::string password);
        Channel(const Channel &copy);
        ~Channel();

        Channel &operator=(const Channel &copy);

        std::string getName() const;
        std::map<std::string, int> getUsers() const;
        std::vector<std::string> getBans() const;
        std::vector<std::string> getOldMessages() const;
        std::map<std::string, bool> getModes() const;
        std::map<std::string, int> getOperator() const;

        void setName(std::string name);
        void setUsers(std::map<std::string, int> users);
        void setBans(std::vector<std::string> bans);
        void setOldMessages(std::vector<std::string> old_messages);
        void setModes(std::map<std::string, bool> modes);
        void setOperator(std::map<std::string, int> operator);

        void addUser(std::string nick, int fd);
        void addBan(std::string nick);
        void addOldMessage(std::string message);
        void addOperator(std::string nick, int fd);

        void removeUser(std::string nick);
        void removeBan(std::string nick);
        void removeOldMessage(std::string message);
        void removeOperator(std::string nick);

        void clearUsers();
        void clearBans();
        void clearOldMessages();
        void clearModes();
        void clearOperator();

        void sendToAll(std::string message, Server *server);
};

#endif