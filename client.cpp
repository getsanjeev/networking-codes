#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

using namespace std;

int main(){
	int domain = AF_INET;
	int type = SOCK_STREAM;
	int protocol = 0;
	int status = 0;
	int file_descriptor = 0;
	int port_number = 6010;	
	struct sockaddr_in client_socket;
   	struct hostent *server;   	

	file_descriptor = socket(domain,type,protocol);
	if(file_descriptor < 0){
		cout<<"Error, failed to create sockets"<<endl;
	}
	else{
		server = gethostbyname("localhost");
		if (server == NULL) {
    	  	cout<<"No such host available"<<endl;      		
   		}
   		else{
   			bzero((char *) &client_socket, sizeof(client_socket));
   			client_socket.sin_family = AF_INET;
   			bcopy((char *)server->h_addr, (char *)&client_socket.sin_addr.s_addr, server->h_length);
   			client_socket.sin_port = htons(port_number);   			
   			status = connect(file_descriptor, (struct sockaddr*)&client_socket, sizeof(client_socket));
   			if(status < 0){
   				cout<<"Could not connect, please try again..."<<endl; 
   			}
   			else{
   				
   			}   				

   		}

	}
}