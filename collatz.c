#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[]) {

  //initialize vars
  int step;
  unsigned long long n;
  int vb;
  char *compare = "-s";
  char *scheck = argv[1];
  n = 0;
  vb = 0;
  step = 0;

  if (argc < 2) {
    printf("Usage:\n collatz [-s] INTEGER\n");
    exit(0);
  }
  if (argc > 3) {
    printf("Too many arguments\n");
    exit(0);
  }
  //check for step verbosity
  if (strncmp(scheck, compare, 4) == 0) {
    vb = 1;
    n = strtol(argv[2], NULL, 10);
  }
  else {
    n = strtol(argv[1], NULL, 10);
  }

  //sanity check
  if (n == 0) {
    printf("\nAn error has occurred, exiting...\n");
    exit(0);
  }
  
  //start loop
  while (n != 1) {
    //overflow check
    if (n == 9223372036854775806) {
      printf("\n\nOverflow error, exiting!\n");
      exit(0);
    }
    if (n%2 == 0) {
	  n = n/2;
	  step = step + 1;
	    if (vb == 1) {
	      printf("%llu, ", n);
	    }
	}
      else if (n%2 == 1) {
	  n = 3*n + 1;
	  step = step + 1;
	    if (vb == 1) {
	      printf("%llu, ", n);
	    }
	}
      else {
	//error handling, shouldn't happen
	printf("\nAn error has occurred, exiting...\n");
	exit(0);
      }
    }
  //print end result
  if (n == 1) {
      printf("\nTotal number of steps: %d\n", step);
    }
}
