#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int A[6700][200];
int count[10][66670][200];
void fill_array(char c[],int i,int A[][200])
{
	int j,k=0;
	for(j=0;j<strlen(c);j=j+2)
	{
		A[i][k]=(int)c[j]-'0';
		k++;
	}
}
int main()
{
	char c[500];
	int i=0,j;

	FILE *ptr_file=fopen("shuffled.dat", "r");
	while(fscanf(ptr_file,"%[^\n]\n",c)!= EOF)
	{
		fill_array(c,i,A);
  		i++;
	}
	fclose(ptr_file);
	for(i=0;i<10;i++)
	{
		for(j=0;j<6670;j++)
		{
			
		}
	}

	return 0;
}