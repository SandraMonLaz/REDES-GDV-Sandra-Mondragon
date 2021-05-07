#include <sys/types.h> 	//socket creation
#include <sys/socket.h> //socket creation

#include <netdb.h>	//socket AF_INET

#include <string.h>

using namespace std;
/*
 * args[0] -> P2.1_Ejericio1
 * args[1] -> host
 * args[2] -> port
 * */
int main(int argc, char* args[]){
	//To check if everything is okay
	int error = 0; 
	//-------------Configuration----------------
  	struct addrinfo hints;
	struct addrinfo* res;

	hints.ai_family = AF_UNSPEC;

	error = getaddrinfo(args[1],args[2],&hints,&res);
	if(error != 0){
		cout << "Error: Get Address Info \n";
		return -1;
	}
    //------------Conection Management-----------
    int socket = socket(res->ai_family, res->ai_socktype,0);
	if(socket == -1) 
	{
        cout << "Error: Creating Socket \n";
		return -1;
	}
    bind(socket, res->ai_addr, res->ai_addrlen);
    //-------------Loop-----------------------------
    char input ='';
    while(input != 'q'){
        char buffer[80]; 

        struct sockaddr client;
        socklent_t clientSize;

		char host[NI_MAXHOST];
		char serv[NI_MAXSERV];

        int bytes = recvfrom(socket, (void *) buffer, 80,0, &client, &clientSize);
        if(bytes == -1){
            cout << "Error: While Reciving from Client \n";
		    return -1;
        }
        getnameinfo(&client, &clientSize, host, NI_MAXHOST, serv,NI_MAXSERV, NI_NUMERICHOST);
    }
	// for(addrinfo* i = res; i!= NULL; i = i->ai_next){
		
	// 	getnameinfo(i->ai_addr, i->ai_addrlen, host, NI_MAXHOST, serv,NI_MAXSERV, NI_NUMERICHOST);

	// 	cout << "Host: " << host << << " Family: " << i->ai_family << " Socktype: " << i->ai_socktype << "\n";
	// }
	freeaddrinfo(res);
	return 0;

}
