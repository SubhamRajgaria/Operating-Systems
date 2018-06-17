#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<bitset>
#include<deque>
#include<map>
#include<vector>
#include<deque>
#include<set>
#include<math.h>
#include<stdlib.h>
#include <iomanip>
#define S 32
#define VS 64
using namespace std;
typedef struct p
{
  bitset<S> *pages;//29-valid,30-modified,31-referenced,0-28 frame no from least to most significant 
}page_table;
set<int> free_list;//contains free memory frames
deque<int> fifo;//deque for fifo
vector<int> rv;//vector for random
vector<int> lru;//deque for least recently used
int min1(int a,int b)
{
  if(a<b)
    return a;
  return b;
}
void map_frame(bitset<S> &a,int frame_no)
{
  for(int i=0;i<29;++i)
    {
      a[i]=frame_no%2;
      frame_no=frame_no>>1;
    }
}
int extract_frame(bitset<S> &a)
{
  int tmp=0;
  for(int i=28;i>=0;--i)
    {
      tmp=tmp << 1;
      tmp+=a[i];
    }
  return tmp;
}
int nru_page(page_table &pt)
{
  for(int i=0;i<VS;++i)
    {
      bitset<S> page=pt.pages[i];
      if(page[29]==1)//if valid
			{
			  if(page[30]==0 && page[31]==0)//not modified and not referenced
			    {
			      return i;
			    }
			}
    }
  for(int i=0;i<VS;++i)
    {
      bitset<S> page=pt.pages[i];
      if(page[29]==1)
			{
			  if(page[30]==1 && page[31]==0)
			    {
			      return i;
			    }
			}
    }
  for(int i=0;i<VS;++i)
    {
      bitset<S> page=pt.pages[i];
      if(page[29]==1)
			{
			  if(page[30]==0 && page[31]==1)
			    {
			      return i;
			    }
			}
    }
  for(int i=0;i<VS;++i)
    {
      bitset<S> page=pt.pages[i];
      if(page[29]==1)
			{
			  if(page[30]==1 && page[31]==1)
			    {
			      return i;
			    }
			}
    }
  cout << "error in nru\n";
  return -1;
}
void print_page_table(page_table &pt)
{
  cout << setw(10) << "page no " << setw(40) << " content(in bits) " << setw(10) << " frame no\n";
  for(int i=0;i<VS;++i)
    {
      cout << setw(10) << i << setw(40) << pt.pages[i] << setw(10) << extract_frame(pt.pages[i]) << "\n";
    }
}
int main()
{
  page_table pt;
  ifstream infile("ip.txt");
  string line;
  char optype;
  int page_number,log=1,policy,nru_count,num_frames,instr_count=0,page_fault=0,page_transfers=0,exec_time=0;
  set<int>::iterator it;
  vector<int>::iterator itv;
  cout << "input the number of frames\n";
  cin >> num_frames;
  cout << "What will be page relacement policy? enter corresponding integer\n";
  cout << "1.FIFO\n";
  cout << "2.Random\n";
  cout << "3.LRU\n";
  cout << "4.NRU\n";
  cout << "5.Second chance\n";
  cin >> policy;
  for(int i=0;i<num_frames;++i)
    {
      free_list.insert(i);
    }
  pt.pages=new bitset<S>[VS];
  nru_count=0;
  while (getline(infile,line))
    {
      istringstream iss(line);
      iss >> optype;
      if(optype=='0'|| optype=='1')
	{
	  exec_time+=1;
	  ++instr_count;
	  iss >> page_number;
	  //bringing the frame in page table first
	  if(pt.pages[page_number][29]==0)//if frame is not loaded in memory
	    {
	      ++page_fault;
	      if(free_list.size()>0)//free frame available
		{
		  it=free_list.begin();
		  free_list.erase(it);
		  map_frame(pt.pages[page_number],*it);
		  pt.pages[page_number][29]=1;//page is now valid
		  cout << instr_count << ": IN " << page_number << " " << *it << "\n";
		  ++page_transfers;
		  cout << instr_count << ": MAP " << page_number << " " << *it << "\n";
		  exec_time+=250;
		  fifo.push_back(page_number);//for policy fifo and second chance
		  rv.push_back(page_number);//for policy random
		  lru.push_back(page_number);
		}
	      else//free frame not available(apply replacement algorithm)
		{
		  int rpage;
		  int rframe;
		  if(policy==1)//fifo poicy
		    {
		      rpage=fifo.front();
		      fifo.pop_front();
		      fifo.push_back(page_number);
		    }
		  else if(policy==2)//Random 
		    {
		      int r=rand()%rv.size();
		      rpage=rv[r];
		      rv.erase(rv.begin()+r);
		      rv.push_back(page_number);
		    }
		  else if(policy==3)//LRU
		    {
		      rpage=lru[0];
		      itv=lru.begin();
		      lru.erase(itv);
		      lru.push_back(page_number);
		    }
		  else if(policy==4)
		    {
		      rpage=nru_page(pt);
		    }
		  else //second chance
		  {
		      while(1)
					{
					  rpage=fifo.front();
					  if(pt.pages[rpage][31]==0)//checking if the refernced bit is 0
					    {
					      fifo.pop_front();
					      fifo.push_back(page_number);
					      break;
					    }
					  pt.pages[rpage][31]=0;
					  fifo.pop_front();
					  fifo.push_back(rpage);
					}
		  }
		  rframe=extract_frame(pt.pages[rpage]);
		  pt.pages[rpage][29]=0;//making the replaced page invalid
		  cout << instr_count << ": UNMAP " << rpage << " " << rframe << "\n";
		  exec_time+=250;
		  if(pt.pages[rpage][30]==1)//if the frame was modified
		    {
		    cout << instr_count << ": OUT " << rpage  << " " << rframe << "\n";
		    ++page_transfers;
		    }
		  map_frame(pt.pages[page_number],rframe);
		  pt.pages[page_number][29]=1;//page is now valid
		  cout << instr_count << ": IN " << page_number << " " << rframe << "\n";
		  ++page_transfers;
		  cout << instr_count << ": MAP " << page_number << " " << rframe << "\n";
		  exec_time+=250;
		  pt.pages[rpage][30]=0;
		  pt.pages[rpage][31]=0;
		}
	    }
	  else//frame is already loaded in memory
	    {
	      //do nothing
	    }
	  //now performing read and write
	  if(policy==3)//for every acess we update lru
	    {
	      itv=lru.begin();
	      while(1)
		{
		  if(*itv==page_number)
		    {
		      lru.erase(itv);
		      lru.push_back(page_number);
		      break;
		    }
		  ++itv;
		}
	    }
	  if(policy==4 && page_fault%10==0)//making referenced bit 0 for nru after 10 page_faults
	    {
	      for(int i=0;i<VS;++i)
		{
		  pt.pages[i][31]=0;
		}
	    }
	  if(optype=='1')
	    {
	      pt.pages[page_number][30]=1;
	      if(log)
		{
		  cout << instr_count << " page_number: " << page_number << " frame_number:" << extract_frame(pt.pages[page_number]) << " modified\n";
		}
	    }
	  else
	    {
	      if(log)
		{
		  cout << instr_count << " page_number: " << page_number << " frame_number:" << extract_frame(pt.pages[page_number]) << " read\n";
		}
	    }
	  pt.pages[page_number][31]=1;//making referenced bit of page to be 1 after read or write
	}
    }
  cout << "Printing Statistics\n";
  cout << "page faults: " << page_fault << "\n";
  cout << "page_transfers: " << page_transfers << "\n";
  exec_time+=page_transfers*3000;
  cout << "execution time: " << exec_time << "\n";
  print_page_table(pt);
  return 0;
}
