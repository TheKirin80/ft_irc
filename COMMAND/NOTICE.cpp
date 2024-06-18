#include "libIRC.hpp"

void	Server::NOTICE(int fd, std::string param)
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
		return ;
	}
	list_param = ft_split_once(param.get_params(), " ");
	if (list_param.size() == 1)
    {
		return ;
	}
	if (this->inListClientServ(list_param.at(0)) == EXIT_SUCCESS)
    {
		//Message au client 
	}
	else if (this->inListChannelServ(list_param.at(0) == EXIT_SUCCESS)
    {
        //message a tout les personnes du channel   
	}
    else
    {
        return;
    }
}