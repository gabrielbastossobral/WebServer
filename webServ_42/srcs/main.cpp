#include "../includes/ServerManager.hpp"
#include "../includes/Config_Parser.hpp"

int main(int argc, char **argv)
{
    if (argc > 2)
        return 0;
    
    std::string config = (argc == 1) ? "./config/default.config" : argv[1];
    Config_Parser configParser(config.c_str());
    std::vector<Server> *servers = configParser.parse();
    ServerManager manager(*servers);
    manager.print_servers_info();
    manager.create_servers();

    while(1)
    {
        manager.wait_to_client();
        manager.accept_sockets();
        manager.treat_request();
    }
    manager.close_servers();
    delete servers;
    return 0;
}