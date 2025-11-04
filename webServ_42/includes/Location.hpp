#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "DataStructs.hpp"
#include "Utils.hpp"

/**
 * Location Class
 * 
 * Represents a location block in the webserver configuration (similar to nginx location blocks).
 * Each location defines how the server handles requests for a specific URL path pattern.
 * 
 * Representa um bloco de localização na configuração do webserver (similar aos blocos location do nginx).
 * Cada localização define como o servidor manipula requisições para um padrão de caminho de URL específico.
 * 
 * MEMBERS / MEMBROS:
 * - path: URL path pattern that this location matches (e.g., "/board/content")
 * - root: Root directory for serving files from this location
 * - index: List of index files to try when accessing a directory
 * - allow_methods: HTTP methods permitted for this location (GET, POST, DELETE)
 * - cgi_info: Map of file extensions to CGI binary paths for dynamic content execution
 * - client_body_limit: Maximum allowed size of request body in bytes
 */
class Location
{
public:
	int client_body_limit;
	std::string path;
	std::string root;
	std::vector<std::string> index;
	std::vector<MethodType> allow_methods;
	std::map<std::string, std::string> cgi_info;

public:
	Location();
	~Location();

	void print_location_info();
	std::string get_root();
	std::string get_path();
	static MethodType s_to_methodtype(std::string str);
	std::string getCgiBinary(std::string &extension);
};

#endif