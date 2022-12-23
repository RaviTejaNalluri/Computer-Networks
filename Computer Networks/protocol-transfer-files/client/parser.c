#include<stdlib.h> 
#include<stdio.h>
#include<string.h>
#include <stdbool.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#define PORT 8080 
#include<sys/stat.h>




/* 

Unique header code

IndexGet Short :1
IndexGet Long :2
IndexGet RegEx :3

FileHash Verify:4

FileUpload :5
FileDownload :6

*/

int connection(char**a,int n,int k)
{
    int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
  


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
	
	
	// /printf("%ld\n",sizeof(a));
	int i=0;
	while(i<n)
	{
		send(sock,a[i],20,0);
		i++;
	}
	if(strcmp(a[0],"1")==0)
	{
		int numfiles;
		char name[20];
		valread=read(sock,&numfiles,4);
		int size1;
		char hash[34];
		char lastmod[50];

		for(i=0;i<numfiles;i++)
		{

			valread=read(sock,name,20);
			valread=read(sock,&size1,sizeof(int));
			valread=read(sock,hash,34);
			valread=read(sock,lastmod,50);
			printf("%s %d %s %s\n\n",name , size1,lastmod,hash);
			// printf("filename = %s\n",name);
			// printf("filesize = %d\n",size1);
			// printf("md5hash = %s\n",hash);
			// printf("last mod = %s",lastmod);
		}
			printf("Request Completed\n");

	}
	if(strcmp(a[0],"2")==0)
	{
		int numfiles;
		char name[20];
		valread=read(sock,&numfiles,4);
		int size1;
		char hash[34];
		char lastmod[50];

		for(i=0;i<numfiles;i++)
		{

			valread=read(sock,name,20);
			valread=read(sock,&size1,sizeof(int));
			valread=read(sock,hash,34);
			valread=read(sock,lastmod,50);
			printf("%s %d %s %s\n\n",name , size1,lastmod,hash);
			// printf("filename = %s\n",name);
			// printf("filesize = %d\n",size1);
			// printf("md5hash = %s\n",hash);
			// printf("last mod = %s",lastmod);
		}
			printf("Request Completed\n");

	}
	if(strcmp(a[0],"3")==0)
	{
		int numfiles;
		char name[20];
		char flag[20];

		valread=read(sock,&numfiles,4);
		int size1;
		char hash[34];
		char lastmod[50];
		valread=read(sock,flag,20);
		int count=0;
		while(strcmp(flag,"0")==0)
		{
			valread=read(sock,name,20);
			valread=read(sock,&size1,sizeof(int));
			valread=read(sock,hash,34);
			valread=read(sock,lastmod,50);
			printf("%s %d %s %s\n\n",name , size1,lastmod,hash);
			// printf("filename = %s\n",name);
			// printf("filesize = %d\n",size1);
			// printf("md5hash = %s\n",hash);
			// printf("last mod = %s",lastmod);
			valread=read(sock,flag,20);
		}
			printf("Request Completed\n");

	}

	if(strcmp(a[0],"4")==0)
	{
		char hash[34];
		char lastmod[50];
		int size1;
		valread=read(sock,&size1,sizeof(int));
		valread=read(sock,hash,34);
		valread=read(sock,lastmod,50);
		printf("filename = %s\n",a[1]);
		printf("filesize = %d\n",size1);
		printf("md5hash = %s\n",hash);
		printf("last mod = %s",lastmod);
		printf("Request Completed\n");


	}

	if(strcmp(a[0],"5")==0)
	{
		char msg[20];
		valread=read(sock,msg,20);
		if(strcmp(msg,"0")==0)
		{
			FILE * f = fopen (a[1], "rb");
			int length;
			char * buffer;
			if (f)
			{
				fseek (f, 0, SEEK_END);
				length = ftell (f);
				fseek (f, 0, SEEK_SET);
				buffer =(char *) malloc (length);
				if (buffer)
				{
					fread (buffer, 1, length, f);
				}
				fclose (f);
			}
			//printf("hello2\n");
			send(sock,buffer,length,0);
			valread=read(sock,msg,20);
			printf("%s\n",msg);
		}
		else if(strcmp(msg,"1")==0)
		{
			printf("File too large\n");
		}


		
		
	}
	if(strcmp(a[0],"6")==0)
	{
		char msg[20];
		valread=read(sock,msg,20);
		if(strcmp(msg,"1")==0)
		{
			printf("File Found\n");
			char msg2[20],size[20];
			int size1;
			//valread=read(new_socket,name,20);
			valread=read(sock,&size1,sizeof(int));
			if(size1>1024*1024)
			{
				printf("file too big\n");
				
			}
			else
			{
				//sscanf(size, "%d", &size1); 
				//printf("size= %s\n",size);
				printf("Downloading data");
				//printf("hello\n");
				//delay(10);
				char file[size1];
				valread=read(sock,file,size1);
				FILE* fptr = fopen(a[1],"a");
				if (fptr != NULL)
				{
					fputs(file, fptr);
					fclose(fptr);
				}
				printf("Download Completed\n");
			}
		}		
		
            

	}
	


	//valread = read( sock , buffer, 1024); 
	//printf("%s\n",buffer ); 
	return 0; 

}

