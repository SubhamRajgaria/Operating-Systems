#include <bits/stdc++.h>
#include <iostream>

using namespace std;


int main(){
	srand(time(NULL));
	set<int> working_set;
	vector< int> references_vector;
	int no_of_pages, number_of_references, working_set_size;
	double prob, r_w_prob;
	cout << "Enter total number of pages\n";
	cin >> no_of_pages;
	cout << "Enter number of references done\n";
	cin >> number_of_references;
	cout << "Enter working set size" << endl;
	cin >> working_set_size;
	cout << "Enter probability that a each reference will be in the working set" << endl;
	cin >> prob;
	cout << "Enter probability of reading" << endl;
	cin >> r_w_prob;
	int reference, r_w;
	int check_prob, check_prob2;
	FILE *f = fopen("instructions.txt","wb");
	char *space  = (char *)" ", *newline=(char *)"\n";
	char buffer[16]={0};
	for(int i=0; i< number_of_references;i++){
		check_prob = rand()%1000;
		if(check_prob> (int(prob*1000)) || working_set.size()==0){
			reference = rand()%no_of_pages;
			references_vector.push_back(reference);
			check_prob2 = rand()%1000;
			if(check_prob2>(int)(r_w_prob*1000))
				r_w = 1;
			else
				r_w =0;
		}
		else{
			int index = rand()%working_set.size();
			reference = *next(working_set.begin(), index);
		}
		int vector_size = references_vector.size();
		int insert_size = min(vector_size, working_set_size);
		for(int j=references_vector.size()-insert_size; j < references_vector.size();j++){
			working_set.insert(references_vector[j]);
		}
		sprintf(buffer, "%d", r_w);
		fputs(buffer, f);
		fputs(space, f);
		sprintf(buffer, "%d", reference);
		fputs(buffer, f);
		fputs(newline, f);
		// fwrite(&r_w, sizeof(int), 1, f);
		// fwrite(space, sizeof(space), 1, f);
		// fwrite(&reference, sizeof(int), 1, f);
		// fwrite(newline, sizeof(newline), 1, f);
		//cout << reference<< endl;
	}
	fclose(f);

}