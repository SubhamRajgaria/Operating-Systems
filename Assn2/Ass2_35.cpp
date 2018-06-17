#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <fcntl.h>
#include <boost/algorithm/string.hpp>

using namespace std;

#define MAXSIZE 10000

vector <string> tokenize(string str){
	vector <string> tokens;
	if(str.find('|')==string::npos) tokens.push_back(str);
	else{
		for (int i=0; i<str.size(); i++){
			if(str[i] != '|'){
				string token = str.substr(i, str.find('|',i));
				tokens.push_back(token);
				i= str.find('|',i);
			}
		}
	}
	return tokens;
}

int main(){
	pid_t x;
	int child_count;
	//locale loc;
	string choice, command;
	cout << "A. Run an internal command" << endl;
	cout << "B. Run an external command" << endl;
	cout << "C. Run an external command by redirecting standard input from a file" << endl;
	cout << "D. Run an external command by redirecting standard output from a file" << endl;
	cout << "E. Run an external command in the background" << endl;
	cout << "F. Run several external commands in the pipe mode" << endl;
	cout << "G. Quit the shell" << endl;
	cout << "Enter your choice" << endl;
	cin >> choice;
	getchar();
	while(1){
		if(choice == "a" || choice=="A"){
			cout << "Enter internal command to execute" <<endl;
			getline(cin, command);
			istringstream args(command);
			string arg;
			char *arg_list[MAXSIZE];
			int i=0;
			while(args >> arg){
				arg_list[i]= new char[arg.size()+1];
				copy(arg.begin(), arg.end(), arg_list[i]);
				arg_list[i][arg.size()]='\0';
				i++;
			}
			arg_list[i]= NULL;
			if(strcmp(arg_list[0],"cd")==0 || strcmp(arg_list[0],"chdir")==0){
				chdir(arg_list[1]);
				cout << "A. Run an internal command" << endl;
				cout << "B. Run an external command" << endl;
				cout << "C. Run an external command by redirecting standard input from a file" << endl;
				cout << "D. Run an external command by redirecting standard output from a file" << endl;
				cout << "E. Run an external command in the background" << endl;
				cout << "F. Run several external commands in the pipe mode" << endl;
				cout << "G. Quit the shell" << endl;
				cout << "Enter your choice" << endl;
				cin >> choice;
				getchar();

			}
			else if(strcmp(arg_list[0],"echo")==0){
				string echo = "echo ";
				string cmd = echo+arg_list[1];
				system(cmd.c_str());
				cout << "A. Run an internal command" << endl;
				cout << "B. Run an external command" << endl;
				cout << "C. Run an external command by redirecting standard input from a file" << endl;
				cout << "D. Run an external command by redirecting standard output from a file" << endl;
				cout << "E. Run an external command in the background" << endl;
				cout << "F. Run several external commands in the pipe mode" << endl;
				cout << "G. Quit the shell" << endl;
				cout << "Enter your choice" << endl;
				cin >> choice;
				getchar();
			}
			else{
				x = fork();
				if(x==0){
					execvp(arg_list[0],arg_list);
				}
				else{
					wait(NULL);
					cout << "A. Run an internal command" << endl;
					cout << "B. Run an external command" << endl;
					cout << "C. Run an external command by redirecting standard input from a file" << endl;
					cout << "D. Run an external command by redirecting standard output from a file" << endl;
					cout << "E. Run an external command in the background" << endl;
					cout << "F. Run several external commands in the pipe mode" << endl;
					cout << "G. Quit the shell" << endl;
					cout << "Enter your choice" << endl;
					cin >> choice;
					getchar();
				}
			}
		}
		else if(choice == "b" || choice=="B"){
			x = fork();
			if(x==0){
				cout << "Enter external command to execute" <<endl;
				getline(cin, command);
				istringstream args(command);
				string arg;
				char *arg_list[MAXSIZE];
				int i=0;
				while(args >> arg){
					arg_list[i]= new char[arg.size()+1];
					copy(arg.begin(), arg.end(), arg_list[i]);
					arg_list[i][arg.size()]='\0';
					i++;
				}
				arg_list[i]= NULL;
				//const char* inp = str.c_str();
				//const char* arg = args.c_str();
				execvp(arg_list[0],arg_list);
			}
			else{
				wait(NULL);
				cout << "A. Run an internal command" << endl;
				cout << "B. Run an external command" << endl;
				cout << "C. Run an external command by redirecting standard input from a file" << endl;
				cout << "D. Run an external command by redirecting standard output from a file" << endl;
				cout << "E. Run an external command in the background" << endl;
				cout << "F. Run several external commands in the pipe mode" << endl;
				cout << "G. Quit the shell" << endl;
				cout << "Enter your choice" << endl;
				cin >> choice;
				getchar();
			}
		}
		else if(choice == "c"|| choice=="C"){
			x=fork();
			if(x==0){
				cout << "Enter external command rediecting input from a file" <<endl;
				getline(cin, command);
				istringstream args(command);
				string arg;
				char *arg_list[MAXSIZE];
				int i=0;
				int pos=0;
				while(args >> arg){
					if(arg=="<")
						pos=i;
					arg_list[i]= new char[arg.size()+1];
					copy(arg.begin(), arg.end(), arg_list[i]);
					arg_list[i][arg.size()]='\0';
					i++;
				}
				cout << pos << endl;
				arg_list[i]= NULL;
				int fd = open(arg_list[pos+1],O_RDONLY);
				close(0);
				cout << fd << endl;
				int in = dup(fd);
				//const char* inp = str.c_str();
				//const char* arg = args.c_str();
				//int fd = open(arg_list[pos+1],O_RDONLY);
				execvp(arg_list[0],arg_list);
				close(fd);
			}
			else{
				wait(NULL);
				cout << "A. Run an internal command" << endl;
				cout << "B. Run an external command" << endl;
				cout << "C. Run an external command by redirecting standard input from a file" << endl;
				cout << "D. Run an external command by redirecting standard output from a file" << endl;
				cout << "E. Run an external command in the background" << endl;
				cout << "F. Run several external commands in the pipe mode" << endl;
				cout << "G. Quit the shell" << endl;
				cout << "Enter your choice" << endl;
				cin >> choice;
				getchar();
			}
		}
		else if(choice == "d"|| choice=="D"){
			x = fork();
			if(x==0){
				cout << "Enter external command to execute and redirect output" <<endl;
				getline(cin, command);
				istringstream args(command);
				string arg;
				char *arg_list[MAXSIZE];
				int i=0;
				int pos =0;
				while(args >> arg){
					cout << arg << endl;
					if(arg==">")
						pos=i;
					arg_list[i]= new char[arg.size()+1];
					copy(arg.begin(), arg.end(), arg_list[i]);
					arg_list[i][arg.size()]='\0';
					i++;
				}
				cout << pos<<endl;
				arg_list[i]= NULL;
				int fd = creat(arg_list[pos+1], 0666);
				close(1);
				cout << fd << endl;
				int out = dup(fd);
				//dup2(fd,1);
				//close(1);
				//dup(fd);
				//const char* inp = str.c_str();
				//const char* arg = args.c_str();
				execvp(arg_list[0],arg_list);
				close(fd);
				
			}
			else{
				wait(NULL);
				cout << "A. Run an internal command" << endl;
				cout << "B. Run an external command" << endl;
				cout << "C. Run an external command by redirecting standard input from a file" << endl;
				cout << "D. Run an external command by redirecting standard output from a file" << endl;
				cout << "E. Run an external command in the background" << endl;
				cout << "F. Run several external commands in the pipe mode" << endl;
				cout << "G. Quit the shell" << endl;
				cout << "Enter your choice" << endl;
				cin >> choice;
				getchar();
			}
		}
		else if(choice == "e"|| choice=="E"){
			cout << "Enter external command to execute in background" <<endl;
			getline(cin, command);
			istringstream args(command);
			string arg;
			char *arg_list[MAXSIZE];
			int i=0;
			while(args >> arg){
				arg_list[i]= new char[arg.size()+1];
				copy(arg.begin(), arg.end(), arg_list[i]);
				arg_list[i][arg.size()]='\0';
				i++;
			}
			arg_list[i]= NULL;
			//const char* inp = str.c_str();
			//const char* arg = args.c_str();
			x = fork();
			if(x==0){
				execvp(arg_list[0],arg_list);
			}
			else{
				//wait(NULL);
				cout << "A. Run an internal command" << endl;
				cout << "B. Run an external command" << endl;
				cout << "C. Run an external command by redirecting standard input from a file" << endl;
				cout << "D. Run an external command by redirecting standard output from a file" << endl;
				cout << "E. Run an external command in the background" << endl;
				cout << "F. Run several external commands in the pipe mode" << endl;
				cout << "G. Quit the shell" << endl;
				cout << "Enter your choice" << endl;
				cin >> choice;
				getchar();
			}
		}
		else if(choice == "f"|| choice=="F"){
			cout << "Enter the commands "<< endl;
			getline(cin, command);
			vector<string> commands;
			boost::split(commands, command, [](char c){return c=='|';});
			int pipes[commands.size()-1][2];
			int i,in,out;
			for(i=1; i< commands.size(); i++){
				pipe(pipes[i-1]);
				//cout << commands[i-1]<< endl;
			}
			//cout << commands[i-1]<< endl;
			int count =1;
			while(count <= commands.size()){
				x= fork();
				if(x==0 && count==1){
					cout << "First command execution"<<endl;
					cout << "Count value in child = " << count << endl;
					istringstream args(commands[count-1]);
					string arg;
					char *arg_list[MAXSIZE];
					int i=0;
					while(args >> arg){
						arg_list[i]= new char[arg.size()+1];
						copy(arg.begin(), arg.end(), arg_list[i]);
						arg_list[i][arg.size()]='\0';
						i++;
					}
					arg_list[i]= NULL;
					//const char* inp = str.c_str();
					//const char* arg = args.c_str();
					close(1);
					out = dup(pipes[count-1][1]);
					//count++;
					execvp(arg_list[0],arg_list);
					close(out);
				}
				else if(x==0 && count == commands.size()){
					cout << " Last command execution" << endl;
					close(0);	
					in = dup(pipes[count-2][0]);
					cout << "Count value in child = " << count << endl;
					istringstream args(commands[count-1]);
					string arg;
					char *arg_list[MAXSIZE];
					int i=0;
					while(args >> arg){
						arg_list[i]= new char[arg.size()+1];
						copy(arg.begin(), arg.end(), arg_list[i]);
						arg_list[i][arg.size()]='\0';
						i++;
					}
					arg_list[i]= NULL;
					//close(in);
					//close(out);
					//close(1);
					//int out = dup(pipes[count-1][1]);
					//count++;
					execvp(arg_list[0],arg_list);
				}
				else if(x==0 && count >1){
					char* ch[100];
					close(0);
					in = dup(pipes[count-2][0]);
					cout << "Count value in child = " << count << endl;
					//read(pipes[count-2][0],ch,100);
					//for(int j=0; j< 100; j++)
					//	cout << ch[j] << endl;
					istringstream args(commands[count-1]);
					string arg;
					char *arg_list[MAXSIZE];
					int i=0;
					while(args >> arg){
						arg_list[i]= new char[arg.size()+1];
						copy(arg.begin(), arg.end(), arg_list[i]);
						arg_list[i][arg.size()]='\0';
						i++;
					}
					arg_list[i]= NULL;
					//close(in);
					close(out);
					close(1);
					out = dup(pipes[count-1][1]);
					//count++;
					execvp(arg_list[0],arg_list);
					close(out);
				}
				else if(x!=0 && count==commands.size()){
					//wait(NULL);
					//wait(NULL);
					close(pipes[count-1][1]);
					for(int i=0; i<commands.size();i++)
						wait(NULL);
					cout << "Count value = "<< count << endl;

					count++;
					cout << "A. Run an internal command" << endl;
					cout << "B. Run an external command" << endl;
					cout << "C. Run an external command by redirecting standard input from a file" << endl;
					cout << "D. Run an external command by redirecting standard output from a file" << endl;
					cout << "E. Run an external command in the background" << endl;
					cout << "F. Run several external commands in the pipe mode" << endl;
					cout << "G. Quit the shell" << endl;
					cout << "Enter your choice" << endl;
					cin >> choice;
					getchar();
				}
				else{
					//wait(NULL);
					//wait(NULL);
					cout << "Count value =" << count << endl;
					close(pipes[count-1][1]);
					count++;
					/*cout << "A. Run an internal command" << endl;
					cout << "B. Run an external command" << endl;
					cout << "C. Run an external command by redirecting standard input from a file" << endl;
					cout << "D. Run an external command by redirecting standard output from a file" << endl;
					cout << "E. Run an external command in the background" << endl;
					cout << "F. Run several external commands in the pipe mode" << endl;
					cout << "G. Quit the shell" << endl;
					cout << "Enter your choice" << endl;
					cin >> choice;*/
					//getchar();
				}
			}
		}
		else if(choice == "g"|| choice=="G"){
			break;
		}
		else{
			cout << "Incorrect command please restart" << endl;
			break;
			cout << "A. Run an internal command" << endl;
			cout << "B. Run an external command" << endl;
			cout << "C. Run an external command by redirecting standard input from a file" << endl;
			cout << "D. Run an external command by redirecting standard output from a file" << endl;
			cout << "E. Run an external command in the background" << endl;
			cout << "F. Run several external commands in the pipe mode" << endl;
			cout << "G. Quit the shell" << endl;
			cout << "Enter your choice" << endl;
			cin >> choice;
			getchar();
		}
	}
}