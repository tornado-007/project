#include<fcntl.h>
#define NULL 0
int close(int);
int fork(void );
unsigned int sleep(unsigned int );
unsigned int read(int,void *,unsigned int);
unsigned int write(int ,void *,unsigned int);
int printf(const char * ,...);
#define LEN 4000
void main()
{
char *buf;
int device;
//close(1);
device=open("/dev/DemoDev",O_RDWR|O_CREAT,0646);
if(device<0)
printf("file is not opened\n");
buf=malloc(LEN);
if(buf==NULL)
printf("memory no allocated");

//if(fork()!=0){
//printf("buffer %x and len%d\n",buf,10);
//dup(device);
if(read(device,buf,LEN)<=0)
{
printf("NO reading is done\n");
}
//intf("%s",buf);
//}

//else
//{
sleep(10);
if(write(device,buf,10)<=0)
{
printf("NO Writing is done");
}
sleep(10);
//system("ps -el");
close(device);
//}
}
