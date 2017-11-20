#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <gmp.h>

//TODO: Implement robust error handling ("dbg.h" maybe?)

void checkInput(char* input) {
  int i = 0;
  int fail = 0;

  if(input[0] == '0' && input[1] == '\0') {
    printf("Invalid number, cannot be zero\n");
    exit(1);
  }

  for(i = 0; input[i] != '\0'; i++) {
    fail = isdigit(input[i]);
    if(!fail) {
      printf("Invalid input (must be positive integer)\n");
      exit(1);
    }
  }
}
      
unsigned long long collatz(int* vb, mpz_t n) {
  unsigned long long step = 0;

  while (mpz_cmp_d(n, 1)) {
    if(!mpz_fdiv_ui(n, 2)) {
      mpz_fdiv_q_ui(n, n, 2);
      step++;
    }
    else if (mpz_fdiv_ui(n,2) == 1) {
      mpz_mul_si(n, n, 3);
      mpz_add_ui(n, n, 1);
      step++;
    }
    else {
      printf("Unexpected error, aborting!\n");
      exit(0);
    }
    
    switch (*vb) {
    case 1:
      gmp_printf("%Zd, ", n);
      break;
    case 2:
      gmp_printf("%Zd\n", n);
      break;
    default:
      break;
    }
    
  }
  
  return step;
}

void singleTest(int* ptr_vb, char* input) {
  mpz_t n;
  unsigned long long step;

  mpz_init(n);

  gmp_sscanf(input, "%Zd", n);
  
  step = 0;

  step = collatz(ptr_vb, n);

  mpz_clear(n);

  printf("\nTotal number of steps: %llu\n", step);
}

void multiTest(int* ptr_vb, char* startS, char* endS, char* filepath) {
  mpz_t end;
  mpz_t curr;
  mpz_t buff;
  unsigned long long step;

  mpz_init(curr);
  mpz_init(buff);
  mpz_init(end);

  gmp_sscanf(startS, "%Zd", curr);
  if(*ptr_vb != 4)
    gmp_sscanf(endS, "%Zd", end);
  switch(*ptr_vb) {
  case 0:
    while(mpz_cmp(end, curr) >= 0) {
      mpz_set(buff, curr);
      step = collatz(ptr_vb, buff);
      mpz_add_ui(curr, curr, 1);
    }
    break;
  default:
    ;
    //TODO: Actually read stderr, and create if nonexistant
    if(access(filepath, W_OK | R_OK)) {
      printf("Could not read or write to file\n");
      exit(1);
    }
      
    FILE *file;
    file = fopen(filepath, "a");
    
    while(mpz_cmp(end, curr) >= 0) {
      mpz_set(buff, curr);
      step = collatz(ptr_vb, buff);

      gmp_fprintf(file, "%Zd, %llu\n", curr, step);
      
      mpz_add_ui(curr, curr, 1);
    }

    fclose(file);
    
    break;
  }

  mpz_clear(curr);
  mpz_clear(end);
}

int main(int argc, char** argv) {

  if (argc < 2 || argc > 5) {
    printf("Usage:\n collatz [options] integer [end] [file]\n"
	   "Single number tests:\n"
	   "\t-s0: Only print total number of steps [default]\n"
	   "\t-s1: Print all steps separated by commas\n"
	   "\t-s2: Print all steps separated by newline\n"
	   "Multiple number tests:\n"
	   "\t-m0: Test from integer to end, no output\n"
	   "\t-m1: Test from integer to end, write to file\n"
	   "\t-m2: Test from integer until interrupted, write to file\n");
    if (argc > 5)
      printf("Too many arguments\n");
    exit(1);
  }

  if (argc == 2) {
    char* input = argv[1];
    goto singlearg;
  }

  int vb = 0;
  int* ptr_vb = &vb;

  checkInput(argv[2]);

  switch(argv[1][1]) {
  case 's':
    ;
    char* input = argv[2];
    switch(argv[1][2]) {
    case '0':
    singlearg:
      vb = 0;
      break;
    case '1':
      vb = 1;
      break;
    case '2':
      vb = 2;
      break;
    default:
      printf("Invalid option\n");
      exit(1);
      break;
    }
    singleTest(ptr_vb, input);
    break;
    
  case 'm':
    ;
    char* start = argv[2];
    char* end = '\0';
    char* filepath = '\0';
    
    switch(argv[1][2]) {
    case '0':
      checkInput(argv[3]);
      vb = 0;
      end = argv[3];
      break;
    case '1':
      checkInput(argv[3]);
      end = argv[3];
      filepath = argv[4];
      vb = 3;
      break;
    case '2':
      filepath = argv[3];
      vb = 4;
      break;
    default:
      printf("Invalid option\n");
      exit(1);
      break;
    }
    multiTest(ptr_vb, start, end, filepath);
    break;
    
  default:
    printf("Invalid option\n");
    exit(1);
    break;
  }
  
  return 0;
}
