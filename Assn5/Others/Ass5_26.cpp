#include <bits/stdc++.h>
#define mp make_pair
#define ff first
#define ss second
#define pb push_back
#define N_PAGES 10
using namespace std;

typedef pair<int,int> pi;

struct instr{
	int opn;
	int page;
	instr(int x,int y){
		opn=x; page=y;
	}
};


vector< instr >  vseq; // instruction sequence
int nframes; // no. of frames
int pgt_sz = 0; // page table size
int pagetable[64]; // page table
// 3 lower bits will be valid , modified , referenced
set<int> freeframe;
vector<int> lru;
vector<int> fifo;
vector< int > sc;
int counter;
int no_of_page_faults;
int no_of_page_transfer_ops;
long long execution_time;


int LRU( vector<int>& lru){
	
	int lval = INT_MAX,fno = -1;
	for(int i=0;i<nframes ;i++){
		if(lru[i] < lval){
			lval = lru[i];
			fno = i;
		}
	}
	return fno;

}
int FIFO(vector<int>& fifo){
	int lval = INT_MAX,fno = -1;
	for(int i=0;i<nframes ;i++){
		if(fifo[i] < lval){
			lval = fifo[i];
			fno = i;
		}
	}
	return fno;
} 
int RANDOM_ALGO(){

	return (rand()%nframes);
}

int find_page(int fno){
	int pg = -1;
	for(int i=0;i<64;i++){
		int fr = pagetable[i] >> 3;
		int vbit = pagetable[i] & (1<<2) ;
		if(fr == fno and vbit){
			pg = i; break;
		}
	}
	return pg;
}

int NRU()
{  
	srand(time(NULL));
   vector< int > class1,class2,class3,class4;
   for(int it=0;it<nframes;it++)
   {
   	 int pg = find_page(it);
   	 int rbit = pagetable[pg] & (1<<0);
   	 int mbit = pagetable[pg] & (1<<1);
   	 if(rbit && mbit)
   	 	class1.push_back(it);
   	 else if(rbit)
   	 	class2.push_back(it);
   	 else if(mbit)
   	 	class3.push_back(it);
   	 else class4.push_back(it);
   }
   if(class4.size())
   {
   	   int temp = rand()%(class4.size());
   	   return class4[temp];
   }else if(class3.size())
   {
   	   int temp = rand()%(class3.size());
   	   return class3[temp];
   }else if(class2.size())
   {
   	   int temp = rand()%(class2.size());
   	   return class2[temp];
   }else if(class1.size())
   {
   	   int temp = rand()%(class1.size());
   	   return class1[temp];
   }
   return -1;
}

int SECOND_CHANCE( vector<int> & sc,int &maxP)
{
   int it;
   vector< pair< int, int > > sc1;
   for(it=0;it<nframes;it++)
   	{
   		sc1.push_back(make_pair(sc[it],it));
   	}
   sort(sc1.begin(),sc1.end());
   for(it=0;it<nframes;it++)
   {
   	 int pg = find_page(sc1[it].second);
   	 int rbit = pagetable[pg] & (1<<0);
   	 if(rbit == 1)
   	 {
       
       pagetable[pg] ^= (1<<0);
       sc[sc1[it].second] = ++maxP;
   	 

   	 }else{

        ++maxP;
   	 	return sc1[it].second;
   	 }
   }
   ++maxP;
   return FIFO(sc);

}


