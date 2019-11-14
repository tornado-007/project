#include<kvm.h>
#include<fcntl.h>
#include<stdio.h>
//#include <kernel.h>
typedef int kvm_t;

kvm_t * kvm_openfiles(const char *execfile, const char *corefile,const char *swapfile, int flags, char *errbuf);
int main()
{
kvm_t *kd;
char * nlistf, *memf,*swapf,*errbuf;
nlistf=memf=swapf=NULL;

 if (nlistf == NULL && memf == NULL && swapf == NULL) {
                   printf("no executable ans core and swapfile");
                kd = kvm_openfiles(NULL, NULL, NULL, O_RDONLY, errbuf);
        } else {
                kd = kvm_openfiles(nlistf, memf, swapf, O_RDONLY, errbuf);
        }
      if (kd == NULL)
           printf("No strucrtured is returned\n");


//kvm_getprocs();



return(0);
}
