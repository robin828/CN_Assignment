#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 3000
   
int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    // printf("%d",AF_INET);
    // printf("%d",SOCK_STREAM);
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    printf("%d" ,PORT);

    int a = inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    // printf("%d", a);
       
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    printf("%lu", sizeof(serv_addr));
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        printf("%d", connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)));
        return -1;
    }
    send(sock , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}
