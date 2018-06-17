#include <bits/stdc++.h>

using namespace std;

typedef struct {
	int index;
	int processTime;
	int fin_time;
}pairs;

typedef struct {
	int index;
	int burst;
}arr;

void sort(int *arrival_times, int * burst_times, int n){
	int temp;
	for (int i=0; i< n-1; i++){
		for(int j=0; j< n-i-1; j++){
			if(arrival_times[j]>arrival_times[j+1]){
				temp = arrival_times[j];
				arrival_times[j] = arrival_times[j+1];
				arrival_times[j+1] = temp;
				temp = burst_times[j];
				burst_times[j] = burst_times[j+1];
				burst_times[j+1] = temp;
			}
		}
	}
}

bool compare(const arr& l, const arr &m){
	return l.burst<m.burst;
}

void FCFS(int * arrival_times, int *burst_times, int n){
	float avg = 0;
	int total_time = 0;
	for (int i=0; i< n; i++){
		if(arrival_times[i]>total_time)
			total_time = arrival_times[i];
		total_time = total_time + burst_times[i];
		avg = avg + total_time- arrival_times[i];
		//cout << avg << endl;
	}
	avg = avg/n;
	cout << " Average Turnaround time with FCFS = " << avg << endl;
}

void PSJF(int *arrival_times, int* burst_times, int n){
	float avg =0;
	int time =0;
	arr times[n];
	int job =1;
	int finTime[n];
	vector <arr> process;
	arr temp;
	for(int i=0;i<n;i++){
		times[i].index =i;
		times[i].burst = burst_times[i];
	}
	process.push_back(times[0]);
	while(!process.empty() || job<n){
		time++;
		while(job<n && arrival_times[job]<=time){
			process.push_back(times[job]);
			job++;
		}
		if(!process.empty()){
			temp = process[0];
			process.erase(process.begin());
			temp.burst = temp.burst-1;
			if(temp.burst==0)
				finTime[temp.index]=time;
			else
				process.push_back(temp);
			sort(process.begin(), process.end(), compare);
		}
	}
	for (int i=0; i<n;i++){
		avg += (finTime[i]- arrival_times[i]);
	}

	avg = avg/n;
	cout << "Average turnaround time for PSJF is : " << avg << endl;

}

void RoundRobin(int *arrival_times, int *burst_times, int n, int delta){
	float avg=0;
	int time  =0;
	queue <pairs> t;
	int job =0;
	pairs times[n];
	for(int i=0;i<n;i++){
		times[i].index =i;
		times[i].fin_time = 0;
		times[i].processTime =0;
	}
	t.push(times[0]);
	pairs temp;
	while(job<n){
		time = time+delta;
		while(arrival_times[job+1]<= time && (job+1)<n){
			t.push(times[job+1]);
			job++;
		}
		if(t.empty())
			continue;
		temp = t.front();
		t.pop();
		temp.processTime+=delta;
		times[temp.index].processTime+=delta;
		if(temp.processTime > burst_times[temp.index]){
			time = time-(temp.processTime-burst_times[temp.index]);
			times[temp.index].fin_time = time;
		}
		else if (temp.processTime == burst_times[temp.index]){
			times[temp.index].fin_time = time;
		}
		else
			t.push(times[temp.index]);
		if(job==n-1)
			break;
	}
	while(!t.empty()){
		time = time+delta;
		temp = t.front();
		t.pop();
		temp.processTime+=delta;
		times[temp.index].processTime+=delta;
		if(temp.processTime > burst_times[temp.index]){
			time = time-(temp.processTime-burst_times[temp.index]);
			times[temp.index].fin_time = time;
		}
		else if (temp.processTime == burst_times[temp.index]){
			times[temp.index].fin_time = time;
		}
		else
			t.push(times[temp.index]);
	}

	for (int i=0; i<n;i++){
		avg += (times[i].fin_time- arrival_times[i]);
	}

	avg = avg/n;
	cout << "Average turnaround time for RoundRobin is : " << avg << endl;

}

int main(){
	int n;
	float mean;
	srand(210);
	cout << "Enter number of processes " << endl;
	cin >> n;
	int arrivals[n];
	int bursts[n];
	cout << "Enter mean " << endl;
	cin >> mean;
	arrivals[0]=0;
	int fin_time;
	for(int i=1; i< n; i++)
	{
		float time = ((float)rand())/RAND_MAX;
		fin_time  = (int)(fmod((-1.0/mean)*log(time),11.0));
		arrivals[i] = arrivals[i-1]+fin_time;
		//cout << arrivals[i] << " ";
	} 
	cout << "\n";
	for(int i=0; i<n ;i++)
	{
		float time = ((float)rand())/RAND_MAX;
		//cout << i << " ";
		fin_time  = (int)(fmod(((-1.0/mean)*log(time)),20.0)) + 1;
		bursts[i] = fin_time;
	}
	ofstream f;
	f.open("Timedetails1009.txt");
	f << "Time arrivals are :\n";
	for(int i=0; i<n ;i++){
		f << arrivals[i];
		f << " ";
	}
	f << "\n Bursts time are : \n";
	for(int i=0; i<n;i++){
		f << bursts[i];
		f << " ";
	}
	f.close();
	sort(arrivals, bursts,n);
	// First Come First Serve
	FCFS(arrivals, bursts, n);
	// Preemptive Shortest Job First
	PSJF(arrivals, bursts, n);
	// Round Robins with varying delta
	RoundRobin(arrivals, bursts, n, 1);
	RoundRobin(arrivals, bursts, n, 2);
	RoundRobin(arrivals, bursts, n, 5);
	return 0;
}