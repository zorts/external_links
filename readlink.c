#define _POSIX_SOURCE
#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char**argv){
  if ((argc != 2) || (0 == strcmp(argv[1], "-h"))){
    printf("usage: %s path\n", argv[0]);
    return 1;
  }
  int retval = 0;
  /* pathconf is weird. The xlc library reference says:

     If unsuccessful, pathconf() returns -1. If a particular variable
     has no limit, such as PATH_MAX, pathconf() returns -1 but does 
     not change errno.

     Hence, all this stop fiddling around.
  */

  errno = 0;
  long PATH_MAX = pathconf(argv[1], _PC_PATH_MAX);
  if (PATH_MAX < 0){
    if (errno != 0){
      int error = 0;
      fprintf(stderr,
              "pathconf(%s, _PC_PATH_MAX) returned %d with error %d (%s)\n",
              argv[1], (int) PATH_MAX, error, strerror(error));
    } else{
      fprintf(stderr,
              "pathconf(%s, _PC_PATH_MAX) returned %d without changing errno; "
              "PATH_MAX is unlimited.\n",
              argv[1], (int) PATH_MAX);
    }
    PATH_MAX = 10000;  /* arbitrary */
  }

  char* buf = (char*) malloc((size_t) (PATH_MAX+1));
  memset(buf, 0, (size_t) (PATH_MAX+1));
  ssize_t status = readlink(argv[1], buf, (size_t) (PATH_MAX+1));
  if (0 > status){
    int error = errno;
    fprintf(stderr, "readlink(%s) failed: %d (%s)\n",
            argv[1], error, strerror(error));
    retval = 1;
  } else{
    printf("%s\n", buf);
  }
  free(buf);
  return retval;
}
