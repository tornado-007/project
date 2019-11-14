#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#define LEN 8000

static struct kpidnice
{
unsigned int pid;
int nice;
}kpid;
void main()
{
	char *buf;
	int device;                                    //POSSIBLILITY TO HAVE NEGATIVE VALUE
        unsigned int len=0;
	device=open("/dev/Demo",O_RDWR);
	if(device<0)
	{
		printf("file is not opened\n");
		return;
	}
		printf("file is start opening\n");
	buf=malloc(LEN);
	if(buf==NULL)
	{
		printf("memory no allocated");
		return;
	}
		printf("memory allocated\n");
	if((len=read(device,buf,LEN))<=0)
	{
		printf("NO reading is done\n");
		return;
	}

		printf(" reading is done with %d\n",len);
		printf("%s\n",buf);
        printf("PLEASE Enter PID\n");
        scanf("%u",&kpid.pid);
        printf("PLESE Enter PRIRITY\n");
        scanf("%d",&kpid.nice);

	if(write(device,(void*)&kpid,sizeof(struct kpidnice))<=0)
	{
		printf("NO Writing is done");
		return;
	}
        free(buf);
	close(device);
}
