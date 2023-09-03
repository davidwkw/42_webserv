#include "WebServer.hpp"

namespace ft
{

#pragma region Constructors

WebServer::WebServer(const std::string &conf_path) : _webserver_config(conf_path)
{
    FD_ZERO(&this->_all_fds);
    FD_ZERO(&this->_read_fds);
    FD_ZERO(&this->_write_fds);
}

WebServer::WebServer(const WebserverConfig &config) : _webserver_config(config)
{
    FD_ZERO(&this->_all_fds);
    FD_ZERO(&this->_read_fds);
    FD_ZERO(&this->_write_fds);
}

WebServer::~WebServer(){}

#pragma endregion Constructors

#pragma region OperatorOverloads

#pragma endregion OperatorOverloads

#pragma region ClassUtility

void WebServer::_initialise_socket_fd()
{
    for(std::map<unsigned int, std::vector<long> >::const_iterator cit = this->_webserver_config.get_port_server_config_map().begin(); cit != this->_webserver_config.get_port_server_config_map().end(); cit++)
    {
        std::vector<ServerConfig>   server_configs;

        for (std::vector<long>::const_iterator cit2 = cit->second.begin(); cit2 != cit->second.end(); cit2++)
        {
            std::map<long, ServerConfig>    server_map; 
            
            server_map = this->_webserver_config.servers();
            server_configs.push_back(server_map[*cit2]);
        }
        this->_port_http_server_map.insert(std::make_pair(cit->first, HTTPServer(cit->first, BACKLOG, MAX_CLIENTS, BUFFER_SIZE, server_configs)));
    }
}

void WebServer::_reinitialize_fd_sets()
{   
    this->_read_fds = this->_write_fds = this->_all_fds;
}

void WebServer::_append_listen_sockets_to_allfd(int &max_fd)
{
    for (std::map<unsigned int, HTTPServer>::iterator it = this->_port_http_server_map.begin(); it != this->_port_http_server_map.end(); it++)
    {
        int socket_fd = it->second.get_listen_socket_fd();

        FD_SET(socket_fd, &this->_all_fds);
        if (socket_fd > max_fd)
            max_fd = socket_fd;
    }
}

void WebServer::_append_read_sockets_to_readfd(int &max_fd)
{
    for (std::map<unsigned int, HTTPServer>::iterator it = this->_port_http_server_map.begin(); it != this->_port_http_server_map.end(); it++)
    {
        for (std::list<int>::iterator it2 = it->second.get_client_read_fds().begin(); it2 != it->second.get_client_read_fds().end(); it2++)
        {
            FD_SET(*it2, &this->_read_fds);
            if (*it2 > max_fd)
                max_fd = *it2;
        }
    }
}

void WebServer::_append_write_sockets_to_writefd(int &max_fd)
{
    for (std::map<unsigned int, HTTPServer>::iterator it = this->_port_http_server_map.begin(); it != this->_port_http_server_map.end(); it++)
    {
        for (std::list<int>::iterator it2 = it->second.get_client_write_fds().begin(); it2 != it->second.get_client_write_fds().end(); it2++)
        {
            FD_SET(*it2, &this->_write_fds);
            if (*it2 > max_fd)
                max_fd = *it2;
        }
    }
}

void WebServer::_accept_incoming_connections()
{
    for (std::map<unsigned int, HTTPServer>::iterator it = this->_port_http_server_map.begin(); it != this->_port_http_server_map.end(); it++)
    {
        if (FD_ISSET(it->second.get_listen_socket_fd(), &this->_read_fds))
            it->second.accept_connection();
    }
}

void WebServer::_perform_socket_io()
{
    for (std::map<unsigned int, HTTPServer>::iterator it = this->_port_http_server_map.begin(); it != this->_port_http_server_map.end(); it++)
    {
        for (std::list<int>::iterator it2 = it->second.get_client_read_fds().begin(); it2 != it->second.get_client_read_fds().end(); it2++)
        {
            if (FD_ISSET(*it2, &this->_read_fds))
                it->second.handle_request(*it2);
        }

        for (std::list<int>::iterator it2 = it->second.get_client_write_fds().begin(); it2 != it->second.get_client_write_fds().end(); it2++)
        {
            if (FD_ISSET(*it2, &this->_write_fds))
                it->second.handle_response(*it2);
        }
    }
}

#pragma endregion ClassUtility

void WebServer::run()
{   
    int             activity = 0;
    struct timeval  timeout = {};
    int             server_max_fd = 0;
    int             max_fd = 0;

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
