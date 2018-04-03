#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<math.h>
int Main1[6700][200],Main2[4000][200],D1[2500][200],D2[2500][200],D3[2500][200];
int sub_xtrain1[5000][200],sub_xtrain2[5000][200],sub_xtest[5000][200];
int array[4000][30];
float val1[30];//val2[20];
int n,m,n1,n2,n3,glob,col1=6670,col2=3333;
int error(int array1[][30],int array2[][200],int q,int w)
{
	int i,j,err=0;
	if(q==25)
	{
		//val=val1;
		i=1;
	}
	else
	{
		//val=val2;
		i=q;
		memset(val1,0,sizeof(val1));

	}
	for(;i<=q;i++)
	{
		err=0;
		for(j=0;j<w;j++)
		{
			if(array1[j][i]!=array2[j][192])
			{
				err++;
			}
		}
		val1[i]=(val1[i]+err);
		printf("%f  ",val1[i]);
	}
	printf("-----------------\n");
}
int cmp ( const void *pa, const void *pb ) {
	const float (*a)[2] = pa;
	const float (*b)[2] = pb;
	if ( (*a)[0] < (*b)[0] ) return -1;
	if ( (*a)[0] > (*b)[0] ) return +1;
	return 0;
}
void scan(FILE *f, int A[][200])
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
		m=j;   
		i++ ;   	
	}
	n=i;
}
void sort(float distances[][2],int z)
{
	int i,j,temp[1][2];
	for(i=0;i<z-1;i++)
	{
		for(j=0;j<z-i-1;j++)
		{
			if(distances[j][0]>distances[j+1][0])
			{
				temp[0][0]=distances[j][0];
				temp[0][1]=distances[j][1];
				distances[j][0]=distances[j+1][0];
				distances[j][1]=distances[j+1][1];
				distances[j+1][0]=temp[0][0];
				distances[j+1][1]=temp[0][1];
			}
		}
	}
}
int find_class(float distances[][2],int k)
{
	int count[10],i,max=-50,ind;
	memset(count, 0, 10*sizeof(count[0]));
	for(i=0;i<k;i++)
	{
		count[(int)distances[i][1]]++;
	}
	for(i=0;i<10;i++)
	{
		if(max<count[i])
		{
			max=count[i];
			ind=i;
		}
	}
	return ind;
}
int predict(int a[][200],int b[][200],int c[200],int length1,int length2,int k)
{	
	int z,l,euclid,p;
	float distances[1000000][2];
	for(z=0;z<length1;z++)
	{
		euclid=0;
		for(l=0;l<192;l++)
		{
			euclid+=(a[z][l]-c[l])*(a[z][l]-c[l]);
		}
		distances[z][0]=sqrt(euclid);
		distances[z][1]=a[z][l];
	}
	if(length2!=0)
	{
		for( ;z<length1+length2;z++)
		{
			euclid=0;
			for(l=0;l<192;l++)
			{
				euclid+=(b[z-length1][l]-c[l])*(b[z-length1][l]-c[l]);
			}
			distances[z][0]=sqrt(euclid);
			distances[z][1]=b[z-length1][l];
		}
	}
	//sort(distances,z);
	qsort( distances, z, sizeof distances[0], cmp );
	if(k==0)
	{
		for(p=1;p<=25;p++)
		{
			array[glob][p]=find_class(distances,p);
		}
	}
	else
	{
		array[glob][k]=find_class(distances,k);
	}
}
void one_by_one(int sub_xtrain1[][200],int sub_xtrain2[][200],int sub_xtest[][200],int k,int p,int q,int w)
{
	for(int z=0;z<w;z++)
	{	
		glob=glob+1;
		predict(sub_xtrain1,sub_xtrain2,sub_xtest[z],p,q,k);
	}
}
int finding_k()
{
	int rate=10000000,z,k_val,i;
	for(i=1;i<=4;i++)
	{
		if(i==1)
		{
			glob=-1;
			one_by_one(D2,D3,D1,0,n2,n3,n1);
		}
		else if(i==2)
		{	
			printf("i=1\n");
			error(array,D1,25,n1);
			glob=-1;
			one_by_one(D1,D3,D2,0,n1,n3,n1);
		}
		else if(i==3)
		{
			printf("i=2\n");
			error(array,D2,25,n2);
			glob=-1;
			one_by_one(D1,D2,D3,0,n1,n2,n3);
		}
		else if(i==4)
		{	
			printf("i=3\n");
			error(array,D3,25,n3);
			glob=-1;
			for(z=1;z<=25;z++)
			{

				//val1[z]=(val1[z]/3)*100;
				if(rate>val1[z])
				{
					rate=val1[z];
					k_val=z;
					printf("%d\n",z );
				}
			}
		}
	}
	return k_val;
}
void partion()
{
	int i,j,k,index1,index2;
	index1=(int)floor(n/3);
	index2=(int)floor(2*n/3);

	k=0;
	for (i = 0; i < index1; i++,k++)
	{
		for(j=0; j < m; j++)
		{
			D1[k][j]=Main1[i][j];
		}
	}
	n1=k;
	k=0;
	for (; i < index2; i++,k++)
	{
		for(j=0; j < m; j++)
		{
			D2[k][j]=Main1[i][j];
		}
	}
	n2=k;
	k=0;
	for (; i < n; i++,k++)
	{
		for(j=0; j < m; j++)
		{
			D3[k][j]=Main1[i][j];
		}
	}
	n3=k;
}
void print(long int A[][200],int n,int m)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			printf("%ld ",A[i][j]);
		}
		printf("\n%d*%d",n,m);
		printf("------------------------------\n------------------------------------------\n");
	}
}

int main()
{
	memset(val1,0,sizeof(val1));
	FILE *fstream1= fopen("shuffled.dat","r"); 
	scan(fstream1,Main1);
	partion();
	int k,j,sum=0;
	k=finding_k();
	printf("\nk value%d %f\n",k,val1[k]);
	double accuracy=(6670-val1[k])/6670;
	printf("%lf\n",accuracy);
	glob=-1;
	FILE *fstream2= fopen("pp_tes.dat","r"); 
	scan(fstream2,Main2);
	one_by_one(Main1,D2,Main2,k,col1,0,col2);
	error(array,Main2,k,col2);
	accuracy=(3333-val1[k])/3333*100;
	printf("Accuracy:%lf",accuracy);
	
	return 0;

} 
