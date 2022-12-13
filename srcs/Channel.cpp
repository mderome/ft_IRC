#include "../inc/Channel.hpp"

Channel::Channel(void)
{
    _limit = 100;
    _modes.insert(std::pair<std::string, bool>("m", false));
    _modes.insert(std::pair<std::string, bool>("n", false));
    _modes.insert(std::pair<std::string, bool>("p", false));
    _modes.insert(std::pair<std::string, bool>("s", false));
    _modes.insert(std::pair<std::string, bool>("t", false));
    _modes.insert(std::pair<std::string, bool>("i", false));
    _modes.insert(std::pair<std::string, bool>("l", false));
    _modes.insert(std::pair<std::string, bool>("k", false));
}

Channel::Channel(User *user, std::string name)
{
    _name = name;
    _limit = 0;
    // _pwd = NULL;
    _users.insert(std::pair<std::string, User *>(user->getNickname(), user));
    _operator.insert(std::pair<std::string, int>(user->getNickname(), true));
    _modes.insert(std::pair<std::string, bool>("m", false));
    _modes.insert(std::pair<std::string, bool>("n", false));
    _modes.insert(std::pair<std::string, bool>("p", false));
    _modes.insert(std::pair<std::string, bool>("s", false));
    _modes.insert(std::pair<std::string, bool>("t", false));
    _modes.insert(std::pair<std::string, bool>("i", false));
    _modes.insert(std::pair<std::string, bool>("l", false));
    _modes.insert(std::pair<std::string, bool>("k", false));
}

Channel::Channel(User *user, std::string name, std::string password)
{
    _name = name;
    _limit = 0;
    _pwd = password;
    _users.insert(std::pair<std::string, User *>(user->getNickname(), user));
    _operator.insert(std::pair<std::string, int>(user->getNickname(), true));
    _modes.insert(std::pair<std::string, bool>("m", false));
    _modes.insert(std::pair<std::string, bool>("n", false));
    _modes.insert(std::pair<std::string, bool>("p", false));
    _modes.insert(std::pair<std::string, bool>("s", false));
    _modes.insert(std::pair<std::string, bool>("t", false));
    _modes.insert(std::pair<std::string, bool>("i", false));
    _modes.insert(std::pair<std::string, bool>("l", false));
    _modes.insert(std::pair<std::string, bool>("k", false));
}

Channel::Channel(const Channel &copy)
{
    *this = copy;
}

Channel::~Channel()
{
}

Channel &Channel::operator=(const Channel &copy)
{
    if (this != &copy)
    {
        _name = copy._name;
        _pwd = copy._pwd;
        _users = copy._users;
        _bans = copy._bans;
        _old_messages = copy._old_messages;
        _modes = copy._modes;
        _operator = copy._operator;
        _limit = copy._limit;
    }
    return (*this);
}

std::string Channel::getName() const
{
    return (_name);
}

std::string Channel::getTopic() const
{
    return (_topic);
}

std::map<std::string, User *> Channel::getUsers() const
{
    return (_users);
}

std::vector<std::string> Channel::getBans() const
{
    return (_bans);
}

std::vector<std::string> Channel::getOldMessages() const
{
    return (_old_messages);
}

std::map<std::string, bool> Channel::getModes() const
{
    return (_modes);
}

std::map<std::string, int> Channel::getOperator() const
{
    return (_operator);
}

void Channel::setName(std::string name)
{
    _name = name;
}

void Channel::setTopic(std::string topic)
{
    _topic = topic;
}

void Channel::setUsers(User *users)
{
    _users.insert(std::pair<std::string, User *>(users->getNickname(), users));
}


void Channel::setUsersMode(std::string user, std::string mode, int action)
{
    if (action < 0)
        _users[user]->setModes(mode, false);
    else if (action > 0)
        _users[user]->setModes(mode, true);
}

void Channel::setBans(std::string bans)
{
    _bans.push_back(bans);
}

void Channel::setOldMessages(std::string old_messages)
{
    _old_messages.push_back(old_messages);
}

void Channel::setModes(std::string modes, bool value)
{
    _modes[modes] = value;
}

void Channel::setOperator(User *users)
{
    _operator.insert(std::pair<std::string, bool>(users->getNickname(), true));
}

