#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_BUF_SIZE 5
#define KEY_CODE 42

using namespace std;


vector<int> buffer;
vector<int> primes;

void prime_gen(int n)
{
    // Create a boolean array "prime[0..n]" and initialize
    // all entries it as true. A value in prime[i] will
    // finally be false if i is Not a prime, else true.
    bool prime[n+1];
    memset(prime, true, sizeof(prime));
 
    for (int p=2; p*p<=n; p++)
    {
        // If prime[p] is not changed, then it is a prime
        if (prime[p] == true)
        {
            // Update all multiples of p
            for (int i=p*2; i<=n; i += p)
                prime[i] = false;
        }
    }
 
    for (int p=2; p<=n; p++)
       if (prime[p])
          primes.push_back(p);
}

//int primes[]={2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 
//	61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 
//	139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199};

void producer(int smem_id, int no){
	srand(smem_id+no);
	int* mem_seg = (int *)shmat(smem_id, NULL, 0);
	int i = rand()%75000;
	float waittime = ((float)rand())/RAND_MAX*5;
	//cout << "Genrated " << waittime << endl;
	if(buffer.size()< MAX_BUF_SIZE){
		sleep(waittime);
		buffer.push_back(primes[i]);
		cout << "Producer " << no << ":" << primes[i] << " time:" << waittime << endl;
		shmdt(mem_seg); 
	}
	else{
		const clock_t begin_time = clock();
		while(1){
			if(buffer.size()<MAX_BUF_SIZE)
				break;
		}
		buffer.push_back(primes[i]);
		cout << "Producer " << no << ":" << primes[i] << " time:" << float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl; 
		shmdt(mem_seg);
	}
}

void consumer(int smem_id, int no){
	srand(smem_id+no);
	int* mem_seg = (int*)shmat(smem_id, NULL, 0);
	float waittime = ((float)rand())/RAND_MAX*5;
	//cout << "Genrated " << waittime << endl;
	if(buffer.size()>0){
		sleep(waittime);
		int cons = buffer.back();
		buffer.pop_back();
		cout << "Consumer " << no << ":" << cons << " time:" << waittime << endl;
		shmdt(mem_seg);
	}
	else{
		const clock_t begin_time = clock();
		while(1){
			if(buffer.size()>0)
				break;
		}
		int cons = buffer.back();
		buffer.pop_back();
		cout << "Consumer " << no << ":" << cons << " time:" << float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;
		shmdt(mem_seg);
	}
}




int main(){
	prime_gen(1000000);
	int np, nc;
	cout << "Enter number of producers" << endl;
	cin >> np;
	cout << "Enter number of consumers" << endl;
	cin >> nc;
	vector<int> producers;
	vector<int> consumers;
	pid_t pid;
	int smem_id; // Shared memory id
	key_t smem_key = KEY_CODE;
	struct shared_mem_struct *shared_mem;
	if((smem_id = shmget(smem_key, 5*sizeof(int), IPC_CREAT|0666))==-1){
		perror("shmget: shmget failed");
		exit(1);
	}
	else{
		cout << "smem_id: shmget returned " << smem_id<< endl;
	}


	int  count=0;
	for(int i=1;i<=np;i++)
	{
		int x=fork();
		if(x==0){
			//cout << "PID " << getpid()<<endl; 
			producer(smem_id,i);
			//exit(1);
		}
		else{
			producers.push_back(x);
		}
	}
	for(int i=1;i<=nc;i++){
		int x= fork();
		if(x==0){
			consumer(smem_id,i);
			//exit(1);
		}
		else{
			consumers.push_back(x);
		}
	}

	sleep(30);
	for(int i=0;i<producers.size();i++){
		kill(producers[i],SIGKILL);
	}
	for(int i=0; i<consumers.size();i++){
		kill(consumers[i], SIGKILL);
	}
	shmctl(smem_id,IPC_RMID,NULL);
	return 0;
	
}