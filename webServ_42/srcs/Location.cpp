/**
 * Location.cpp
 * 
 * WHAT DOES THE LOCATION CLASS DO? / O QUE O LOCATION FAZ?
 * =========================================================
 * 
 * The Location class represents a location block in the webserver configuration,
 * similar to nginx's location directive. It defines how the server should handle
 * requests for specific URL paths.
 * 
 * A classe Location representa um bloco de localização na configuração do webserver,
 * similar à diretiva location do nginx. Define como o servidor deve manipular
 * requisições para caminhos de URL específicos.
 * 
 * KEY RESPONSIBILITIES / RESPONSABILIDADES PRINCIPAIS:
 * ---------------------------------------------------
 * 1. Store path-specific configuration settings
 *    Armazenar configurações específicas para cada caminho
 * 
 * 2. Define allowed HTTP methods (GET, POST, DELETE) for each path
 *    Definir métodos HTTP permitidos (GET, POST, DELETE) para cada caminho
 * 
 * 3. Configure CGI execution for dynamic content (PHP, Python, etc.)
 *    Configurar execução CGI para conteúdo dinâmico (PHP, Python, etc.)
 * 
 * 4. Set custom root directories and index files for specific paths
 *    Definir diretórios raiz e arquivos de índice customizados para caminhos específicos
 * 
 * 5. Control client body size limits per location
 *    Controlar limites de tamanho do corpo do cliente por localização
 * 
 * EXAMPLE USAGE IN CONFIG / EXEMPLO DE USO NA CONFIGURAÇÃO:
 * ---------------------------------------------------------
 * location /board/content {
 *     allow_methods GET POST DELETE;
 *     root ./www/html/contents;
 *     index board.html;
 *     cgi_info .php php-cgi;
 * }
 * 
 * In this example, requests to /board/content will:
 * - Allow GET, POST, and DELETE methods
 * - Serve files from ./www/html/contents directory
 * - Use board.html as the default index file
 * - Execute .php files using php-cgi for dynamic content
 */

#include "../includes/Location.hpp"

Location::Location(/* args */)
{
	path = "";
	root = "";
}

Location::~Location()
{
}

/**
 * print_location_info()
 * 
 * Prints the configuration details of this location block for debugging purposes.
 * Imprime os detalhes de configuração deste bloco de localização para depuração.
 */
void Location::print_location_info()
{
	std::cout << "----------------- Location Info -----------------\n";
	std::cout << "> path: " << path << "\n";
	std::cout << "> root: " << root << "\n";
	std::cout << "> index: " << index << "\n";
	std::cout << "> allow_methods: " << allow_methods << "\n";
	for (std::map<std::string, std::string>::iterator i = cgi_info.begin(); i != cgi_info.end(); i++)
	{
		std::cout << "> cgi_info: " << (*i).first << ", " << (*i).second << "\n";
	}
}

/**
 * s_to_methodtype()
 * 
 * Converts a string representation of an HTTP method to the MethodType enum.
 * Converte uma representação em string de um método HTTP para o enum MethodType.
 * 
 * @param str HTTP method name (e.g., "GET", "POST", "DELETE")
 * @return Corresponding MethodType enum value, or INVALID if not recognized
 */
MethodType Location::s_to_methodtype(std::string str)
{
	if (str == "GET")
	{
		return GET;
	}
	else if (str == "POST")
	{
		return POST;
	}
	else if (str == "DELETE")
	{
		return DELETE;
	}
	return INVALID;
}

/**
 * getCgiBinary()
 * 
 * Retrieves the CGI binary path for a given file extension.
 * Recupera o caminho do binário CGI para uma determinada extensão de arquivo.
 * 
 * @param extension File extension (e.g., "php", "py")
 * @return Path to the CGI binary that handles this extension, or empty string if not found
 */
std::string Location::getCgiBinary(std::string &extension)
{
	for (std::map<std::string, std::string>::const_iterator it = this->cgi_info.begin();
	it != this->cgi_info.end(); ++it)
	{
		if (it->first == "." + extension)
			return it->second;
	}
	return "";
}

std::string Location::get_root(void)
{
	return this->root;
}

std::string Location::get_path(void)
{
	return this->path;
}
