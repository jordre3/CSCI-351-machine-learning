#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

struct distance_metric {
  int viewer_id;
  double distance;
};

int compare(const void *a, const void *b){
    struct distance_metric *d1 = ((struct distance_metric*)a);
    struct distance_metric *d2 = ((struct distance_metric*)b);
    return(d1->distance > d2->distance);
}

int main(int argc, char* argv){
    FILE *file;
    char *filename = "ML-ratings-medium.txt";
    /* open the file for writing */
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "File %s could not be opened\n", filename);
        exit(1);
    }

    int numReviewers;
    int numMovies;

    fscanf(file, "%d %d", &numReviewers, &numMovies);

    double * movies = malloc(numMovies * numReviewers * sizeof(*movies));
    assert(movies);

    /* loop while reading a line at a time from the file and printing */
    for(int i = 0; i<numReviewers; ++i) {
        for(int j = 0; j < numMovies; ++j){
            fscanf(file, "%lf", &movies[i * numMovies + j]);
        }
    }

    for(int i = 0; i<numReviewers; i++) {
        for(int j = 0; j<numMovies; j++) {
            printf("%.1f ", movies[i * numMovies + j]);   
        }
        printf("\n");
    }
    /* close the file */
    fclose(file);

    double userMovies[numMovies - 1];
    printf("Enter four movie ratings:");
    scanf("%lf %lf %lf %lf", &userMovies[0], &userMovies[1], &userMovies[2], &userMovies[3]);
    printf("you entered : %.1f %.1f %.1f %.1f\n", userMovies[0], userMovies[1], userMovies[2], userMovies[3]);

    printf("Viewer ID   Distance\n");
    printf("----------------------\n");

    struct distance_metric * distances = malloc(numReviewers * (sizeof(struct distance_metric)));

    double viewerDist;
    for(int i = 0; i<numReviewers; i++) {
        viewerDist = 0;
        for(int j = 0; j<numMovies - 1; j++) {
           viewerDist += fabs(userMovies[j] - movies[i * numMovies + j]); 
        }
        distances[i].distance = viewerDist;
        distances[i].viewer_id = i;
        printf("       %d        %.1f\n", i + 1, viewerDist);
    }
    printf("----------------------\n");

    qsort(distances, numReviewers, sizeof(struct distance_metric), compare);

    int k;
    printf("Enter a value for k:");
    scanf("%d", &k);

    printf("Viewer ID      Movie 5      Distance\n");
    printf("---------------------------------------\n");
    double sum = 0;
    for(int i = 0; i<k; i++){
        sum += movies[distances[i].viewer_id * numMovies + (numMovies - 1)];
        printf("    %d         %.1f          %.1f\n", distances[i].viewer_id + 1, movies[distances[i].viewer_id * numMovies + (numMovies - 1)], distances[i].distance);      
    }
    printf("---------------------------------------\n");
    printf("The predicted rating for movie %d is %.1f\n", numMovies, (sum/k));

    free(distances);
    free(movies);
    return EXIT_SUCCESS;
}
