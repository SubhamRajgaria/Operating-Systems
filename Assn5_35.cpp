#include <math.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <fcntl.h>
#include <bits/stdc++.h>
#include <string>
#include <stdbool.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

using namespace std;

#define SHIFT  5 // 32 = 2^5
#define MASK  0x1F // 11111 in binary 
#define PAGE_ENTRY_SIZE 32
#define NO_OF_PAGES 64

typedef struct{
	int instr_no;
	int r_w;
	int page_no;
}instruction;

vector <instruction> instructions;

int num_of_set_pages;

bitset<PAGE_ENTRY_SIZE> virtualmem [NO_OF_PAGES];
set<int> freeframes;
vector<int> fifo;
vector<int> lru;

int noOfPageFaults;
int executionTime;
int noOfPageTransfers;

long FdGetFileSize(int fd){
    struct stat stat_buf;
    int rc = fstat(fd, &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

void mapFrame(bitset<PAGE_ENTRY_SIZE> &t, int fno){
	int i;
	for(i=0;i<29;i++){
		t[i]=fno%2;
		fno = fno>>1;
	}
}

int extractFrame(bitset<PAGE_ENTRY_SIZE> &t){
	int temp=0;
	for(int i=28;i>=0;--i){
		temp = temp << 1;
		temp += t[i];
	}
	return temp;
}

void printStatistics(){
	cout << "Printing Statistics :: " << endl;
	cout << "Number of page faults: " << noOfPageFaults << endl;
	cout << "Number of Number of Page Transfers: " << noOfPageTransfers << endl;
	cout << "Execution time : " << executionTime << endl;
}

int nru_policy(){
	for(int i=0; i<NO_OF_PAGES;i++){
		bitset<PAGE_ENTRY_SIZE> temp_page = virtualmem[i];
		if(temp_page[29]==1){
			if(temp_page[30]==0 && temp_page[31]==0)
				return i;
		}
	}
	for(int i=0; i<NO_OF_PAGES;i++){
		bitset<PAGE_ENTRY_SIZE> temp_page = virtualmem[i];
		if(temp_page[29]==1){
			if(temp_page[30]==1 && temp_page[31]==0)
				return i;
		}
	}
	for(int i=0; i<NO_OF_PAGES;i++){
		bitset<PAGE_ENTRY_SIZE> temp_page = virtualmem[i];
		if(temp_page[29]==1){
			if(temp_page[30]==0 && temp_page[31]==1)
				return i;
		}
	}
	for(int i=0; i<NO_OF_PAGES;i++){
		bitset<PAGE_ENTRY_SIZE> temp_page = virtualmem[i];
		if(temp_page[29]==1){
			if(temp_page[30]==1 && temp_page[31]==1)
				return i;
		}
	}
	cout << "Error in nru \n" << endl;
	return -1;
}


int main(){
	int nframes, policy;
	srand(time(NULL));
	char *fileName = (char *)("ip.txt");
	char *token, *token2 ;
	char *delim = (char *)"\n", *delim2 = (char *)" ";
	char *str1, *saveptr, *str2, *saveptr2 ;
	int fd = open(fileName, O_RDONLY);
	FILE * fileToBeRead = fopen(fileName, "rb");
	int fileSize = FdGetFileSize(fd);
	char *buffer= (char *)malloc(fileSize*sizeof(char));
	//while(!feof(fileToBeRead)){
	fread(buffer, fileSize, 1, fileToBeRead);
	cout << buffer << endl;
	num_of_set_pages =0;
	//}
	int j,k;
	for(j=1, str1=buffer; ;j++,str1=NULL){
		token = strtok_r(str1, delim, &saveptr);
		if(token==NULL)
			break;
		if(token[0]=='#')	// Removing comments
			continue;
		instruction temp;
		for(k=1, str2=token; ;str2= NULL, k++){
			token2 = strtok_r(str2, delim2, &saveptr2); 
			if(token2==NULL)
				break;
			if(k==1)
				temp.r_w = atoi(token2); // Either read(0) or write(1)
			else if(k==2)
				temp.page_no = atoi(token2); // Reading page number to be read or written
			
		}
		temp.instr_no = j;	// Getting instruction number
		instructions.push_back(temp);  
	}
	cout << "Enter number of free frames \n";
	cin >> nframes; 
	cout << "Enter replacement algorithm you want to apply \n";
	cout << "1: FIFO\n";
	cout << "2: Random\n";
	cout << "3: LRU\n";
	cout << "4: NRU\n";
	cout << "5: Second Chance\n";
	cin >> policy;
	executionTime =0;
	noOfPageTransfers =0;
	noOfPageFaults = 0;
	for (int i=0; i< nframes; i++)
		freeframes.insert(i);
	int fno;
	cout << "Number of instructions = " << instructions.size() << endl;
	for (int i=0; i< instructions.size(); i++){
		executionTime+=1;	// For each read write operation one cycle
		if(virtualmem[instructions[i].page_no][29]==0){ // If not valid
			noOfPageFaults ++;
			if(freeframes.size()>0){	// If memory has space
				fno = *(freeframes.begin());
				freeframes.erase(fno);
				mapFrame(virtualmem[instructions[i].page_no], fno);
				virtualmem[instructions[i].page_no][29]=1;
				noOfPageTransfers++;
				cout << instructions[i].instr_no << ": IN " << instructions[i].page_no << " " << fno << endl;
				cout << instructions[i].instr_no << ": MAP " << instructions[i].page_no << " " << fno << endl;
				executionTime= executionTime+250;
				fifo.push_back(instructions[i].page_no);
				lru.push_back(instructions[i].page_no);
			}
			else{ // If memory is full apply page replacement
				int rframe, rpage;
				if(policy==1){ // FIFO
					rpage = *(fifo.begin());
					fifo.erase(fifo.begin());
					fifo.push_back(instructions[i].page_no);
				}
				else if(policy==2){ // Random
					int index = rand()%fifo.size();
					rpage = fifo[index];
					fifo.erase(fifo.begin()+index);
					fifo.push_back(instructions[i].page_no);
				}
				else if(policy==3){ // LRU
					rpage = *(lru.begin());
					lru.erase(lru.begin());
					lru.push_back(instructions[i].page_no);
				}
				else if(policy==4){ // NRU
					rpage = nru_policy();
				}
				else if(policy==5){ // Second chance
					while(1){ 
						rpage = *(fifo.begin());
						if(virtualmem[rpage][31]==0){
							fifo.erase(fifo.begin());
							fifo.push_back(instructions[i].page_no);
							break;
						}
						virtualmem[rpage][31]=0; // Making reference bit 0
						fifo.erase(fifo.begin());
						fifo.push_back(rpage);
					}
				}
				rframe = extractFrame(virtualmem[rpage]);
				virtualmem[rpage][29]=0;
				cout << instructions[i].instr_no << ": UNMAP " << rpage << " " << rframe << endl;
				executionTime+=250;
				if(virtualmem[rpage][30]==1){
					cout << instructions[i].instr_no << ": OUT " << rpage << " " << rframe << endl;
					noOfPageTransfers++;
				}
				mapFrame(virtualmem[instructions[i].page_no], rframe);
				virtualmem[instructions[i].page_no][29]=1;
				noOfPageTransfers++;
				cout << instructions[i].instr_no << ": IN " << instructions[i].page_no << " " << rframe << endl;
				cout << instructions[i].instr_no << ": MAP " << instructions[i].page_no << " " << rframe << endl;
				executionTime= executionTime+250; // For mapping 250 cycles are used
				virtualmem[instructions[i].page_no][30]=0;
				virtualmem[instructions[i].page_no][31]=0;
			}
		}
		else{
			cout << "Found page " << instructions[i].page_no << " in memory in frame number " << extractFrame(virtualmem[instructions[i].page_no]) << endl;
		}
		if(policy==3){// For LRU putting recently used pages at the back in the queue
			for(int c=0; c< lru.size();c++){
				if(lru[c]==instructions[i].page_no){
					lru.erase(lru.begin()+c);
					lru.push_back(instructions[i].page_no);
					break;
				}
			}
		}
		else if(policy==4 && (noOfPageFaults%10)==0){ // Setting referenced bit 0 at every 10 page faults
			for(int c =0; c<NO_OF_PAGES;c++){
				virtualmem[c][31]=0;
			}
		}
		if(instructions[i].r_w==1){
			virtualmem[instructions[i].page_no][30]=1;	// Setting dirty bit 1 if the file is written onto
		}
		virtualmem[instructions[i].page_no][31]=1; // Setting referenced bit irrespective of read and write
	}
	executionTime+= noOfPageTransfers*3000; // Every time page transfer happens 3000 memory cycles used
	printStatistics();
	return 0;
}