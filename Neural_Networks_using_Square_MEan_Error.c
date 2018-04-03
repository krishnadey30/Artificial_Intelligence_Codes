/*
   Assignment II
   Artificial Inteligence
Institute: Indian Institute Of Information Technology, Chittoor ,SriCity
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
void filescan(FILE *f,int *n,int *m,int A[][20])
{
	int i=-1,j=0;
	char *record,*line,buffer[1000] ;
	while((line=fgets(buffer,sizeof(buffer),f))!=NULL)   
	{ 
		record = strtok(line,","); 
		j=0;    
		while(record != NULL)     
		{    
			if(i>=0)    
				A[i][j++] = atoi(record) ;     
			record = strtok(NULL,",");     
		}  
		*m=j;   
		i++ ;   
	}
	*n=i;
}
double WeightIH[20][30],WeightHO[30][20];
void print(int A[][20],int n,int m)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			printf("%d ",A[i][j]);
		}
		printf("\n");
	}
}
double randoutput()
{
	return ((double)rand()/((double)RAND_MAX+1));
}

void train()
{
	int training[5000][20],n,m,i,j,k,NumHidden,NumInput,NumOutput,p;
	double DeltaWeightIH[10][20],DeltaWeightHO[20][10];
	double Hidden[5000][10],Output[5000][20],sum,DeltaO[20];  
	double eta=0.001,Error,norm,value;
	double DeltaH[10];
	FILE *f= fopen("train1.csv","r"); 
	filescan(f,&n,&m,training);
	NumHidden=9;
	NumInput=m-1;
	NumOutput=10;
	for( j = 1 ; j <= NumHidden ; j++ ) 
	{   
		for( i = 0 ; i <= NumInput ; i++ ) 
		{ 
			DeltaWeightIH[i][j] = 0.0 ;
			WeightIH[i][j] = 2.0 * ( randoutput() - 0.5 ) * 0.5 ;

		}
	}
	for( k = 1 ; k <= NumOutput ; k++ ) 
	{   
		for( j = 0 ; j <= NumHidden ; j++ ) 
		{
			DeltaWeightHO[j][k] = 0.0 ;              
			WeightHO[j][k] =2.0 * ( randoutput() - 0.5 ) * 0.5 ;
		}
	}
	int t=100;
	while(t--){
		//printf("%d\n",t );
		for (p = 0; p < n; p++)
		{
			Error = 0.0 ;
			for( j = 1 ; j <= NumHidden ; j++ )
			{ 
				sum=WeightIH[0][j];
				for( i = 1; i <= NumInput ; i++ ) {
					sum += (double)training[p][i] * WeightIH[i][j] ;
				}
				Hidden[p][j] = (double)1.0/(double)(1.0 + exp(-sum)) ;
			}
			for( j = 1 ; j <= NumOutput ; j++ ) 
			{ 
				sum = WeightHO[0][j] ;
				for( i = 1 ; i <= NumHidden ; i++ ) 
				{
					sum += Hidden[p][i] * WeightHO[i][j] ;
				}
				Output[p][j] = 1.0/(1.0 + exp(-sum)) ;  
				//printf("output : %lf\n",Output[p][j] ); 
				if(j==training[p][0])
				{
					Error += 0.5 * (1 - Output[p][j]) * (1 - Output[p][j]) ;   
					DeltaO[j] = (1 - Output[p][j]) * Output[p][j] * (1.0 - Output[p][j]) ;
				}
				else
				{
					Error += 0.5 * (-Output[p][j]) * (-Output[p][j]) ; 
					DeltaO[j] = (-Output[p][j]) * Output[p][j] * (1.0 - Output[p][j]) ;
				}  
				//printf("%lf\n",Error );
			}
			/* update weights WeightHO */
			for( k = 1 ; k <= NumOutput ; k ++ ) 
			{
				DeltaWeightHO[0][k] = eta * DeltaO[k] ;
				WeightHO[0][k] += DeltaWeightHO[0][k] ;
				for( j = 1 ; j <= NumHidden ; j++ ) {
					DeltaWeightHO[j][k] = eta * Hidden[p][j] * DeltaO[k] ;
					WeightHO[j][k] += DeltaWeightHO[j][k] ;
				}
			}
			for( j = 1 ; j <= NumHidden ; j++ ) 
			{  
				sum = 0 ;
				for( k = 1 ; k <= NumOutput ; k++ ) 
				{
					sum += WeightHO[j][k] * DeltaO[k] ;
				}
				DeltaH[j] = sum * Hidden[p][j] * (1.0 - Hidden[p][j]) ;
			}

			/* update weights WeightIH */
			for( j = 1 ; j <= NumHidden ; j++ ) 
			{     
				DeltaWeightIH[0][j] = eta * DeltaH[j];
				WeightIH[0][j] += DeltaWeightIH[0][j] ;
				for( i = 1 ; i <= NumInput ; i++ ) 
				{ 
					DeltaWeightIH[i][j] = eta * training[p][i] * DeltaH[j];
					WeightIH[i][j] += DeltaWeightIH[i][j] ;
				}
			}

			value=0.0;
			for( k = 1 ; k <= NumOutput ; k ++ ) 
			{
				for( j = 1 ; j <= NumHidden ; j++ ) 
				{
					value+=(DeltaWeightHO[j][k]*DeltaWeightHO[j][k]);
				}
			}
			norm=sqrt((double)value);
			if( norm < 0.05 ) 
				break ;

		}
		printf("%lf\n",norm );
		if( norm < 0.001 ) 
			break ;
	}
}
void test()
{
	int test[5000][20],n,m,i,j,k,NumHidden,NumInput,NumOutput,p,x=0,max,ind;
	double Hidden[5000][10],Output[5000][20],sum;  
	FILE *f= fopen("test.csv","r"); 
	filescan(f,&n,&m,test);
	NumHidden=9;
	NumInput=m-1;
	NumOutput=10;
	for (p = 0; p < n; p++)
	{
		max=-10,ind=0;
		for( j = 1 ; j <= NumHidden ; j++ )
		{ 
			sum=WeightIH[0][j];
			for( i = 1; i <= NumInput ; i++ ) {
				sum += (double)test[p][i] * WeightIH[i][j] ;
			}
			Hidden[p][j] = (double)1.0/(double)(1.0 + exp(-sum)) ;
		}
		for( j = 1 ; j <= NumOutput ; j++ ) 
		{ 
			sum = WeightHO[0][j] ;
			for( i = 1 ; i <= NumHidden ; i++ ) 
			{
				sum += Hidden[p][i] * WeightHO[i][j] ;
			}
			Output[p][j] = 1.0/(1.0 + exp(-sum)) ;  
			if(Output[p][j]>max)
			{
				max=Output[p][j];
				ind=j;
			}
		}
		if(ind==test[p][0])
			x++;

	}
	double ans=(float)(x*100.0)/n;
	printf("%lf\n",ans);
}

int main()
{
	train();
	test();
	return 0;
}
