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
        //utilitaire
        int	checkNickname(std::string param);
        int checkDoublon(int fd, std::string param);
        //COMMAND
        void	PASS(int fd, std::string param);
        void    NICK(int fd, std::string param);

    private :
        std::string			    _name;
		int						_port;
		std::string				_passwrd_serv;
		std::vector<Client>		_list_client_serv;
		std::vector<Channel>	_list_channel_serv;
        
};

#endif