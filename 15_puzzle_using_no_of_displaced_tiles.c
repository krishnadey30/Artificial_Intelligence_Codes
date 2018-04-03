#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
long long int nodes_generated=0,nodes_present=0;
int N = 4;
int final[4][4]={
			    {1, 2, 3, 4},
			    {5, 6, 7, 8},
			    {9, 10, 11, 12},
			    {13, 14, 15, 0}
			};
typedef struct node
{
	int cost;
	struct node *parent;
	int Mat[4][4];
	int level;
	int x,y;
}Node;

typedef struct linked
{
	Node * node;
	struct linked *next;
}list;
list *head=NULL;
list *closed_head=NULL;

int row[] = { 1, 0, -1, 0 };
int col[] = { 0, -1, 0, 1 };

int isSafe(int x, int y)
{
    return (x >= 0 && x < N && y >= 0 && y < N);
}

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

int compare_matrix(int mat1[N][N],int mat2[N][N])
{
	for (int i = 0; i < N; i++)
	{
      	for (int j = 0; j < N; j++)
      	{
        	if (mat1[i][j] != mat2[i][j])
	        {
	           return 0;
	        }
    	}
	}

    return 1;
}

int check_open_list(Node *child)
{
	list *temp=head;
	if(temp!=NULL)
	{
		if(compare_matrix(head->node->Mat,child->Mat))
		{
			if((head->node->cost+head->node->level)>child->cost+child->level)
			{
				head=head->next;
				nodes_present--;
				return 1;
			}
			return 2;
		}
		else
		{
			while(temp->next!=NULL)
			{
				if(!compare_matrix(temp->next->node->Mat,child->Mat))
				{
					temp=temp->next;
				}
				else
				{
					if((temp->next->node->cost+temp->next->node->level)>child->cost+child->level)
					{
						nodes_present--;
						temp->next=temp->next->next;
						return 1;
					}
					return 2;
				}
			}
		}
	}
	return 0;
}

int check_closed_list(Node *child)
{
	list *temp=closed_head;
	if(temp!=NULL)
	{
		if(compare_matrix(closed_head->node->Mat,child->Mat))
		{
			if((closed_head->node->cost+closed_head->node->level)>child->cost+child->level)
			{
				closed_head=closed_head->next;
				return 1;
			}
			return 2;
		}
		else
		{
			while(temp->next!=NULL)
			{
				if(!compare_matrix(temp->next->node->Mat,child->Mat))
				{
					temp=temp->next;
				}
				else
				{
					if((temp->next->node->cost+temp->next->node->level)>child->cost+child->level)
					{
						temp->next=temp->next->next;
						return 1;
					}
					return 2;
				}
			}
		}
	}
	return 0;
}

void printMatrix(int mat[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            printf("%d ", mat[i][j]);
        printf("\n");
    }
}
 
void print_path(Node* root)
{
    if (root == NULL)
        return;
    print_path(root->parent);
    printMatrix(root->Mat);
 
    printf("\n");
}

Node* create(int Mat[N][N], int x, int y, int newX, int newY, int level, Node* parent)
{
	nodes_generated++;
    Node* node = (Node *)malloc(sizeof(Node));
    node->parent = parent;
    memcpy(node->Mat, Mat, sizeof(node->Mat));
    swap(&(node->Mat[x][y]), &(node->Mat[newX][newY]));
    node->cost = 0;
    node->level = level;
    node->x = newX;
    node->y = newY;
    return node;
}

int calculate_misplased_tiles(int Mat[N][N])
{
	int count = 0;
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
        if (Mat[i][j] && Mat[i][j] != final[i][j])
           count++;
    return count;
}

list *create_list(Node *root)
{
	nodes_present++;
	if(head==NULL)
	{
		head=(list *)malloc(sizeof(list));
		head->node=root;
		head->next=NULL;
	}
	else
	{
		list *temp=head,*new;
		new=(list *)malloc(sizeof(list));
		new->node=root;
		new->next=NULL;
		int cost=(new->node->cost+new->node->level);
		if((head->node->cost+head->node->level)>cost)
		{
			new->next=head;
			head=new;
		}
		else
		{
			while(temp->next!=NULL && (temp->next->node->cost+temp->next->node->level)<=cost)
			{
				temp=temp->next;
			}
			new->next=temp->next;
			temp->next=new;
		}
	}
}

Node *extract_min()
{
	nodes_present--;
	list *temp=head;
	head=head->next;
	return temp->node;
}
void solve(int Mat[N][N], int x, int y)
{
	Node *root=create(Mat,x,y,x,y,0,NULL);
	root->cost = calculate_misplased_tiles(Mat);
	create_list(root);
	while(head!=NULL)
	{
		Node* min = extract_min();

		if(min->cost==0)
		{
			print_path(min);
			return;
		}
		for (int i = 0; i < 4; i++)
        {
            if (isSafe(min->x + row[i], min->y + col[i]))
            {
                Node* child = create(min->Mat, min->x, min->y, min->x + row[i], min->y + col[i], min->level + 1, min);
                printMatrix(child->Mat);
                printf("\n");
                child->cost = calculate_misplased_tiles(child->Mat);
                int o=check_open_list(child);
                int c=check_closed_list(child);
                if(o==1 || o==2 )
                {
                	if(o==1)
                		create_list(child);
                }
                else if(c==1 || c==2)
                {
                	if(c==1)
                		create_list(child);
                }
                else
                {
                	create_list(child);
                }
            }
        }
        list *X=(list *)malloc(sizeof(list));
        X->node=min;
        X->next=closed_head;
        closed_head=X;
	}
	//printMatrix(head->node->Mat);
}
int getInvCount(int arr[])
{
    int inv_count = 0;
    for (int i = 0; i < N * N - 1; i++)
    {
        for (int j = i + 1; j < N * N; j++)
        {
            if (arr[j] && arr[i] && arr[i] > arr[j])
                inv_count++;
        }
    }
    return inv_count;
}

int findXPosition(int Mat[N][N])
{
    for (int i = N - 1; i >= 0; i--)
        for (int j = N - 1; j >= 0; j--)
            if (Mat[i][j] == 0)
                return N - i;
}

int isSolvable(int puzzle[N][N])
{
    int invCount = getInvCount((int*)puzzle);
    if (N & 1)
        return !(invCount & 1);
 
    else 
    {
        int pos = findXPosition(puzzle);
        if (pos & 1)
            return !(invCount & 1);
        else
            return invCount & 1;
    }
}
int main()
{
	printf("Please Enter the Matrix\n");
	int Mat[N][N],x,y;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			scanf("%d",&Mat[i][j]);
			if(Mat[i][j]==0)
			{
				x=i;
				y=j;
			}
		}
	}
	// printMatrix(Mat);
	// printf("%d %d\n",x,y );
	printf("\n");
	if(isSolvable(Mat))
		solve(Mat, x, y);
	else
		printf("Puzzle is Not Solvable\n");
	printf("No of Nodes Generated = %lld\n",nodes_generated);
	printf("No of Nodes Present = %lld\n",nodes_present);
	return 0;
}