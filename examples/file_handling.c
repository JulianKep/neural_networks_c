#include <stdio.h>
#include <stdlib.h>

int main() {
    
    // File pointer to store the 
    // value returned by fopen
    FILE* fptr;

    // Opening the file in read mode
    fptr = fopen("output2.svg", "w");

    // checking if the file is 
    // opened successfully
    if (fptr == NULL) {
        printf("The file is not opened.");
    }



    char buffer[10000];
    
    //buffer is initialized with random stuff on the stack, set everything to zero
    memset(buffer, 0, 10000);

    int pos_x = 100;
    int pos_y = 200;
    
    int written = snprintf(buffer,
                             sizeof(buffer),
                            "<svg width=\"500\" height=\"500\" viewBox=\"0 0 500 500\" xmlns=\"http://www.w3.org/2000/svg\">\n"
                            "<rect width=\"500\" height=\"500\" fill=\"white\" />\n"
                            "<circle cx=\"%d\" cy=\"%d\" r=\"5\" fill=\"red\" />\n"
                            "</svg>",
                             pos_x, pos_y);

    if (written >= sizeof(buffer)){
        printf("Buffer truncation occured");
    }

    fputs(buffer, fptr);


    fclose(fptr);


    return 0;
}