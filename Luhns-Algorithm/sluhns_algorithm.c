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
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t number_list_length = 0;

struct CreditCardIssuer
{
    char* name;
    uint8_t id;
};

struct CreditCard
{
    uint64_t number;
    uint8_t num_length;
    uint8_t double_even_counter;
    uint8_t odd_counter;
    struct CreditCardIssuer issuer;
    bool valid;
};


void get_num_from_args(int argc, char* argv[], struct CreditCard* credit_cards_p);
void get_num_from_file(struct CreditCard* credit_cards_p);
void print_credit_card_details(struct CreditCard* credit_cards_p);
void *set_number_length(void *ptr);
void *set_issuer_id(void *ptr);
void *set_double_even_sum(void *ptr);
void *set_odd_sum(void *ptr);


int main (int argc, char *argv[])
{
    number_list_length = argc - 1;

    /* allocate memory for numbers */
    struct CreditCard *credit_cards = NULL;
    credit_cards = malloc(number_list_length * sizeof(struct CreditCard));

    /* program */
    // get_num_from_file(credit_cards);
    get_num_from_args(argc, argv, credit_cards);

    set_number_length(credit_cards);
    set_issuer_id(credit_cards);
    set_double_even_sum(credit_cards);
    set_odd_sum(credit_cards);

    const uint8_t RADIX = 10;

    uint16_t sum = 0;

    for (int i = 0; i < number_list_length; i++)
    {
        sum = 0;
        sum = credit_cards[i].odd_counter + credit_cards[i].double_even_counter;
        
        if (sum % RADIX != 0)
        {
            credit_cards[i].valid = false;
        }
    }

    print_credit_card_details(credit_cards);

    /* free allocated memory */
    free(credit_cards);

    return 0;
}


void *set_odd_sum(void *ptr)
{
    struct CreditCard *credit_cards_p = (struct CreditCard *) ptr;
    printf("Executing function: set_odd_sum\n");

    const uint8_t RADIX = 10;

    uint64_t number;
    uint8_t remainder;
    uint8_t counter;

    for (int i = 0; i < number_list_length; i++)
    {
        credit_cards_p[i].odd_counter = 0;
        number = credit_cards_p[i].number;
        counter = 0;

        while (number > 0)
        {
            remainder = number % RADIX;
            number = number / RADIX;
            counter++;

            if (counter % 2 == 1)
            {
                credit_cards_p[i].odd_counter += remainder;
            }
        }
    }

    return NULL;
}


void *set_double_even_sum(void *ptr)
{
    struct CreditCard *credit_cards_p = (struct CreditCard *) ptr;
    printf("Executing function: set_double_even_sum\n");

    const uint8_t RADIX = 10;

    uint64_t number;
    uint8_t remainder;
    uint8_t double_remainder;
    uint8_t counter;

    for (int i = 0; i < number_list_length; i++)
    {
        credit_cards_p[i].double_even_counter = 0;
        number = credit_cards_p[i].number;
        counter = 0;

        while (number > 0)
        {
            remainder = number % RADIX;
            number = number / RADIX;
            counter++;

            if (counter % 2 == 0)
            {
                double_remainder = 2 * remainder;

                if (double_remainder >= RADIX)
                {
                    credit_cards_p[i].double_even_counter += double_remainder % RADIX;
                    credit_cards_p[i].double_even_counter += double_remainder / RADIX;
                }
                else
                {
                    credit_cards_p[i].double_even_counter += double_remainder; 
                }
            }
        }
    }

    return NULL;
}


void *set_issuer_id(void *ptr)
{
    struct CreditCard *credit_cards_p = (struct CreditCard *) ptr;
    printf("Executing function: set_issuer_id\n");
    
    const uint8_t RADIX = 10;
    const uint8_t THREE_DIGITS = 100;

    uint64_t number;

    for (int i = 0; i < number_list_length; i++)
    {
        number = credit_cards_p[i].number;

        while(number > THREE_DIGITS)
        {
            number = number / RADIX;
        }

        credit_cards_p[i].issuer.id = number;

        switch(credit_cards_p[i].issuer.id)
        {
            case 34:
            case 37:
                credit_cards_p[i].issuer.name = "American Express";
                break;
            case 40 ... 49:
                credit_cards_p[i].issuer.id = 4;
                credit_cards_p[i].issuer.name = "Visa";
                break;
            case 51 ... 55:
                credit_cards_p[i].issuer.name = "Master";
                break;
            case 60 ... 69:
                credit_cards_p[i].issuer.id = 6;
                credit_cards_p[i].issuer.name = "Discover";
                break;
            default:
                credit_cards_p[i].issuer.id = 0;
                credit_cards_p[i].valid = false;
        }
    }

    return NULL;
}

void *set_number_length(void *ptr)
{
    struct CreditCard *credit_cards_p = (struct CreditCard *) ptr;
    printf("Executing function: set_number_length\n");

    const uint8_t RADIX = 10;
    const uint8_t MIN_NUM_LENGTH = 13;
    const uint8_t MAX_NUM_LENGTH = 19;

    uint64_t number;
    uint8_t counter;

    for (int i = 0; i < number_list_length; i++)
    {
        number = credit_cards_p[i].number;
        counter = 0;

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
    for (int i = 0; i < number_list_length; i++)
    {
        if (credit_cards_p[i].valid)
        {
            printf("%8s", "valid");
            printf(" | ");
            printf("%20"PRIu64" ", credit_cards_p[i].number);
            printf("%2u ", credit_cards_p[i].num_length);
            printf("%2u ", credit_cards_p[i].issuer.id);
            printf("%17s ", credit_cards_p[i].issuer.name);
            printf("\n");
        }
        else
        {
            printf("%8s", "invalid");
            printf(" | ");
            printf("%20"PRIu64"", credit_cards_p[i].number);
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
    for (int i = 0; i < number_list_length; i++)
    {
        fscanf(file_ptr, "%"PRIu64"", &credit_cards_p[i].number);
        credit_cards_p[i].num_length = 0;
        credit_cards_p[i].double_even_counter = 0;
        credit_cards_p[i].odd_counter = 0;
        credit_cards_p[i].issuer.id = 0;
        credit_cards_p[i].issuer.name = "NONE";
        credit_cards_p[i].valid = true;
    }

    fclose(file_ptr);
}


void get_num_from_args(int argc, char *argv[], struct CreditCard* credit_cards_p)
{
    for (int i = 1; i < argc; i++)
    {
        credit_cards_p[i - 1].number = strtoll(argv[i], NULL, 10);
        credit_cards_p[i - 1].num_length = 0;
        credit_cards_p[i - 1].double_even_counter = 0;
        credit_cards_p[i - 1].odd_counter = 0;
        credit_cards_p[i - 1].issuer.id = 0;
        credit_cards_p[i - 1].issuer.name = "NONE";
        credit_cards_p[i - 1].valid = true;
    }
}

