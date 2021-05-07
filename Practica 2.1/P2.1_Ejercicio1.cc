#include <sys/types.h> 	//socket creation
#include <sys/socket.h> //socket creation

#include <netdb.h>	//socket AF_INET

#include <string.h>

using namespace std;
/*
 * args[0] -> P2.1_Ejericio1
 * args[1] -> host
 * */
int main(int argc, char* args[]){
	//To check if everything is okay
	int error = 0; 
	//-------------Configuration----------------
  	struct addrinfo hints;
	struct addrinfo* res;

	hints.ai_family = AF_UNSPEC;

	error = getaddrinfo(args[1],NULL,&hints,&res);
	if(error != 0){
		cout << "Error: Get Address Info \n";
		return -1;
	}
	for(addrinfo* i = res; i!= NULL; i = i->ai_next){
		char host[NI_MAXHOST];
		
		getnameinfo(i->ai_addr, i->ai_addrlen, host, NI_MAXHOST, serv,NI_MAXSERV, NI_NUMERICHOST);

		cout << "Host: " << host << << " Family: " << i->ai_family << " Socktype: " << i->ai_socktype << "\n";
	}
	freeaddrinfo(res);
	return 0;

}
