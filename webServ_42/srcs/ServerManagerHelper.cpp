#include "../includes/ServerManager.hpp"

void ServerManager::print_servers_info()
{
    std::cout << "=================================================\n";
    std::cout << "            Total Server Informations            \n";
    std::cout << "=================================================\n";
    std::map<std::string, Server*>::iterator it;
    for (it = default_servers.begin(); it != default_servers.end(); it++)
        (*it).second->print_server_info();
    std::cout << "=================================================\n";
}

bool	ServerManager::is_allowed_method(std::vector<MethodType> allow_methods, std::string method) 
{
    std::cout << GREEN << "[DEBUG] Checking allowed method: " << method << RESET << std::endl;
    if (method == "GET")
        return true;
    for (std::vector<MethodType>::iterator it = allow_methods.begin(); 
    it != allow_methods.end(); it++)
    {
        if (method == methodtype_to_s(*it))
            return true;
    }
    return false;
}

bool	ServerManager::is_loc_check(std::string path, Client &client)
{
    std::cout << GREEN << "[DEBUG] Checking location for path: " << path << RESET << std::endl;
    Location *cur_loc = client.server->get_cur_location(path);
    if (!cur_loc)
    {
        std::cout << YELLOW << "[DEBUG] No location found" << RESET << std::endl;
        return false;
    }
    std::string root = cur_loc->path;
    if (path == root)
        return true;
    return false;
}

bool	ServerManager::is_request_done(char *request)
{
    char *body = strstr(request, "\r\n\r\n");
    if (!body)
        return false;
    body += 4;
    if (ft_strnstr(request, "chunked", strlen(request) - strlen(body)))
    {
        if (strstr(body, "\r\n\r\n"))
            return true;
        return false;
    }
    else if (ft_strnstr(request, "Content-Length", strlen(request) - strlen(body)))
    {
        if (strstr(body, "\r\n\r\n"))
            return true;
        char *start = ft_strnstr(request, "Content-Length: ", strlen(request) - strlen(body)) + 16;
        char *end = strstr(start, "\r\n");
        char *len = strndup(start, end - start);
        int len_i = atoi(len);
        free(len);
        if ((size_t)len_i <= strlen(body))
            return true;
        return false;
    }
    else if (ft_strnstr(request, "boundary=", strlen(request) - strlen(body)))
    {
        if (strstr(body, "\r\n\r\n"))
            return true;
        return false;
    }
    return true;
}

bool ServerManager::is_response_timeout(Client& client)
{
    static timeval tv;
    
    gettimeofday(&tv, NULL);
    if (tv.tv_sec - client.get_last_time().tv_sec > client.server->recv_timeout.tv_sec)
	{
        std::cout << RED << "[DEBUG] Timeout detected!" << RESET << std::endl;
        return true;
    }
    client.set_last_time_sec(tv);
    return false;
}

bool	ServerManager::is_cgi(Request *request, Location *loc)
{
    std::cout << GREEN << "[DEBUG] Checking if is CGI request" << RESET << std::endl;
    for (std::map<std::string, std::string>::iterator it = loc->cgi_info.begin();
    it != loc->cgi_info.end(); it++)
    {
        std::cout << "get_path: " << request->get_path() << "\n";
        std::cout << "it->first: " << it->first << "\n"; 
        if (request->get_path().find(it->first) != std::string::npos)
            return true;
    }
    return false;
}

std::string ServerManager::methodtype_to_s(MethodType method)
{
    if (method == GET)
        return "GET";
    else if (method == POST)
        return "POST";
    else if (method == DELETE)
        return "DELETE";
    return "";
}

const char *ServerManager::find_content_type(const char *path)
{
    std::cout << GREEN << "[DEBUG] Finding content type for: " << path << RESET << std::endl;
    const char *last_dot = strrchr(path, '.');
    if (last_dot)
    {
        if (strcmp(last_dot, ".css") == 0) return "text/css";
        if (strcmp(last_dot, ".csv") == 0) return "text/csv";
        if (strcmp(last_dot, ".html") == 0) return "text/html";
        if (strcmp(last_dot, ".js") == 0) return "application/javascript";
        if (strcmp(last_dot, ".json") == 0) return "application/json";
        if (strcmp(last_dot, ".pdf") == 0) return "application/pdf";
        if (strcmp(last_dot, ".gif") == 0) return "image/gif";
        if (strcmp(last_dot, ".jpeg") == 0) return "image/jpeg";
        if (strcmp(last_dot, ".jpg") == 0) return "image/jpeg";
        if (strcmp(last_dot, ".png") == 0) return "image/png";
        if (strcmp(last_dot, ".svg") == 0) return "image/svg+xml";
    }
    return "text/plain";
}

std::string ServerManager::find_path_in_root(std::string path, Client &client)
{
    std::cout << GREEN << "[DEBUG] Finding path in root: " << path << RESET << std::endl;
    std::string full_path = "";
    std::string location;
    full_path.append(client.get_root_path(path));
    Location *loc = client.server->get_cur_location(path);
    if (loc)
        location = loc->path;
    else
        location = "";
    std::string str = path.substr(location.length());
    full_path.append(str);
    std::cout << GREEN << "[DEBUG] Full path: " << full_path << RESET << std::endl;
    return full_path;
}

std::string ServerManager::get_status_cgi(std::string& cgi_ret)
{
    std::string ret;
    size_t pos = cgi_ret.find("Status:");
    
    if (pos == std::string::npos)
    {
        // Se não tem Status: na resposta, assumir 200 OK
        std::cout << "[DEBUG] No Status header found, assuming 200 OK" << std::endl;
        return "200";
    }
    
    size_t end = cgi_ret.find("\n", pos);
    if (end == std::string::npos)
        return "";
    
    std::string status_line = cgi_ret.substr(pos, end - pos);
    size_t space = status_line.find(" ");
    if (space == std::string::npos)
        return "";
    
    ret = status_line.substr(space + 1, 3);
    std::cout << "[DEBUG] Found Status: " << ret << std::endl;
    return ret;
}

void	ServerManager::write_file_in_path(Client &client, std::string content, std::string path)
{
    std::cout << YELLOW << "[DEBUG] Writing file to: " << path << RESET << std::endl;
    std::cout << "> write in: " << path << "\n";
    size_t index = path.find_last_of("/");
    std::string file_name = path.substr(index + 1);
    std::string folder_path = path.substr(0, index);

    std::string command = "mkdir -p " + folder_path;
    system(command.c_str());
    FILE *fp = fopen(path.c_str(), "w");
    if (!fp)
    {
        std::cout << RED << "[DEBUG] Failed to open file for writing" << RESET << std::endl;
        send_error_page(500, client);
        return;
    }

    fwrite(content.c_str(), content.size(), 1, fp);
    fclose(fp);
    std::cout << GREEN << "[DEBUG] File written successfully" << RESET << std::endl;
}