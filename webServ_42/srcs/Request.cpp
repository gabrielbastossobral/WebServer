#include  "../includes/Request.hpp"

Request::Request(int client_fd)
{
    this->client_fd = client_fd;
}

Request::~Request()
{
}

int Request::get_client_fd()
{
    return client_fd;
}

std::string Request::get_port()
{
    int i = headers["Host"].find_first_of(":",0);
    return headers["Host"].substr(i + 1, headers["Host"].size() - i - 1);
}

static bool check_protocol(std::map<std::string, std::string>::mapped_type& parsed)
{
    if (parsed != "HTTP/1.1")
        return false;
    return true;
}

static std::string parse_request_body_chunked(std::string& request, int index)
{
    std::size_t size = 1;
    std::string ret, size_buf, line = request.substr(index + 2, request.size());
    int i = 0;

    while (true)
    {
        size_t r = line.find("\r\n");
        size_buf = line.substr(i, r);
        size = string_to_hexa(size_buf);
        i += 2 + size_buf.size();
        if (size == 0)
            break ;
        std::string buf = line.substr(i, i + size - 2);
        ret += buf;
        i += size + 2;
    }
    return ret;
}

static std::string parse_request_body(std::string& request, int index)
{
    return request.substr(index + 2, request.size());
}

static int parse_headers_line(std::map<std::string, std::string>& headers, std::string& request, int index)
{
    int deli = request.find_first_of(":", index);
    int end = request.find_first_of("\r\n", deli);
    
    // Extrair header name e remover espaços/quebras de linha no início
    std::string header_name = request.substr(index, deli - index);
    size_t name_start = header_name.find_first_not_of(" \t\r\n");
    if (name_start != std::string::npos)
        header_name = header_name.substr(name_start);
    
    // Extrair header value (já remove o espaço após o ':')
    std::string header_value = request.substr(deli + 2, end - deli - 2);
    
    headers[header_name] = header_value;
    
    return end;
}

int Request::parsing(std::string request)
{
    unsigned int i;
    int j;

    std::cout << " Request parsing\n";
    
    // ===== DEBUG: MOSTRAR REQUEST COMPLETO =====
    std::cout << YELLOW << "=== RAW REQUEST ===" << RESET << std::endl;
    std::cout << "[" << request << "]" << std::endl;
    std::cout << "Request size: " << request.size() << " bytes" << std::endl;
    std::cout << YELLOW << "===================" << RESET << std::endl;
    // ===========================================
    
    i = request.find_first_of(" ", 0);
    method = request.substr(0, i);
    
    // ===== DEBUG: MOSTRAR METHOD =====
    std::cout << "Parsed Method: [" << method << "]" << std::endl;
    // =================================
    
    if (method == "PUT")
    {
        std::cout << RED << "PUT method not supported (returning 200)" << RESET << std::endl;
        return 200;
    }
    if (is_not_method(method))
    {
        std::cout << RED << "Invalid method detected: [" << method << "]" << RESET << std::endl;
        return 400;
    }
    
    if ((unsigned long)(j = request.find_first_of(" ", i + 1)) == std::string::npos)
    {
        std::cout << RED << "No second space found in request line" << RESET << std::endl;
        return 400;
    }
    
    path = request.substr(i + 1, j - i - 1);
    
    // ===== DEBUG: MOSTRAR PATH =====
    std::cout << "Parsed Path: [" << path << "]" << std::endl;
    // ===============================
    
    headers["HTTP"] = request.substr(j + 1, request.find_first_of("\r" ,i) - j - 1);
    
    // ===== DEBUG: MOSTRAR HTTP VERSION =====
    std::cout << "Parsed HTTP Version: [" << headers["HTTP"] << "]" << std::endl;
    // =======================================
    
    if (check_protocol(headers["HTTP"]) == false)
    {
        std::cout << RED << "Invalid HTTP protocol version" << RESET << std::endl;
        return 505;
    }
    
    i = request.find_first_of("\r\n", j) + 1;
    while (i < request.size())
    {
        if(request[i] == '\r' && request[i + 1] == '\0')
            break ;
        if (request[i] == '\r' && request[i + 1] == '\n')
        {
            if (strstr(headers["Transfer-Encoding"].c_str(), "chunked") != NULL)
            {
                this->body = parse_request_body_chunked(request, i);
            }else
            {
                this->body = parse_request_body(request, i);
            }
            break;
        }
        int end = parse_headers_line(this->headers, request, i);
        if (end + 1 == '\0')
            break ;
        i = end + 2;
    }
    
    // ===== DEBUG: MOSTRAR HEADERS =====
    std::cout << "Parsed Headers:" << std::endl;
    for (std::map<std::string, std::string>::iterator it = headers.begin(); 
         it != headers.end(); ++it)
    {
        std::cout << "  [" << it->first << "] = [" << it->second << "]" << std::endl;
    }
    // ==================================
    
    if (headers["Host"] == "")
    {
        std::cout << RED << "Host header is missing or empty" << RESET << std::endl;
        return 400;
    }
    
    std::cout << GREEN << "Request parsed successfully!" << RESET << std::endl;
    return 0;
}

std::string Request::get_path()
{
    unsigned long i = path.find_first_of("?", 0);
    if ( i == std::string::npos)
        return path;
    if ((int) i == -1)
        i = path.length();
    return path.substr(0, i);
}

std::string Request::get_query()
{
    unsigned long i = path.find_first_of("?", 0);
    if ( i == std::string::npos)
        return "";
    return path.substr(i + 1, path.size() - i);
}

bool Request::is_not_method(const std::string method)
{
    if (method.empty())
        return true;
    for (unsigned long i = 0; i < method.length(); i++)
    {
        if (!isupper(static_cast<unsigned char>(method[i])))
            return true;
    }
    return false;
}