#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


int Main1[6700][200],Main2[4000][200],n,m,N,M;
int count[193][6][11],freq[11];
float prob[193][6][11];

void find_probability(int x,int y)
{
	int i,j;
}
void scan(FILE *f, int A[][200],int *a,int *b)
{
	int i=0,j=0;
	char *record,*line;
	char buffer[1024] ;
	int t=0;
	while(line=fgets(buffer,sizeof(buffer),f))   
	{ 	
		record = strtok(line," "); 
		j=0;    
		while(record != NULL)     
		{      
			if(i>=0)    
				A[i][j++] = atoi(record) ;     
			record = strtok(NULL," ");     
		}  
		*b=j;   
		i++ ;   	
	}
	*a=i;
}
int main()
{
	FILE *fstream1= fopen("pp_tra.dat","r"); 
	scan(fstream1,Main1,&n,&m);
	memset(count,0,sizeof count);
	memset(freq,0,sizeof freq);
	//printf("%d\n",Main1[1][192] );
	int i,j,k;
	//printf("%d %d\n",n,m );
	for (i = 0; i < 6670; i++)
	{
		freq[Main1[i][192]]++;
		for(j = 0 ; j < 192; j++)
		{
			count[j][Main1[i][j]][Main1[i][192]]++;
		}
		
	}
	// for (i = 0; i < 192; ++i)
	// {
	// 	for(j =0 ;j<5; j++)
	// 	{
	// 		for(k=0;k<10;k++)
	// 		{
	// 			printf("%f ",prob[i][j][k]);
	// 		}
	// 		printf("\n");
	// 	}
	// 	printf("\n");
	// }
	// for (i = 0; i < 10; i++)
	// {
	// 	printf("%d\n",freq[i] );
	// }
	//printf("%d %d\n",n,m );
	for (i = 0; i < 192; ++i)
	{
		for(j =0 ;j<5; j++)
		{
			for(k=0;k<10;k++)
			{
				prob[i][j][k]=(float)count[i][j][k]/(float)freq[k] ;
			}
			
		}
		
	}
	FILE *fstream2= fopen("pp_tes.dat","r"); 
	scan(fstream2,Main2,&N,&M);
	float sum,max=-50000;
	int error=0;
	for (i = 0; i < N; i++){
		int class=0;
		for (j = 0; j < 10; j++)
		{
			sum=0.0;
			for (k = 0; k < 192; k++)
			{
				sum+=log(prob[k][Main2[i][k]][j]);
			}
			if(j==0)
				max=sum;
			else
			{
				if(sum>max)
				{
					max=sum;
					class=j;
				}
			}
		}
		if(class!=Main2[i][192])
			error++;
	}
	printf("No of Errors: %d\n",error );
	printf("Efficiency: %lf\n",((double)(N-error)/N)*100);
	
	return 0;
}