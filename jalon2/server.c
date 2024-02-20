/*#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "common.h"
#include "msg_struct.h"
void echo_server(int sfd) {

		struct pollfd fds[maxim_client];
	char buff[MSG_LEN];
	struct message msgstruct;
	memset(fds, 0, sizeof(struct pollfd)* maxim_client); // Cleaning memory
	// List list_client=new_list();
	fds[0].fd = sfd;
	fds[0].events = POLLIN;
	fds[0].revents = 0;

	struct sockaddr cli;
	int connfd;
	socklen_t len;
	len = sizeof(cli);

    char ch[MSG_LEN];
   

	while(1){

		if(-1 == poll(fds, maxim_client ,-1)){
			perror("Poll");
		}
		

		for(int i = 0; i<maxim_client; i++){


			int data_to_be_read = sfd && fds[i].revents;

			if (i==0 && (data_to_be_read == POLLIN)){

			if ((connfd = accept(sfd, (struct sockaddr*) &cli, &len)) < 0) {
				perror("accept()\n");
				exit(EXIT_FAILURE);
			}
			
			

			fds[i].revents = 0; 
			fds[i+1].fd = connfd;
			fds[i+1].events = POLLIN;
		}

          

	
			if( (i > 0) && (data_to_be_read == POLLIN)){

				// Cleaning memory
				memset(buff, 0, MSG_LEN);
				
		          // buff[MSG_LEN-1] = '\0';
		       // Receiving structure
		if (recv(fds[i].fd, &msgstruct, sizeof(struct message), 0) <= 0) {
			break;
		}
		// Receiving message
		if (recv(fds[i].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}

		        if ((strcmp(buff, "/quit\n")) == 0) {
		            printf("tu va te deconnecter...\n");
		            break;  }
		    
				
		        printf("pld_len: %i / nick_sender: %s / type: %s / infos: %s\n", msgstruct.pld_len, msgstruct.nick_sender, msg_type_str[msgstruct.type], msgstruct.infos);
		printf("Received: %s", buff);
		  
				
			
			
		// Sending structure (ECHO)
	if (send(fds[i].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[i].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}
				printf("Message sent!\n");
				fds[i].revents = 0;	
		}
	}


int handle_bind() {
	struct addrinfo hints, *result, *rp;
	int sfd;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, SERV_PORT, &hints, &result) != 0) {
		perror("getaddrinfo()");
		exit(EXIT_FAILURE);
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		
		sfd = socket(rp->ai_family, rp->ai_socktype,
		rp->ai_protocol);
		
		if (sfd == -1) {
			continue;
		}

		if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) {
			break;
		}
		close(sfd);
	}

	if (rp == NULL) {
		fprintf(stderr, "Could not bind\n");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(result);
	return sfd;
}

int main() {
	int sfd;
	sfd = handle_bind();

	if ((listen(sfd, SOMAXCONN)) != 0) {
		perror("listen()\n");
		exit(EXIT_FAILURE);
	}

	echo_server(sfd);
	close(sfd);
	return EXIT_SUCCESS;
}
*/
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "msg_struct.h"

#include "common.h"
char* itoa(int value, char* result, int base) { // check that the base if valid
 if (base < 2 || base > 36) { *result = '\0'; return result; } 
 char* ptr = result, *ptr1 = result, tmp_char; 
 int tmp_value; 
 do 
{ tmp_value = value; value /= base; *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)]; } while ( value ); 
// Apply negative sign 
if (tmp_value < 0) *ptr++ = '-'; *ptr-- = '\0'; while(ptr1 < ptr) { tmp_char = *ptr; *ptr--= *ptr1; *ptr1++ = tmp_char; } return result; }

