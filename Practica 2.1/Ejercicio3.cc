#include <sys/types.h> 	//socket creation
#include <sys/socket.h> //socket creation

#include <netdb.h>	//socket AF_INET
#include <unistd.h>
#include <string.h>
#include <iostream>

#define BUFFER 80
using namespace std;

/*
 * args[0] -> Ejercicio3
 * args[1] -> host
 * args[2] -> port
 * args[3] -> option
 * */
int main(int argc, char* args[]){
	//To check if everything is okay
	int error = 0; 
	//-------------Configuration----------------
  	struct addrinfo hints;
	struct addrinfo* res;

    memset((void*) &hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

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
    char buffer[BUFFER + 1]; 

    error = sendto(sc, args[3], strlen(args[3]) + 1, 0, res->ai_addr, res->ai_addrlen);	
    if(error == -1){
        cout << "Error: Sending Data by Client \n";
		return -1;
    }

    error = recvfrom(sc, (void *) buffer, BUFFER,0, res->ai_addr, &res->ai_addrlen);
    if(error == -1){
        cout << "Error: Receving Data from Server\n";
		return -1;
    }
    buffer[error] = '\0';
    cout << buffer << "\n";
	freeaddrinfo(res);

	close(sc);
	return 0;
}


