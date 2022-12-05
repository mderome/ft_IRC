#include "../inc/user.hpp"

User::User() {}

User::User(int userFd, struct sockaddr_storage *userAddr)
{
	this->_fd = userFd;
	this->_userAddr = userAddr;
}

User::~User() {}

void	User::setUsername(std::string username){
	_username = username;
}

void	User::setNickname(std::string nickname){
	_nickname = nickname;
}

void    User::setHostname(std::string hostname){
	_hostname = hostname;
}

void    User::setRealname(std::string realname){
	_realname = realname;
}

void    User::setServer(std::string server){
	_server = server;
}

void    User::setRegistered(){
	_registered = !_registered;
}

void    User::setMessage(std::string message){
	_message = message;
}

std::string	User::getUsername(){
	return(_nickname);
}

std::string	User::getNickname(){
	return(_nickname);
}

std::string    User::getHostname(){
	return(_hostname);
}

std::string    User::getRealname(){
	return(_realname);
}

std::string    User::getServer(){
	return(_server);
}

bool    User::getRegistered(){
	return(_registered);
}

std::string    User::getMessage(){
	return(_message);
}

int    User::getFd(){
    return(_fd);
}

void	User::clearMessage(){
	_message.clear();
}