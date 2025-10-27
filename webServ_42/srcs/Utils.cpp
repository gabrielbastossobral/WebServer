/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 10:38:51 by gcosta-m          #+#    #+#             */
/*   Updated: 2025/10/21 10:46:40 by gcosta-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Utils.hpp"

int replace(std::string original, std::string to_replace, std::string replacement)
{
	size_t pos = original.find(to_replace);
	if (pos != std::string::npos)
	{
		original.erase(pos, to_replace.length());
		original.insert(pos, replacement);
		return 0;
	}
	return -1;
}

// escapar caracteres especiais em HTML
static std::string html_escape(const std::string &input)
{
	std::string output;
	output.reserve(input.size());
	for (size_t i = 0; i < input.size(); ++i)
	{
		unsigned char c = input[i];
		switch (c)
		{
			case '&': output += "&amp;"; break;
			case '<': output += "&lt;"; break;
			case '>': output += "&gt;"; break;
			case '"': output += "&quot;"; break;
			case '\'': output += "&#39;"; break;
			default: output += static_cast<char>(c);
		}
	}
	return output;
}

// URL-encode uma string para uso em links
static std::string url_encode(const std::string &input)
{
	std::ostringstream escaped;
	escaped.fill('0');
	for (size_t i = 0; i < input.size(); ++i)
	{
		unsigned char c = input[i];
		if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
		{
			escaped << c;
		}
		else
		{
			escaped << '%' << std::uppercase << std::hex
			 << std::setw(2) << static_cast<int>(c)
			 << std::nouppercase << std::dec;
		}
	}
	return escaped.str();
}


// Verifica se o nome do arquivo/diretório é seguro para exibição e linkagem
static bool is_safe_name(const std::string &name)
{
	if (name.empty()) return false;

	if (name[0] == '/' || name[0] == '\\') return false;

	if (name.find("..") != std::string::npos) return false;

	if (name.find(':') != std::string::npos) return false;

	std::string low;
	low.resize(name.size());
	for(size_t i = 0; i < name.size(); ++i)
		low[i] = static_cast<char>(std::tolower(name[i]));
	
	const char *bad_prefixes[] = {"javascript", "data", "file", 0};
	for (const char **p = bad_prefixes; *p; ++p)
	{
		size_t plen = std::strlen(*p);
		if (low.size() >= plen + 1 && low.compare(0, plen, *p) == 0) return false;	
	}

	for (size_t i = 0; i < name.size(); ++i)
	{
		unsigned char c = name[i];
		if (c < 0x20 || c == 0x7F) return false;
	}
	return true;
}


//lista o conteúdo de um diretório em formato HTML
//aplica html_escape e url_encode conforme necessário
//url_encode apenas em nomes considerados seguros
std::string dir_listing(const std::string &path = "www/html/data/")
{
	DIR *dir = opendir(path.c_str());
	if (!dir)
	{
		std::ostringstream error;
		error << "<p>Error opening directory \"" << html_escape(std::string(path))
		<< "\": " << html_escape(std::string(strerror(errno))) << "</p>";
		return error.str();
	}

	std::vector<std::string> names;
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		std::string name(entry->d_name);
		if (name == "." || name == "..") continue;
			names.push_back(name);
	}
	closedir(dir);

	std::sort(names.begin(), names.end());
	std::ostringstream html;
	html << "<ul>";

	for (size_t i = 0; i < names.size(); ++i)
	{
		const std::string &name = names[i];
		std::string full = path;
		
		if (!full.empty() && full[full.size() - 1] != '/') full += '/';
		full += name;

		struct stat st;
		bool is_dir = false;
		if (stat(full.c_str(), &st) == 0)
		{
			if (S_ISDIR(st.st_mode)) is_dir = true;
		}

		std::string display_name = html_escape(name);
		if (!is_safe_name(name))
		{
			if (is_dir)
			{
				html << "li><strong>" << display_name << "/</strong></li>";
			}else
			{
				html << "<li>" << display_name << "</li>";
			}
			continue;
		}
		std::string href = url_encode(name);
		if (is_dir)
		{
			html << "<li><strong><a href=\"" << href << "/\">" << display_name << "/</a></strong></li>";
		}else
		{
			html << "<li><a href=\"" << href << "\">" << display_name << "</a></li>";
		}
	}
	html << "</ul>";
	return html.str();
}