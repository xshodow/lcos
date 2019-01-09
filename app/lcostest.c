#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define LCOS_COMD_POWERON   0x100001
#define LCOS_COMD_POWEROFF  0x100002

void print_usage(char *cmd)
{
    printf("Usage:\n");
    printf("%s on\n", cmd);
    printf("%s off\n", cmd);
}

int main(int argc, char **argv)
{
    int do_on=0;
    int do_off=0;
    int fd=0;
    int ret=0;

    if (argc == 2 && !strcmp(argv[1], "on"))
        do_on = 1;
    if (argc == 2 && !strcmp(argv[1], "off"))
        do_off= 1;
    fd = open("/dev/HX7097N", O_RDWR);
    if (fd < 0)
    {
        printf("can't open /dev/HX7097N \n");
        return -1;
    }
    if (do_on) 
    {
    	ret = ioctl(fd,LCOS_COMD_POWERON );
			if (ret == -1)
			printf("LCOS_COMD_POWERON is failed.\n");
    	
    }
    
		if (do_off) 
    {
    	ret = ioctl(fd,LCOS_COMD_POWEROFF );
			if (ret == -1)
			printf("LCOS_COMD_POWEROFF is failed.\n");
    	
    }
	  close(fd);
    return 0;
}







