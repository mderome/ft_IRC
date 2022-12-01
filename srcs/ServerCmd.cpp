#include "../inc/User.hpp"

void	cmd_pass(User *user, std::string param){
	if (!param.length())
		return;//si pas de param  ERR_NEEDMOREPARAMS
	if (user->getRegistered())
		return; // throw Err ERR_ALREADYREGISTRED
	if (!param.compare(_password))//private value in server
		return;// ERR_PASSWDMISMATCH 
	user->setRegister();
}

void	nick_cmd(User *user, std::string param){
	if (param.empty())
		return; // ERR_NONICKNAMEGIVEN
	// parse for nickname
	// boucle for avec iterator je check nicname
	user->setNickname(param);
}