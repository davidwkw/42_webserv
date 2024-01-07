# Webserv

## Score: 100/100

The goal of this project is to create a webserver only using C++98 to handle static file GET requests, directory autoindexing, file uploading, and the ability to run CGI scripts on the server. All of the features above are to be configured using an nginx-like configuration file with a .conf extension.

This webserver programme is compliant to HTTP/1.1 standards as well as for the most part compliant to CGI/1.1 standards. Autoindexing is handled via a HTML template. Multi-file upload is supported if it is within the max_body_size limit.

Readme for 42 programme's webserver project

Notes for .conf files:
1. Comments are supported
2. Location needs to be formatted between forward slashes, i.e. /endpoint/
3. root directive should be a relative path to webserv binary, i.e. public
4. server->location directive inheritance is also supported. location directives overwrite server directives.

Footnotes:
1. Select used for this implementation but not ideal due to number of fd limitation. A lot more fds are used than expected.
2. May be possible to actually use 1 select/poll/epoll with cgi handling by keeping a duplicate of the socket fd and replacing/dup2-ing the original socket fd with the cgi script input pipe fd after receiving full request body. After the CGI script has completed, dup2 the socket descriptor copy with the descriptor that was previously replaced. If an exception occured while processing the script, handle as usual. Otherwise,
tack on whatever additional headers and write the captured cgi output into the socket descriptor.
3. An additional select is used for CGI script processing to prevent potential read/write blocking. Technically, the select used for the cgi script is used as a tool for inter-process communication rather than client/server communication.
