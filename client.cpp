#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include<cstring>
#include<math.h>

#define SERVER_PORT 6000
#define BUFF_LEN 512
#define SERVER_IP "172.0.5.182"


struct dataBlock{
    int seqNum;
    char bufmsg[BUFF_LEN];

};


void udp_msg_sender(int fd, struct sockaddr* dst)
{

    socklen_t len;
    dataBlock msg;
    struct sockaddr_in src;
    // char buff[BUFF_LEN] = "1";
    int ack = 1;
     bool data = false; // decision to send the data and start flow control
     msg.seqNum = htonl(1); // conversion of int to be sent in network

    while(1)
    {
        //This portion of code we are not sending data, just send recv seqNum for ACK
        if (data == false)
        {
                len = sizeof(*dst);
                printf("client sending out ack:%d\n",ack);  //print out
                htonl(ack); //Prepare int to be sent through network
                sendto(fd, &ack, sizeof(ack), 0, dst, len);
                //memset(buff, 0, BUFF_LEN);
                recvfrom(fd, &ack, sizeof(ack), 0, (struct sockaddr*)&src, &len);  // receive from server
                ntohl(ack); //Unpacking ack
                printf("server contact, received ACK:%d\n Ready to send data!! \n",ack);
                sleep(1);  // sleep 1 second
        }//end of data = false
        data = true;
        //Were now sending the data structure
                if (data = true)
                {
                        printf(" Sending data and sequence number...\n");
                        //my implemented algorithm for sending fractured messages
                        int prevWin = 2;
                        char original[BUFF_LEN] = "Hello! I love Fordham University in New York City and I love to code!!!";
                        int maxWin;

                        for(int i = 0; i < 8; i++) // change 7 if message length changes and replace with variable
                        {
                                //ackknowledgement increases and is managed by client to become seqNum var
                                ack ++;
                                msg.seqNum = htonl(ack);
                                //Becuase 0 and 1 cannot be squared
                                if( i < 2)
                                {
                                        msg.bufmsg[0] = original[i];
                                        printf("%s \n",msg.bufmsg);
                                        sendto(fd, &msg, sizeof(msg), 0, dst, len);
                                        bzero(msg.bufmsg, BUFF_LEN);
                                }
                                //seq num above two is fine to be powered
                                if(i >= 2)
                                {
                                        //sets max array window to send exponential bytes
                                        maxWin = pow (2.0 , i);
                                        int j = 0;
                                        while((prevWin <= maxWin)&&(maxWin < 200))
                                        {
                                                msg.bufmsg[j] = original[prevWin];
                                                prevWin++;
                                                j++;
                                        }
                                        printf("%s \n",msg.bufmsg);
                                        sendto(fd, &msg, sizeof(msg), 0, dst, len);
                                        bzero(msg.bufmsg, BUFF_LEN);
                                }
                        }
                //to send flag for receiver to stop on sender side
                memcpy(msg.bufmsg, "$$$", BUFF_LEN);
                sendto(fd, &msg, sizeof(msg), 0, dst, len);
                printf("Broadcasting finished!");
                //end of data sending algorithm
                break; //all data has been sent
                }//end of data = true
    }

}
/*
    client:
            socket-->sendto-->revcfrom-->close
*/

int main(int argc, char* argv[])
{
    int client_fd;
    struct sockaddr_in ser_addr;

    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(client_fd < 0)
    {
        printf("create socket fail!\n");
        return -1;
    }

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    //ser_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_port = htons(SERVER_PORT);

    udp_msg_sender(client_fd, (struct sockaddr*)&ser_addr);

    close(client_fd);

    return 0;
}

~

