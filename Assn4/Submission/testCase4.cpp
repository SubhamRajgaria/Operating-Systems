#include "myfs2.h"
#include <wait.h>

int main(){
	create_myfs(10);
	char *dir1 =(char *)"mydocs";
	char *dir2 =(char *)"mycode";
	char *dir3 =(char *)"mytext";
	char *dir4 = (char *)"mypapers";
	char *fileN = (char *)"a.txt";
	mkdir_myfs(dir1);
	mkdir_myfs(dir2);
	ls_myfs();
	chdir_myfs(dir1);
	mkdir_myfs(dir3);
	mkdir_myfs(dir4);
	ls_myfs();
	int x = fork();
	if(x==0){
		chdir_myfs(dir3);
		copy_pc2myfs(fileN,fileN);
		int fd = open_myfs(fileN,'w');
		for(char i='A';i<='Z';i++){
			write_myfs(fd, sizeof(char),(char *)&i);
		}
		showfile_myfs(fileN,-1);
		cout << endl;
		sleep(5);
		ls_myfs();
		chdir_myfs((char *)"..");
		cout << endl;
		cout << endl;
		ls_myfs();
	}
	else{
		
		copy_pc2myfs(fileN, fileN);
		//ls_myfs();
		showfile_myfs(fileN,-1);
		cout << endl;
		//ls_myfs();
		wait(NULL);
	}

}