void echo_server(int sfd) {
 
	struct pollfd fds[maxim_client];
	char buff[MSG_LEN];
	struct message msgstruct;
	memset(fds, 0, sizeof(struct pollfd)* maxim_client); // Cleaning memory
	 List list_client=new_list();
	fds[0].fd = sfd;
	fds[0].events = POLLIN;
	fds[0].revents = 0;

	struct sockaddr cli;
	int connfd;
	socklen_t len;
	len = sizeof(cli);
 char string[MSG_LEN];
    char ch[MSG_LEN];
    char ch2[MSG_LEN];
     char ch3[MSG_LEN];
     char time_conn[MSG_LEN];
      char ch4[MSG_LEN];
         char ch5[MSG_LEN];
     
     

	while(1){

		if(-1 == poll(fds, maxim_client ,-1)){
			perror("Poll");
		}
		

		for(int i = 0; i<maxim_client; i++){


			int data_to_be_read = sfd && fds[i].revents;

			if (i==0 && (data_to_be_read == POLLIN)){

			if ((connfd = accept(sfd, (struct sockaddr*) &cli, &len)) < 0) {
				perror("accept()\n");
				exit(EXIT_FAILURE);
			}
			
			

			  
			  for(int j = 1;j<maxim_client;j++){
			    if(fds[j].fd == 0){
			      fds[j].fd = connfd;
			      fds[j].events = POLLIN;
			      fds[j].revents = 0;
				  break;
			    }}
		
 fds[0].revents = 0;
          }
          
			if( (i > 0) && (data_to_be_read == POLLIN)){

				// Cleaning memory
				memset(buff, 0, MSG_LEN);
				memset(&msgstruct, 0, sizeof(struct message));
				
				
			
			
				
		
		          // buff[MSG_LEN-1] = '\0';
		       // Receiving structure
		if (recv(fds[i].fd, &msgstruct, sizeof(struct message), 0) <= 0) {
			break;
		}
		// Receiving message
		if (recv(fds[i].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}

		        if ((strcmp(buff, "/quit\n")) == 0) {
		            printf("tu va te deconnecter...\n");
		            break;}
		    
				
		        printf("pld_len: %i / nick_sender: %s / type: %s / infos: %s\n", msgstruct.pld_len, msgstruct.nick_sender, msg_type_str[msgstruct.type], msgstruct.infos);
		printf("Received: %s", buff);
		   
		        struct sockaddr_in *client_in = (struct sockaddr_in *) &cli;
		        time_t timestamp = time(NULL);
    			struct tm * pTime = localtime(&timestamp); 
				char date_str[MSG_LEN];
    			strftime(date_str, MSG_LEN, "%d/%m/%Y %H:%M:%S", pTime);
                   strcpy(time_conn, date_str);
				
           list_client=insert(list_client,htons(client_in->sin_port),connfd,inet_ntoa(client_in->sin_addr),msgstruct.nick_sender,time_conn);
			print_list(list_client);
			if((rechval(list_client,msgstruct.nick_sender)==1)){list_client=supprimerElement(list_client,msgstruct.nick_sender);}
		if(strncmp(buff, "/nick ", 6) == 0){
		
		if(rechval(list_client,msgstruct.nick_sender)==1){
		strcpy(msgstruct.infos, "AlreadyUsed");
		printf("\tOperation rejeter: votre nom est déja utilisé.\n");}
		
		
		else printf("vous aetes accepter nom  entrant pour  la premiere fois welcome");
		
		// Sending structure (ECHO)
	if (send(fds[i].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[i].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}
		}
		if(strncmp(buff,"/who ",5)==0){
		strcpy(msgstruct.nick_sender, "Server");
		msgstruct.pld_len=nbre_client(list_client)*NICK_LEN;
		
		
		 while(list_client!=NULL){
		 
		 strcat(ch2,list_client->nickname);
		 strcat(ch2," ");
                  list_client=list_client->next;}     
                  strcpy(ch3,ch2); 
                  printf("printed in ch3");  
                  printf("%s",ch3);  
                 strcpy(msgstruct.infos,ch3);
		
		
		// Sending structure (ECHO)
	if (send(fds[i].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[i].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}
		
		}
		 if(strncmp(buff,"/whois ",7)==0){
 
	 while(list_client!=NULL)
	  {
		if(strcmp(list_client->nickname,buff+7)==0)
		
 {strcat(ch4,list_client->nickname);
  strcat(ch4,"connected since");
 strcat(ch4,list_client->connection);
 strcat(ch4,"with IP address");
 strcat(ch4,list_client->addre);
 strcat(ch4,"and port number");
  itoa(list_client->port,string,10);
 strcat(ch4,string);
 
  strcpy(msgstruct.infos,ch4);
  /* if (vsprintf(&num, "%d",temp->port) == -1) {
        perror("vsprintf");
    } else {
        strcat(ch,num);
        
        free(num);
    }*/

/* if(strncmp(buff,"/msgall ",8)==0){
	//strcpy(temp,&buff[8])
	*List element = list_client;
	while(element!=NULL){
		if (
	}
	
}

*/
}
list_client=list_client->next;}

		// Sending structure (ECHO)
	if (send(fds[i].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[i].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}
		
 
		 }
		if(strncmp(buff,"/msgall ",8)==0){
		strcpy(ch5,buff+8);
		listclient * temp = list_client;
		while(temp!=NULL){
		if((fds[i].fd)!= temp->socket){
		
			// Sending structure (ECHO)
		if (send(temp->socket, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(temp->socket, ch5, strlen(ch5), 0) <= 0) {
			break;
		}
		}temp=temp->next;}
		}
			
			printf("Message sent!\n");
				fds[i].revents = 0;
				

			}	
		}
	}

}

int handle_bind(char *port) {
	struct addrinfo hints, *result, *rp;
	int sfd;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, port, &hints, &result) != 0) {
		perror("getaddrinfo()");
		exit(EXIT_FAILURE);
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		
		sfd = socket(rp->ai_family, rp->ai_socktype,
		rp->ai_protocol);
			
		if (sfd == -1) {
			continue;
		}

		int value = 1;
		if (-1 == setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int))){
        	perror("Setsockopt:");
    	}

		if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == -1) {
			perror("Bind:");
		}
		break;
	}

	if (rp == NULL) {
		fprintf(stderr, "Could not bind\n");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(result);
	return sfd;
}


int main(int argc, char *argv[]) {
	int sfd;
	
	sfd = handle_bind(argv[1]);
	

	if ((listen(sfd, SOMAXCONN)) != 0) {
		perror("listen()\n");
		exit(EXIT_FAILURE);
	}
      
	echo_server(sfd);
	close(sfd);
	
	return EXIT_SUCCESS;
}