int page_replacement_algo(int choice,int& tm){
	int fno;		
	if(choice == 1) fno = FIFO(fifo);
	else if(choice == 2) fno = RANDOM_ALGO();
	else if(choice == 3) fno = LRU(lru); 
	else if(choice == 4)  fno = NRU();
	else if(choice == 5) {fno = SECOND_CHANCE(sc,tm);}
	else return -1;
}
void summary_statistics()
{
	cout<<"Number of page faults : "<<no_of_page_faults<<endl;
	cout<<"Number of page transfer operations : "<<no_of_page_transfer_ops<<endl;
	cout<<"Overall execution time : "<<execution_time<<endl;
}
int main(int argc , char* argv[]){
	// test file passed as argv[1]
	srand(time(NULL));
	string str;
	ifstream f(argv[1]);
	if(f.is_open()){
		while(getline(f,str)){
			if(str[0] == '#') continue ;
			int opn=0,page_no=0; 
			// read or write instr in opn
			opn = (str[0] == '1');
			int i=1;
			while(str[i] != ' ');
			stringstream sst(str.substr(i));
			sst>>page_no; // page no. retreived
			pgt_sz = max(pgt_sz , page_no);
			vseq.pb(instr(opn,page_no));
		}
	}
	f.close();
	/*for(auto x:vseq){
		cout<<x.opn <<" "<<x.page <<endl;
	}*/
	cout<<"enter no. of frames in main memory :";
	cin>>nframes;
	cout<<"Trace mode? or not??";
	int tr;
	cin>>tr;
	cout<<"\nchoice --> algorithm\n";
	cout<<"1 --> FIFO\n2 --> RANDOM\n3 --> LRU\n4 --> NRU\n5 --> 2nd chance\n";
	cout<<"enter replacement algo choice :";
	int choice;
	cin>>choice;
	lru.resize(nframes);
	fifo.resize(nframes);
	sc.resize(nframes);
    time_t temp = time(0);
	for(int i=0;i<nframes ;i++){
		freeframe.insert(i);
		lru[i]=fifo[i]=sc[i]=-1;
	}
	string output = "";
	//pagetable = new unsigned int[pgt_sz];
	memset(pagetable,0,sizeof pagetable);

	int instrcnt = vseq.size();
	int tm = 0 ,ttm = 0;
	for(int i=0;i< instrcnt ;i++){
		int vbit = pagetable[vseq[i].page] & (1<<2) ;
		int fno ;
		execution_time += 1;
		if(vbit){
			fno = pagetable[vseq[i].page] >> 3 ;
			pagetable[vseq[i].page] |= (1<<0);
			if(tr==1)
			printf("%d : FOUND %d %d\n",i,vseq[i].page,fno);
		    else output = output + to_string(i) + " : FOUND " + to_string(vseq[i].page) + " " + to_string(fno) + "\n";
		}
		else{
            
            counter++;
            no_of_page_faults++;
            if(freeframe.empty()){

				// page replacement algorithm
				int pg; // virtual page to be replaced
				fno = page_replacement_algo(choice,ttm);
				pg = find_page(fno);
				//unmap set valid bit = 0
				pagetable[pg] ^= (1<<2);
				if(tr==1)
				printf("%d : UNMAP %d %d\n",i,pg,fno);
		       else output = output + to_string(i) + " : UNMAP " + to_string(pg) + " " + to_string(fno) + "\n";			   
				execution_time+=250;
				if(pagetable[pg] & (1<<1)){
					// modified bit set
					if(tr==1)
					printf("%d : OUT %d %d\n",i,pg,fno);
		    else output = output + to_string(i) + " : OUT " + to_string(pg) + " " + to_string(fno) + "\n";
					execution_time+=3000;
					no_of_page_transfer_ops++;
				}
			}
			else{
				fno = *(freeframe.begin());
				freeframe.erase(fno);
			}
			// setting the frame no. alloted to this page
			pagetable[vseq[i].page] = 0;
			pagetable[vseq[i].page] |= (fno<<3);
			//setting valid bit 1
			pagetable[vseq[i].page] |= (1<<2);
		    if(tr==1)
			printf("%d : IN %d %d\n",i,vseq[i].page,fno);
		    else output = output + to_string(i) + " : IN " + to_string(vseq[i].page) + " " + to_string(fno) + "\n";
			execution_time+=3000;
			no_of_page_transfer_ops++;
			if(tr==1)
			printf("%d : MAP %d %d\n",i,vseq[i].page,fno);
		    else output = output + to_string(i) + " : MAP " + to_string(vseq[i].page) + " " + to_string(fno) + "\n";
			execution_time+=250;
			fifo[fno] = ttm;
			sc[fno] = ttm++;
		}

		if(vseq[i].opn == 1){
			// write instruction
			pagetable[vseq[i].page] |= 1<<1;
		}
		if(counter >= 10 && choice == 4)
		{
			for(int it=0;it<nframes;it++)
			{
				int pg = find_page(it);
				if(pagetable[pg] & (1<<0))
				{
					pagetable[pg] ^= (1<<0);
				}
			}
			counter = 0;
		}

		lru[fno] = tm++ ; // for lru


	}
	summary_statistics();
	if(tr==0)cout<<output;
	return 0;
}