#include <sys/types.h> 	//socket creation
#include <sys/socket.h> //socket creation

#include <netdb.h>	//socket AF_INET
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <thread>

#define BUFFER 80
#define MAX_THREADS 10
using namespace std;

class ThreadMessageUDP {
public:
    ThreadMessageUDP(int socket): sc(socket){}
    ~ThreadMessageUDP(){}

    char message(){
        char input =' ';
        while(input != 'q'){
            char buffer[BUFFER]; 
            char input = ' ';

            struct sockaddr client;
            socklen_t clientSize = sizeof(struct sockaddr);

            char host[NI_MAXHOST];
            char serv[NI_MAXSERV];
            //Recieve data from the client
            int bytes = recvfrom(sc, (void *) buffer, BUFFER,0, &client, &clientSize);
            buffer[bytes] ='\0';//Last byte recieve
            if(bytes == -1){
                cout << "Error: While Reciving from Client \n";
                return -1;
            }
            getnameinfo(&client, clientSize, host, NI_MAXHOST, serv,NI_MAXSERV, NI_NUMERICHOST);
            cout << bytes << "bytes de " << host << ":" << serv << "\n";
            
            //If the client send something
            if(bytes >= 0){
                input = buffer[0];
                //Time
                char timeBuffer[BUFFER + 1]; 	

                time_t t = time(NULL);
                struct tm* time = localtime(&t);

                int timeSize = 0;
                //Procces the data
                if('t' == input)
                    timeSize = strftime(timeBuffer, BUFFER, "%r", time);
                else if('d' == input)
                    timeSize = strftime(timeBuffer, BUFFER, "%D", time);
                else if('q' == input)
                    cout << "Closing... \n";
                else 
                    cout << "Command not supported \n";

                if(timeSize > 0){
                    sendto(sc,timeBuffer,timeSize,0, &client, clientSize);
                }
            }
            else cout << "Command not supported \n";
        }
        return input;
    }
private:
    int sc;
};
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
    bind(sc, res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res);
    //-------------Threads-----------------------------
    thread pool[MAX_THREADS];

    for(int i = 0; i< MAX_THREADS ; i++){
        //Make the thread with lambda function
        ThreadMessageUDP *ms = new ThreadMessageUDP(sc);
        pool[i] = thread([&ms](){
            ms->message();
            delete ms;
        }).detach();
    }
    for(int i = 0; i< MAX_THREADS; i++){
        pool[i].join();
    }
	close(sc);
	return 0;
}


