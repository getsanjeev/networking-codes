#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <sys/dir.h>
#include <sys/param.h>
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

vector<string> ls()
{
	int count,i;
	struct dirent **files;
	int file_select(const struct dirent*);
	char pathname[MAXPATHLEN];
	if (getcwd(pathname, sizeof(pathname)) == NULL)
	{
		cout<<"Error getting path"<<endl;
	}
	count = scandir(pathname, &files, file_select, alphasort);
	vector<string> filename(count);
	if (count <= 0)
	{
		cout<<"No files in this directory"<<endl;
	}
	else
	{
		for (i=1;i<count+1;++i)
		{
			cout<<files[i-1]->d_name<<" ";
			filename.at(i-1) = files[i-1]->d_name;
		}
	}
	return filename;
}

int file_select(const struct dirent *entry)
{
	if ((entry->d_name == ".") ||(entry->d_name == "..")){
		return (0);
	}
	
	else
	return (1);
}

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
	vector<int> space_index(20);
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
	vector<string> tokens (10);
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


void map_command(vector <string> command_args,int file_descriptor){
	char buffer[1024];
	int n = -1;
	string reply;
	vector<string> file_name;
	if(command_args[0] == "ls"){
		file_name = ls();				
		string msg = "";
		for(int i=0;i<file_name.size();i++){
			msg = msg + " " + file_name.at(i);			
		}
		str2char(msg,buffer);
		n = send(file_descriptor,buffer,msg.size());					
		if(n<0){
			cout<<"Error sending file description"<<endl;
		}
	}
	else if(command_args[0] == "get"){
		file_name = ls();
		int flag = 0;
		for(int i =0;i<file_name.size();i++){
			if(command_args[1] == file_name.at(i)){
				flag = 1;
			}
		}
		if(flag==0){
			cout<<"No such files found"<<endl;
		}
		else{
			string data = get_data_string(command_args[1]);
			char temp_buffer [data.size()+10];
			str2char(data,temp_buffer);
			n = send(file_descriptor,temp_buffer,data.size());
			if(n<0){
				cout<<"error writing to ports"<<endl;
			}
			else{
				cout<<"file sent successfully"<<endl;
			}
		}
	}
	else if(command_args[0] == "put"){
		string incoming_file_name = command_args[1];
		ofstream file;
	  	file.open(incoming_file_name.c_str());
	  	string content = recieve(file_descriptor,1024);
  		file << content;
  		file.close();
	}
}


void ftpmode(int file_descriptor){
	char array [1024];	
	string command;	
	command  = recieve(file_descriptor,256);
	vector<string> command_args =  tokenizer(strtrim(command));
	if(command_args.size() > 0){
		//cout<<"command form client is : "<<command_args.at(0)<<endl;
		map_command(command_args,file_descriptor);
	}
	else{		
		send(file_descriptor,"invalid command",20);
	}
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
					cout<<"HEY! CONNECTED SUCCESSFULLY"<<endl;
					cout<<client_socket.sin_addr.s_addr<<endl;
					pid = fork();
					if(pid<0){						
						cout<<"Unable to create new process for new client..."<<endl;
					}
					if(pid==0){
						close(file_descriptor);
						if(server_type == "echo_server"){
							getline(cin, buffer);
							int n = send(new_file_descriptor,buffer,buffer.size());
							if(n<0){
								cout<<"Error sending the message"<<endl;
							}
							string reply = recieve(new_file_descriptor,256);
							cout<<reply<<endl;
						}
						else if(server_type == "ftp_server"){
							ftpmode(new_file_descriptor);
						}
						break;
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
	//turn_on_server("echo_server");	
	turn_on_server("ftp_server");	
}