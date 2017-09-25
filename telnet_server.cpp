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

string strtrim(string sentence)
{
	int i = 0;
	sentence = sentence.c_str();
	int j = sentence.size()-1;
	char x;
	while(i < j)
	{
		if(sentence[0] != ' ')
		{
			i = 0;
			break;
		}
		else if((sentence[i] == ' ') && (sentence[i+1] != ' '))
		{
			i = i+1;
			break;
		}
		else
		{
			i++;
		}
	}
	while(j>0)
	{
		if(sentence[sentence.size()-1] != ' ') break;
		else if((sentence[j] == ' ') && (sentence[j-1]!=' '))
		{
			j = j-1;
			break;
		}
		else j--;
	}
	string trm_str;
	trm_str.clear();
	for(int k = i; k<=j; k++)
	{
		x = sentence[k];
		trm_str = trm_str + x;
	}
	return trm_str;
}

vector<int> get_white_spaces(string sentence)
{
	vector<int> space_index(100);
	char x;
	int i = 0;
	int j,k;
	k = 0;
	int diff = 0;
	space_index.at(k) = 0;
	k++;
	while(i<sentence.size())
	{
		if(sentence[i] == ' ')
		{
			space_index.at(k) = i-1;
			k++;
			j = i;
			while(j<sentence.size()-1)
			{
				if((sentence[j]==' ')&&(sentence[j+1]!=' '))
				{
					space_index.at(k) = j+1;
					k++;
					break;
				}
				else j++;
			}
			diff = j-i;
			i = j+1;
		}
		else i++;
	}
space_index.at(k) = sentence.size()-1;
return space_index;
}


vector<string> tokenizer(string sentence)
{
	int p;
	sentence = strtrim(sentence);
	vector<string> tokens (100);
	vector<int> index_vector = get_white_spaces(sentence);
	int pre,post,length,i;
	i= 0;
	length = 0;
	string temp;
	while(i < index_vector.size())
	{
		if((i ==0)||(index_vector.at(i) !=0))
		{
			temp.clear();
			pre = index_vector.at(i);
			post = index_vector.at(i+1);
			for(int j = pre;j<=post;j++)
			{
				temp = temp + sentence[j];
			}
			tokens.at(length) = temp;
			length++;
		}
		i = i+2;
	}
	return tokens;
}




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


void sum_telnet(int file_descriptor){
	cout<<"sum calculator"<<endl;
	int p = 0;
	int sum = 0;
	stringstream stream;		
	stream.str("");
	stream.clear();
	string ans;
	char buffer[1024];	
	ans.clear();
	ans = recieve(file_descriptor,100);
	//ans = "1 2 3 4 5 6 7 8 9";
	vector<string> numbers = tokenizer(ans);		
	int len = atoi(buffer);
	cout<<"size of vec"<<atoi(buffer)<<endl;
	for(int i = 0;i<2;i++){		
		bzero(buffer,1024);		
		str2char(numbers.at(i),buffer);
		cout<<atoi(buffer)<<endl;
		sum = sum + atoi(buffer);						   							
	}		
	cout<<"Sending "<<sum<<endl;
	stream << sum;
	send(file_descriptor,stream.str().c_str(),256);
}

void telnetmode(int file_descriptor){	
	sum_telnet(file_descriptor);
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
	
	file_descriptor = socket(domain,type,protocol);
	cout<<"hhhhh"<<endl;
	if(file_descriptor < 0){
		cout<<"Error, failed to create sockets"<<endl;
	}
	else{
		cout<<"BInding"<<endl;
		status =  bind(file_descriptor, (struct sockaddr *)&server_socket, sizeof(server_socket));
		if(status < 0){
			cout<<"Error, failed to bind to address"<<endl;
		}
		else{
			cout<<"listening"<<endl;
			status = listen(file_descriptor, 10);
			if(status != 0){
				cout<<"Error, failed to get to listen mode"<<endl;
			}
			else{	
			cout<<"waiting ot accept"<<endl;
				new_file_descriptor =  accept(file_descriptor, (struct sockaddr *)&client_socket, &length_client);
				if(new_file_descriptor<0){
					cout<<"Error, failed to create dedicated sockets for client"<<endl;
				}
				else{
					cout<<"HEY! CONNECTED SUCCESSFULLY"<<endl;										
					if(server_type == "telnet_server"){
						telnetmode(new_file_descriptor);
					}						
				}				
			}				
		}
	}
}



int main(){
	turn_on_server("telnet_server");
}