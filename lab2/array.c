//
// Created by arvid on 2016-02-10.
//

#include <stdio.h>

int main(void) {

  /* an array with 5 elements */
  int balance[5] = {10, 20, 30, 40, 50};
  int *p;

  p = balance;

  /* output each array element's value */
  printf( "Array values using pointer\n");

  for (int i = 0; i < 5; i++ ) {
    printf("*(p + %d) : %d\n",  i, *(p + i) );
  }

  printf( "Array values using balance as address\n");

  for (int i = 0; i < 5; i++ ) {
    printf("*(balance + %d) : %d\n",  i, *(balance + i) );
  }

  return 0;
}