void getIndexShort(char* start ,char* end)
{
	printf("get index short between %s and %s \n",start,end);

	int i;
	char** a=(char**)malloc(3*20*sizeof('a'));
	for(i=0;i<3;i++)
	{
		a[i]=(char*)malloc(20*sizeof('a'));
	}
	strcpy(a[0],"1");
	strcpy(a[1],start);
	strcpy(a[2],end);
	// /printf("%ld\n",sizeof(a));
	connection(a,3,1);
	for(i=0;i<3;i++)
	{
		free(a[i]);
	}
	free(a);

}
void getIndexLong()
{
	printf("get index long\n");
	int i;
	char** a=(char**)malloc(1*20*sizeof('a'));
	for(i=0;i<1;i++)
	{
		a[i]=(char*)malloc(20*sizeof('a'));
	}
	strcpy(a[0],"2");
	// /printf("%ld\n",sizeof(a));
	connection(a,1,2);
	for(i=0;i<1;i++)
	{
		free(a[i]);
	}
	free(a);
}
void getIndexRegx(char * type)
{
	printf("get index regx of type : %s\n",type);
	int i;
	char** a=(char**)malloc(2*20*sizeof('a'));
	for(i=0;i<2;i++)
	{
		a[i]=(char*)malloc(20*sizeof('a'));
	}
	strcpy(a[0],"3");
	strcpy(a[1],type);
	// /printf("%ld\n",sizeof(a));
	connection(a,2,3);
	for(i=0;i<2;i++)
	{
		free(a[i]);
	}
	free(a);
}
void fileHashVerify(char * name)
{
	//printf("verifying for %s\n",name);
	int i;
	char** a=(char**)malloc(2*20*sizeof('a'));
	for(i=0;i<2;i++)
	{
		a[i]=(char*)malloc(20*sizeof('a'));
	}
	strcpy(a[0],"4");
	strcpy(a[1],name);
	// /printf("%ld\n",sizeof(a));
	connection(a,2,4);
	for(i=0;i<2;i++)
	{
		free(a[i]);
	}
	free(a);
}
void fileUpload(char * name)
{
	//printf("uploading file %s\n",name);

	
	struct stat st; /*declare stat variable*/
     
    /*get the size using stat()*/
	char size[20];
     
    if(stat(name,&st)==0)
		sprintf(size, "%ld", st.st_size); 
    else
        printf("no\n");


	int i;
	char** a=(char**)malloc(3*20*sizeof('a'));
	for(i=0;i<3;i++)
	{
		a[i]=(char*)malloc(20*sizeof('a'));
	}
	//sprintf(size, "%d", res); 
	//printf("%s\n",size);
	strcpy(a[0],"5");
	strcpy(a[1],name);
	strcpy(a[2],size);
	connection(a,3,5);
	for(i=0;i<3;i++)
	{
		free(a[i]);
	}
	free(a);
}
void fileDownload(char * name)
{
	printf("Download file %s\n",name);
	int i;
	char** a=(char**)malloc(2*20*sizeof('a'));
	for(i=0;i<2;i++)
	{
		a[i]=(char*)malloc(20*sizeof('a'));
	}
	strcpy(a[0],"6");
	strcpy(a[1],name);

	// /printf("%ld\n",sizeof(a));
	connection(a,2,6);
	for(i=0;i<2;i++)
	{
		free(a[i]);
	}
	free(a);
}
int main(void) 
{ 
	char name[20];
	//printf("Enter name :");
	//scanf("%s",name);
	while(1)
	{
		int n=5;
		char temp;
		int i=0;
		int a[n];
		char s[10*n];
		//printf("enter the numbers\n");
		printf("\nREQUEST:  \n");
		// scanf("%c", &temp);
		fgets(s,10*n,stdin);
		//printf("%s\n",s);
		strtok(s,"\n");
		char* token = strtok(s, " ");
		char str[5][20]; 
    	while(token!=NULL)
    	{
			strcpy(str[i],token);
			//printf("%s %s \n",str[i],token);
        	i++;
        	token = strtok(NULL, " "); 
    	}

		strcpy(str[i],"NULL");
		//printf("hi\n");
		i=0;
		
		if(strcmp(str[0],"IndexGet" )==0)
		{
			if(strcmp(str[1],"ShortList" )==0)
			{
				printf("RESPONSE:  \n");
				//char a[3][20];
				getIndexShort(str[2],str[3]);
			}
			else if (strcmp(str[1],"LongList" )==0)
			{
				printf("RESPONSE:  \n");
				getIndexLong();
			}
			else if(strcmp(str[1],"RegEx" )==0)
			{
				printf("RESPONSE:  \n");
				getIndexRegx(str[2]);
			}
			
		}
		else if(strcmp(str[0],"FileHash" )==0)
		{
			if(strcmp(str[1],"Verify" )==0)
			{
				printf("RESPONSE:  \n");
				fileHashVerify(str[2]);
			}
		}
		else if(strcmp(str[0],"FileDownload" )==0)
		{
			printf("RESPONSE:  \n");
			fileDownload(str[1]);
		}
		else if(strcmp(str[0],"FileUpload" )==0)
		{
			printf("RESPONSE:  \n");
			fileUpload(str[1]);
		}
		
		
		

	}
	
	return 0; 
} 
