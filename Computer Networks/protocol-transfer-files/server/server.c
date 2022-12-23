// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include<time.h>
#include<sys/stat.h>
#include"md5.h"
#include <sys/types.h>
#include <stdbool.h>
#include <dirent.h>



#define PORT 8080 
int countfiles(char *path) {
    DIR *dir_ptr = NULL;
    struct dirent *direntp;
    char *npath;
    if (!path) return 0;
    if( (dir_ptr = opendir(path)) == NULL ) return 0;

    int count=0;
    while( (direntp = readdir(dir_ptr)))
    {
        if (strcmp(direntp->d_name,".")==0 ||
            strcmp(direntp->d_name,"..")==0) continue;
        switch (direntp->d_type) {
            case DT_REG:
                ++count;
                break;
            case DT_DIR:            
                npath=malloc(strlen(path)+strlen(direntp->d_name)+2);
                sprintf(npath,"%s/%s",path, direntp->d_name);
                count += countfiles(npath);
                free(npath);
                break;
        }
    }
    closedir(dir_ptr);
    return count;
}

char* nametofile(char name[])
{
    FILE * f = fopen (name, "rb");
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
    return buffer;

}

static time_t getFileCreationTime(char *path,char lastmod[]) {
    struct stat attr;
    stat(path, &attr);
    strcpy(lastmod,ctime(&attr.st_mtime));
    return attr.st_mtime;
    // /printf("Last modified time: %s",ctime(&attr.st_mtime) );
}

void getIndexShort()
{
    

}
void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Storing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not achieved 
    while (clock() < start_time + milli_seconds) 
        ; 
} 
  
