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

string remove_file(string name_file)
{
	int ret;
	const char* filename = name_file.c_str();
	ret = remove(filename);
	if(ret == 0)
	{
		cout<<"File deleted successfully"<<endl;cout<<endl;
	}
	else
	{
		cout<<"Error: unable to delete the file"<<endl;cout<<endl;
	}
}

void rename_file(string oldi, string newi)
{
	const char* oldname;
	const char* newname;
	oldname = oldi.c_str();
	newname = newi.c_str();
	int ret;
	ret = rename(oldname, newname);
	if(ret == 0)
	{
		cout<<"File renamed succesfully"<<endl;cout<<endl;
	}
	else
	{
		cout<<"Error renaming the file"<<endl;cout<<endl;
	}
}

string get_current_directory()
{
	char buff [1024];
	if (getcwd(buff,sizeof(buff))!=NULL);	
	string stri(buff);
	cout<<stri<<endl;
	return stri;	
}

string curtime()
{
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	return asctime(timeinfo);
	//cout<<"Current time and date: "<< asctime (timeinfo)<<endl ; // asctime() converts tm structure to string format
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


void map_command(vector <string> command_args,int file_descriptor){
	char buffer[1024];
	int n = -1;
	string reply;
	vector<string> file_name;
	if(command_args.at(0) == "ls"){
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
	else if(command_args.at(0) == "get"){
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
			string data = get_data_string(command_args.at(1));
			cout<<data<<endl;
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
	else if(command_args.at(0) == "put"){
		string incoming_file_name = command_args.at(1);
		ofstream file;
	  	file.open(incoming_file_name.c_str());	  	
	  	string content = recieve(file_descriptor,1024);
  		file << content;
  		file.close();
	}

	else if(command_args.at(0) == "delete"){
		string incoming_file_name = command_args.at(1);
		remove_file(incoming_file_name);
		//str2char("File deleted successfully.",buffer);
		//send(file_descriptor,buffer,30);
	}

	else if(command_args.at(0) == "rename"){
		string incoming_file_name_first = command_args.at(1);
		string incoming_file_name_second = command_args.at(2);
		rename_file(incoming_file_name_first,incoming_file_name_second);
		str2char("File renamed successfully.",buffer);
		send(file_descriptor,buffer,30);
	}

	else if(command_args.at(0) == "whereami"){
		string path = get_current_directory();
		str2char(path,buffer);
		send(file_descriptor,buffer,256);
	}

	else if(command_args.at(0) == "time"){		
		string time  = curtime();		
		str2char(time,buffer);
		send(file_descriptor,buffer,256);
	}

	else if(command_args.at(0) == "touch"){
		string incoming_file_name = command_args.at(1);
		ofstream file;
	  	file.open(incoming_file_name.c_str()); 	
	  	file.close();
	}
}

void ftpmode(int file_descriptor){
	char array [1024];	
	string command;
	while(1){
	command  = recieve(file_descriptor,256);
	cout<<command<<" :exact command is: "<<endl;
	vector<string> command_args =  tokenizer(strtrim(command));
	if(command_args.size() > 0){
		cout<<"command from client is : "<<command_args.at(0)<<" "<<command_args.at(1)<<endl;
		map_command(command_args,file_descriptor);
	}
	else{		
		send(file_descriptor,"invalid command",20);
	}

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
				new_file_descriptor =  accept(file_descriptor, (struct sockaddr *)&client_socket, &length_client);
				if(file_descriptor<0){
					cout<<"Error, failed to create dedicated sockets for client"<<endl;
				}
				else{					
						if(server_type == "ftp_server"){
							ftpmode(new_file_descriptor);							
						}					
				}				
			}
		}
		}
}	



int main(){	
	turn_on_server("ftp_server");	
}