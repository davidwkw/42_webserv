#include "WebServer.hpp"

namespace ft
{

WebServer::WebServer(const std::string &conf_path) : _webserver_config(conf_path), _port_http_server_map(), _all_fds(), _read_fds(), _write_fds()
{
    FD_ZERO(&this->_all_fds);
    FD_ZERO(&this->_read_fds);
    FD_ZERO(&this->_write_fds);
}

WebServer::WebServer(const WebserverConfig &config) : _webserver_config(config), _port_http_server_map(), _all_fds(), _read_fds(), _write_fds()
{
    FD_ZERO(&this->_all_fds);
    FD_ZERO(&this->_read_fds);
    FD_ZERO(&this->_write_fds);
}

WebServer::~WebServer(){}

void WebServer::_initialise_socket_fd()
{
    std::map<long, ServerConfig>    index_to_server_config_map; 

    index_to_server_config_map = this->_webserver_config.servers();
    // Go through each port/server config index
    for(std::map<unsigned int, std::vector<long> >::const_iterator cit = this->_webserver_config.get_port_server_config_index_map().begin(); cit != this->_webserver_config.get_port_server_config_index_map().end(); cit++)
    {
        unsigned int const          &current_port = cit->first;
        std::vector<long> const     &server_config_indexes = cit->second;
        std::vector<ServerConfig>   server_configs;

        // Go through each server config index of a particular port
        for (std::vector<long>::const_iterator cit2 = server_config_indexes.begin(); cit2 != server_config_indexes.end(); cit2++)
        {
            long const &index = *cit2;
            // Collect all server configs that belong to a particular port
            server_configs.push_back(index_to_server_config_map.at(index));
        }
        // construct HTTPServer based on port number and pass all server configs to the server
        this->_port_http_server_map.insert(std::make_pair(current_port, HTTPServer(current_port, BACKLOG, MAX_CLIENTS, BUFFER_SIZE, server_configs)));
    }
}

void WebServer::_reinitialize_fd_sets()
{   
    // Reset all fd_sets
    this->_read_fds = this->_write_fds = this->_all_fds;
}

void WebServer::_append_listen_sockets_to_allfd(int &max_fd)
{
    // Go through all port/http server;set fd into fd_set; update highest socket_fd; 
    for (std::map<unsigned int, HTTPServer>::iterator it = this->_port_http_server_map.begin(); it != this->_port_http_server_map.end(); it++)
    {
        HTTPServer &current_server = it->second;
        int socket_fd = current_server.get_listen_socket_fd();

        FD_SET(socket_fd, &this->_all_fds);
        if (socket_fd > max_fd)
        {
            max_fd = socket_fd;
        }
    }
}

void WebServer::_append_read_sockets_to_readfd(int &max_fd)
{
    for (std::map<unsigned int, HTTPServer>::iterator it = this->_port_http_server_map.begin(); it != this->_port_http_server_map.end(); it++)
    {
        HTTPServer &current_server = it->second;

        for (std::list<int>::iterator it2 = current_server.get_client_read_fds().begin(); it2 != current_server.get_client_read_fds().end(); it2++)
        {
            int &current_fd = *it2;

            FD_SET(current_fd, &this->_read_fds);
            if (current_fd > max_fd)
            {
                max_fd = current_fd;
            }
        }
    }
}

void WebServer::_append_write_sockets_to_writefd(int &max_fd)
{
    for (std::map<unsigned int, HTTPServer>::iterator it = this->_port_http_server_map.begin(); it != this->_port_http_server_map.end(); it++)
    {
        HTTPServer &current_server = it->second;

        for (std::list<int>::iterator it2 = current_server.get_client_write_fds().begin(); it2 != current_server.get_client_write_fds().end(); it2++)
        {
            int &current_fd = *it2;

            FD_SET(current_fd, &this->_write_fds);
            if (current_fd > max_fd)
            {
                max_fd = current_fd;
            }
        }
    }
}

void WebServer::_accept_incoming_connections()
{
    // Go through each HTTPServer fd and check if set
    for (std::map<unsigned int, HTTPServer>::iterator it = this->_port_http_server_map.begin(); it != this->_port_http_server_map.end(); it++)
    {
        HTTPServer &current_server = it->second;
        
        if (FD_ISSET(current_server.get_listen_socket_fd(), &this->_read_fds))
        {
            current_server.accept_connection();
        }
    }
}

void WebServer::_perform_socket_io()
{
    // Go through each port/httpserver
    for (std::map<unsigned int, HTTPServer>::iterator it = this->_port_http_server_map.begin(); it != this->_port_http_server_map.end(); it++)
    {
        HTTPServer &current_server = it->second;
        // Go through each httpserver's read fds;
        for (std::list<int>::iterator it2 = current_server.get_client_read_fds().begin(); it2 != current_server.get_client_read_fds().end(); it2++)
        {
            int &current_fd = *it2;

            if (FD_ISSET(current_fd, &this->_read_fds))
            {
                current_server.handle_request(current_fd);
            }
        }

        // Go through each httpserver's write fds;
        for (std::list<int>::iterator it2 = it->second.get_client_write_fds().begin(); it2 != it->second.get_client_write_fds().end(); it2++)
        {
            int &current_fd = *it2;

            if (FD_ISSET(current_fd, &this->_write_fds))
            {
                it->second.handle_response(current_fd);
            }
        }
    }
}

std::string WebServer::get_config_string()
{
    return this->_webserver_config.config_string();
}

void WebServer::run()
{   
    struct timeval  timeout = {};
    int             activity;
    int             server_max_fd;
    int             max_fd;

    this->_initialise_socket_fd();
    this->_append_listen_sockets_to_allfd(server_max_fd);

    while(true)  
    {
        timeout.tv_sec = TIMEOUT_SECONDS;
        timeout.tv_usec = TIMEOUT_MICROSECONDS;
        max_fd = server_max_fd;
        this->_reinitialize_fd_sets();
        this->_append_read_sockets_to_readfd(max_fd);
        this->_append_write_sockets_to_writefd(max_fd);
        activity = select(max_fd + 1 , &this->_read_fds , &this->_write_fds , NULL , &timeout);
        if ((activity < 0) && (errno != EINTR))  
        {  
            throw std::runtime_error("Select error");
        }
        this->_accept_incoming_connections();
        this->_perform_socket_io();
    }  
}

}
