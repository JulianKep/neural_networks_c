#include <stdlib.h>
#include <stdio.h>


int array_sum(const int *array, size_t array_size){
    int sum = 0;

    for (int i = 0; i < array_size; i++){
        sum += *(array + i);
    }

    return sum;

}

int main() {

    int num = 3;
    int *pointer = &num; // get pointer to num. that pointer has the type "int *"
    printf("%d", *pointer); // access the value at that point


    //create array by creating pointer that points to a region on the stack (alloca) that has the size
    //of the array

    int *p = alloca(3 * sizeof(int));
    *p = 3;
    *(p+1) = 4;
    *(p+2) = 4;


    //create a 2d-array by first creating two 1d arrays:

    printf("%d\n", *(p+1));

    int *row_a = alloca(3 * sizeof(int));
    *(row_a + 0) = 10;
    *(row_a + 1) = 55;
    *(row_a + 2) = 342;

    int *row_b = alloca(3 * sizeof(int));
    *(row_b + 0) = 23545;
    *(row_b + 1) = 686858;
    *(row_b + 2) = 4;

    //allocate an array that stores pointers to arrays (the previous rows)

    int **m = alloca(2 * sizeof(row_a));
    *(m + 0) = row_a;
    *(m + 1) = row_b;


    printf("%d\n", *(*(m + 1) +1 ));

    //instead of using pointer arithmetic to fill an array, use "[]" notation

    int *q = alloca(3 * sizeof(int));
    q[0] = 3; //exactyl same as *(q + 1) = 6;
    q[1] = 6;
    q[2] = 1;

    printf("%d\n", q[1]);

    //now use "[]" notation to create an array:

    //this is an object of type "int[3]":
    int arr[3] = {1, 2, 9};


    //arrays can ONLY be passed as pointers to their first element, they cannot be passed by value (copies)
    //passing the array as: &arr would be wrong since that represents a special pointer of type int (*)[3]

    printf("%d\n", array_sum(&arr[0], 3));


    //by passing the array directly, it is transformed to a pointer to the first element (decay):

    printf("%d\n", array_sum(arr, 3));


}
