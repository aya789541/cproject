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
#include "salon_client_file.h"
#include "common.h"
#include<ctype.h>
//permet  de  creer  une nouvelle liste de fichiert
struct liste_f *new_file(){
    struct liste_f *l = malloc(sizeof(struct liste_f));
    l->premier = NULL;
    l->nbr = 0;
    
    return l;
}

void ajouter_fichier(
   
    char *nom,
    char *destinataire,
    int fd,
    struct liste_f *l){ struct fichier *fich = malloc(sizeof(struct fichier));
    

 
    
   strcpy(fich->nom, nom);
    fich->fichier_fd = fd;

     fich->destination.nick_name = malloc(MSG_LEN * sizeof(char));
    strcpy( fich->destination.nick_name, destinataire);

     fich->destination.socket_fd = NULL;
    fich->destination.addresse = NULL;
    fich->destination.port = NULL;
    fich->destination.connect_time = NULL;
    fich->destination.next = NULL;


    fich->next = l->premier;
    l->premier = fich;
    l->nbr++;
    
    
}
int validation_identite_nom(char nickname[]){

	int length = 0;
	char ca = nickname[0];
	while(ca != '\0'){
		
		if(isalnum(ca)==0){
			printf("contient des caracteres speciaux.\n");
			return 0;
		}
		ca= nickname[length++];
		if(length > NICK_LEN-1){
			printf("essayer encore le nom que  vous avez indiqué dépasse la taille \n");
			return 0;
		}
	}
	length=length+1;

	if(length==1){
		printf("essayer de  nouveau votre caine ne peut pas etre vide \n");
		return 0;
	}

	return 1;
}

