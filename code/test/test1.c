#include "syscall.h"
int a[500];
int b[500];
main()
	{
		int	n;
		//printf("this is test 1\n");
		for (n=0;n<500;n++){
			a[n]=0;
			b[500-n] = 1;
		}
			
			// Sleep(n);
	}
