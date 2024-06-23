#include "libIRC.hpp"


int main(int ac, char **av)
{
    // VERIFICATION NOMBRE PARAMETRE
    if (ac != 3)
    {
        std::cerr << RED << "Bad format (./ircserv <port> <password>)" << RESET << std::endl;
        return (1);
    }
    // VERIFICATION PORT VALIDE 
    std::string port;
    port.append(av[1]);
    if ((port.find_first_not_of("0123456789") != std::string::npos) || (std::atoi(port.c_str()) < 1024 || std::atoi(port.c_str()) > 65535))
    {
        std::cerr << RED << "Bad format of port param. Only number in 1023 < x < 65536 wanted" << RESET << std::endl;
        return (1);
    }
    // VERIFICATION PASSWORD
    std::string passwrd;
    passwrd.append(av[2]);
    if (passwrd == "" || passwrd.length() > 20)
    {
        std::cerr << RED << "Bad format of password param. Need a password with 20 carac max" << RESET << std::endl;
        return (1);
    }
    //BOUCLE DE LANCEMENT DES SOCKETS
    
    return (0);
}