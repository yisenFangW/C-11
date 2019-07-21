////
//// Created by 方伟 on 2019-07-20.
////
//#include <cstdio> // perror
//#include <cstring> // string
//#include <strings.h> // memset
//#include <iostream>
//#include <sys/types.h> // AF_INET, SOCK_STREAM
//#include <sys/socket.h> // socket , connect
//#include <arpa/inet.h> // inet_aton
//#include <netinet/in.h>
//#include <unistd.h>
//
//
//#define SERVER_PORT 8801
//#define SERVER_IP "127.0.0.1"
//#define MAXLINE 1024*2
//
//using namespace std ;
//
//
//int main ( int argc , char **argv )
//{
//    char buf [MAXLINE] ;
//    ssize_t n ; // message content length
//    struct sockaddr_in server_addr ;
//    int connfd ;
//    int ret ;
//    string msg ;
//
//    connfd = socket (AF_INET , SOCK_STREAM , 0 ) ;
//    bzero (&server_addr , sizeof(struct sockaddr_in)) ;
//
//    server_addr.sin_family = AF_INET ;
//    server_addr.sin_port = htons (SERVER_PORT) ;
//    inet_aton (SERVER_IP , &server_addr.sin_addr ) ;
//
//    ret = connect ( connfd , (struct sockaddr*)&server_addr , sizeof(struct sockaddr_in) ) ;
//
//
//    if ( ret < 0 )
//    {
//        perror ("failed connect") ;
//        return -1;
//    }
//
//    cout << "input message "<< endl ;
//    cin >> msg ;
//
//    write (connfd , msg.c_str() , msg.size() ) ;
//
//
//    return 0 ;
//}


#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <poll.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include<arpa/inet.h>

#define MAXLINE     1024
#define IPADDRESS   "127.0.0.1"
#define SERV_PORT   8787

#define max(a,b) (a > b) ? a : b

static void handle_connection(int sockfd);

int main(int argc,char *argv[])
{
    int                 sockfd;
    struct sockaddr_in  servaddr;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET,IPADDRESS,&servaddr.sin_addr);
    connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    //处理连接描述符
    handle_connection(sockfd);
    return 0;
}

static void handle_connection(int sockfd)
{
    char    sendline[MAXLINE],recvline[MAXLINE];
    int     maxfdp,stdineof;
    struct pollfd pfds[2];
    int n;
    //添加连接描述符
    pfds[0].fd = sockfd;
    pfds[0].events = POLLIN;
    //添加标准输入描述符
    pfds[1].fd = STDIN_FILENO;
    pfds[1].events = POLLIN;
    for (; ;){
        poll(pfds,2,-1);
        if (pfds[0].revents & POLLIN){
            n = read(sockfd,recvline,MAXLINE);
            if (n == 0){
                fprintf(stderr,"client: server is closed.\n");
                close(sockfd);
            }
            write(STDOUT_FILENO,recvline,n);
        }
        //测试标准输入是否准备好
        if (pfds[1].revents & POLLIN){
            n = read(STDIN_FILENO,sendline,MAXLINE);
            if (n  == 0)
            {
                shutdown(sockfd,SHUT_WR);
                continue;
            }
            write(sockfd,sendline,n);
        }
    }
}
