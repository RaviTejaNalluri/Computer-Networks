// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT 8080 


#include<stdlib.h>


void encode_mat(int **str_bin_mat,int l,char*str)
{
    int i,j,a;
    int bin[8]={0,0,0,0,0,0,0,0};
    for(i=0;i<l;i++)
    {
        
        a=str[i];
        int_bin(a,str_bin_mat[i]);
        // /str_bin_mat[i]=bin;  

    }
}

void print_mat(int **a,int l)
{
    int i,j;
    for(i=0;i<l;i++)
    {
        for(j=0;j<8;j++)
        {
            printf("%d",a[i][j]);
        }   
        printf("\n");
    }
}

void int_bin(int n ,int * bin)
{
    int j;
    for(j=0;j<8;j++)
    {
        bin[j]=0;
    }
    int r,i=0;
    while(n>0)
    {   
        r=n%2;
        bin[7-i]=r;
        i++;
        n=(int)(n/2);
    }
    // for(i=0;i<8;i++)
    // {
    //     printf("%d",bin[i]);
    // }

}
void matrix(int n,int **a)
{
    int i,j;
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            a[i][j]=rand()%2;
        }   
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
            //col[j]=col[j]^a[i][j];   

        }
    }
    for(j=0;j<8;j++)
    {
        for(i=0;i<l;i++)
        {
            //row[i]=row[i]^a[i][j];   
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
void induce(int x , int y,int**a,int l)
{
    int i,j;
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
    }
    printf("the error induced matrix looks like :\n");
    for(i=0;i<l;i++)
    {
        for(j=0;j<8;j++)
        {
            printf("%d ",a[i][j]);
        }   
        printf("\n");
    }
    
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
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	char *hello = "Hello from client"; 

/*
// rohans code
    int arr[10]={1,0,1,1,1,0,0,1,0,1};
    char arry[10];
    int i;
    int * new_arr=hamming_encode(arr);
    char string_new_arr[11];
    for(i=0;i<10;i++)
    {
        string_new_arr[i]=new_arr[i]+'0';
    }
// rohans code end
*/    

    
	char buffer[1024] = {0}; 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 
	//send(sock , hello , strlen(hello) , 0 ); 
    
    //rohans code start
    int n,i,j,x,y;
    




    char str[10];
    int counter=0;
    // while(counter<5)
    // {
    printf("enter a 10 char string \n");
    gets(&str);
    //puts(str);
    int l=strlen(str);
    int a;
    for(i=0;i<l;i++)
    {
        a=str[i];
        printf("%d ",a);
    }
    printf("\n");
    int** str_bin_mat=(int**)malloc(l*8*sizeof(int));
    for(i=0;i<l;i++)
    {
        str_bin_mat[i] =(int*)malloc(8*sizeof(int));
    }
    encode_mat(str_bin_mat,l,str);

    print_mat(str_bin_mat,l);








    
    //matrix(n,a);
    printf("enter x-coord to induce error at :\n");
    scanf("%d",&x);
    printf("enter y-coord to induce error at :\n");
    scanf("%d",&y);
    /*char string_new_arr[11];
    for(i=0;i<10;i++)
    {
        string_new_arr[i]=new_arr[i]+'0';
    }*/
    //send(sock,&n,sizeof(n),0);
    int *col=(int*)malloc(8*sizeof(int));
    int *row=(int*)malloc(l*sizeof(int));

    int* arr=(int*)malloc((8*l+l+8+1)*sizeof(int));

    check_parity(str_bin_mat,row,col,l);
    induce(x,y,str_bin_mat,l);

    int count=0;
    arr[count]=l;
    count++;
    for(i=0;i<l;i++)
    {
        for(j=0;j<8;j++)
        {
            arr[count]=str_bin_mat[i][j];
            count++;

        }   
    }
    i=0;
    while(i<l)
    {
        arr[count]=row[i];
        count++;
        i++;
    }
    i=0;
    while(i<8)
    {
        arr[count]=col[i];
        count++;
        i++;
    }
    for(j=0;j<(count);j++)
        {
            printf("%d ",arr[j]);

        }   
    // printf("%d\n",arr);
    send(sock,arr,count*sizeof(arr),0);
    //rohans code end

	printf("Hello message sent\n"); 
	valread = read( sock , buffer, 1024); 
	printf("%s\n",buffer ); 
	return 0; 

    clear_code(str_bin_mat,col,row,n);
    // count++;
    // }
} 
