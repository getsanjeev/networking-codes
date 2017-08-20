#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

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
	int port_number = 6010;
	struct sockaddr_in server_socket, client_socket;	

	//initialising socket stucture
	server_socket.sin_family = AF_INET;
	server_socket.sin_addr.s_addr = INADDR_ANY;
	server_socket.sin_port = htons(port_number);

	cout<<"Welcome"<<endl;
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
				file_descriptor =  accept(file_descriptor, (struct sockaddr *)&client_socket, (socklen_t*)sizeof(client_socket));
				if(file_descriptor<0){
					cout<<"Error, failed to create dedicated sockets for client"<<endl;
				}
				else{
					status = connect(file_descriptor, (struct sockaddr *)&client_socket, sizeof(client_socket));
					if(status==0){
						cout<<"HEY! CONNECTED SUCCESSFULLY"<<endl;
					}
					else{
						cout<<"Error, failed to connect to client, Please try again"<<endl;
					}
				}
			}
		}
	}
}