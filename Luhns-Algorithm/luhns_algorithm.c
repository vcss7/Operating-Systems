/*
 * File:    luhn_algorithm.c
 *
 * Purpose: Determine whether a user provided number is a valid credit card
 *          number.
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

#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const uint8_t NUMBER_LIST_LENGTH = 10;

struct CreditCardIssuer
{
    char* name;
    uint8_t id;
};

struct CreditCard
{
    uint64_t number;
    uint8_t length;
    struct CreditCardIssuer issuer;
    bool valid;
};


void get_num_from_file(struct CreditCard* credit_card_p);
void print_credit_card_details(struct CreditCard* credit_card_p);


int main (int argc, char *argv[])
{
    /* allocate memory for numbers */
    struct CreditCard* credit_card = NULL;
    credit_card = malloc(NUMBER_LIST_LENGTH * sizeof(struct CreditCard));

    /* program */
    get_num_from_file(credit_card);
    print_credit_card_details(credit_card);

    /* free allocated memory */
    free(credit_card);

    return 0;
}


void print_credit_card_details(struct CreditCard* credit_card_p)
{
    for (int i = 0; i < NUMBER_LIST_LENGTH; i++)
    {
        if (credit_card_p[i].valid)
        {
            printf("%8s", "valid");
            printf(" | ");
            printf("%20"PRIu64"", credit_card_p[i].number);
            printf("%2u", credit_card_p[i].length);
            printf("%2u", credit_card_p[i].issuer.id);
            printf("%s", credit_card_p[i].issuer.name);
            printf("\n");
        }
        else
        {
            printf("%8s", "invalid");
            printf(" | ");
            printf("%"PRIu64"", credit_card_p[i].number);
            printf("\n");
        }
    }
}


void get_num_from_file(struct CreditCard* credit_card_p)
{
    char filename[] = "list_of_numbers.txt";
    FILE *file_ptr = fopen(filename, "r+e");

    if(file_ptr == NULL)
    {
        printf("Could not open file: %s", filename);
        exit(1);
    }

    /* Read number list from file */
    #pragma unroll
    for (int i = 0; i < NUMBER_LIST_LENGTH; i++)
    {
        fscanf(file_ptr, "%"PRIu64"", &credit_card_p[i].number);
        credit_card_p[i].length = 0;
        credit_card_p[i].issuer.id = 0;
        credit_card_p[i].issuer.name = "NONE";
        credit_card_p[i].valid = false;
    }

    fclose(file_ptr);
}

