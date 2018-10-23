#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>

#define N_      5

int main(int argv, char * argc[]){
    
    int clientSocket;   /*---- ClientScoket is this personal clients socket.  ----*/
    
    int port_number;    /*---- Port_number is the port number that is changed for this specific client  ----*/
    
    char buffer[1024];  /*---- Buffer is an input buffer  ----*/
    struct sockaddr_in serverAddr;/*---- removes the '\n' since it will  ----*/
    socklen_t addr_size;
    
    if(argv < 2){   /*---- The below checks and sets the port numbers  ----*/
        printf("argv < 1\n");
        exit(1);
    } else if (argv == 2 && strstr(argc[1],"localhost:") != NULL){
        char *str = strchr(argc[1], ':');
        str += 1;
        
        port_number = atoi(str);    /*---- Changes the number past the ':' to integers  ----*/
    } else {
        printf("Port number not set");
    }
    
    clientSocket = socket(AF_INET, SOCK_STREAM, 0); /*---- generates the socket ----*/
    serverAddr.sin_family = AF_INET;    /*---- Specifies that we want to use TCP connection thingy ----*/
    serverAddr.sin_port = htons(port_number);   /*---- sets the port number ----*/
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");    /*---- sets the host to be local --> this is not set by the argument list above  ----*/
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  /*---- Set all bits of the padding field to 0  ----*/
    
    if(connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) <0) { /*---- Connect the socket to the server using the address struct  ----*/
        printf("Error in client connection");
        exit(1);
    }
    
    while(1) {  /*---- loop repeats the receive / send of the client ----*/
        
        printf("CLIENT: ");
        
        fgets(buffer, 1024, stdin); /*---- gets the characters from the terminal ----*/
        
        printf("%s\n", buffer);
        
        if(buffer[0] == 'q' && buffer[1] == 'u' && buffer[2] == 'i' && buffer[3] == 't'){    /*---- if the user enters "quit" as input the terminal quits ----*/
            break;
        }
        
        if(buffer[0] == 'j' && buffer[1] == 'o' && buffer[2] == 'b' && buffer[3] == 's'){
            bzero(buffer, sizeof(buffer));
            buffer[0] = 'p';
            buffer[1] = 's';
        }
        
        int buff_size = strlen(buffer);     /* Encryption */
        int i;
        
        for(i = 0; i < buff_size - 1; i++){
            buffer[i] = buffer[i] + N_;
        }
        
        send(clientSocket, buffer, sizeof(buffer), 0);
        bzero(buffer, sizeof(buffer));
        
        char rec_buffer[1048576];   /*---- a buffer that is set empty to recieve the message from the client ----*/
        
        // bzero(rec_buffer, sizeof(rec_buffer));  /*---- zeros the buffer ----*/
        
        if(recv(clientSocket, rec_buffer, sizeof(rec_buffer), 0) < 0){  /*---- recieves the socket connection message and sets it to buffer ----*/
            perror("recv"); /*---- prints a standar output ----*/
            break;
        } else {
            printf("SERVER: %s\n", rec_buffer);
            bzero(rec_buffer, sizeof(rec_buffer));  /*---- zeros the buffer if successful ----*/
        }
    }
    
    close(clientSocket);    /*---- closes the socket when client exits main loop ----*/
    return 0;
}