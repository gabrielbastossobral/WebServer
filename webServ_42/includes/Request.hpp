#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Utils.hpp"
#include "DataStructs.hpp"
#include <cstring>

class Request
{
    private:
        int client_fd;
    public:
        std::string method;
        std::string path;
        std::map<std::string, std::string> headers;
        std::string body;
        size_t content_length;
        Request(int client_fd);
        ~Request();

        int parsing(std::string request);
        std::string get_path();
        std::string get_query();
        int get_client_fd();
        std::string get_port();
        bool is_not_method(const std::string method);
        friend std::ostream & operator<<(std::ostream &out, const Request &req)
        {
            out << "Method: " << req.method << "\npath: " << req.path << "\nHeaders:\n"
            << req.headers << "\nBody:" << req.body << "\n";
            return out;
        }
};

#endif