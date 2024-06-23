#include "../libIRC.hpp"

void	Server::PRIVMSG(int fd, std::string param)
{
	std::vector<std::string>	list_param;
    Client client = this->getClientWithFd(fd);

	if (client.getUserCheckState() == false)
    {
        //USER not registered yet
		return ;
	}
	if (param.empty())
    {
		//ERR_NORECIPIENT
		return ;
	}
	list_param = this->once_split(param, " ");
	if (list_param.size() == 1)
    {
		//ERR_NOTEXTTOSEND
		return ;
	}
	if (this->inListClientServ(list_param.at(0)) == OK)
    {
		//Message au client 
	}
	else if (this->inListChannelServ(list_param.at(0)) == OK)
    {
        //message a tout les personnes du channel   
	}
    else
    {
        //ERR_NOSUCHNICK
        return;
    }
}