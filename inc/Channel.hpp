#ifndef CHANEL_HPP
#define CHANEL_HPP

#include "server.hpp"

class Server;
class User;

class Channel
{
    typedef std::map<std::string, User>::iterator users_iterator;
    private:
        std::string                 _name; // channel name
        std::string                 _pwd; // channel password
        std::map<std::string, User>  _users; // users in channel with fd's
        std::vector<std::string>    _bans; // banned users
        std::vector<std::string>    _old_messages; // all messages in channel
        std::map<std::string, bool> _modes; // channel modes
        std::map<std::string, int>  _operator; // channel operators
        std::string                 _topic; // channel topic
        int                         _limit; // channel limit if mode +l is set

    public:
        Channel(void);
        Channel(User user, std::string name);
        Channel(User user, std::string name, std::string password);
        Channel(const Channel &copy);
        ~Channel();

        Channel &operator=(const Channel &copy);

        std::string getName() const;
        std::map<std::string, User> getUsers() const;
        std::vector<std::string> getBans() const;
        std::vector<std::string> getOldMessages() const;
        std::map<std::string, bool> getModes() const;
        std::map<std::string, int> getOperator() const;

        int getLimit() const;
        std::string getPwd() const;
        std::string getTopic() const;

        void setName(std::string name);
        void setUsers(User users);
        void setBans(std::string bans);
        void setTopic(std::string topic);
        void setOldMessages(std::string old_messages);
        void setModes(std::string modes, bool value);
        void setUsersMode(std::string user, std::string mode, int action);
        void setOperator(User users);
        void setLimit(int limit);
        void setPwd(std::string pwd);

        void addUser(User user);
        void addBan(User user);
        void addOldMessage(std::string message);
        void addOperator(User user);

        void removeUser(User user);
        void removeBan(std::string nick);
        void removeOldMessage(std::string message);
        void removeOperator(User user);

        void clearUsers();
        void clearBans();
        void clearOldMessages();
        void clearModes();
        void clearOperators();
        bool checkUserIsOperatorOnChannel(std::string user);
        std::string getChannelMode();
        void sendToAll(std::string message);
        void sendToAllSaufALui( std::string user, std::string message);

        bool userIsIn(User *user);
        void callPrivmsg(User *user, std::string msg);
};

#endif