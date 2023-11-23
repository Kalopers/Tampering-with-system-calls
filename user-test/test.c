#include<stdio.h>
#include<sys/time.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, char *argv[]){
    long long arg1 = atoi(argv[1]);
    long long arg2 = atoi(argv[2]);
    printf("a=%lld, b=%lld\n", arg1, arg2);
    long long ret = syscall(96, arg1, arg2); 
    if(ret != (arg1 + arg2)) {
	    printf("Failed: %lld\n", ret);
        perror("syscall");
    } else {
        printf("ret = %lld\n", ret);
    }
    return 0;
}