int main(int argc, char const *argv[]) 
{ 
    printf("\n\n\nListening...\n\n\n");

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
	while((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))>0) 
    {
        if (new_socket<0) 
        { 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        } 
        //send(new_socket , "Menu\n1)" , strlen("Menu\n1)") , 0 );
        char sizec;
        int i,j;
        char n[20];
        valread=read(new_socket,n,20);
        if(strcmp(n,"1")==0)
        {
            printf("%s\n","IndexGet Request");
            char t1[20],t2[20];
            int t1i,t2i;
            valread=read(new_socket,t1,20);
            valread=read(new_socket,t2,20);
            printf("time between %s and %s\n",t1,t2);
            sscanf(t1,"%d",&t1i);
            sscanf(t2,"%d",&t2i);
            int count=0;
            DIR *d;
            struct dirent *dir;
            d = opendir(".");
            if (d)
            {
                int filecount=countfiles(".");
                printf("%d\n",filecount);
                send(new_socket,&filecount,sizeof(int),0);
                while ((dir = readdir(d)) != NULL)
                {
                    printf("\n%s", dir->d_name);
                    if(count>1)
                    {
                        struct stat st; /*declare stat variable*/
                        char size[20];
                        int size1;
                        char hash[34]="";
                        char lastmod[50];

                        

                        //time_t time1= getFileCreationTime(dir->d_name,lastmod);
                        //if((difftime(time1,t1)>=0)&&(difftime(time1,t2)<=0))
                        //{
                            if(stat(dir->d_name,&st)==0)
                            sprintf(size, "%ld", st.st_size); 
                            else
                                printf("no\n");
                            sscanf(size,"%d",&size1);
                            hashfn(nametofile(dir->d_name),hash,size1);
                            printf("%s\n",size);
                            printf("%s\n",hash);
                            printf("last mod : %s\n",lastmod);
                            getFileCreationTime(dir->d_name,lastmod);
                            send(new_socket,dir->d_name,20,0);
                            send(new_socket,&size1,sizeof(size1),0);
                            send(new_socket,hash,34,0);
                            send(new_socket,lastmod,50,0);
                        // }

                    }
                    count++;
                }
                closedir(d);
            }

        }
        
        else if(strcmp(n,"2")==0)
        {
            printf("%s\n","IndexGet Request");
            int count=0;
            DIR *d;
            struct dirent *dir;
            d = opendir(".");
            if (d)
            {
                int filecount=countfiles(".");
                //printf("%d\n",filecount);
                send(new_socket,&filecount,sizeof(int),0);
                while ((dir = readdir(d)) != NULL)
                {
                    //printf("\n%s", dir->d_name);
                    if(count>1)
                    {
                        struct stat st; /*declare stat variable*/
                        char size[20];
                        int size1;
                        char hash[34]="";
                        char lastmod[50];

                        if(stat(dir->d_name,&st)==0)
                            sprintf(size, "%ld", st.st_size); 
                        else
                            printf("no\n");
                        sscanf(size,"%d",&size1);
                        hashfn(nametofile(dir->d_name),hash,size1);
                        //printf("%s\n",size);
                        //printf("%s\n",hash);
                        getFileCreationTime(dir->d_name,lastmod);
                        //printf("last mod : %s\n",lastmod);
                        send(new_socket,dir->d_name,20,0);
                        send(new_socket,&size1,sizeof(size1),0);
                        send(new_socket,hash,34,0);
                        send(new_socket,lastmod,50,0);

                    }
                    count++;
                }
                closedir(d);
            }
                    
            
        }
        else if(strcmp(n,"3")==0)
        {
            printf("%s\n","IndexGet Request");
            char type[20];
            valread=read(new_socket,type,20);

            int count=0;
            DIR *d;
            struct dirent *dir;
            d = opendir(".");
            if (d)
            {
                int filecount=countfiles(".");
                printf("%d\n",filecount);
                send(new_socket,&filecount,sizeof(int),0);
                while ((dir = readdir(d)) != NULL)
                {
                    //printf("\n%s", dir->d_name);
                    if(count>1)
                    {
                        struct stat st; /*declare stat variable*/
                        char size[20];
                        int size1;
                        char hash[34]="";
                        char lastmod[50];
                        getFileCreationTime(dir->d_name,lastmod);

                        char *dot = strrchr(dir->d_name, '.');
                        if (dot && !strcmp(dot, type))
                        {
                            send(new_socket,"0",20,0);
                            printf("%s\n",dir->d_name);
                            if(stat(dir->d_name,&st)==0)
                            sprintf(size, "%ld", st.st_size); 
                            else
                                printf("no\n");
                            sscanf(size,"%d",&size1);
                            hashfn(nametofile(dir->d_name),hash,size1);
                            printf("%s\n",size);
                            printf("%s\n",hash);
                            printf("last mod : %s\n",lastmod);
                            send(new_socket,dir->d_name,20,0);
                            send(new_socket,&size1,sizeof(size1),0);
                            send(new_socket,hash,34,0);
                            send(new_socket,lastmod,50,0);
                        }

                    }
                    count++;
                }
                send(new_socket,"1",20,0);

                closedir(d);
            }

        }
        
        else if(strcmp(n,"4")==0)
        {
            printf("FileHash Verify Request\n");

            char name[20],size[20];
            int size1;
            valread=read(new_socket,name,20);
            char lastmod[50];
            
            //if(name)
            //{
	            struct stat st; /*declare stat variable*/

                if(stat(name,&st)==0)
                    sprintf(size, "%ld", st.st_size); 
                else
                    printf("no\n");
                sscanf(size,"%d",&size1);
                char hash[34]="";
                
                hashfn(nametofile(name),hash,size1);

                //printf("%ld\n",strlen(hash));
                // printf("filename = %s\n",name);
                // printf("filesize = %d\n",size1);
                // printf("md5hash = %s\n",hash);
                getFileCreationTime(name,lastmod);
                //printf("last mod = %s",lastmod);

                send(new_socket,&size1,sizeof(size1),0);
                send(new_socket,hash,34,0);
                send(new_socket,lastmod,50,0);


            //}
            
            
        }
        else if(strcmp(n,"5")==0)
        {
            printf("%s\n","FileUpload Request");

            char name[20],size[20];
            int size1;
            valread=read(new_socket,name,20);
            valread=read(new_socket,size,20);
            //printf("name= %s\n",name);
            sscanf(size, "%d", &size1); 
            //printf("size= %s\n",size);
            if(size1>1024*1024)
            {
                send(new_socket,"1",sizeof("1"),0);
            }
            else
            {
                send(new_socket,"0",sizeof("0"),0);
                //printf("hello\n");
                //delay(10);
                char file[size1];
                valread=read(new_socket,file,size1);
                FILE* fptr = fopen(name,"w");
                if (fptr != NULL)
                {
                    fputs(file, fptr);
                    //fseek(fptr, 0, SEEK_SET);
                    fclose(fptr);
                }
                //char* hash=(char*)malloc(sizeof(char)*34);
                char hash[34]="";
                hashfn(file,hash,size1);
                //printf("%ld\n",strlen(hash));
                printf("md5hash = %s\n",hash);
                //free(hash);
                char lastmod[50];
                getFileCreationTime(name,lastmod);
                printf("last mod = %s",lastmod);
                send(new_socket,"Request Completed",sizeof("Request Completed"),0);

            }
            

        }
        else if(strcmp(n,"6")==0)
        {
            printf("%s\n","FileDownload Request");

            char name[20];
            valread=read(new_socket,name,20);
            //if(name) //check if name exists in file and then check size 
            send(new_socket,"1",sizeof("1"),0);
            

            struct stat st; /*declare stat variable*/
     
            /*get the size using stat()*/
            char size[20];
            
            if(stat(name,&st)==0)
                sprintf(size, "%ld", st.st_size); 
            else
                printf("no\n");

            FILE * f = fopen (name, "rb");
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
            sscanf(size,"%d",&length);
            send(new_socket,&length,4,0);

            if(length>1024*1024)
		    {
			    
		    }
		    else
		    {
                send(new_socket,buffer,length,0);
            }
        }
        
        //rohan's code
        

        
        printf("Response Sent!\n"); 

        printf("\n\n\nListening...\n\n\n");
    }

        
        //clear_code(arr,row,col,n);
	return 0; 
} 
