// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 



int bin_int(int *bin)
{
    int i,j,pow=1;
    int num=0;
    for(i=7;i>=0;i--)
    {
        num=num+pow*bin[i];
        pow=pow*2;
    }
    return num;
}

void decode_mat(int **str_bin_mat,int l,char * alpha)
{
    int i,j,a;
    //char alpha[l];
    int bin[8]={0,0,0,0,0,0,0,0};
    for(i=0;i<l;i++)
    {
        a=bin_int(str_bin_mat[i]);
        //printf("%d ",a);
        alpha[i]=a;
        //printf("%c",a);   
        //puts(alpha);
        // /str_bin_mat[i]=bin;  

    }
    alpha[i]=NULL;
}


void check_parity(int**a,int *row,int* col,int l)
{
    int i,j;
    row[0]=0;
    col[0]=0;

    for(i=0;i<l;i++)
    {
        for(j=0;j<8;j++)
        {
            row[i]=row[i]^a[i][j];   
            col[j]=col[j]^a[i][j];   

        }
    }
    printf("col:\n");
    for(i=0;i<8;i++)
    {
        printf("%d ",col[i]);
    }
    printf("\n");
    
}
void clear_code(int**a,int*col,int*row,int l)
{
    int i;
    for(i=0;i<l;i++)
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
    int i,j,n,l;
    int counter=0;
    //while(counter<5)
    //{
    valread=read(new_socket,&l,4);
    
    printf("n== %d\n",l);
    //rohan's code
    int **a = (int**)malloc(sizeof(int)*l*8);
    for(i=0;i<l;i++)
    {
        int* b=(int *)malloc(sizeof(int)*8);
        a[i]=b;
    }
    int* arr=(int*)malloc((l*8+l+8)*sizeof(int));
    char val;
    valread=read(new_socket,arr,4*(l*8+l+8));
    int count=0;
    //count++;
    for(i=0;i<l;i++)
    {
        for(j=0;j<8;j++)
        {
            a[i][j]=arr[count];
            count++;
        }   
    }
    
    int *col=(int*)malloc(8*sizeof(int));
    int *row=(int*)malloc(l*sizeof(int));
    int *cols=(int*)malloc(8*sizeof(int));
    int *rows=(int*)malloc(l*sizeof(int));
    i=0;
    while(i<l)
    {
        row[i]=arr[count];
        count++;
        i++;
    }
    i=0;
    while(i<8)
    {
        col[i]=arr[count];
        count++;
        i++;
    }

    printf("the matrix looks like :\n");
    for(i=0;i<l;i++)
    {
        for(j=0;j<8;j++)
        {
            printf("%d ",a[i][j]);
        }   
        printf("\n");
    }
    check_parity(a,rows,cols,l);
    int x=0,y=0;
    for(i=0;i<l;i++)
    {
        //printf("%d ",col[i]);
        if(rows[i]!=row[i])
        {
            x=i;
            break;
        }

    }
    for(i=0;i<8;i++)
    {
        if(cols[i]!=col[i])
        {
            y=i;
            break;
        }
        
    }
    for(i=0;i<l;i++)
    {
        for(j=0;j<8;j++)
        {
            if(i==x && j==y)
            {
                if(a[i][j]==0)
                {
                    a[i][j]=1;
                }
                else
                {
                    a[i][j]=0;
                }
                
            }
        }   
        //printf("\n");
    }
    printf("new matrix :\n");
    for(i=0;i<l;i++)
    {
        for(j=0;j<8;j++)
        {
            printf("%d ",a[i][j]);
        }   
        printf("\n");
    }
    printf("position of error is (%d,%d)\n",x,y);

    char * alpha[l];
    decode_mat(a,l,alpha);
    printf("msg is  : ");
    puts(alpha);
    printf("\n");
    /*for(int i=0;i<size;i++)
    {
        printf("%d ",arr[i]);
    }
    printf("\n");*/
    

    //rohans code end

	printf("%s\n",buffer ); 
	send(new_socket , hello , strlen(hello) , 0 ); 
	printf("Hello message sent\n"); 
    clear_code(arr,row,col,n);
    //counter++;
    //}
	return 0; 
} 
