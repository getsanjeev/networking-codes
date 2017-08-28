#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

using namespace std;

int main(){
	int domain = AF_INET;
	int type = SOCK_STREAM;
	int protocol = 0;
	int status = 0;
	int file_descriptor = 0;
	int port_number = 5010;	
	struct sockaddr_in server_socket;   
	char buffer[256];

	file_descriptor = socket(domain,type,protocol);
	if(file_descriptor < 0){
		cout<<"Error, failed to create sockets"<<endl;
	}
	else{		
   			bzero((char *) &server_socket, sizeof(server_socket));
   			server_socket.sin_family = AF_INET;   			
   			server_socket.sin_addr.s_addr = inet_addr("127.1.0.1");
   			server_socket.sin_port = htons(port_number);   			
   			status = connect(file_descriptor, (struct sockaddr*)&server_socket, sizeof(server_socket));
   			if(status < 0){
   				cout<<"Could not connect, please try again..."<<endl; 
   			}
   			else{
   				cout<<"status "<<status<<" ..........connected succesfully.........."<<endl;
   				while(1){  					   								
    				cin.getline( buffer, '\n');			    				
					int n = write(file_descriptor,buffer,256);
					if(n<0){
						cout<<"ERROR reading from socket"<<endl;						
					}
					bzero(buffer,256);
					n = read(file_descriptor,buffer,255);
					cout<<buffer<<endl;
					if(n<0){
						cout<<"Error writing to port"<<endl;
					}
   				}
   				
   			}
	}
}