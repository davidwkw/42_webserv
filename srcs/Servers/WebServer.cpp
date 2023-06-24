#include "WebServer.hpp"

namespace ft
{

WebServer::WebServer(){}

WebServer::WebServer(const WebserverConfig &config) : _webserver_config(config)
{
    long index = 0;

    for (std::map<long, ServerConfig>::const_iterator cit = config.servers().begin(); cit != config.servers().end(); cit++)
    {
        const std::vector<unsigned int> ports = cit->second.ports();

        for (std::vector<unsigned int>::const_iterator cpit = ports.begin(); cpit != ports.end(); cpit++)
        {
            this->_servers.insert(std::make_pair(index, HTTPServer(*cpit, 5)));
        }
        index++;
    }
}

WebServer::~WebServer()
{
	freeaddrinfo(this->_servinfo);
}

WebServer::WebServer(const WebServer &ref) : _read_fds(ref._read_fds), _write_fds(ref._write_fds)
{
	*this = ref;
}

WebServer &WebServer::operator=(const WebServer &ref)
{
	if (this != &ref)
	{
        this->_read_fds = ref._read_fds;
        this->_write_fds = ref._write_fds;
	}
	return *this;
}


void WebServer::run() const
{
    for(std::map<long, HTTPServer>::const_iterator cit = this->_servers.begin(); cit != this->_servers.end(); cit++)
    {

    }
    
    while(true)  
    {  
        //clear the socket set 
        FD_ZERO(&this->_read_fds);  
        FD_ZERO(&this->_write_fds);
        //add master socket to set


        FD_SET(master_socket, &this->_read_fds);  
        max_sd = master_socket;  
             
        //add child sockets to set 
        for ( i = 0 ; i < max_clients ; i++)  
        {  
            //socket descriptor 
            sd = client_socket[i];  
                 
            //if valid socket descriptor then add to read list 
            if(sd > 0)  
                FD_SET( sd , &this->_read_fds);  
                 
            //highest file descriptor number, need it for the select function 
            if(sd > max_sd)  
                max_sd = sd;  
        }  
     
        //wait for an activity on one of the sockets , timeout is NULL , 
        //so wait indefinitely 
        activity = select( max_sd + 1 , &this->_read_fds , NULL , NULL , NULL);  
       
        if ((activity < 0) && (errno!=EINTR))  
        {  
            printf("select error");  
        }  
             
        //If something happened on the master socket , 
        //then its an incoming connection 
        if (FD_ISSET(master_socket, &this->_read_fds))  
        {  
            if ((new_socket = accept(master_socket, 
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)  
            {  
                perror("accept");  
                exit(EXIT_FAILURE);  
            }  
             
            //inform user of socket number - used in send and receive commands 
            printf("New connection , socket fd is %d , ip is : %s , port : %d 
                  \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs
                  (address.sin_port));  
           
            //send new connection greeting message 
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )  
            {  
                perror("send");  
            }  
                 
            puts("Welcome message sent successfully");  
                 
            //add new socket to array of sockets 
            for (i = 0; i < max_clients; i++)  
            {  
                //if position is empty 
                if( client_socket[i] == 0 )  
                {  
                    client_socket[i] = new_socket;  
                    printf("Adding to list of sockets as %d\n" , i);  
                         
                    break;  
                }  
            }  
        }  
             
        //else its some IO operation on some other socket
        for (i = 0; i < max_clients; i++)  
        {  
            sd = client_socket[i];  
                 
            if (FD_ISSET( sd , &readfds))  
            {  
                //Check if it was for closing , and also read the 
                //incoming message 
                if ((valread = read( sd , buffer, 1024)) == 0)  
                {  
                    //Somebody disconnected , get his details and print 
                    getpeername(sd , (struct sockaddr*)&address , \
                        (socklen_t*)&addrlen);  
                    printf("Host disconnected , ip %s , port %d \n" , 
                          inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
                         
                    //Close the socket and mark as 0 in list for reuse 
                    close( sd );  
                    client_socket[i] = 0;  
                }  
                     
                //Echo back the message that came in 
                else 
                {  
                    //set the string terminating NULL byte on the end 
                    //of the data read 
                    buffer[valread] = '\0';  
                    send(sd , buffer , strlen(buffer) , 0 );  
                }  
            }  
        }  
    }  

	// for (p = this->_servinfo; p != NULL; p = p->ai_next){
    //     if ((this->_sock_fd = socket(p->ai_family, p->ai_socktype,
    //             p->ai_protocol)) == -1) {
    //         perror("server: socket");
    //         continue;
    //     }

    //     if (setsockopt(this->_sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes,
    //             sizeof(int)) == -1) {
    //         perror("setsockopt");
    //         exit(1);
    //     }

    //     if (bind(this->_sock_fd, p->ai_addr, p->ai_addrlen) == -1) {
    //         close(this->_sock_fd);
    //         perror("server: bind");
    //         continue;
    //     }

    //     break;
	// }
}

}
