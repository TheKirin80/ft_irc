#include "libIRC.hpp"

void	Server::TOPIC(int fd, std::string param)
{
	std::vector<std::string>	args;
    Client client = this->getClientWithFd(fd);

	if (client.getUserCheckState() == false)
    {
		return ;
	}
	if (param.empty())
    {
		return ;
	}
	args = once_split(param, " ");
	if (client.in_channel(args.at(0)) == ERROR)
    {
		return ;
	}
	if (this->getChannelWithName(args[0]).getRestrictedTopicState() == true) //Verification si le client est op en cas de restricted topic
    {
		if (this->getChannelWithName(args.at(0)).in_list_op_client(client.getNickname()) == ERROR)
        {
			return ;
		}
	}
	if (args.size() == 1)
    { // Cas ou l'on passe uniquement le channel sans topic, il faut afficher le topic s'il y en a un
		if (!(this->getChannelWithName(args.at(0)).getTopic().empty()))
        {
            //message affichant le topic avec les bons usages 
		}
		else
        {
			//message prevenant l'absence de topic
		}
	} 
	else // Modification du topic avec annonce a tout les utilisateurs du channel via une alerte
    {
        this->getChannelWithName(args.at(0)).setTopic(args.at(1));
        //message prevenant tout les utilisateurs du channel que le topic vient d'etre modifie
	}
}