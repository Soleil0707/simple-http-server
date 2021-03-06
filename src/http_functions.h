#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
// for reading directory
#include <sys/types.h>
#include <dirent.h>
// for openssl
#include <openssl/ssl.h>
#include <openssl/err.h>
// self-write header file
#include "http_response.h"

// http header params
#define HTTP_HDR_METHOD_LEN 10
#define HTTP_HDR_URL_LEN (1 << 10)
#define HTTP_HDR_VERSION_LEN 10
#define HTTP_HDR_BOUNDARY_LEN (1 << 8)

// server root path
#define SERVER_ROOT_DIR "htdocs"
// server certificate
#define SERVER_CRT "CA/server.crt"
// server certificate key
#define SERVER_KEY "CA/server.key"

// process params
#define MAX_PATH_LEN 512
#define MAX_LINE_LEN 1024

// html strings
#define HTML_BEFORE_BODY                                             \
    "<!DOCTYPE html>"                                                \
    "<html>"                                                         \
    "<link rel=\"stylesheet\" "                                      \
    "href=\"https://cdn.staticfile.org/twitter-bootstrap/4.3.1/css/" \
    "bootstrap.min.css\">"                                           \
    "<head>"                                                         \
    "<meta charset=\"UTF-8\">"                                       \
    "<title>WUW Web Server Resources Page</title>"                   \
    "</head>"                                                        \
    "<body><div class=\"container\">"                                \
    "<h1 class=\"display-4\">Directory listing for /resources/</h1>" \
    "<dl><dt>Files List</dt>"
#define HTML_AFTER_BODY                         \
    "</dl><input type=\"button\" "              \
    "onclick=\"location.href=\'index.html\'\" " \
    "value=\"Back\" "                           \
    "class=\"btn btn-primary\"/> </br>"         \
    "</div></body>"                             \
    "</html>"

// function define
#define isSpace(x) isspace((int)(x))

// http method enum
enum http_method {
    NOT_IMPLEMENT = -1,
    GET,
    POST,
    /*
       the below methods
       are not implemented.
    */
    HEAD,
    PUT,
    DELETE,
    OPTIONS,
    TRACE,
    CONNECT,
    OTHERS
};

// http header struct
typedef struct http_headers_t {
    char version[HTTP_HDR_VERSION_LEN];
    char method[HTTP_HDR_METHOD_LEN];
    char url[HTTP_HDR_URL_LEN];
    char query[MAX_LINE_LEN];
    char boundary[HTTP_HDR_BOUNDARY_LEN];
    int mode;
    int length;
    int alive;
} http_headers_t;

/*
    function declarations
 */
// initialize http server
evutil_socket_t http_init();
// callback of handing request
void do_accept_cb(bfevent_t* bev, void* arg);
// send directory to client
void send_directory_to_client(bfevent_t* bev, char* directory);
// send file to client
void send_file_to_client(bfevent_t* bev, char* path);
// send file main function
void send_file(bfevent_t* bev, FILE* fp);
// receive file from client
void recv_file_from_client(bfevent_t* bev, char* path, http_headers_t* hdr);
// get first header
int get_first_header(bfevent_t* bev, http_headers_t* hdr);
// get other headers
int get_other_headers(bfevent_t* bev, http_headers_t* hdr);
// parse http header from socket
int parse_http_header(bfevent_t* bev, http_headers_t* hdr);
// get line string from socket
int get_line_from_bufferevent(bfevent_t* bev, char* buf);
// parse method string from given string
char* get_method_from_str(char* buf, http_headers_t* hdr);
// parse url string from given string
char* get_url_from_str(char* buf, http_headers_t* hdr);
// parse version string from given string
void get_version_from_str(char* buf, http_headers_t* hdr);
// format path on server from http headers
void get_file_path_on_server(char* path, http_headers_t* hdr);

#endif