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

	if (client.getUserCheckState() == false)
    {
		this->sendErrMessage(fd, "USER not registered yet so command rejected\r\n");
		return ;
	}
	args = this->multi_split(param, " ");
	if (args.size() < 2)
    {
        this->sendErrMessage(fd, ERR_NEEDMOREPARAMS(this->_name, client.getNickname(), "KICK"));
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
		this->sendErrMessage(fd, "\"The server MUST NOT send KICK messages with multiple channels or users to clients.\" CF RFC 2812\r\n");
		return ;
	}
    if (list_channels.size() > 1) // Cas de plusieurs channels
    {
        if (this->inListClientServ(param_users) == ERROR) // client a kick existe ?
        {
			this->sendErrMessage(fd, ERR_NOSUCHNICK(this->_name, this->getClientWithName(param_users).getNickname()));
            return;
        }
        std::vector<std::string>::iterator it = list_channels.begin();
        for (; it != list_channels.end() ; it++)
        {
            if (this->inListChannelServ(*it) == ERROR) // channel existe ? 
            {
                this->sendErrMessage(fd, ERR_NOSUCHCHANNEL(this->_name, (*it)));
				return;
			}
            if (client.in_channel(*it) == ERROR) // client qui kick possede le channel ?
            {
                this->sendErrMessage(fd, ERR_NOTONCHANNEL(this->_name, client.getNickname(), (*it)));
				return ;
			}
            if (this->getChannelWithName(*it).in_list_op_client(client.getNickname()) == ERROR) // client qui kick operateur ? 
            {
                this->sendErrMessage(fd, ERR_CHANOPRIVSNEEDED(this->_name, client.getNickname(), (*it)));
				return ;
			}
            if (this->getChannelWithName(*it).in_list_client(param_users) == ERROR) // client a kick appartient au channel ? 
            {
                this->sendErrMessage(fd, ERR_USERNOTINCHANNEL(this->_name, client.getNickname(), this->getClientWithName(param_users).getNickname(), (*it)));
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
            this->sendErrMessage(fd, ERR_NOSUCHCHANNEL(this->_name, param_channels));
            return;
        }
		if (client.in_channel(param_channels) == ERROR) // client qui kick possede le channel ?
		{
			this->sendErrMessage(fd, ERR_NOTONCHANNEL(this->_name, client.getNickname(), param_channels));
			return ;
		}
		if (this->getChannelWithName(param_channels).in_list_op_client(client.getNickname()) == ERROR) // client qui kick operateur ? 
		{
			this->sendErrMessage(fd, ERR_CHANOPRIVSNEEDED(this->_name, client.getNickname(), param_channels));
			return ;
		}
        std::vector<std::string>::iterator it = list_users.begin();
        for (; it != list_users.end() ; it++)
        {
            if (this->inListClientServ(*it) == ERROR) // client a kick existe ?
			{
				this->sendErrMessage(fd, ERR_NOSUCHNICK(this->_name, this->getClientWithName(*it).getNickname()));
				return;
			}
			if (this->getChannelWithName(param_channels).in_list_client(*it) == ERROR) // client a kick appartient au channel ? 
            {
                this->sendErrMessage(fd, ERR_USERNOTINCHANNEL(this->_name, client.getNickname(), this->getClientWithName(*it).getNickname(), param_channels));
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