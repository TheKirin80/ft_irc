#ifndef Server_HPP
#define Server_HPP

#include "libIRC.hpp"

class Server
{
    public :
        Server(int port, std::string passwrd);
        ~Server(void);
        //getset
        std::string getName(void) const;
        int         getPort(void) const;
        std::string getPasswrdServ(void) const;
        Client      &getClientWithName(std::string client);
        Client      &getClientWithFd(int fd);
        Channel     &getChannelWithName(std::string channel);
        //utilitaire
        void                     rm_channel_serv(std::string channel);
        void                     rm_client_serv(std::string client);
        int	                     checkNickname(std::string param);
        int                      checkDoublon(int fd, std::string param);
        std::vector<std::string> multi_split(std::string param, std::string lim);
        std::vector<std::string> once_split(std::string param, std::string lim);
        int	                     inListChannelServ(const std::string name_chan);
        //COMMAND
        void	PASS(int fd, std::string param);
        void    NICK(int fd, std::string param);
        void	USER(int fd, std::string param);
        void	QUIT(int fd, std::string param);
        void	PART(int fd, std::string param);
        void    CAP(void);
        void	TOPIC(int fd, std::string param);
        void	JOIN(int fd, std::string param); 

    private :
        std::string			    _name;
		int						_port;
		std::string				_passwrd_serv;
		std::vector<Client>		_list_client_serv;
		std::vector<Channel>	_list_channel_serv;
        
};

#endif

