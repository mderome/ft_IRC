#include "../inc/user.hpp"

User::User() : _username(""), _nickname(""), _hostname(""), _realname(""), _server(""), _userAddr(NULL), _password(false), _DoWelcome(false), _user("") {}

User::User(int userFd, struct sockaddr_storage *userAddr) : _nickname(""), _hostname("localhost"), _realname(""), _server("IRC"), _password(false), _DoWelcome(false), _user("")
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

void    User::setPassword(bool password) {
	_password = password;
}

void	User::setUser(std::string user) {
	_user = user;
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

bool	User::getPassword(void) const {
	return (_password);
}

std::string		User::getUser(void) const {
	return (_user);
}

void	User::clearMessage(){
	_message.clear();
}

void	User::sendReply(std::string reply)
{
	reply.append("\r\n");
	size_t	total = 0;
	size_t	nbytes = reply.length();
	int		n;

	while (total < nbytes)
	{
		n = send(_fd, &(reply[total]), nbytes - total, 0);
		if (n == -1) break;
		total += n;
	}

	if (n == -1)
	{
		//std::cout << "Error User::sendReply" << std::endl;//Throw erreur ici
	}
}

bool	User::getDoWelcome(void) const {
	return (_DoWelcome);
}

void	User::DoWelcome(void)
{
	sendReply(RPL_WELCOME(_nickname, _server, _username, _hostname));
	sendReply(RPL_YOURHOST(_nickname, _server));
	sendReply(RPL_CREATED(_nickname, _server, timestamp()));
	sendReply(RPL_MYINFO(_nickname, _server));
	_DoWelcome = true;
}