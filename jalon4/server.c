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
#include "salon_client_file.h"
#define existe 1
#define abscent 0

struct client_in *new_cl(){
    struct client_in *cl = malloc(sizeof(struct client_in));

    cl->socket_fd = malloc(sizeof(int));
    cl->port = malloc(sizeof(int));

    cl->nick_name = malloc(NICK_LEN * sizeof(char));
    cl->addresse = malloc(ch_len * sizeof(char));
    cl->connect_time = malloc(ch_len * sizeof(char));

    cl->next = NULL;

    return cl;
}
//initialisation des  parametres 
  struct liste_client * new_list(){
    struct liste_client *lc = malloc(sizeof(liste_client));
    lc->premier_client = NULL;
    lc->nbr_client = 0;

    return lc;
}

char* itoa(int value, char* result, int base) { // check that the base if valid
 if (base < 2 || base > 36) { *result = '\0'; return result; } 
 char* ptr = result, *ptr1 = result, tmp_char; 
 int tmp_value; 
 do 
{ tmp_value = value; value /= base; *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)]; } while ( value ); 
// Apply negative sign 
if (tmp_value < 0) *ptr++ = '-'; *ptr-- = '\0'; while(ptr1 < ptr) { tmp_char = *ptr; *ptr--= *ptr1; *ptr1++ = tmp_char; } return result; }

void insertion_client(liste_client *l,  struct client_in *cl){
    cl->next = l->premier_client;
    l->premier_client = cl;
    l->nbr_client= l->nbr_client+1;
}
void print_list(liste_client *liste)
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

     struct client_in *temp = liste->premier_client;

    while (temp != NULL)
    {
        printf( "[%d] [%d] [%s]  [%s] [%s] ",*(temp->port),*(temp->socket_fd),temp->addresse,temp->connect_time,temp->nick_name);
        temp = temp->next;
    }
   
}

int rech_exist(liste_client *liste,char *sender, int sock){
    struct client_in *cl = liste->premier_client;

    while(cl != NULL){
        if(*(cl->socket_fd) != sock && strcmp(cl->nick_name, sender) == 0){
            return existe;
        }
        cl = cl->next;
    }

    return abscent;
}
/*int rech_exist_lis(liste_client *liste,char *client){
    struct client_in *cl = liste->premier_client;

    while(cl != NULL){
        if(strcmp(cl->nick_name, sender) == 0){
            return existe;
        }
        cl = cl->next;
    }

    return abscent;
}*/
 struct client_in * get_client(struct liste_client *liste, int sockfd){
    struct client_in *temp = liste->premier_client;

    while(temp != NULL){
        if(*(temp->socket_fd) == sockfd){
            return temp;
        }
        temp = temp->next;
    }

  printf("there's no client with this socket ");
    //exit(EXIT_FAILURE);
    
}
struct client_in *get_user(struct liste_client *liste, char *user){
    struct client_in *temp = liste->premier_client;

    while(temp != NULL){
        if(strcmp(temp->nick_name, user) == 0){
            return temp;
        }
        temp = temp->next;
    }

    return NULL;
}
struct salon *new_salon(char *nom, struct client_in *creator_ch){

    struct salon *s = malloc(sizeof(struct salon));

    s->nom = malloc(sizeof(char)*NICK_LEN);
    strcpy(s->nom, nom);

    s->membres = new_list();
   insertion_client(s->membres,creator_ch);

    return s;
}

int verif_deja_attribuer(list_s *list, char *salon_nom){

    struct salon *s = list->premier_salon;

    while(s != NULL){
        if(strcmp(s->nom, salon_nom) == 0){
            return 0;
        }
        s = s->next;
    }

    return 1;
}

int recupere_client_by_fd(struct liste_client *cl, int sockfd){
    struct client_in *c = cl->premier_client;
    struct client_in *c_avant = NULL;

    // trouver le c avant 
    while(c != NULL){

        if(*(c->socket_fd) == sockfd){
            break;
        }

        c_avant = c;
        c = c->next;
    }

    if(c == NULL){
        return -1;
    }

    // deleting client
    if(c_avant == NULL){
        cl->premier_client = c->next;
    }else{
        c_avant->next = c->next;
    } 

    cl->nbr_client--;

    // vider le tampon  mais conservez les données pour d'autres listes
    free(c);

    return 0;
}
//supprimer les elements de salon
void salon_free(struct salon *sal){
    free(sal->nom);
    free(sal->membres);
}

