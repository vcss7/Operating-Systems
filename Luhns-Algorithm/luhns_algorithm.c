/*
 * File:    luhn_algorithm.c
 *
 * Purpose: Determine whether a user provided number is a valid credit card
 *      number.
 *
 * Compile: gcc -g -Wall -o luhn_algorithm.a luhn_algorithm.c
 * Run:     ./luhn_algorithm.a      # from inside the directory
 *
 * Input:   Number(s)
 *
 * Output:  A list of the numbers provided and the result
 *
 * IPP:     Operating System's Project 1
 */

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const uint8_t NUMBER_LIST_LENGTH = 10;

int main (int argc, char *argv[])
{
    char filename[] = "list_of_numbers.txt";
    FILE *file_ptr = fopen(filename, "r+e");
    unsigned int numbers[NUMBER_LIST_LENGTH];

    if(file_ptr == NULL)
    {
        printf("Could not open file: %s", filename);
        exit(1);
    }

    /* Read number list from file */
    #pragma unroll
    for (int i = 0; i < NUMBER_LIST_LENGTH; i++)
    {
        fscanf(file_ptr, "%u", &numbers[i]);
    }

    fclose(file_ptr);

    /* Print number list */
    #pragma unroll
    for (int i = 0; i < NUMBER_LIST_LENGTH; i++)
    {
        printf("Number[%d] = %u\n", i, numbers[i]);
    }

    return 0;
}

