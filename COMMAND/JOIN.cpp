#include "../libIRC.hpp"

static std::string	getChannelParam(std::string param){
	std::string	channels;

	for (size_t i = 0; param[i]; i++){
		if (param[i] == ' '){
			for (size_t j = 0; j < i; j++){
				channels += param[j];
			}
			break ;
		}
		else if (!param[i+1]){
			for (size_t j = 0; j < i+1; j++){
				channels += param[j];
			}
			break ;
		}
	}
	return (channels);
}

static std::string	getKeysParam(std::string param){
	std::string	keys;
	int			flag = 0;
	size_t		begin = 0;
	size_t		end = param.size();

	for (size_t i = 0; param[i]; i++){
		if (param[i] == ' ')
			flag++;
		if (flag == 1){
			begin = i + 1;
			flag++;
		}
		else if (flag == 3){
			end = i;
			break ;
		}
	}
	for (size_t i = begin; i < end; i++){
		keys += param[i];
	}
	return (keys);
}

void	Server::JOIN(int fd, std::string param)
{
	std::string channels = getChannelParam(param);
	std::string keys = getKeysParam(param);
	std::vector<std::string>	list_channels;
	std::vector<std::string>	list_keys;
    Client client = this->getClientWithFd(fd);

	if (client.getUserCheckState() == false)
    {
		this->sendErrMessage(fd, "USER not registered yet so command rejected\r\n");
		return ;
	}
	if (param.empty())
    {
		this->sendErrMessage(fd, ERR_NEEDMOREPARAMS(this->_name, client.getNickname(), "JOIN"));
		return ;
	}
	list_channels = this->multi_split(channels, ",");
	list_keys = this->multi_split(keys, ",");
	if (!list_keys.empty() && list_keys.size() != list_channels.size()) // Penser a check le cas ou pas de keys 
		return ;
	if (channels == "0"){ // Le cas "0" est un cas parti-culier qui fait quitter tout les channels du clients ! A tester en premier !
		if (client.countChannel() == 0)
			return ;
		this->PART(fd, client.list_channel());
		return ;
	}
	std::vector<std::string>::iterator itchannel = list_channels.begin();

	for (; itchannel != list_channels.end(); itchannel++)
    {
		if (this->checkChannelName(*itchannel) == ERROR)
        {
            this->sendErrMessage(fd, ERR_NOSUCHCHANNEL(this->_name, (*itchannel))); 
			return ;
        }
    }
	std::vector<std::string>::iterator itkeys = list_keys.begin();
    itchannel = list_channels.begin();
	for (; itchannel != list_channels.end(); itchannel++)
    {
        Channel chanel = this->getChannelWithName(*itchannel);
		if (inListChannelServ(*itchannel) == OK){ // Cas ou le channel est existant
			if (chanel.in_list_client(client.getNickname()) == OK)
				continue ;
			if (chanel.getOnlyInviteState() == true)
            {
				this->sendErrMessage(fd, ERR_INVITEONLYCHAN(this->_name, client.getNickname(), (*itchannel)))
				return ;
			}
			if (chanel.getLimitClient() != 0){ // Presence d'une limite de client 
				if (chanel.count_client() == chanel.getLimitClient())
                {
                    this->sendErrMessage(fd, ERR_CHANNELISFULL(this->_name, client.getNickname(), (*itchannel)))
					return ;
				}
			}
			if (!chanel.getPasswrd().empty()){ //Presence d'un mot de passe
				if (keys.empty() || (chanel.getPasswrd() != (*itkeys)))
                {
					this->sendErrMessage(fd, ERR_BADCHANNELKEY(this->_name, client.getNickname(), (*itchannel)))
					return ;
				}
			}
			this->getClientWithFd(fd).add_channel(this->getChannelWithName(*itchannel));
			this->getChannelWithName(*itchannel).add_client(this->getClientWithFd(fd));
            //message d'ajout dans un channel existant
		}
		else //Creation d'un nouveau channel
        {
			add_channel_serv(fd, *itchannel);
            //message d'ajout dans un nouveau channel
		}
		itkeys++;
	}
}