int recuperer_client_de_salon(struct salon *sal, int sockfd){

    return recupere_client_by_fd(sal->membres, sockfd);
}
int salon_list_drop_salon(struct list_salon *l, struct salon *s){

    struct salon *sal = l->premier_salon;
    struct salon *sal_avant = NULL;

    // trouver le salon qui est avant 
    while(sal != NULL){

        if(sal == s){
            break;
        }

        sal_avant = sal;
        sal = sal->next;
    }

    if(sal == NULL){
        return -1;
    }

    // supprimer salon
    if(sal_avant == NULL){
        l->premier_salon = sal->next;
    }else{
        sal_avant->next = sal->next;
    }

    salon_free(s);

    l->nombre--;

    return 0;
}
//insertion des membresau niveau de salon
void sa_list_insertion(struct list_salon *sal, struct salon *sa){
    sa->next = sal->premier_salon;
    sal->premier_salon = sa;
    sal->nombre++;
}
//creation d'une nouvelle liste salon
 struct list_salon *new_list_s(){
    struct list_salon *list = malloc(sizeof(struct list_salon));
    list->premier_salon = NULL;
    list->nombre = 0;
    return list;
}
struct salon *salon_list_detache_client_by_fd(struct list_salon *salist, int c_fd){
    struct salon *sal = salist->premier_salon;
    while(sal != NULL){
        if(recuperer_client_de_salon(sal, c_fd) != -1){
            return sal;
        }
        sal = sal->next;
    }

    return NULL;
}
void print_list_s(struct list_salon *liste)
{
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

     struct salon *temp = liste->premier_salon;

    while (temp != NULL)
    {
        printf( "[%s] ",temp->nom);
        temp = temp->next;
    }
   
}
//getter de client en nickname
struct client_in *get_client_by_nickname(struct liste_client *cl, char *nickname){
   struct client_in *c = cl->premier_client;

    while(c != NULL){
        if(strcmp(c->nick_name, nickname) == 0){
            return c;
        }
        c = c->next;
    }

    //il nya pas de client avec ce nom 
    return NULL;
}
void supprimer_client(struct client_in *cl){
    
    if(cl != NULL){
          if(cl->socket_fd != NULL) free(cl->socket_fd);
    if(cl->addresse != NULL) free(cl->addresse);
    if(cl->port != NULL) free(cl->port);
    if(cl->nick_name != NULL) free(cl->nick_name);
    if(cl->connect_time != NULL) free(cl->connect_time);
        free(cl);
    }
}
//supprimer de  la liste completement free de  tampon 
void free_en_list(struct liste_client*cl){
 struct client_in *c = cl->premier_client;
    while(c != NULL){
        
        supprimer_client(c);
          c = c->next;
        
    }
}

//meme chose  pour la  liste de sallon on coommence à faire  un free  de chaque  element  de la  memoire tempon ensuite un free definitive  de  la  liste 

void free_element_salon(struct salon *s){
    if(s != NULL){
        free_element_salon(s->next);
        free(s->nom);
    free(s->membres);
    }
    
}
void salon_list_free(struct list_salon *sal){
struct salon *sa = sal->premier_salon;
    while(sa != NULL){
        free_element_salon(sa);
        sa = sa->next;
    }

    
}



