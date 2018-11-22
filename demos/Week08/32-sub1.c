/*
 * Copyright (C) 2018 Rahmat M. Samik-Ibrahim
 * You are free to SHARE (copy and redistribute the material in any medium or 
 * format) and to ADAPT (remix, transform, and build upon the material for any 
 * purpose, even commercially).  This program is distributed in the hope that 
 * it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * REV02 Wed Nov 21 20:49:00 WIB 2018
 * REV01 Thu Nov 15 12:27:16 WIB 2018
 * START Wed Nov 14 20:30:05 WIB 2018
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MYFLAGS      O_CREAT | O_RDWR
#define MYPROTECTION PROT_READ|PROT_WRITE
#define MYVISIBILITY MAP_SHARED
char*  sfile="demo-file.bin";

typedef  struct {
   sem_t  sync1;
   sem_t  sync2;
   int    share;
   int    loop;
} myshare;

myshare* mymap;

void flushprintf(char* tag1, char* tag2) {
   printf("%s[%s] S/L(%4d/%d) - P/PPID(%5d/%d)\n", tag1,
      tag2, mymap->share, mymap->loop, getpid(), getppid());
   fflush(NULL);
}

void main(int argc, char* argv[]) {
   int     fd   =open(sfile,MYFLAGS,S_IRWXU);
   int     ssize=sizeof(myshare);
   mymap=mmap(NULL, ssize, MYPROTECTION, MYVISIBILITY, fd, 0);
   sem_wait (&(mymap->sync1));
   flushprintf(argv[0], "PASS");
   sleep(1);
   mymap->share=2000;
   while (mymap->loop) {
      for(int ii=0; ii<1000000; ii++);
      mymap->share--;
   }
   sem_post (&(mymap->sync2));
   close(fd);
   flushprintf(argv[0], "EXIT");
}
