#pragma once

namespace ft
{

void cache_autoindex(const std::map<long, ft::ServerConfig> &server_configs, const std::string &template_path, const std::string &cache_dir);
void clear_auto_index_cache(const std::string &dir);

}
