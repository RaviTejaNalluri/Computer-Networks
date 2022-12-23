// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 


void check_parity(int**a,int *row,int* col,int n)
{
    int i,j;
    row[0]=0;
    col[0]=0;

    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            row[i]=row[i]^a[i][j];   
            col[j]=col[j]^a[i][j];   

        }
    }
    printf("row:\n");
    for(i=0;i<n;i++)
    {
        printf("%d ",col[i]);
    }
    printf("\n");
    
}
void clear_code(int**a,int*col,int*row,int n)
{
    int i;
    for(i=0;i<n;i++)
    {
        free(a[i]);
    }
    free(a);
}
int main(int argc, char const *argv[]) 
{ 
	int server_fd, new_socket, valread; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	char buffer[1024] = {0}; 

	char *hello = "Hello from server"; 
	
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 
    char sizec;
    int i,j,n;
    valread=read(new_socket,&n,4);
    printf("n== %d\n",n);
    //rohan's code
    int **a = (int**)malloc(sizeof(int)*n*n);
    for(i=0;i<n;i++)
    {
        int* b=(int *)malloc(sizeof(int)*n);
        a[i]=b;
    }
    int* arr=(int*)malloc((n*n+2*n)*sizeof(int));
    char val;
    valread=read(new_socket,arr,4*(n*n+2*n));
    int count=0;
    //count++;
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            a[i][j]=arr[count];
            count++;
        }   
    }
    
    int *col=(int*)malloc(n*sizeof(int));
    int *row=(int*)malloc(n*sizeof(int));
    int *cols=(int*)malloc(n*sizeof(int));
    int *rows=(int*)malloc(n*sizeof(int));
    i=0;
    while(i<n)
    {
        row[i]=arr[count];
        count++;
        i++;
    }
    i=0;
    while(i<n)
    {
        col[i]=arr[count];
        count++;
        i++;
    }

    printf("the matrix looks like :\n");
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            printf("%d ",a[i][j]);
        }   
        printf("\n");
    }
    check_parity(a,rows,cols,n);
    int x=0,y=0;
    for(i=0;i<n;i++)
    {
        //printf("%d ",col[i]);
        if(rows[i]!=row[i])
        {
            x=i;
            break;
        }

    }
    for(i=0;i<n;i++)
    {
        if(cols[i]!=col[i])
        {
            y=i;
            break;
        }
        
    }
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            if(i==x && j==y)
            {
                if(a[i][j]==0)
                {
                    a[i][j]=1;
                }
                else
                {
                    a[i][j]=1;
                }
                
            }
        }   
        printf("\n");
    }
    printf("new matrix :\n");
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            printf("%d ",a[i][j]);
        }   
        printf("\n");
    }
    printf("position of error is (%d,%d)",x,y);
    /*for(int i=0;i<size;i++)
    {
        printf("%d ",arr[i]);
    }
    printf("\n");*/
    

    //rohans code end

	printf("%s\n",buffer ); 
	send(new_socket , hello , strlen(hello) , 0 ); 
	printf("Hello message sent\n"); 
    //clear_code(arr,row,col,n);
	return 0; 
} 
