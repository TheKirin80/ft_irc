#include "../libIRC.hpp"

static std::string	getChannels(std::string param_channel){
	std::string	channels;

	for (size_t i = 0; param_channel[i]; i++){
		if (param_channel[i] == ' '){
			for (size_t j = 0; j < i; j++){
				channels += param_channel[j];
			}
			break ;
		}
	}
	return channels;
}

static std::string	getUsers(std::string param_user){
	std::string	users;
	int			count = 0;
	size_t		begin = 0;
	size_t		end = param_user.size();

	for (size_t i = 0; param_user[i]; i++){
		if (param_user[i] == ' ')
			count++;
		if (count == 1){
			begin = i + 1;
			count++;
		}
		else if (count == 3){
			end = i;
			break ;
		}
	}
	for (size_t i = begin; i < end; i++){
		users += param_user[i];
	}
	return users;
}

static std::string	getComment(std::string param_comment){
	std::string	comment = "";
	int 		count = 0;

	for (size_t i = 0; param_comment[i]; i++){
		if (param_comment[i] == ' ')
			count++;
		if (count == 2){
			for (; param_comment[i];++i){
				comment += param_comment[i];
			}
			break ;
		}
	}
	comment.erase(comment.begin());
	return comment;
}

void	Server::KICK(int fd, std::string param)
{
	std::string					param_channels;
	std::string					param_users;
	std::vector<std::string>	list_channels;
	std::vector<std::string>	list_users;
	std::string					comment = "";
	std::vector<std::string>	args;
    Client                      client = this->getClientWithFd(fd);

	if (client.getUserCheckState() == false){
		//message user not register
		return ;
	}
	args = this->multi_split(param, " ");
	if (args.size() < 2)
    {
        //Error number of parameters
        return;
	}
	param_users = getUsers(param);
	param_channels = getChannels(param);
    if (args.size() > 2) // Cas ou j'ai un commentaire
		comment = getComment(param);
	list_users = this->multi_split(param_users, ",");
	list_channels = this->multi_split(param_channels, ",");
	if (list_users.size() > 1 && list_channels.size() > 1)
    {
		//to_send = "referring to RFC2812: \"The server MUST NOT send KICK messages with multiple channels or users to clients.\"\r\n";
		return ;
	}
    if (list_channels.size() > 1) // Cas de plusieurs channels
    {
        if (this->inListClientServ(param_users) == ERROR) // client a kick existe ?
        {
            //Error no such nickname
            return;
        }
        std::vector<std::string>::iterator it = list_channels.begin();
        for (; it != list_channels.end() ; it++)
        {
            if (this->inListChannelServ(*it) == ERROR) // channel existe ? 
            {
                //Erreur channel n'existe pas
				return;
			}
            if (client.in_channel(*it) == ERROR) // client qui kick possede le channel ?
            {
                //Erreur client n'a pas le channel
				return ;
			}
            if (this->getChannelWithName(*it).in_list_client(client.getNickname()) == ERROR) // client qui kick appartient au channel ? 
            {
                //Erreur client pas dans le channel
                return;
            }
            if (this->getChannelWithName(*it).in_list_op_client(client.getNickname()) == ERROR) // client qui kick operateur ? 
            {
                //Erreur non operateur
				return ;
			}
            if (this->getChannelWithName(*it).in_list_client(param_users) == ERROR) // client a kick appartient au channel ? 
            {
                //Erreur client a kick n'est pas dans le channel
                return;
            }
        }
        it = list_channels.begin();
        for (; it != list_channels.end() ; it++)
        {
            this->PART(this->getClientWithName(param_users).getFd(), *it);
        }
    }
    else // Cas d'un channel
    {
        if (this->inListChannelServ(param_channels) == ERROR) // channel existe ?
        {
            //Error no such channel 
            return;
        }
		if (client.in_channel(param_channels) == ERROR) // client qui kick possede le channel ?
		{
			//Erreur client n'a pas le channel
			return ;
		}
		if (this->getChannelWithName(param_channels).in_list_client(client.getNickname()) == ERROR) // client qui kick appartient au channel ? 
		{
			//Erreur client pas dans le channel
			return;
		}
		if (this->getChannelWithName(param_channels).in_list_op_client(client.getNickname()) == ERROR) // client qui kick operateur ? 
		{
			//Erreur non operateur
			return ;
		}
        std::vector<std::string>::iterator it = list_users.begin();
        for (; it != list_users.end() ; it++)
        {
            if (this->inListClientServ(*it) == ERROR) // client a kick existe ?
			{
				//Error no such nickname
				return;
			}
			if (this->getChannelWithName(param_channels).in_list_client(*it) == ERROR) // client a kick appartient au channel ? 
            {
                //Erreur client a kick n'est pas dans le channel
                return;
            }
        }
		it = list_users.begin();
        for (; it != list_users.end() ; it++)
        {
            this->PART(this->getClientWithName(*it).getFd(), param_channels);
        }
    }
}