#include <sys/types.h> 	//socket creation
#include <sys/socket.h> //socket creation

#include <netdb.h>	//socket AF_INET
#include <sys/types.h> 	//socket AF_iFET

using namespace std;
/*
 * argsv[0] -> port
 * argsv[1] -> host
 * */
int main(int args, void* argsv){
	//To check if everything is okay
	int error = 0; 
	//Create the socket
  	int socket = socket(AF_INET, SOCK_STREAM,0);
	if(socket == -1) 
	{
		perror();
		return -1;
	}
	struct addrinfo hints;
	struct addrinfo* res;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	error = getaddrinfo(argsv[1],argsv[0],&hints,&res);
	if(error != 0){
		perror();
		return -1;
	}

	for(auto i = res; i!= nullptr; i->ai_next){
		char host[NI_MAXHOST];
		char serv[NI_MAXSERV];
		
		getnameinfo(i->ai_addr, i->ai_addrlen, host, NI_MAXHOST, serv,NI_MAXSERV,NI_NUMERICHOST | NI_NUMERISERV);

		cout << "Host: " << host << "Port: " << serv << "\n";
	}
	freeaddrinfo(res);
	return 0;

}


