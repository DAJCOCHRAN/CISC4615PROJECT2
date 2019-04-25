#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include<cstring>
using namespace std;

struct struct_sample {
    char str[50];
    uint32_t num;
};

int main(int argc, char **argv)
{
    int r_sockfd, s_sockfd; // receiving and sending port. cannot use same port yet.
    struct sockaddr_in recvaddr, sendaddr;

    r_sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    bzero(&recvaddr, sizeof(recvaddr));
    recvaddr.sin_family = AF_INET;
    recvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    recvaddr.sin_port = htons(19990);
    bind(r_sockfd, (struct sockaddr *)&recvaddr, sizeof(recvaddr));

    s_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&sendaddr, sizeof(sendaddr));
    recvaddr.sin_family = AF_INET;
    recvaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    recvaddr.sin_port = htons(8000);
    //bind(s_sockfd, (struct sockaddr *)&sendaddr, sizeof(sendaddr));

    cout<<"receiving and sending Wating...."<<endl;
    int buff;
    struct struct_sam sample;
    recvfrom(r_sockfd, &buff, sizeof(buff), 0, NULL, NULL);
    printf("Buff received is %d \n",buff);
    buff++;
    //memcpy(&sample,buff,sizeof(sample));
    //int num;
    //sprintf(num,"%d",sample.num);
    //cout<<"string:"<<sample.str<<endl<<"num::"<<num<<endl;
    //sample.num = 2;
    printf("Contact from client...\n Sending ACK with %d \n", buff);
    sendto(s_sockfd, &buff, sizeof(buff), 0, (struct sockaddr *)&sendaddr, sizeof(struct sockaddr_in));
    close(r_sockfd);
    close(s_sockfd);
}

