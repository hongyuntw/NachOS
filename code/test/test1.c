#include "syscall.h"

main()
	{
		int a[3000];
		int	n,k;
		//printf("this is test 1\n");
		for (n=0;n<3000;n++){
			// a[n]=0;
			a[n] = n;
			// b[n] = n;
		}
		// for(n=0;n<2000;n++){
		// 	b[n] = a[n];
		// }
		// a[0] = 0;
		// b[1999] = 0;
		// for(k=0;k<1000;k++){
		// 	b[n] = a[n];
			// Sleep(n);
		// }
	}
