#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <iostream>
#include <stdbool.h>
#include <queue>

using namespace std;

#define SCHEDULING_INTERVALL (1) /* seconds */

typedef struct{
	pthread_t thread;
	int id;
	int status;
}thread_details;

thread_details STATUS[1000];
int current, n;

int compare(const void *a, const void *b){
	return (*(int*)a-*(int*)b);
}

void sighandler(int signo){
	if(signo==SIGUSR1)
	{
		pause();
	}
	else{

	}
	//signal(SIGUSR1, sighandler);
	//signal(SIGUSR2, sighandler);
}

void* worker(void *pv){
	int iThread = *(int *) pv;
	//pause();
	
	cout << "Thread no " << iThread << endl;
	STATUS[iThread].status=1;
	int *arr= (int *) malloc(1000*sizeof(int));
	for(int i=0; i<1000;i++){
		arr[i]=rand()%10000+1;
		//cout << i << " "<< iThread << endl;
		//sleep(1);
	}
	//sleep(1);
	qsort(arr, 1000, sizeof(int), compare);
	cout << "Sorted numbers are :\n";
	for(int i=0; i<1000;i++)
		cout << arr[i] << " ";
	cout << "\n";
	//sleep(1);
	int waittime = rand()%10+1;
	while(waittime){
		waittime= sleep(waittime);
	}
	STATUS[iThread].status = 2;
	//pthread_exit(NULL);
}

void* schedule(void *param){
	queue<int> ready;
	for(int i=0; i<n; i++)
		ready.push(i);
	int last;
	current = ready.front();
	cout << "Scheduler started \n";
	pthread_kill(STATUS[current].thread, SIGUSR2);
	sleep(SCHEDULING_INTERVALL);
	while(!ready.empty() && ready.size()!=1){
		if(STATUS[current].status==2){

		}
		else{
			pthread_kill(STATUS[current].thread, SIGUSR1);
			ready.push(current);
		}
		last = current;
		ready.pop();
		if(ready.empty())
			pthread_exit(NULL);
		else{
			current = ready.front();
			pthread_kill(STATUS[current].thread, SIGUSR2);
			sleep(SCHEDULING_INTERVALL);
		}
	}
	if(!ready.empty()){
		last = ready.front();
		current = last;
		pthread_kill(STATUS[current].thread, SIGUSR2);
		while(STATUS[current].status!=2);
	}
	else
		pthread_exit(NULL);
}

void* report(void* param){
	int temp = current;
	cout << current << endl;
	int prints[n];
	for(int i=0; i<n;++i){
		prints[i]=0;
	}
	int count =0;
	while(1){
		if(temp!=current){
			cout << "Thread number " << temp << " has been suspended and number " << current << " started" << endl;
			temp = current;
		}
		for(int i=0; i<n;i++){
			if(prints[i]==0 && STATUS[i].status==2){
				cout << "Thread number " << i << " has been terminated\n";
				++count;
				prints[i]=1;
			}
		}
		if(count==n){
			cout << "Completed all threads\n";
			pthread_exit(NULL);
		}
	}
}

int main(int argc, char** argv){
	struct sigaction signal_action;
	memset(&signal_action, 0, sizeof(signal_action));
	signal_action.sa_handler = sighandler;
	sigemptyset(&signal_action.sa_mask);

	sigaction(SIGUSR1, &signal_action, NULL);
	sigaction(SIGUSR2, &signal_action, NULL);

	//signal(SIGUSR1, sighandler);
	//signal(SIGUSR2, sighandler);

	cout << "Enter number of threads" << endl;
	cin >> n;

	pthread_t scheduler, reporter;
	int iResult;
	intptr_t iThread = 0;
	int i;
	for(i=0;i<n;++i){
		STATUS[i].id = i;
		iResult = pthread_create(&STATUS[i].thread, NULL, worker,&STATUS[i].id);
		if(iResult){
			errno = iResult;
            perror("pthread_created()");
            exit(1);
		}
		//STATUS[iThread].id = (int)iThread;
		pthread_kill(STATUS[i].thread, SIGUSR1);
	}
	pthread_create(&reporter, NULL, report, NULL);
	pthread_create(&scheduler, NULL, schedule, NULL);

		//pthread_kill(scheduler, SIGUSR2);
		//pthread_kill(reporter, SIGUSR2);
	for(int i=0; i<n; i++)
		pthread_join(STATUS[i].thread, NULL);

	pthread_join(scheduler, NULL);
	pthread_join(reporter, NULL);

		//pthread_join(scheduler, NULL);
		//pthread_join(reporter, NULL);
		/*for(iThread=0;;++iThread){
			if(iThread==n){
				iThread=0;
			}
			{
				if(status[iThread]==2)
					continue;
				else{
					int temp = pthread_kill(threads[iThread], SIGUSR2);
					status[iThread] = 1;
					if(temp){
						errno = temp;
	                	perror("pthread_kill(..., SIGUSR2)");
	                	exit(2);
					}
					cout << "Restarted " << iThread  << endl;
				}
			}
			sleep(SCHEDULING_INTERVALL);
			{
				if(status[iThread]==2){
					continue;
				}
				else{
					int temp  = pthread_kill(threads[iThread], SIGUSR1);
					status[iThread] =0;
					if (temp)
		            {
		                errno = temp;
		                perror("pthread_kill(..., SIGUSR1)");
		                exit(3);
		            }
		            cout << "Paused " << iThread << endl;
				}
			}
		}*/
	return 0;
}