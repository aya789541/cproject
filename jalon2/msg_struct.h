#define NICK_LEN 128
#define INFOS_LEN 128
#define MSG_LEN 1024
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
#include<stddef.h>

typedef struct listclient{
uint16_t port;
int socket;
char *addre;
char nickname[MSG_LEN];
char connection[MSG_LEN];
struct listclient * next;
}listclient,*List;

enum msg_type { 
	NICKNAME_NEW,
	NICKNAME_LIST,
	NICKNAME_INFOS,
	ECHO_SEND,
	UNICAST_SEND, 
	BROADCAST_SEND,
	MULTICAST_CREATE,
	MULTICAST_LIST,
	MULTICAST_JOIN,
	MULTICAST_SEND,
	MULTICAST_QUIT,
	FILE_REQUEST,
	FILE_ACCEPT,
	FILE_REJECT,
	FILE_SEND,
	FILE_ACK
};

struct message {
	int pld_len;
	char nick_sender[NICK_LEN];
	enum msg_type type;
	char infos[INFOS_LEN];
};

static char* msg_type_str[] = {
	"NICKNAME_NEW",
	"NICKNAME_LIST",
	"NICKNAME_INFOS",
	"ECHO_SEND",
	"UNICAST_SEND", 
	"BROADCAST_SEND",
	"MULTICAST_CREATE",
	"MULTICAST_LIST",
	"MULTICAST_JOIN",
	"MULTICAST_SEND",
	"MULTICAST_QUIT",
	"FILE_REQUEST",
	"FILE_ACCEPT",
	"FILE_REJECT",
	"FILE_SEND",
	"FILE_ACK"
};
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
