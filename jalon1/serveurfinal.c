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


List new_list(void){return NULL; }
 int is_empty(List lis){ 
if(lis==NULL)
   {return 1;}
return 0;
}

 
 List insert(List lis,int x ,int sockett,char *addresse,char name[MSG_LEN],char time[MSG_LEN]){
 listclient *client_element;
 client_element=malloc(sizeof(client_element));
 if(client_element ==NULL){
 
     fprintf(stderr,"eurreur:probleme allocation dynamique .\n");
     exit(EXIT_FAILURE);}
     client_element->port=x;
     client_element->socket=sockett;
     client_element->addre=addresse;
     strcpy(client_element->nickname,name);
     strcpy(client_element->connection,time);
     client_element->next=NULL;
	 if(is_empty(lis)==1)
	 return  client_element;
	 listclient *temp;
	 temp=lis;
	 while(temp->next !=NULL)
	  {temp=temp->next;}
	 temp->next=client_element;
	 return lis;


     
 }
 int rechval(List l,char * msg){
 listclient *temp;
	 temp=l;
	 while(temp->next !=NULL)
	  {
		if(strcmp(temp->nickname,msg)==0)
 {return 1;temp=temp->next;}}
 
 return 0;}
 void print_list(List l){if(is_empty(l)){printf("rien a afficher  votre liste chainee est vide ");
 return;}
 while(l!=NULL){printf("[%d] [%d] [%s] [%s] [%s]",l->port,l->socket,l->addre,l->nickname,l->connection);
 l=l->next;}
  printf("\n");}
  int  nbre_client(List l){ int nb ; nb=0;while(l!=NULL){nb=nb+1; l=l->next; } return nb;}
  int nbre_occurence(List l,char * msg){listclient *temp;
	 temp=l;
	 int nb=0;
	 while(temp->next !=NULL)
	  {
		if(strcmp(temp->nickname,msg)==0){nb=nb+1;
temp=temp->next;}}
 
 return nb;}
 List supprimerElement(List lis, char* ch)
{

   listclient *courant, *precent;
    courant=lis;
    if(lis == NULL)
        return NULL;


    if(strcmp(courant->nickname,ch)==0)
    {


        precent = courant->next;
        free(courant);

        return precent;
    }
    else
    {
        lis->next = supprimerElement(lis->next, ch);
        return lis;
    }
}
char* itoa(int value, char* result, int base) { // check that the base if valid
 if (base < 2 || base > 36) { *result = '\0'; return result; } 
 char* ptr = result, *ptr1 = result, tmp_char; 
 int tmp_value; 
 do 
{ tmp_value = value; value /= base; *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)]; } while ( value ); 
// Apply negative sign 
if (tmp_value < 0) *ptr++ = '-'; *ptr-- = '\0'; while(ptr1 < ptr) { tmp_char = *ptr; *ptr--= *ptr1; *ptr1++ = tmp_char; } return result; }

void echo_server(int sfd) {

	struct pollfd fds[MAX_CLIENTS];
	char buff[MSG_LEN];
	memset(fds, 0, sizeof(struct pollfd)* MAX_CLIENTS); // Cleaning memory
	fds[0].fd = sfd;
	fds[0].events = POLLIN;
	fds[0].revents = 0;
         List list_client=new_list();
	struct sockaddr_in cli;
	int connfd = -1;
	socklen_t len = sizeof(cli);
	struct message msgstruct;
		char string[MSG_LEN];
    char ch[MSG_LEN];
    char ch2[MSG_LEN];
     char ch3[MSG_LEN];
     char time_conn[MSG_LEN];
      char ch4[MSG_LEN];
         char ch5[MSG_LEN];
	int i, j;


	while(1){

		if(-1 == poll(fds, MAX_CLIENTS ,-1)){
			perror("Poll");
		}

		for(i = 0; i<MAX_CLIENTS; i++){


			int data_to_be_read = fds[i].revents & POLLIN;

			if (i==0 && (data_to_be_read == POLLIN)){

			  if ((connfd = accept(sfd, (struct sockaddr*) &cli, &len)) < 0) {
			    perror("accept()\n");
			    exit(EXIT_FAILURE);
			  }
			  
			  for(j = 1;j<MAX_CLIENTS;j++){
			    if(fds[j].fd == 0){
			      fds[j].fd = connfd;
			      fds[j].events = POLLIN;
			      fds[j].revents = 0;
				  break;
			    }
			  }
			 
			  fds[0].revents = 0;			
			}
			 
	
			if( i > 0 && (data_to_be_read == POLLIN)){

			  // Cleaning memory
			  memset(&msgstruct, 0, sizeof(struct message));
			  memset(buff, 0, MSG_LEN);
			  
			  // Receiving message
	      		       
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
		
		
		else 
		strcpy(msgstruct.infos, "vous aetes accepter nom  entrant pour  la premiere fois welcome");
		printf("vous aetes accepter nom  entrant pour  la premiere fois welcome");
		
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

			
			    printf("Message sent!\n");
			    // Cleaning memory
			      memset(&msgstruct, 0, sizeof(struct message));
			    memset(buff, 0, MSG_LEN);
			    
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
