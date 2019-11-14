#include<fcntl.h>

void main()
{
char buf[10]={0};
int device;
//close(1);
device=open("/dev/DemoDevfile",O_RDWR|O_CREAT,0646);
if(device<0)
printf("file is not opened\n");

printf("buffer %x and len%d\n",buf,10);
//dup(device);
if(read(device,buf,10)<=0)
{
printf("NO reading is done");
}

printf("%s",buf);

sleep(10);
if(write(device,buf,10)<=0)
{
printf("NO Writing is done");
}
sleep(10);
//system("ps -el");
close(device);
}
