// 313467441 Lee alima

#include <stdio.h>
#include <stdlib.h>

/**
 * this method checks if the machine is build on little or big indian.
 * i saved the number 1's bytes in char*, than i checked the pointer
 * to this number, if the pointer points to the value 1 than the machine is
 * build on little indian, else, on big indian.
 * @return 1 - if the machine is build on little indian
 * and 0 if on big indian.
 */
int is_little_endian() {
    // I created an int variable in order to check it's first byte.
    int number = 1;
    // I saved the bytes with pointer
    char *c = (char*) &number;
    // if the first byte is 1, then the machine switches between the bytes
    if (*c == 1){
        return 1;
    }
    // the machine is build on big indian
    return 0;
}

/**
 * the method merges the bytes - it changes the last LSB of number
 * x - based on the y's LSB.
 * i had to check in what approch(little or big indian) to deal
 * with the merge. in both ways i had pointers to both numbers
 * and with a pointer to a new number i created it by giving
 * the correct value to each byte based on the pointer's values.
 * @param x - the first number
 * @param y - the second number
 * @return the merged number
 */
unsigned long merge_bytes(unsigned long x, unsigned long int y) {
    // creating a new number and pointer to it
    unsigned  long newNum ;
    char *newNumBytes = (char*) &newNum;
    // creating pointers to the bytes of x and y
    char *xBytes = (char*) &x;
    char *yBytes = (char*) &y;
    int loopIndex;
    if (is_little_endian()) {
        // updating the pointer until reaching the MSB
        for (loopIndex = 0 ;loopIndex < 7 ;loopIndex++){
            *newNumBytes++;
            *xBytes++;
        }
        // copying the bytes from the MSB to the LSB
        for (loopIndex = 0 ;loopIndex < 7 ;loopIndex++){
            *newNumBytes = *xBytes;
            *newNumBytes--;
            *xBytes --;
        }
        // copying the LSB from y
        *newNumBytes = *yBytes;
    } else { // if the machine works with big indian
        // copying the bytes from the MSB to the LSB and updating the pointers
        for (loopIndex = 0 ;loopIndex < 7 ;loopIndex++) {
            *newNumBytes = *xBytes;
            *newNumBytes ++;
            *xBytes ++;
            // updating the pointer of y
            *yBytes++;
        }
        // coping the LSB
        *newNumBytes = *yBytes;
    }
    return  newNum;
}

/**
 * this method puts a byte in index based on the user choice.
 * in the method i checked the machine approch, and with a
 * pointer to the number i created i changed the chosen byte
 * based on index and byte.
 * @param x - as the number
 * @param b - as the wanted char
 * @param i - as the index
 * @return new number
 */
unsigned long put_byte(unsigned long x, unsigned char b, int i) {
    // if the index is more than biggest byte location - exit.
    if (i > 7) {
        printf("index is illegal.\n");
        exit(0);
    }
    int loopIndex;
    // saving the number from the user
    unsigned long newNum = x;
    // creating a pointer to the number
    char * pointer = (char *) & newNum;
    // in little indian the counting begins with the LSB, therefor
    // all I have to do is updating the pointer to the right location
    // with the value of the index
    if (is_little_endian()) {
        for (loopIndex = 0; loopIndex < i; loopIndex++){
            *pointer++;
        }
        // updating the right byte
        *pointer = b;
    } else { // if the machine works with big indian
        // updating the pointer 7-i times because the pointer
        // first point to the MSB
        for( loopIndex =0;loopIndex<7-i;loopIndex++){
            *pointer++;
        }
        *pointer = b;
    }
    return  newNum;
}