/*
Copyright (c) 2016-2020 Jeremy Iverson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

/* assert */
#include <assert.h>

/* fabs */
#include <math.h>

/* printf, fopen, fclose, fscanf, scanf */
#include <stdio.h>

/* EXIT_SUCCESS, malloc, calloc, free, qsort */
#include <stdlib.h>

struct distance_metric {
  size_t viewer_id;
  double distance;
};

int
main(int argc, char * argv[])
{
  size_t n, m;

  /* Validate command line arguments. */
//   assert(2 == argc);

//   /* ... */
  char const * const fn = argv[1];
  // char const * const fn = "ML-ratings.txt";

  /* Validate input. */
  assert(fn);

  /* Open file. */
  FILE * const fp = fopen(fn, "r");
  assert(fp);

  /* Read file. */
  fscanf(fp, "%zu %zu", &n, &m);

  /* Allocate memory. */
  double * const rating = malloc(n * m * sizeof(*rating));

  /* Check for success. */
  assert(rating);

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < m; j++) {
      fscanf(fp, "%lf", &rating[i * m + j]);
    }
  }

  /* Close file. */
  int const ret = fclose(fp);
  assert(!ret);

  /* Allocate more memory. */
  double * const urating = malloc(m * sizeof(*urating));

  /* Check for success. */
  assert(urating);

  /* Get user input. */
  for (size_t j = 0; j < m - 1; j++) {
    printf("Enter your rating for movie %zu: \n", j + 1);
    scanf("%lf", &urating[j]);
  }

  /* Allocate more memory. */
  double prob[10] = { 0.0 };

  /* Compute probabilities */
  for (int k = 0; k < 10; k++) {
    for (size_t i = 0; i < n; i++) {
      prob[k] += (rating[i * m + 4] == (k + 1) / 2.0);
    }
  }

  double * prob2 = calloc(10 * (m-1), (m -1) * 10 * sizeof(double));
  for (int i = 0; i < (m-1); i++){
      for(int k = 0; k < n; k++){
            if(rating[k*m + i] == urating[i]){
                int newRate = (rating[k*m + (m - 1)] * 2) - 1;
                prob2[10*i + newRate] += 1;
            }
        }
  }


  for(int i = 0; i < (m-1); i++){
      for(int k = 0; k < 10; k++){
          if(prob[k] > 0){
            prob2[10*i + k] /= prob[k];
          }
          else{
              prob2[10*i + k] = 0.0;
          }
      }
  }


  /* Finalize computation of probabilities. */
  for (int k = 0; k < 10; k++) {
    prob[k] /= n;
  }



  for(int i = 0; i < (m-1); i++){
      for(int k = 0; k < 10; k++){
         prob[k] *= prob2[i * 10 + k];
      }
  }

  for (int k = 0; k < 10; k++) {
    printf("prob[%d] = %lf\n", k, prob[k]);
  }

  int maxIndex = 0;
  for(int k = 1; k < 10; k++){
      if(prob[k] > prob[maxIndex]){
          maxIndex = k;
      }
  }

  double predRating = maxIndex;
  predRating = (predRating + 1)/ 2;
  /* Output prediction. */
  printf("The predicted rating for movie five is %.1lf.\n", predRating);

  /* Deallocate memory. */
  free(rating);
  free(urating);
  free(prob2);

  return EXIT_SUCCESS;
}
