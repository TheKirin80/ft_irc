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
		return ;
	}
	if (client.getNickCheckState() == false)
    {
		return ;
	}
	// if (client.getUserCheckState() == true) // A CHECK LORS DE L EXEC
    // {
	// 	this->QUIT(fd);
	// 	return ;
	// }
	if (param.empty())
    {
		return ;
	}
	args = multi_split(param, " ");
	if (args.size() < 4)
    {
		return ;
	}
    client.setUsername(args.at(0));
    client.setRealname(formRealname(args));
	client.setUserCheckTrue();
}