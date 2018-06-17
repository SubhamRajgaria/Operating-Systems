#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

int compare(const void *a, const void *b){
	return (*(int*)a-*(int*)b);
}

int main(){
	pid_t x,y,z;
	int a[100];
	int b[100];
	int c[100];
	//char* aa[100];
	int i,j,k,l,m;
	int p1[2], p2[2], p3[2];
	int inp1[101], inp2[101], inp3[101];
	inp1[100] = INT_MAX;
	inp2[100] = INT_MAX;
	inp3[100] = INT_MAX;
	//char *inp[100];	
	int out[300];
	
	pipe(p1);
	
	
	x = fork();
	
	if(x==0){
		srand(time(NULL));
		printf("Process A started\n");
		for(i=0; i<100;i++)
		{
			a[i] = rand()+1;
		}
		qsort(a, 100, sizeof(int), compare);
		//for(i=0;i<100;i++){
		//	printf("%d$ ", a[i]);
			//sprintf(aa[i], "%d", a[i]);
		//}
		write(p1[1], a, sizeof(int)*100);
		close(p1[1]);
		printf("Process A ended\n");
		//wait(NULL);
		
	}
	else{
		pipe(p2);
		y= fork();
		if(y==0){
			srand(2*time(NULL));
			printf("Process B started\n");
			for(i=0; i<100;i++)
			{
				b[i] = rand()+1;
			}
			qsort(b, 100, sizeof(int), compare);
			//for(i=0;i<100;i++)
			//	printf("%d& ", b[i]);
			//printf("\n");
			write(p2[1], b, sizeof(int)*100);
			close(p2[1]);
			printf("Process B ended\n");
		}
		else{
			pipe(p3);
			z = fork();
			if(z==0){
				srand(3*time(NULL));
				printf("Process C started\n");
				for(i=0; i<100;i++)
				{
					c[i] = rand()+1;
				}
				qsort(c, 100, sizeof(int), compare);
				//for(i=0;i<100;i++)
				//	printf("%d* ", c[i]);
				//printf("\n");
				write(p3[1], c, sizeof(int)*100);
				close(p3[1]);
				printf("Process C ended\n");
			}
			else{
				//sleep(1);
				printf("Process D started\n");
				close(p3[1]);
				close(p2[1]);
				close(p1[1]);
				read(p1[0], inp1, sizeof(int)*100);
				close(p1[0]);
				read(p2[0], inp2, sizeof(int)*100);
				close(p2[0]);
				read(p3[0], inp3, sizeof(int)*100);
				close(p3[0]);
				j=0, k=0, l=0;
				m=0;
				while(m<300){
					if(inp1[j]<inp2[k] && inp1[j]<inp3[l] && j<100)
					{
						out[m] = inp1[j];
						j++;
					}
					else if(inp2[k]<inp1[j] && inp2[k]<inp3[l] && k<100){
						out[m] = inp2[k];
						k++;
					}
					else if(inp3[l]<inp1[j] && inp3[l]<inp2[k] && l<100){
						out[m] = inp3[l];
						l++;
					}
					m++;
				}
				/*for(j=0;j<100;j++){
					out[j]= inp1[j];
				}
				for(k=0; k<100; k++)
					out[k+100] = inp2[k];
				for(l=0; l<100; l++)
					out[l+200] = inp3[l];
				qsort(out,300, sizeof(int), compare);*/
				printf("The sorted list is:\n");
				for(m=0; m<300; m++)
					printf("%d ", out[m]);
				printf("\n");
				printf("Process D ended\n");
			}
		}
	}
	
}
