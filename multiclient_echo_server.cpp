#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <sys/dir.h>
#include <sstream>
#include <sys/param.h>
#include <fstream>

using namespace std;


void str2char(string data, char* destination){
	int size = data.size();
	char strarray[size];
	for(int i = 0;i < size;i++){
		destination[i] = (char)data[i];
	}	
}

int send(int file_descriptor,string message,int count){
	char buffer[1024];	
	str2char(message,buffer);
	int n = -1;	
	n = write(file_descriptor,buffer,count);
	if(n<0){
		cout<<"Error writing to port"<<endl;
		}
	return n;
}

string recieve(int file_descriptor, int count){
	char buffer[2*count];
	bzero(buffer,2*count);
	int n = read(file_descriptor,buffer,count);
	if(n<0){
		cout<<"ERROR reading from socket"<<endl;
	}	
	string stri(buffer);
	cout<<"from recieve function "<<buffer<<endl;
	return stri;
}



void turn_on_server(string server_type){
	int domain = AF_INET;
	int type = SOCK_STREAM;
	int protocol = 0;
	int status = 0;
	int file_descriptor = 0,new_file_descriptor =0;
	int port_number = 5010;
	struct sockaddr_in server_socket, client_socket;	
	socklen_t length_client;	
	int pid;
	string buffer;

	bzero((char *) &server_socket, sizeof(server_socket));
	bzero((char *) &client_socket, sizeof(client_socket));

	//initialising socket stucture
	server_socket.sin_family = AF_INET;
	server_socket.sin_addr.s_addr = inet_addr("127.1.0.1");
	server_socket.sin_port = htons(port_number);
	length_client = sizeof(client_socket);

	cout<<"Welcome"<<server_socket.sin_addr.s_addr<<" "<<server_socket.sin_port<<endl;
	file_descriptor = socket(domain,type,protocol);
	if(file_descriptor < 0){
		cout<<"Error, failed to create sockets"<<endl;
	}
	else{
		status =  bind(file_descriptor, (struct sockaddr *)&server_socket, sizeof(server_socket));
		if(status < 0){
			cout<<"Error, failed to bind to address"<<endl;
		}
		else{
			status = listen(file_descriptor, 10);
			if(status != 0){
				cout<<"Error, failed to get to listen mode"<<endl;
			}
			else{
				cout<<"Server has started....Please make requests to connect."<<endl;								
				while(1){	
				new_file_descriptor =  accept(file_descriptor, (struct sockaddr *)&client_socket, &length_client);
				if(file_descriptor<0){
					cout<<"Error, failed to create dedicated sockets for client"<<endl;
				}
				else{
					cout<<"...................CONNECTED SUCCESSFULLY...................."<<endl;
					pid = fork();
					if(pid<0){						
						cout<<"Unable to create new process for new client..."<<endl;
					}
					if(pid==0){
						close(file_descriptor);
						if(server_type == "echo_server"){
							string reply = recieve(new_file_descriptor,256);
							cout<<reply<<endl;
							int n = send(new_file_descriptor,"hello from server",18);
							if(n<0){
								cout<<"Error sending the message"<<endl;
							}
							break;			
						}						
					}
					else{
						close(new_file_descriptor);
					}
				}

				}				
			}
		}
	}
}



int main(){
	turn_on_server("echo_server");
}