#include <stdio.h> //For standard input/output
#include <math.h>  //For mathematical functions

//function prototype for reading data from file
void read_data(FILE *fptr, float *sum_X, float *sum_Y, float *sum_XY, float *sum_Xsq, int *n, FILE *new_fptr);

int main() {
    //declaring variables
    int i;
    FILE *fptr[4]; //array of file pointers for four datasets
    FILE *new_fptr = fopen("DatasetLR5.txt", "w"); // File pointer for the new dataset
    float sum_X = 0, sum_Y = 0, sum_XY = 0, sum_Xsq = 0; //initializing sum variables
    int n = 0; //initializing
    float calc_A, calc_B, y_Pred, user_x; 

    //Opening files in read mode
    fptr[0] = fopen("datasetLR1.txt", "r");
    fptr[1] = fopen("datasetLR2.txt", "r");
    fptr[2] = fopen("datasetLR3.txt", "r");
    fptr[3] = fopen("datasetLR4.txt", "r");

    //checking if files are opened successfully
    for (i = 0; i < 4; i++) {
        if (fptr[i] == NULL) {
            fprintf(stderr, "Error opening file %d.\n", i + 1);
            return 1; //if files cannot open, it displays error opening file
        }
    }

    //reading data from each file
    for (i = 0; i < 4; i++)
        read_data(fptr[i], &sum_X, &sum_Y, &sum_XY, &sum_Xsq, &n, new_fptr);

    if ((n * sum_Xsq) - pow(sum_X, 2) == 0) { //calculating determinant

        //when calculating, the determinant should not be zero
        fprintf(stderr, "Error: Division by zero.\n"); 
        return 1; //if determinant is zero it display error
    }

    //closing new dataset file
    fclose(new_fptr);

    //calculating coefficients of linear regression equation
    calc_A = ((sum_Y * sum_Xsq) - (sum_X * sum_XY)) / ((n * sum_Xsq) - pow(sum_X, 2));
    calc_B = ((n * sum_XY) - (sum_X * sum_Y)) / ((n * sum_Xsq) - pow(sum_X, 2));

    // Printing coefficients of linear regression equation
    printf("A = %0.2f\n", calc_A);
    printf("B = %0.2f\n", calc_B);
    printf("The linear equation obtained from the given calculation is : %0.2fx + %0.2f\n", calc_A, calc_B);

    //asking user for input x
    printf("\nEnter x coordinate to predict y: ");
    scanf("%f", &user_x);

    y_Pred = calc_A * user_x + calc_B;

    //printing the result
    printf("The value of x = %0.2f\n", user_x);
    printf("The predicted result is y = %0.2f\n", y_Pred);

    //closing all files
    for (i = 0; i < 4; i++)
        fclose(fptr[i]);

    return 0; 
}

//void function to read data from file and update sum variables
void read_data(FILE *fptr, float *sum_X, float *sum_Y, float *sum_XY, float *sum_Xsq, int *n, FILE *new_fptr) {
    int x, y;
    //reading data until end of file
    while (fscanf(fptr, "%d, %d", &x, &y) != EOF) {
        //updating sum variables
        (*sum_X) += x;
        (*sum_Y) += y;
        (*sum_Xsq) += pow(x, 2);
        (*sum_XY) += (x * y);
        (*n); 
        
        // writing to new dataset file
        fprintf(new_fptr, "%d, %d\n", x, y);
    }
}
