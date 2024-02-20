
#include "common.h"
 struct client_in{

    int *socket_fd;
    char *addresse;
    int *port;

    char *nick_name;

    char *connect_time;
    
    struct client_in *next;
    
};

typedef struct liste_client{
    struct client_in *premier_client;

     int nbr_client;
}liste_client;
struct salon{

    liste_client *membres;

     char *nom;
    struct salon *next;
    
};

typedef struct list_salon{
    struct salon *premier_salon;

   int nombre;
} list_s;

struct fichier{
 
    int  etat_de_progression; 
    char nom[MSG_LEN]; 
    int fichier_fd;
  
    struct client_in destination;

    struct fichier *next;
};

struct liste_f{
    struct fichier *premier;

     int nbr;
};