void echo_server(int sfd) {
struct pollfd fds[MAX_CLIENTS];
	struct message msgstruct;
	char buff[MSG_LEN];
	char name[MSG_LEN];
	memset(fds, 0, sizeof(struct pollfd)* MAX_CLIENTS); // Cleaning memory
	fds[0].fd = sfd;
	fds[0].events = POLLIN;
	fds[0].revents = 0;
char ch9[MSG_LEN];
 char string[MSG_LEN];
  char ch10[MSG_LEN];
  char ch12[MSG_LEN];
	struct sockaddr_in cli;
	int connfd = -1;
	socklen_t len = sizeof(cli);
	struct liste_client *liste_cl=new_list();
	struct list_salon*list_salon=new_list_s();
	int i, j;
 int k;
 k=0;
 char ch11[MSG_LEN];
 char ch16[MSG_LEN];

struct client_in *client;
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
			  
			  //mettre  les client  dans une liste chainée
				struct client_in *cl =new_cl();
				struct sockaddr_in *client = (struct sockaddr_in *) &cli;
				*(cl->socket_fd) = connfd;
				*(cl->port) = htons(client->sin_port);
				char *add = inet_ntoa(client->sin_addr);
				strcpy(cl->addresse, add);
				strcpy(cl->nick_name,"");
				
				time_t times = time(NULL);
    			struct tm * periodeTime = localtime(&times); 
				char date[ch_len];
    			strftime(date,ch_len, "%d/%m/%Y %H:%M:%S", periodeTime);

				strcpy(cl->connect_time, date);

				insertion_client(liste_cl, cl);
				print_list(liste_cl);
			  
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
			
                   struct salon *sal;
			  // Cleaning memory
			  memset(&msgstruct, 0, sizeof(struct message));
			  memset(buff, 0, MSG_LEN);
			  //recupération de client 
			 
	      		 // Receiving structure
		if (recv(fds[i].fd, &msgstruct, sizeof(struct message), 0) <= 0) {
			break;
		}
		// Receiving message
		if (recv(fds[i].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}
		printf("pld_len: %i / nick_sender: %s / type: %s / infos: %s\n", msgstruct.pld_len, msgstruct.nick_sender, msg_type_str[msgstruct.type], msgstruct.infos);
		printf("Received: %s", buff);

			

		 struct  client_in *cli=get_client(liste_cl,fds[i].fd);
		 //traitement effectuer pour chaque comparaison
                  //pour le nick 
                  	if(strncmp(buff, "/nick ", 6) == 0){
               
		
		if(rech_exist(liste_cl,buff+6,fds[i].fd )){
		strcpy(msgstruct.infos, "déjà attribué");
		printf("\t already used rejected .\n");
		exit(EXIT_FAILURE);}

		
		
		else{strcpy(cli->nick_name, msgstruct.infos);
		printf("vous etes accepter nom  entrant pour  la premiere fois welcome");}
		
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
		char ch2[MSG_LEN];
		char ch3[MSG_LEN];
		msgstruct.pld_len=liste_cl->nbr_client*NICK_LEN;
		   struct client_in *temp = liste_cl->premier_client;

    while(temp != NULL){
        
		 strcat(ch2,temp->nick_name);
		 strcat(ch2," ");
                  temp=temp->next;}     
                  strcpy(ch3,ch2);  
                  printf("%s \n",ch3);  
                 strcpy(msgstruct.infos,"/who");
                 	
		// Sending structure (ECHO)
	if (send(fds[i].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[i].fd, ch3, strlen(ch3), 0) <= 0) {
			break;
		}
		
		
		
        }
        
       if(strncmp(buff,"/whois ",7)==0){
       char ch4[MSG_LEN];
       strcat(ch4," ");
       strcat(ch4,buff+7);
 struct client_in *temp = liste_cl->premier_client;
	 while(temp!=NULL)
	  {
		if(strcmp(temp->nick_name,buff+7)==0)
	
		
 {strcat(ch10,"                         ");
 strcat(ch10,ch4);
  strcat(ch10,"        connected since");
 strcat(ch10,temp->connect_time);
 strcat(ch10,"with IP address");
 strcat(ch10,temp->addresse);
 strcat(ch10,"and port number");
  itoa(*(temp->port),string,10);
 strcat(ch10,string);
strcpy(msgstruct.infos,ch10);

  break;
  }
  temp=temp->next;}
         	
		// Sending structure (ECHO)
	if (send(fds[i].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[i].fd, ch10, strlen(ch10), 0) <= 0) {
			break;
		}
		}
  
         	if(strncmp(buff,"/msgall ",8)==0){
		strcpy(ch9,buff+8);
		 struct client_in *temp = liste_cl->premier_client;
		while(temp!=NULL){
		if((fds[i].fd)!= *(temp->socket_fd)){
		
			// Sending structure (ECHO)
		if (send(*(temp->socket_fd), &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(*(temp->socket_fd), ch9, strlen(ch9), 0) <= 0) {
			break;
		}
		}temp=temp->next;}
		}
       if(strncmp(buff,"/msg ",5)==0){
        
        
		char ch7[MSG_LEN];
		int l=strlen(msgstruct.infos);
		strcat(msgstruct.infos,"\n");
		strcpy(ch7,buff + l + 5);
		printf("%s",ch7);
		
		struct client_in *temp = liste_cl->premier_client;
		while(temp!=NULL){
             
		if(strcmp(temp->nick_name,msgstruct.infos)==0){
		    
			// Sending structure (ECHO)
		if (send(*(temp->socket_fd), &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(*(temp->socket_fd), ch7, strlen(ch7), 0) <= 0) {
			break;
		}
             break;
		}temp=temp->next;}
		}
		if(strncmp(buff, "/create ", 8) == 0){
		
						if(verif_deja_attribuer(list_salon, msgstruct.infos)==0){
							strcpy(msgstruct.infos, "deja utilise");
							printf("deja utilisee le nom. de salon \n");
						}else{
							sal= salon_list_detache_client_by_fd(list_salon,fds[i].fd);
							if(sal!= NULL){
								printf("%s se dettache de chanel : %s\n", cli->nick_name, sal->nom);
								if(sal->membres->nbr_client == 0){
									if(salon_list_drop_salon(list_salon, sal) == -1){
										printf("dsl salon non trouve dans la liste \n");
										exit(EXIT_FAILURE);
									}
									printf("\tThe last client quit the channel. Channel deleted.\n");
									strcpy(ch11,"tu es le seul membre ,le channal a ete supprime");
									msgstruct.pld_len = (strlen(ch11)+1)*sizeof(char);
									strcpy(msgstruct.nick_sender,"Server");
									msgstruct.type = UNICAST_SEND;
									
										// Sending structure (ECHO)
	                                                if (send(fds[i].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
	                                                    		break;
                                                                         		}
		                                              // Sending message (ECHO)
		                                 if (send(fds[i].fd, ch11, strlen(ch11), 0) <= 0) {
		                                            	break;
		                                                           }
		
									
								}
							}
							
							sal = new_salon(msgstruct.infos, cli);
							sa_list_insertion(list_salon, sal);
							print_list_s(list_salon);
							printf("\tOperation accepted.\n");
	
		 }
		 
		 			// Sending structure (ECHO)
	                                                if (send(fds[i].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
	                                                    		break;
                                                                         		}
		                                              // Sending message (ECHO)
		                                 if (send(fds[i].fd, buff, msgstruct.pld_len, 0) <= 0) {
		                                            	break;
		                                                           }
	
		}
		  if(strncmp(buff,"/channel_list ",14) == 0){
		 strcpy(msgstruct.nick_sender, "Server");
		 
		// msgstruct.pld_len =list_salon->nombre*NICK_LEN ;
		  struct salon *temp = list_salon->premier_salon;

          while (temp != NULL)
        {
        strcat(ch12,temp->nom);
        //strcat(ch12," ");
        temp = temp->next;
           }
           printf("%s",ch12);
           
		 		// Sending structure (ECHO)
	if (send(fds[i].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(fds[i].fd, ch12,strlen(ch12), 0) <= 0) {
			break;
		}
		 
		  }
 if(strncmp(buff, "/join ", 6) == 0){


struct salon *temp = list_salon->premier_salon;
	 while(temp!=NULL)
	  {
		if(strcmp(temp->nom,buff+6)==0)
	
		
 {sal=temp;

  break;
  }
  temp=temp->next;}	
						if(sal == NULL){
							strcpy(msgstruct.infos, "");
							strcpy(msgstruct.nick_sender, "Server");
							
		 			// Sending structure (ECHO)
	                                                if (send(fds[i].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
	                                                    		break;
                                                                         		}
		                                              // Sending message (ECHO)
		                                 if (send(fds[i].fd, buff, msgstruct.pld_len, 0) <= 0) {
		                                            	break;
		                                                           }
		                                                           
							printf("\t\tpas de channel avec ce nom. votre demande de rejoin est bien rejeter\n");
						}else{
							struct salon *old_sal = salon_list_detache_client_by_fd(list_salon, fds[i].fd);
							if(old_sal != NULL){
								printf("\t\t%s was detached from the channel : %s\n", cli->nick_name, old_sal->nom);
								char msg[MSG_LEN];
								sprintf(msg, "Channel info : %s quits %s.\n", cli->nick_name, old_sal->nom);
								
								
								msgstruct.pld_len = sizeof(char)*(strlen(msg)+1);
	                                                         strcpy(msgstruct.nick_sender,"server");
	                                                             strcpy(msgstruct.infos, sal->nom);
	                                                              msgstruct.type = MULTICAST_SEND;
	                                                              struct client_in *cl_recep = sal->membres->premier_client;
                                                              	while(cl_recep!= NULL){
                                                              			
		 			// Sending structure (ECHO)
	                                                if (send(*(cl_recep->socket_fd), &msgstruct, sizeof(msgstruct), 0) <= 0) {
	                                                    		break;
                                                                         		}
		                                              // Sending message (ECHO)
		                                 if (send(*(cl_recep->socket_fd), msg, strlen(msg), 0) <= 0) {
		                                            	break;
		                                                           }
                                                              	

		                                     

		                                           cl_recep = cl_recep->next;
	                                                                       }
	                                                                       
	                                                                  
								if(old_sal->membres->nbr_client == 0){
									if(salon_list_drop_salon(list_salon, old_sal) == -1){
										fprintf(stderr, "Error : salon_list_drop_salon :Salon not in the list.\n");
										exit(EXIT_FAILURE);
									}
									printf("\t\tThe last client quit the channel. Channel deleted.\n");
									char msg[MSG_LEN];
								sprintf(msg, "You was the only member. Channel deleted. \n");
									
	msgstruct.pld_len = sizeof(char)*(strlen(msg)+1);
	strcpy(msgstruct.nick_sender,"Server");
	msgstruct.type = UNICAST_SEND;
			
		 			// Sending structure (ECHO)
	                                                if (send(fds[i].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
	                                                    		break;
                                                                         		}
		                                              // Sending message (ECHO)
		                                 if (send(fds[i].fd, buff, msgstruct.pld_len, 0) <= 0) {
		                                            	break;
		                                                           }
									
								}
							}
							
							insertion_client(sal->membres, cli);
							strcpy(msgstruct.nick_sender, "Server");
							                  // Sending structure (ECHO)
	                                            if (send(fds[i].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
	                                                    		break;
                                                                         		}
		                                              // Sending message (ECHO)
		                                 if (send(fds[i].fd, buff, msgstruct.pld_len, 0) <= 0) {
		                                            	break;
		                                                           }
							printf("\t\tJoin accepted\n");
							char msg[MSG_LEN];
							sprintf(msg, ">info : %s vient de  rejoindre %s. \n", cli->nick_name, sal->nom);
							msgstruct.pld_len = sizeof(char)*(strlen(msg)+1);
	                                                         strcpy(msgstruct.nick_sender,"server");
	                                                             strcpy(msgstruct.infos, sal->nom);
	                                                              msgstruct.type = MULTICAST_SEND;
	                                                             
	                                                              struct client_in *cl_recep = sal->membres->premier_client;
                                                              	while(cl_recep!= NULL){
                                                              	if((fds[i].fd)!= *(cl_recep->socket_fd)){
		
		      	// Sending structure (ECHO)
		if (send(*(cl_recep->socket_fd), &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(*(cl_recep->socket_fd), msg, strlen(msg), 0) <= 0) {
			break;
		}
		}cl_recep=cl_recep->next;}
 }}
  if(strncmp(buff, "/quit ", 6) == 0){
  sal = salon_list_detache_client_by_fd(list_salon, fds[i].fd);
						if(sal != NULL){
							printf(" %s il a été détaché de channel  : %s\n", cli->nick_name, sal->nom);
							char msg[MSG_LEN];
							sprintf(msg, ">info > %s a quitte %s.", cli->nick_name, sal->nom);
							msgstruct.pld_len = sizeof(char)*(strlen(msg)+1);
	                                                         strcpy(msgstruct.nick_sender,"server");
	                                                             strcpy(msgstruct.infos, sal->nom);
	                                                              msgstruct.type = MULTICAST_SEND;
	                                                             
	                                                              struct client_in *cl_recep = sal->membres->premier_client;
                                                              	while(cl_recep!= NULL){
                                                              	if((fds[i].fd)!= *(cl_recep->socket_fd)){
		
		      	// Sending structure (ECHO)
		if (send(*(cl_recep->socket_fd), &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(*(cl_recep->socket_fd), msg, strlen(msg), 0) <= 0) {
			break;
		}
		}cl_recep=cl_recep->next;}
						}else{
							printf("ce channel n'est pas  reconn vous etes rejeter \n");
						}

						if(sal->membres->nbr_client == 0){
							if(salon_list_drop_salon(list_salon, sal) == -1){
								printf("la salon n'existe pas déjà dans la liste");
								exit(EXIT_FAILURE);
							}
							
							/*char string[MSG_LEN];
						strcpy(string,"You were the last user in this channel, channel_name has been destroyed");
							msgstruct.pld_len = sizeof(char)*(strlen(string)+1);
	                            strcpy(msgstruct.nick_sender,"Server");
	                          // msgstruct.type = UNICAST_SEND;
	 		
		 		// Sending structure (ECHO)
	                                                if (send(fds[i].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
	                                                    		break;
                                                                         		}
		                                              // Sending message (ECHO)
		                                 if (send(fds[i].fd, string, strlen(string), 0) <= 0) {
		                                            	break;
		                                                           }*/
								
						}

  }

 /* if(strncmp(buff,"/multi ",7){
 char chaine[MSG_LEN];
 strcpy(chaine,buff+7);
  struct salon *temp = list_salon->premier_salon;
	 while(temp!=NULL)
	  {
		if(strcmp(temp->nom,msgstruct.infos)==0)
	
		
 {sal=temp;

  break;
  }
  temp=temp->next;}
   struct client_in *cl_recep = sal->membres->premier_client;
                                                              	while(cl_recep!= NULL){
                                                              	if((fds[i].fd)!= *(cl_recep->socket_fd)){
		
		      	// Sending structure (ECHO)
		if (send(*(cl_recep->socket_fd), &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(*(cl_recep->socket_fd), chaine,strlen(chaine), 0) <= 0) {
			break;
		}
		}cl_recep=cl_recep->next;}
  }
 */
 if(strncmp(buff, "/send ", 6) == 0){ 
 client= get_client_by_nickname(liste_cl,msgstruct.infos);
if(client==NULL){
                                                     msgstruct.type = FILE_REJECT;
							printf("echec de l'envoie pas d'utilisateur avec ce nom\n");
							strcpy(msgstruct.nick_sender, "Server");
							if (send(fds[i].fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;}
		
		// Sending message (ECHO)
		if (send(fds[i].fd, buff, msgstruct.pld_len, 0) <= 0) {
			break;
		}
		}else{printf("l'envoie à  %s est réussii.\n", msgstruct.infos);
		    
			// Sending structure (ECHO)
		if (send(*(client->socket_fd), &msgstruct, sizeof(msgstruct), 0) <= 0) {
			break;
		}
		// Sending message (ECHO)
		if (send(*(client->socket_fd), buff,msgstruct.pld_len, 0) <= 0) {
			break;
		}			// Sending structure (ECHO)*/
	
		
							
							}}
							if(strncmp(buff, "/echo ", 6) == 0){
							printf("received %s",buff+6);
							
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
			    memset(buff, 0, MSG_LEN);
				fds[i].revents = 0;
			  
			  
			  
			}	
		}
	}
	free_en_list(liste_cl);
	salon_list_free(list_salon);

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
if(argc!=2 ){
        perror("Oops!!vous avez des arguments qui manquent ");
        exit(-1);//on va sortir dans ce cas.
    }

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
