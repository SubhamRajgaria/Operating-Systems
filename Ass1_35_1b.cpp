#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#define MAXSIZE 10000

using namespace std;

int main(){
	string str;
	pid_t x;
	int n_arg;
	string command;
	cout << "Enter command to execute"<< endl;
	getline(cin, command);
	//cin >> str;
	//cout << "Enter the arguments" << endl;
	//cin >> args;
	while(1){
		if(command=="quit")
			break;
		x = fork();
		if(x==0){
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
			cout << "Enter command to execute"<<endl;
			getline(cin, command);
			if(command== "quit")
				break;
			//cout << "Enter the arguments" << endl;
			//cin >> args;
		}
	}
}