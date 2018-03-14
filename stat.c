#pragma runopts(posix(on))
#define _OPEN_SYS
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>

#define CHECK_FOR(CONDITION,WHAT)               \
  if ((CONDITION)){                             \
    printf("%s" #WHAT, (prior ? ", " : ""));    \
    prior = true;                               \
  }

#define CHECK_MODE_FOR(FLAG,WHAT)               \
  if ((FLAG(info->st_mode))){                   \
    printf("%s" #WHAT, (prior ? ", " : ""));    \
    prior = true;                               \
  }

static
void whatItIs(struct stat* info){
  printf(" kind: ");
  bool prior = false;
  CHECK_MODE_FOR(S_ISBLK, block special file);
  CHECK_MODE_FOR(S_ISCHR, character special file);
  CHECK_MODE_FOR(S_ISDIR, directory);
  CHECK_MODE_FOR(S_ISFIFO, pipes or FIFO special file);
  CHECK_MODE_FOR(S_ISLNK, symbolic link);
  CHECK_MODE_FOR(S_ISREG, regular file);
  CHECK_MODE_FOR(S_ISSOCK, socket);
  CHECK_FOR((S_ISEXTL(info->st_mode,info->st_genvalue)), external link);
  printf("\n");
}

static
void showInfo(struct stat* info, const char* function, const char* fname)
{
  printf("%s() returned the following information about %s:\n",
         function, fname);
  whatItIs(info);
  printf(" inode: %d\n", (int) info->st_ino);
  printf(" dev id: %d\n", (int) info->st_dev);
  printf(" mode: %08x\n", info->st_mode);
  printf(" links: %d\n", info->st_nlink);
  printf(" uid: %d\n", (int) info->st_uid);
  printf(" gid: %d\n", (int) info->st_gid);
  printf(" created: %s", ctime(&info->st_createtime));
  printf(" blksize: %" PRIu64 "\n", (uint64_t)info->st_blksize);
  printf(" blocks: %" PRIu64 "\n", (uint64_t)info->st_blocks);
  printf(" stated size  : %" PRIu64 "\n", (uint64_t)info->st_size);
  printf(" computed size: %" PRIu64 "\n", 
         ((uint64_t)info->st_blocks)*((uint64_t)info->st_blksize));
  fflush(stdout);
}

int main(int argc, char** argv){
  setenv("_EDC_ADD_ERRNO2", "1", 1);
  if (argc != 2){
    fprintf(stderr, "usage: %s fname\n", argv[0]);
    exit(1);
  }
  struct stat info;
  const char* fname = argv[1];

  if (lstat(fname, &info) != 0){
    perror("lstat() error");
    return -1;
  } else{
    showInfo(&info, "lstat", fname);
    if (S_ISLNK(info.st_mode) &&
        !(S_ISEXTL(info.st_mode,info.st_genvalue))){
      if (stat(fname, &info) != 0){
        perror("stat() error");
        return -1;
      } else{
        showInfo(&info, "stat", fname);
      }
    }
  }
  return 0;
}
