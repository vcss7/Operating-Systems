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
    uint8_t num_length;
    struct CreditCardIssuer issuer;
    bool valid;
};


void get_num_from_file(struct CreditCard* credit_card_p);
void print_credit_card_details(struct CreditCard* credit_card_p);
void *check_number_length(void *ptr);


int main (int argc, char *argv[])
{
    /* allocate memory for numbers */
    struct CreditCard *credit_cards = NULL;
    credit_cards = malloc(NUMBER_LIST_LENGTH * sizeof(struct CreditCard));

    /* program */
    pthread_t thread1;

    get_num_from_file(credit_cards);

    pthread_create(&thread1, NULL, check_number_length, credit_cards);

    pthread_join(thread1, NULL);

    print_credit_card_details(credit_cards);

    /* free allocated memory */
    free(credit_cards);

    return 0;
}

void *check_number_length(void *ptr)
{
    struct CreditCard *credit_cards_p = (struct CreditCard *) ptr;
    printf("Thread1 executing function: check_number_length\n");

    const uint8_t RADIX = 10;
    const uint8_t MIN_NUM_LENGTH = 13;
    const uint8_t MAX_NUM_LENGTH = 19;

    uint64_t number;
    uint8_t counter;

    for (int i = 0; i < NUMBER_LIST_LENGTH; i++)
    {
        number = credit_cards_p[i].number;
        counter = 0;

        #pragma unroll
        while(number > 0)
        {
            number = number / RADIX;
            counter++;
        }

        credit_cards_p[i].num_length = counter;

        if (credit_cards_p[i].num_length < MIN_NUM_LENGTH ||
            credit_cards_p[i].num_length > MAX_NUM_LENGTH)
        {
            credit_cards_p[i].valid = false;
        }
    }

    return NULL;
}

void print_credit_card_details(struct CreditCard *credit_cards_p)
{
    for (int i = 0; i < NUMBER_LIST_LENGTH; i++)
    {
        if (credit_cards_p[i].valid)
        {
            printf("%8s", "valid");
            printf(" | ");
            printf("%20"PRIu64" ", credit_cards_p[i].number);
            printf("%2u ", credit_cards_p[i].num_length);
            printf("%2u ", credit_cards_p[i].issuer.id);
            printf("%s", credit_cards_p[i].issuer.name);
            printf("\n");
        }
        else
        {
            printf("%8s", "invalid");
            printf(" | ");
            printf("%"PRIu64"", credit_cards_p[i].number);
            printf("\n");
        }
    }
}


void get_num_from_file(struct CreditCard *credit_cards_p)
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
        fscanf(file_ptr, "%"PRIu64"", &credit_cards_p[i].number);
        credit_cards_p[i].num_length = 0;
        credit_cards_p[i].issuer.id = 0;
        credit_cards_p[i].issuer.name = "NONE";
        credit_cards_p[i].valid = true;
    }

    fclose(file_ptr);
}

