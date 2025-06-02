//Writter
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define SHM_SIZE 1024

int main()
{
	key_t key = ftok("shmfile",1096);
	int shmid = shmget(key,SHM_SIZE,IPC_CREAT|0666);
	if(shmid == -1)
	{
		perror("shmget");
		exit(1);
	}
	char * shmaddr = (char*) shmat(shmid,NULL,0);
	if(shmaddr == (char*)-1)
	{
		perror ("shmat");
		exit(1);
	}
	printf("Write data: ");
	fgets(shmaddr,SHM_SIZE,stdin);
	printf("Data written in memory : %s\n",shmaddr);
	shmdt(shmaddr);
	return 0;
}

//reader
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define SHM_SIZE 1024

int main()
{
	
	key_t key = ftok("shmfile",1096);
	int shmid = shmget(key,SHM_SIZE,0666);
	if(shmid == -1)
	{
		perror("shmget");
		exit(1);
	}
	char * shmaddr = (char*) shmat(shmid,NULL,0);
	if(shmaddr == (char*)-1)
	{
		perror ("shmat");
		exit(1);
	}
	printf("Data read from memory : %s\n",shmaddr);
	shmdt(shmaddr);
	shmctl(shmid,IPC_RMID,NULL);
	return 0;
}