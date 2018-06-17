#include "myfs2.h"

int main(){
	int N=0,t,p,q,fd,fd2, nbytes;
	char src[30],dst[30],name[30];
	int newfd;
	char *buff;
	//cout << "Enter file system size: ";
	//cin >> file_system_size;
	//create_myfs(file_system_size);
	char *fileName = (char *)("mytest.txt");
	char *dump = (char *)"mydump-35.backup";
	char *output = (char *)("sorted.txt");
	//dump_myfs(dump);
	//free(file_system);
	restore_myfs(dump);
	int b[100];
	vector<int>v;
	ls_myfs();
	fd = open_myfs(fileName,'r');
	for(int i=0;i<100;i++){
		read_myfs(fd,sizeof(int),(char*)&b[i]);
		v.push_back(b[i]);
	}
	close_myfs(fd);
	sort(v.begin(), v.end());
	creat(output, 0666);
	copy_pc2myfs(output, output);
	fd = open_myfs(output,'w');
	for(int i=0;i<100;i++){
		write_myfs(fd, sizeof(int), (char *)&v[i]);
		//cout << v[i] << " ";
	}
	close_myfs(fd);
	fd = open_myfs(output,'r');
	for(int i=0;i<100;i++){
		read_myfs(fd,sizeof(int),(char*)&b[i]);
		cout << b[i]<< " ";
	}
	//showfile_myfs(output,-1);
	cout << "\n" ;
	return 0;

}