Zip file contains four different server-client pairs:
1. Echo server : A simple server which responds with same message everytime to client.
2. Telnet server :  A server which computes sum of numbers send by client. This type of servers are used when client does not have enough resources to execute certain programs.
3. Chat server :  A server in which two computers can chat for while through direct sockets. This is a slight modification of echo server.
4. FTP server : A server which allows client to perform following functions on server : 
	a) Upload a file
	b) download a file
	c) get list of files in server
	d) get current working directory
	e) get current time
	f) create a new file (touch in ubuntu)
	g) delete a file (not a folder which can be easily done through recursion on folder)



------------------- DESCRIPTION OF IMPORTANT FUNCTIONS -----------------------

1. string get_data_string(string file_name) // converts file contents into a string
2. void str2char(string data, char* destination){ //converts string into char array
3. int send(int file_descriptor,string message,int count){ //send data to given fle descriptor of UNIX
4. string recieve(int file_descriptor, int count){ //returns string read on given file descriptor
5. void show_available_files(int file_descriptor){ //calls ls system call internally and sens list of iles available in current directory
6. void put_file(int file_descriptor, string file_name){ //puts a new file uploaded by client on server
7. void get_file(int file_descriptor,string file_name){ //called by client to download from server
8. void show_menu(int file_descriptor){	// a menu driven program function to help user interct with FTP
9.  string remove_file(string name_file) // removes file form server on client demand
10. void rename_file(string oldi, string newi) //renames a file on server
11. vector<int> get_white_spaces(string sentence) //returns a vector as index of white spaces , called by tokeniser
12. vector<string> tokenizer(string sentence) / tokenize a given string