int validation_nom_ch(char *nk_name){
int longeur_nickname = 0;
	
	char ca =nk_name[0];
	while(ca != '\0'){
        if(longeur_nickname > NICK_LEN-1){
			printf("vous avez entrer un  nom  trés long \n");
			return 0;
		}
		if((('A' <= ca && ca <= 'Z') || ('a' <= ca && ca <= 'z') || ('0' <= ca && ca <= '9'))){
			printf("tu dois pas entrer des caractéres  spéciaux .\n");
			return 1;
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
/*void echo_client(int sockfd) {

}*/

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
 if(argc!=3 ){
        perror("Oops!!vous avez des arguments qui manquent ");
        exit(-1);//on va sortir dans ce cas.
    }

	int sockfd;
	sockfd = handle_connect(argv[1],argv[2]);

	//echo_client(sfd);
//nom Salon 
printf("Connecting to server ... done!\n");
printf("[Server] : please login with /nick <your pseudo>\n");
char nomsalon[NICK_LEN];
	strcpy(nomsalon, "");
	char nom_nick[NICK_LEN];


	char buff[MSG_LEN];
	int n;
	struct message msgstruct;
	struct pollfd fds[2];
	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;
	fds[0].revents = 0;

	fds[1].fd = sockfd;
	fds[1].events = POLLIN;
	fds[1].revents = 0;
	int i;
	int k;
	char ch[MSG_LEN];
	char ch1;
	char ch2;
	int s;
	s=0;
	struct liste_f *newf=new_file();
	
char ch3[MSG_LEN];

	while (1) {
	  if(-1 == poll(fds, 2 ,-1)){
	      	perror("Poll");
	  }
	   
	  for(i = 0; i<2; i++){
	    
	    int data_to_be_read = fds[i].revents & POLLIN;

	    // Cleaning memory
	    memset(&msgstruct, 0, sizeof(struct message));
	    memset(buff, 0, MSG_LEN);
	    if (i==0 && (data_to_be_read == POLLIN)){
	      // Getting message from client
	    
	      n = 0;
	      fflush(stdin);
	      while ((buff[n++] = getchar()) != '\n') {} 
	      while(strncmp(buff, "/nick ", 6) != 0 &&(s==0)){
			printf("[Server] : please login with /nick <your pseudo>");
			fflush(stdin);
			ch2 = getchar();
			i = 0;
			while(ch2 != '\n'){
				buff[i++] = ch2;
				ch2= getchar();
			}
			buff[i++] = '\0';
		}
		
		// trailing '\n' will be sent
	      if(k==0){strcpy(ch,buff+6);k++;}
	      // Filling structure
	       if(strncmp(buff, "/nick ", 6) == 0){
		s=s+1;
		strcat(buff,"\n");
		msgstruct.pld_len = strlen(buff);
		strcpy(msgstruct.nick_sender,buff+6);
		msgstruct.type = NICKNAME_NEW;
		strcpy(msgstruct.infos,buff+6);
		if(s==1){printf("[Serveur] : Welcome on the chat %s \n",buff+6);
		if (send(fds[1].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[1].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}}
		
		
		else{
		char initial = 1;
		while(initial){
		if(validation_identite_nom(buff+6)==1){
			printf("[Serveur] : Welcome on the chat %s \n",buff+6);
			if (send(fds[1].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[1].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}
		initial = 0;	
		}else{
		fflush(stdin);
			ch1 = getchar();
			i = 0;
			while(ch1 != '\n'){
				buff[i++] = ch1;
				ch1 = getchar();
			}
			buff[i++] = '\0';
		}

	}}
		}
		if(strncmp(buff,"/who ",5) == 0){
		msgstruct.pld_len = strlen(buff);
		strcpy(msgstruct.nick_sender,ch);
		msgstruct.type = NICKNAME_LIST;
		strcpy(msgstruct.infos,"");
		if (send(fds[1].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[1].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}	
		}
		 if(strncmp(buff,"/whois ",7)==0){
		msgstruct.pld_len = strlen(buff);
		strcpy(msgstruct.nick_sender,ch);
		msgstruct.type = NICKNAME_INFOS;
		strcpy(msgstruct.infos,buff+7);
		if (send(fds[1].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[1].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}	
		}
		if(strncmp(buff,"/msgall ",8)==0){
		msgstruct.pld_len = strlen(buff);
		strcpy(msgstruct.nick_sender,ch);
		msgstruct.type = BROADCAST_SEND;
		strcpy(msgstruct.infos,"");
		if (send(fds[1].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[1].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}	}
		if(strncmp(buff,"/msg ",5)==0){
		 int buff_indexe = 5;

        while(buff[buff_indexe] != ' '){
            if(buff[buff_indexe++] == '\0'){
               
            }
        }
		msgstruct.pld_len = strlen(buff);
		strcpy(msgstruct.nick_sender,ch);
		msgstruct.type = UNICAST_SEND;
	strncpy(msgstruct.infos, buff+5, buff_indexe-5);
        msgstruct.infos[buff_indexe-5] = '\0';
        if (send(fds[1].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[1].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}}
		
	       if(strncmp(buff, "/create ", 8) == 0){
char ch3[MSG_LEN];
strcpy(ch3,buff+8);
printf("%s",ch3);
        msgstruct.pld_len = strlen(buff);
        strcpy(msgstruct.nick_sender,ch);
        msgstruct.type = MULTICAST_CREATE;
        strcpy(msgstruct.infos, buff+8);
        if(validation_nom_ch(ch3)==1){
      
						// Sending structure
		if (send(fds[1].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[1].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}	
						}
						}
        
        if(strncmp(buff,"/channel_list ",14) == 0){

        msgstruct.pld_len = strlen(buff);
        strcpy(msgstruct.nick_sender, ch);
        msgstruct.type = MULTICAST_LIST;
        strcpy(msgstruct.infos, "");
        // Sending structure
        if (send(fds[1].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[1].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}	
        
        }
        if(strncmp(buff, "/join ", 6) == 0){

        msgstruct.pld_len =strlen(buff);
        strcpy(msgstruct.nick_sender, ch);
        msgstruct.type = MULTICAST_JOIN;
        strcpy(msgstruct.infos, buff+6);
     
        // Sending structure
        if (send(fds[1].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[1].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}	
        
        }
        
    if(strncmp(buff, "/quit ", 6) == 0){

       
        msgstruct.pld_len =strlen(buff);;
        strcpy(msgstruct.nick_sender, ch);
        msgstruct.type = MULTICAST_QUIT;
        strcpy(msgstruct.infos, buff+6);
        // Sending structure
        if (send(fds[1].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[1].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}	
        }

        
if(strncmp(buff, "/send ", 6) == 0){
	 

       int buff_indexe = 6;

        while(buff[buff_indexe] != ' '){
            if(buff[buff_indexe++] == '\0'){
               
            }
        }
      
     strncpy(msgstruct.infos, buff+6, buff_indexe-6);
        msgstruct.infos[buff_indexe-6] = '\0';
         strcpy(msgstruct.nick_sender, ch);
        msgstruct.type = FILE_REQUEST;
       
        
        msgstruct.pld_len = sizeof(char)*(strlen(buff+6)+1);
         // Sending structure
        if (send(fds[1].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[1].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}

       
	}
	if(strncmp(buff, "/file_reject ", 13) == 0){
        msgstruct.type = FILE_REJECT;
        msgstruct.pld_len = (strlen(buff+13)+1)*sizeof(char);
        strcpy(msgstruct.nick_sender, ch);
         // Sending structure
        if (send(fds[1].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[1].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}
        }
	if(strncmp(buff, "/file_accept ", 13) == 0){
        msgstruct.type = FILE_ACCEPT;

        
        strcpy(msgstruct.infos, buff+13);
         // Sending structure
        if (send(fds[1].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[1].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}}
        
	      if ((strcmp(buff, "/quit\n")) == 0) {
		break;
	      }
	/*else{ 
	  msgstruct.type = MULTICAST_SEND;
        strcpy(msgstruct.infos, nomsalon);
       
	   if (send(fds[1].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[1].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}	
        }
	*/
	if(strncmp(buff, "/echo ", 6) == 0){
	strcpy(msgstruct.nick_sender, ch);
        msgstruct.type = ECHO_SEND;
        strcpy(msgstruct.infos, buff+6);
                msgstruct.pld_len =strlen(buff);;
        
         // Sending structure
        if (send(fds[1].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[1].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}
        
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
		
		 if(strcmp(msgstruct.infos,"/who") == 0){
		 strcat(ch3,"[Server] : Online users are ");
		 strcat(ch3,buff);
		 printf("%s",ch3);
		 }
		 
		  if(strncmp(msgstruct.infos, "/whois ",7) == 0){printf(" %s",msgstruct.infos);}
		 if(strncmp(buff, "/create ", 8) == 0){
		 if(strcmp(msgstruct.infos, "isAlreadyUsed") == 0){
						printf("la canal est déjà utilisé votre  demande est rejeté.\n");
					}else{
						strcpy(nomsalon, msgstruct.infos);
						printf("You have created channel %s \n", buff+8);
						printf("[%s]> You have joined %s.\n", buff+8,buff+8);
					}
					}
					 if(strncmp(buff, "/join ", 6) == 0){
					
					if(strlen(msgstruct.infos)==0){
						printf("il n'ya pas de channel avec ce  nom. Join  est Rejete \n");
					}else{
						strcpy(nomsalon, msgstruct.infos);
						printf("You have joined %s\n",buff+6);
					}}
					if(strncmp(buff, "/send ", 6) == 0){
					char ch7[MSG_LEN];
		int l=strlen(msgstruct.infos);
		strcat(msgstruct.infos,"\n");
		strcpy(ch7,buff + l + 6);
		//ouvrir le fichier
		int file = open(ch7, O_RDONLY);
		if(file == -1){
								printf("On peut pas trouver le fichier ");
							}
							else{
							ajouter_fichier(ch7, msgstruct.infos, file,newf);
							 // Sending structure
        if (send(fds[1].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[1].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}	
			printf("was sended succefully.\n");				
		}
			}
			 if(strncmp(buff,"/join ", 6) == 0){printf("Received: \n %s", buff+6);}	
			 				
								
							
					else {printf("Received: \n %s", buff);}
					
					

					
	      fds[1].revents = 0;
	    }
	}
      }


	close(sockfd);
	return 0;
}
