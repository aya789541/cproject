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



int validation_nom(char *nk_name){
int longeur_nickname = 0;
	
	char ca =nk_name[0];
	while(ca != '\0'){
        if(longeur_nickname > NICK_LEN-1){
			printf("vous avez entrer un  nom  trés long \n");
			return 0;
		}
		if(!(('A' <= ca && ca <= 'Z') || ('a' <= ca && ca <= 'z') || ('0' <= ca && ca <= '9'))){
			printf("tu dois pas entrer des caractéres  spéciaux .\n");
			return 0;
		}
        longeur_nickname=longeur_nickname+1;
		ca= nk_name[ longeur_nickname];
		
	}


	if(longeur_nickname ==0){
		printf("Tle nom ne doit  pas contenir  une chaine de  caractére vide.\n");
		return 0;
	}

	return 1;
}

void echo_client(int sockfd) {
struct message msgstruct;
	char buff[MSG_LEN];
	int n;
	struct pollfd fds[2];
	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;
	fds[0].revents = 0;

	fds[1].fd = sockfd;
	fds[1].events = POLLIN;
	fds[1].revents = 0;
	int s;
	s=0;
	int k;
	k=0;
	char ch[MSG_LEN];
	 char ch3[MSG_LEN];
	 int i;

	while (1) {
	if(-1 == poll(fds, 2 ,-1)){
	      	perror("Poll");
	  }
	   
	  for(i = 0; i<2; i++){
  
		// Cleaning memory
		memset(buff, 0, MSG_LEN);

		// Cleaning memory
		memset(&msgstruct, 0, sizeof(struct message));
		memset(buff, 0, MSG_LEN);
	       memset(ch3, 0, MSG_LEN);
		 int data_to_be_read = fds[i].revents & POLLIN;
		if (i==0 && (data_to_be_read == POLLIN)){
		// Getting message from clientQ<W
		if(s==0){printf("Connecting to server ... done!\n");
		printf("[Server] : please login with /nick <your pseudo>");
		s++;}
		printf("Message: ");
		n = 0;
		while ((buff[n++] = getchar()) != '\n') {} // trailing '\n' will be sent
		
	if(k==0){strcpy(ch,buff+6);k++;}
	
	
		
		// Filling structure
		 if(strncmp(buff, "/nick ", 6) == 0){
		
		//if(validation_nom(buff+6)){
		msgstruct.pld_len = strlen(buff);
		strcpy(msgstruct.nick_sender,buff+6);
		msgstruct.type = NICKNAME_NEW;
		strcpy(msgstruct.infos,buff+6);
		//strcpy(temp,&buff[6])
		printf("[Serveur] : Welcome on the chat %s",buff+6);
		
		}
		if(strncmp(buff,"/who ",5) == 0){
		msgstruct.pld_len = strlen(buff);
		strcpy(msgstruct.nick_sender,ch);
		msgstruct.type = NICKNAME_LIST;
		strcpy(msgstruct.infos,"");}
	       if(strncmp(buff,"/whois ",7)==0){
		msgstruct.pld_len = strlen(buff);
		strcpy(msgstruct.nick_sender,ch);
		msgstruct.type = NICKNAME_INFOS;
		strcpy(msgstruct.infos,ch);}
		
		if(strncmp(buff,"/msgall ",8)==0){
		msgstruct.pld_len = strlen(buff);
		strcpy(msgstruct.nick_sender,ch);
		msgstruct.type = BROADCAST_SEND;
		strcpy(msgstruct.infos,"");}
		if(strncmp(buff,"/msg ",5)==0){
		msgstruct.pld_len = strlen(buff);
		strcpy(msgstruct.nick_sender,ch);
		msgstruct.type = UNICAST_SEND;
		strcpy(msgstruct.infos,"");}
			
		// Sending structure
		if (send(fds[1].fd , &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[1].fd , buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}
		printf("Message sent!\n");
		// Cleaning memory
		memset(&msgstruct, 0, sizeof(struct message));
		memset(buff, 0, MSG_LEN);
	       fds[0].revents = 0;
	      }
	      if( i == 1 && (data_to_be_read == POLLIN)){
		// Receiving structure
		if (recv(fds[1].fd , &msgstruct, sizeof(struct message), 0) <= 0) {
			break;
		}
		// Receiving message
		if (recv(fds[1].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}
		printf("pld_len: %i / nick_sender: %s / type: %s / infos: %s\n", msgstruct.pld_len, msgstruct.nick_sender, msg_type_str[msgstruct.type], msgstruct.infos);
		printf("Received: %s", buff);
		 if(strncmp(buff,"/who ",5) == 0){printf("[Server] : Online users are \n %s",msgstruct.infos);
		
		
		
	}
	 if(strncmp(buff,"/whois ",7)==0){printf("[Server] : %s",msgstruct.infos);}
	 fds[1].revents = 0;
}}} printf("\n");
}


int handle_connect(char *addr, char *port) {

	struct addrinfo hints, *result, *rp;
	int sfd;
	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(addr, port, &hints, &result) != 0) {
		perror("getaddrinfo()");
		exit(EXIT_FAILURE);
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype,rp->ai_protocol);
		if (sfd == -1) {
			continue;
		}
		if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1) {
			break;
		}
		close(sfd);
	}

	if (rp == NULL) {
		fprintf(stderr, "Could not connect\n");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(result);
	return sfd;
}

int main(int argc, char *argv[]) {

	int sfd;
	sfd = handle_connect(argv[1],argv[2]);
	printf("sonnecting to server done");

	echo_client(sfd);


	close(sfd);
	return 0;
}

