#include "../libIRC.hpp"

static std::string	formRealname(std::vector<std::string> args){
	std::string realname;

	if (args[3].find(":") == 0)
    {
        std::vector<std::string>::iterator it =  args.begin()+3;
		for (; it != args.end(); it++){
			realname += (*it) + " ";
		}
	}
	else
		return (args[3]);
	realname.erase(realname.end() - 1);
	return (realname);
}

void	Server::USER(int fd, std::string param){
	std::vector<std::string>	args;
    Client client = this->getClientWithFd(fd);

	if (client.getPassCheckState() == false)
    {
		this->sendErrMessage(fd, "PASS not done, need to set it before doing USER\r\n");
		return ;
	}
	if (client.getNickCheckState() == false)
    {
		this->sendErrMessage(fd, "NICK not done, need to set it before doing USER\r\n");
		return ;
	}
	if (client.getUserCheckState() == true) 
    {
		this->sendErrMessage(fd, ERR_ALREADYREGISTRED(this->_name, client.getNickname()));
		this->QUIT(fd, "");
		return ;
	}
	if (param.empty())
    {
		this->sendErrMessage(fd, ERR_NEEDMOREPARAMS(this->_name, client.getNickname(), "USER"));
		return ;
	}
	args = multi_split(param, " ");
	if (args.size() < 4)
    {
		this->sendErrMessage(fd, ERR_NEEDMOREPARAMS(this->_name, client.getNickname(), "USER"));
		return ;
	}
    this->getClientWithFd(fd).setUsername(args.at(0));
    this->getClientWithFd(fd).setRealname(formRealname(args));
	this->getClientWithFd(fd).setUserCheckTrue();
	this->sendRepMessage(fd, RPL_WELCOME(this->_name, client.getNickname()));
	this->sendRepMessage(fd, RPL_YOURHOST(this->_name, client.getNickname()));
	this->sendRepMessage(fd, RPL_INFO(this->_name, client.getNickname()));
}