void Channel::setLimit(int limit)
{
    _limit = limit;
}

void Channel::setPwd(std::string pwd)
{
    _pwd = pwd;
}

void Channel::removeUser(User *user)
{
    std::string tmp = user->getNickname();
    std::cout << RED  << "delete user : " << tmp << std::endl;
    _users.erase(tmp);
}

void Channel::removeBan(std::string ban)
{
    for (std::vector<std::string>::iterator it = _bans.begin(); it < _bans.end(); it++){
        if (ban == it->data())
            _bans.erase(it);
    }
}

void Channel::removeOperator(User *user)
{
    for (std::map<std::string, User *>::iterator it = _users.begin(); it != _users.end(); it++){
        if (user->getNickname() == it->second->getNickname())
            _users.erase(it);
    }
}

void Channel::removeOldMessage(std::string old_message)
{
    for (std::vector<std::string>::iterator it = _old_messages.begin(); it < _old_messages.end(); it++){
        if (old_message == it->data())
            _old_messages.erase(it);
    }
}

int Channel::getLimit() const
{
    return (_limit);
}

std::string Channel::getPwd() const
{
    return (_pwd);
}

void Channel::addUser(User *user)
{
    _users.insert(std::pair<std::string, User *>(user->getNickname(), user));
}

void Channel::addBan(User *user)
{
    _bans.push_back(user->getNickname());
}

void Channel::addOldMessage(std::string old_message)
{
    _old_messages.push_back(old_message);
}   

void Channel::addOperator(User *user)
{
    _operator.insert(std::pair<std::string, int>(user->getNickname(), true));
}

void Channel::clearUsers()
{
    _users.clear();
}

void Channel::clearBans()
{
    _bans.clear();
}

void Channel::clearOldMessages()
{
    _old_messages.clear();
}

void Channel::clearOperators()
{
    _operator.clear();
}

void Channel::clearModes()
{
    _modes.clear();
}

void Channel::sendToAll(std::string message)
{
    for (std::map<std::string, User *>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        it->second->sendReply(message);
    }
}

void Channel::sendToAllSaufALui( std::string user, std::string message)
{
    for (std::map<std::string, User *>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        if (it->first != user)
        {
            if (it->second->getFd() == -1)
                return;
            it->second->sendReply(message);
            // std::cout << "send " << n << " bytes" << std::endl;
            // std::cout << "send " << message << std::endl;
            // std::cout << "send " << message.length() << std::endl;
            // std::cout << "send fd: " << it->second->getFd() << std::endl;
        }
    }
}

bool    Channel::checkUserIsOperatorOnChannel(std::string user){
    try{
        _users.at(user);
        return (true);
    }
    catch(const std::out_of_range &e){
        return (false);
    }
}

std::string Channel::getChannelMode(){
    std::string minus = "-";
    std::string plus = "+";

    for (std::map<std::string, bool>::iterator it = _modes.begin(); it != _modes.end(); it++){
        if (it->second)
            plus += it->first;
        else
            minus += it->second;
    }
    return (minus + plus);
}

bool	Channel::userIsIn(User *user)
{
	try
	{
		_users.at(user->getNickname()); // instead of _users.at(user->getFd())
	}
	catch (const std::out_of_range &e)
	{
		return (false);
	}
	return (true);
}

bool	Channel::userIsIn(std::string user)
{
	try
	{
		_users.at(user); // instead of _users.at(user->getFd())
	}
	catch (const std::out_of_range &e)
	{
		return (false);
	}
	return (true);
}

void	Channel::callPrivmsg(User *user, std::string msg)
{
	std::string	nick = user->getNickname();

	// if (_mode.find('m') != std::string::npos && !userIsOperator(user) && !userIsModerate(user))
	// 	return (user->sendReply(ERR_CANNOTSENDTOCHAN(_name)));
	// for (users_iterator it = _users.begin(); it != _users.end(); ++it)
	// {
    //     // if 
	// 	// if (it->second->getNickname() != user->getNickname())
	// 	// 	it->second->sendReply(":" + user->getNickname() + "@IRC PRIVMSG " + nick + " :" + msg + "\r\n");
	// }
    user->sendReply(":" + user->getNickname() + "@IRC PRIVMSG " + nick + " :" + msg + "\r\n");
}