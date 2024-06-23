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
        void	                 add_channel_serv(int fd, std::string name);
        void                     rm_channel_serv(std::string channel);
        void                     rm_client_serv(std::string client);    
        int	                     checkNickname(std::string param); 
        int                      checkDoublon(int fd, std::string param);
        std::vector<std::string> multi_split(std::string param, std::string lim);
        std::vector<std::string> once_split(std::string param, std::string lim);
        int	                     inListChannelServ(std::string name_chan);
        int	                     checkChannelName(std::string name); 
        int	                     inListClientServ(std::string name_client);
        //Initialisation du server et mainloop
        void                     signalHandler(int sig);
        void                     init_server(void);
        void                     accept_new_client(void);
        void                     exec_command(int fd, std::string param);
        void                     receive_new_data(int fd);
        void                     loop_server();
        void                     rm_fd(int fd);

        //COMMAND
        void	PASS(int fd, std::string param);
        void    NICK(int fd, std::string param);
        void	USER(int fd, std::string param);
        void	QUIT(int fd, std::string param);
        void	PART(int fd, std::string param);
        void    CAP(void);
        void	TOPIC(int fd, std::string param);
        void	JOIN(int fd, std::string param);
        void	INVITE(int fd, std::string param);
        void    KICK(int fd, std::string param);
        void	MODE(int fd, std::string param);
        void	PRIVMSG(int fd, std::string param);
    private :
        std::string			    _name;
		int						_port;
		std::string				_passwrd_serv;
		std::vector<Client>		_list_client_serv;
		std::vector<Channel>	_list_channel_serv;
        std::vector<struct pollfd> _list_fd;
        int                     _serv_fd;
        
};

#endif

