#include "../../includes/webserv.hpp"

namespace ft
{

std::string generate_dir_content_list_html(const std::string &root_prefix, const std::string &dir_to_cache)
{
	std::ostringstream	oss;
    struct dirent		*entry;
	DIR					*folder;

    folder = opendir((root_prefix + dir_to_cache).c_str());
    if(folder == NULL)
        throw std::runtime_error("Couldn't open dir " + (root_prefix + dir_to_cache));
	while ((entry = readdir(folder)) != NULL)
	{
		if (entry->d_type == DT_REG)
			oss << "<a href=\"" << (dir_to_cache + entry->d_name).c_str() << "\">" << entry->d_name << "</a>\n"; 
	}
	closedir(folder);
	return oss.str();
}

void handle_autoindex_caching(const std::string &template_path, const std::string &cache_dir, const std::string &root_prefix, const std::string &filename)
{
	// cache_dir = public/autoindex_cache
	// root_prefix = public
	// filename = /.html or /whatever/.html

	std::ostringstream	temp_autoindex_path;
	std::ifstream		temp_file;
	std::ifstream		autoindex_template;
	std::ofstream		output;
	std::string			line;
	std::string			dir_to_cache = filename.substr(0, filename.find(".html"));

	// public/autoindex_cache/"public/whatever/.html"
	temp_file.open((cache_dir + "/" + root_prefix + filename).c_str()); // check if file was already cached
	if (temp_file.is_open())
	{
		temp_file.close();
		return;
	}
	autoindex_template.open(template_path.c_str()); // should probably be a directive in the config file..
	if (!autoindex_template.is_open())
		throw std::runtime_error("Critical mess up. Couldn't open autoindex.html");
	output.open((cache_dir + "/" + root_prefix + filename).c_str());
	if (!output.is_open())
		throw std::runtime_error("Critical mess up. Couldn't create " + (cache_dir + "/" + root_prefix + filename));
	while (std::getline(autoindex_template, line))
	{
		std::size_t identifier_pos;

		if ((identifier_pos = line.find(DIRECTORY_TEMPLATE_IDENTIFIER)) != std::string::npos)
		{
			output << line.substr(0, identifier_pos);
			output << dir_to_cache;
			output << line.substr(identifier_pos + dir_to_cache.length()) << "\n";
		}
		else if ((identifier_pos = line.find(DIRECTORY_CONTENT_TEMPLATE_IDENTIFIER)) != std::string::npos)
		{
			output << generate_dir_content_list_html(root_prefix, dir_to_cache);
		}
		else 
		{
			output << line << "\n";
		}
	}
	autoindex_template.close();
	output.close();
}

void cache_autoindex(const std::map<long, ft::ServerConfig> &server_configs, const std::string &template_path, const std::string &cache_dir)
{
	// cache_dir = public/autoindex_cache
	for (std::map<long, ft::ServerConfig>::const_iterator cit = server_configs.begin(); cit != server_configs.end(); cit++)
	{
		if (cit->second.autoindex() == "on")
		{
			handle_autoindex_caching(template_path, cache_dir, cit->second.root(), + "/.html"); // filename = /.html
		}
		for (std::map<std::string, ft::LocationConfig>::const_iterator cit2 = cit->second.locations().begin(); cit2 != cit->second.locations().end(); cit++)
		{
			if (cit2->second.autoindex() == "on")
			{
				handle_autoindex_caching(template_path, cache_dir, cit2->second.root(), cit2->first + ".html"); // filename = /whatever/.html
			}
		}
	}
}

void clear_auto_index_cache(const std::string &dir)
{
	std::ostringstream	oss;
    struct dirent		*entry;
	DIR					*folder;

    folder = opendir(dir.c_str());
    if(folder == NULL)
        throw std::runtime_error(dir + "Directory non-existent");
	while ((entry = readdir(folder)) != NULL)
	{
		if (entry->d_type == DT_REG)
		{
			remove((dir + "/" + entry->d_name).c_str());
		}
	}
	closedir(folder);
}

}
