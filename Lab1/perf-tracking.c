#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define CACHE_LINE_SIZE 64
#define BUF_SIZE 1024*1024*1024

//Global declarations
void *buff; //buffer to allocate 1GB memory to

//Function declarations
int buffer_allocate(int *fd, int isFileBacked, int mmap_flags);
int buffer_deallocate(int fd);


int buffer_allocate(int *fd, int isFileBacked, int mmap_flags) {

	if(isFileBacked) {
		if((*fd = open("tempFile.txt", O_CREAT | O_RDWR)) == -1) {
			perror("open failed!");
			exit(EXIT_FAILURE);
		}

		if(posix_fallocate(*fd, 0, BUF_SIZE) != 0) {
			perror("fallocate failed!");
			exit(EXIT_FAILURE);
		}	
		
	} 
	else
		*fd = -1;

	if((buff = (char *)mmap(NULL, BUF_SIZE, PROT_READ | PROT_WRITE, mmap_flags, *fd, 0)) == MAP_FAILED) {
		perror("mmap failed!");
		exit(EXIT_FAILURE);
	}

}

void main(int argc, char *argv[]) {
	
	int fd;

	//buffer_allocate(&fd, 1, MAP_SHARED);
	//printf("Mapping successfull!, size of buff = %llu\n", sizeof(buff));


	if(munmap(buff, BUF_SIZE) != 0) {  //Unmapping the memory
		perror("unmapping of memory failed!");
		exit(EXIT_FAILURE);
	}
	
	if(close(fd) < 0) {
		perror("File close failed");
		exit(EXIT_FAILURE);
	}
	
	exit(EXIT_SUCCESS);
}
