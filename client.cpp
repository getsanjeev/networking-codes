#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
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
	return stri;
}


void get_file(int file_descriptor,string file_name){
	int n = send(file_descriptor,file_name,256);
	string content = "";
	if(n<0){
		cout<<"error requesting file"<<endl;
	}
	else{				
	  	content = recieve(file_descriptor,1024);
	  	if(content.size()<1){
	  		cout<<"error in downloading file"<<endl;
	  	}
	  	else{
	  		ofstream file;
	  		file.open(file_name.c_str());
	  		file << content;
  			file.close(); 
  			cout<<"download successful"<<endl;
	  	}  		
	}
}

int main(){
	int domain = AF_INET;
	int type = SOCK_STREAM;
	int protocol = 0;
	int status = 0;
	int file_descriptor = 0;
	int port_number = 5010;	
	struct sockaddr_in server_socket;   	

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
   			}
	}
}