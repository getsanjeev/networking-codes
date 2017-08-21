#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

/*struct in_addr {
   unsigned long s_addr;
};
*/
/*struct sockaddr_in {
   short int            sin_family;
   unsigned short int   sin_port;
   struct in_addr       sin_addr;
   unsigned char        sin_zero[8];
};*/

int main(){

	int domain = AF_INET;
	int type = SOCK_STREAM;
	int protocol = 0;
	int status = 0;
	int file_descriptor = 0;
	int port_number = 5010;
	struct sockaddr_in server_socket, client_socket;	
	socklen_t length_client;
	char buffer[256];

	bzero((char *) &server_socket, sizeof(server_socket));
	bzero((char *) &client_socket, sizeof(client_socket));

	//initialising socket stucture
	server_socket.sin_family = AF_INET;
	server_socket.sin_addr.s_addr = inet_addr("127.0.0.1");
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
				file_descriptor =  accept(file_descriptor, (struct sockaddr *)&client_socket, &length_client);
				if(file_descriptor<0){
					cout<<"Error, failed to create dedicated sockets for client"<<endl;
				}
				else{																
					cout<<"HEY! CONNECTED SUCCESSFULLY"<<endl;
					cout<<client_socket.sin_addr.s_addr<<endl;
					while(1){
						bzero(buffer,256);
						int n = read( file_descriptor,buffer,255);
						cout<<buffer;
						if(n<0){
							cout<<"ERROR reading from socket"<<endl;						
						}
						n = write(file_descriptor,"hey sherlock",12);
						if(n<0){
							cout<<"Error writing to port"<<endl;
						}

					}				
				}
			}
		}
	}
}