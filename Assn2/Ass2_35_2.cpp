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

char *arg_list[MAXSIZE];

string print_menu(){
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
	return choice;
}

char ** process_command(string command){
	istringstream args(command);
	string arg;
	//char *arg_list[MAXSIZE];
	int i=0;
	while(args >> arg){
		arg_list[i]= new char[arg.size()+1];
		copy(arg.begin(), arg.end(), arg_list[i]);
		arg_list[i][arg.size()]='\0';
		i++;
	}
	arg_list[i]= NULL;
	return arg_list;
}

int main(){
	pid_t x;
	int child_count;
	string choice, command;
	choice = print_menu();
	while(1){
		if(choice == "a" || choice=="A"){
			cout << "Enter internal command to execute" <<endl;
			getline(cin, command);
			char **arg_list= process_command(command);
			if(strcmp(arg_list[0],"cd")==0 || strcmp(arg_list[0],"chdir")==0){
				chdir(arg_list[1]);
				choice = print_menu();

			}
			else if(strcmp(arg_list[0],"echo")==0){
				string echo = "echo ";
				string cmd = echo+arg_list[1];
				system(cmd.c_str());
				choice = print_menu();
			}
			else if(strcmp(arg_list[0],"help")==0){
				system(command.c_str());
			}
			else{
				x = fork();
				if(x==0){
					execvp(arg_list[0],arg_list);
				}
				else{
					wait(NULL);
					choice= print_menu();
				}
			}
		}
		else if(choice == "b" || choice=="B"){
			x = fork();
			if(x==0){
				cout << "Enter external command to execute" <<endl;
				getline(cin, command);
				char **arg_list= process_command(command);
				execvp(arg_list[0],arg_list);
			}
			else{
				wait(NULL);
				choice = print_menu();
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
				//cout << pos << endl;
				arg_list[i]= NULL;
				int fd = open(arg_list[pos+1],O_RDONLY);
				close(0);
				//cout << fd << endl;
				int in = dup(fd);
				execvp(arg_list[0],arg_list);
			}
			else{
				wait(NULL);
				choice = print_menu();
			}
		}
		else if(choice == "d" || choice=="D"){
			x=fork();
			if(x==0){
				cout << "Enter external command to execute and redirect output" <<endl;
				getline(cin, command);
				istringstream args(command);
				string arg;
				char *arg_list[MAXSIZE];
				int i=0;
				int pos =0;
				while(args >> arg){
					//cout << arg << endl;
					if(arg==">")
						pos=i;
					arg_list[i]= new char[arg.size()+1];
					copy(arg.begin(), arg.end(), arg_list[i]);
					arg_list[i][arg.size()]='\0';
					i++;
				}
				//cout << pos<<endl;
				arg_list[i]= NULL;
				int fd = creat(arg_list[pos+1], 0666);
				close(1);
				//cout << fd << endl;
				int out = dup(fd);
				execvp(arg_list[0],arg_list);
			}
			else{
				wait(NULL);
				choice = print_menu();
			}
		}
		else if(choice == "e" || choice=="E"){
			cout << "Enter external command to execute in background" <<endl;
			getline(cin, command);
			char **arg_list= process_command(command);
			x = fork();
			if(x==0)
				execvp(arg_list[0],arg_list);
			else
				choice = print_menu();
		}
		else if(choice == "f" || choice=="F"){
			cout << "Enter the commands "<< endl;
			getline(cin, command);
			vector<string> commands;
			boost::split(commands, command, [](char c){return c=='|';});
			int std_in = dup(0);
			int std_out = dup(1);
			int i,in, out, fd[2];
			int no_of_cmd = commands.size();
			in =0;
			for(i=0; i< no_of_cmd-1;){
				pipe(fd);
				out = fd[1];
				int x = fork();
				if(x==0){
					dup2(in,0);
					close(in);
					dup2(out,1);
					close(out);
					char **arg_list= process_command(commands[i]);
					execvp(arg_list[0],arg_list);
				}
				else{
					wait(NULL);
					close(fd[1]);
					in = fd[0];
					i=i+1;
				}
			}
			dup2(in,0);
			dup2(std_out,1);
			close(std_out);
			x= fork();
			if(x==0){
				char **arg_list = process_command(commands[i]);
				execvp(arg_list[0],arg_list);
			}
			else{
				wait(NULL);
				close(in);
				dup2(std_in,0);
				close(std_in);
				//continue;
				cout << "Done"<< endl;
				choice = print_menu();
			}
			//
		}
		else if(choice == "g" || choice=="G"){
			break;
		}
		else{
			cout << "Incorrect command" << endl;
			choice = print_menu();
			continue;
		}
	}
}