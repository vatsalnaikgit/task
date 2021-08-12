/*
Task: Description: The sensor I am using generates a lot of data and there seems to be a lot of repeating sequences in it.
The top bit is never used in the data format and could be re-purposed for whatever you like.

The format looks like:
0x70, 0x71, 0x34, 0x34, 0x34, 0x20, 0x67, 0x15, 0x15, 0x15, 0x15, 0x15, 0x42, 0x53, 0x68, 0x68, 0x68

This is just one example of the format and there are many other sequences and combinations of the data that you should test.
Design me a program that can store this data in the least number of bytes possible to a file, but that can also re-generate the original data set if needed.

Author: Vatsal Naik
Date: August 08 2021
*/

#include <stdio.h>
#include <stdlib.h>

/* :::Notes:::
This input data buffer can be fixed size.. eg. 8bytes and that can be receive through DMA.
we can call DMA Half RX interrupt to process 4 Bytes of received data.
in the mean time DMA will continue to receive remaining 4 bytes of data.
By the time DMA Complete receive interrupt occcure, we can process remainign higher byte while dma will receive lower 4 bytes into a circular buffer.
*/

void NormalApproach (void);     // This is just simple way to store data as an array.

int main()
{
    NormalApproach();

    return 0;
}

void NormalApproach (void) {
// By using this approach File size will depend on number of elements in Array.
    const int memSize = 10;

    char *buffer;
    long filelen;

    FILE *fptr = NULL;

    //char input[memSize] = {0x70, 0x71, 0x34, 0x34, 0x34, 0x20, 0x67, 0x15, 0x15, 0x15, 0x15, 0x15, 0x42, 0x53, 0x68, 0x68, 0x68};

    char input[10] = {0x70, 0x71, 0x34, 0x34, 0x34, 0x20, 0x67, 0x15, 0x15, 0x15};

    // In PC size of int is 4 bytes
    //int Arraylength = sizeof(input)/sizeof(int);
    char *values = (char *)malloc(memSize * sizeof(char));

    // This For loop can be replace by DMA Interrupt handler...
    for (int i = 0; i < memSize; i++) {
        values[i] = input[i];
    }

    // TODO: if more data needs to be added, use realloc to reassign a memory and continue

    // Store Data to File
    fptr = fopen("data.dat", "wb"); // write the file
    fwrite(values, memSize, sizeof(char), fptr);
    fclose(fptr);


    // Read Data from File
    fptr = fopen("data.dat", "rb");    // Open the file
    fseek(fptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(fptr);             // Get the current byte offset in the file
    rewind(fptr);                      // Jump back to the beginning of the file

    buffer = (char *)malloc(filelen * sizeof(char)); // Enough memory for the file
    fread(buffer, filelen, 1, fptr); // Read in the entire file
    fclose(fptr); // Close the file

    // For Verification
    for (int i = 0; i < filelen; i++) {
            printf("0x%x\n", buffer[i]);
    }

}


