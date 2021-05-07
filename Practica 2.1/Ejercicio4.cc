#include <sys/types.h> 	//socket creation
#include <sys/socket.h> //socket creation

#include <netdb.h>	//socket AF_INET
#include <unistd.h>
#include <string.h>
#include <iostream>

#define BUFFER 80
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

    memset((void*) &hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	error = getaddrinfo(args[1], args[2], &hints, &res);
	if(error != 0){
		cout << "Error: Get Address Info \n";
		return -1;
	}
    //------------Conection Management-----------
    int sc = socket(res->ai_family, res->ai_socktype,0);
	if(sc == -1) 
	{
        cout << "Error: Creating Socket \n";
		return -1;
	}
    bind(sc, res->ai_addr, res->ai_addrlen);
    //-------------TCP Management-------------------
    listen(sc, 16);

    struct sockaddr client;
    socklen_t clientSize = sizeof(struct sockaddr);

    int clientSd = accept(sc,&client, &clientSize);

    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo(&client, clientSize, host, NI_MAXHOST, serv,NI_MAXSERV, NI_NUMERICHOST);
    cout << "Conection from " << host << " " << serv << "\n";
    //-------------Loop-----------------------------
    while(true){
        char buffer[BUFFER + 1]; 

        //Recieve data from the client
        int bytes = recv(clientSd, (void *) buffer, BUFFER,0);

		buffer[bytes] ='\0';//Last byte recieve

        if( bytes == -1 ){
            cout << "Error: While Reciving from Client \n";
		    return -1;
        }
        if( bytes == 0 || buffer[0] == 'Q'){
            cout << "End of conection \n";
            break;
        }
        send(clientSd,buffer,bytes,0);
    }
	close(sc);
	freeaddrinfo(res);
	return 0;
}


