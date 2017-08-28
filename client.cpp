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

string get_data_string(string file_name) //takes name of file as the input and returns a string that contains all the text
{
	ifstream file(file_name.c_str());
    string word;
    char x ;
    word.clear();
    int count  = 0;

    while ((x = file.get())!= EOF)
    {
        word = word + x;
    }
    file.close();
    return word;
}

void str2char(string data, char* destination){
	int size = data.size();
	char strarray[size];
	for(int i = 0;i < size;i++){
		destination[i] = (char)data[i];
	}	
}

int send(int file_descriptor,string message,int count){
	char buffer[2*count];	
	bzero(buffer,2*count);
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

void show_available_files(int file_descriptor){
	int n = send(file_descriptor,"ls",128);
	if(n<0){
		cout<<"error asking to show files"<<endl;
	}
	string list = recieve(file_descriptor,512);
	if(list.size()<1){
		cout<<"error reading list of files"<<endl;		
	}
	else{
		cout<<list<<endl;
	}
}

void put_file(int file_descriptor, string file_name){
	string content = get_data_string(file_name);
	int n = send(file_descriptor,string("put")+" "+file_name,256);
	if(n<0){
		cout<<"error sending file..."<<endl;
	}
	n = send(file_descriptor,content,1024);
		
	if(n<0){
		cout<<"error uploading file"<<endl;
	}	
}

void get_file(int file_descriptor,string file_name){
	int n = send(file_descriptor,string("get")+" "+file_name,256);
	string content = "";
	if(n<0){
		cout<<"error requesting file"<<endl;
	}
	else{				
	  	content = recieve(file_descriptor,1024);
	  	if(content.size()<1){
	  		cout<<"error in downloading file"<<endl;
	  	}
	  	else if(content == "No such files found"){
	  		cout<<content<<endl;
	  		cout<<"try ls to check what files are present"<<endl;
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

void show_menu(int file_descriptor){	
	int loop_invariant = 0;
	int choice = -1;	
	string file_name = "";
	while(loop_invariant == 0){
		cout<<"Enter command to 1. get list of files 2. download file 3. upload file 4. exit"<<endl;
		cin>>choice;
		switch(choice){
			case 1: show_available_files(file_descriptor);
						break;
			case 2:cout<<"enter the file name to download"<<endl;
							cin>>file_name;
							get_file(file_descriptor,file_name);
							break;
			case 3:cout<<"enter the file name to upload"<<endl;
							file_name.clear();
							cin>>file_name;
							put_file(file_descriptor,file_name);
							break;
			case 4: loop_invariant = 1;
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
   				show_menu(file_descriptor);
   			}
	}
}