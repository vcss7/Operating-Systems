#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Issuer
{
    char* name;
    uint8_t id;
};


struct CreditCard
{
    uint64_t number;
    uint8_t num_length;
    uint8_t running_even_sum;
    uint8_t running_odd_sum;
    struct Issuer issuer;
    bool valid;
};


struct ThreadArguments
{
    struct CreditCard* credit_card;
    uint8_t value;
};


const uint8_t NUMBER_LIST_LENGTH = 10;

void set_credit_card_numbers(struct CreditCard* credit_cards_p);
void print_credit_cards(struct CreditCard* credit_cards_p);
void* set_number_length(void* ptr);
void* set_issuer_id(void* ptr);
void* set_running_even_sum(void* ptr);
void* set_running_odd_sum(void* ptr);

int main(int argc, char *argv[])
{
    /* allocate memory for argv array */
    struct CreditCard* credit_cards_p = NULL;
    credit_cards_p = malloc(NUMBER_LIST_LENGTH * sizeof(struct CreditCard));

    /* set up */
    set_credit_card_numbers(credit_cards_p);

    /* threads */
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    pthread_t thread4;

    /* luhns_algorithm */
    const uint8_t RADIX = 10;
    const uint8_t THREE_DIGITS = 100;

    uint8_t i;
    uint64_t number;
    uint8_t remainder;
    uint8_t counter;

    struct ThreadArguments thread1_args_p;
    struct ThreadArguments thread2_args_p;
    struct ThreadArguments thread3_args_p;
    struct ThreadArguments thread4_args_p;
    
    //thread1_args_p = malloc(sizeof(struct ThreadArguments));

    for (i = 0; i < NUMBER_LIST_LENGTH; i++)
    {
        thread1_args_p.credit_card = &credit_cards_p[i];
        thread2_args_p.credit_card = &credit_cards_p[i];
        thread3_args_p.credit_card = &credit_cards_p[i];
        thread4_args_p.credit_card = &credit_cards_p[i];

        number = credit_cards_p[i].number;
        counter = 0;

        while (number > 0)
        {
            remainder = number % RADIX;
            number = number / RADIX;
            counter++;

            if (number < THREE_DIGITS && number > RADIX)
            {
                thread2_args_p.value = number;
                pthread_create(&thread2, NULL, set_issuer_id, &thread2_args_p);
            }

            if (counter % 2 == 0)
            {
                thread3_args_p.value = remainder;
                pthread_create(&thread3, NULL, set_running_even_sum, &thread3_args_p);
            }
            else
            {
                thread4_args_p.value = remainder;
                pthread_create(&thread4, NULL, set_running_odd_sum, &thread4_args_p);
            }
        }

        thread1_args_p.value = counter;

        pthread_create(&thread1, NULL, set_number_length, &thread1_args_p);

        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);
        pthread_join(thread4, NULL);
    }

    /* print results */
    print_credit_cards(credit_cards_p);

    /* free allocated memory */
    //free(thread1_args_p);
    free(credit_cards_p);

    return 0;
}


void* set_running_odd_sum(void* ptr)
{
    printf("Thread4 running function set_running_odd_sum\n");
    struct ThreadArguments* args = (struct ThreadArguments*) ptr;

    args->credit_card->running_odd_sum += args->value;

    return NULL;
}


void* set_running_even_sum(void* ptr)
{
    printf("Thread3 running function set_running_even_sum\n");
    struct ThreadArguments* args = (struct ThreadArguments*) ptr;

    const uint8_t RADIX = 10;

    args->value = 2 * args->value;

    if (args->value >= RADIX)
    {
        args->credit_card->running_even_sum += args->value % RADIX;
        args->credit_card->running_even_sum += args->value / RADIX;
    }
    else
    {
        args->credit_card->running_even_sum += args->value;
    }

    return NULL;
}


void* set_issuer_id(void* ptr)
{
    printf("Thread2 running function set_issuer_id\n");
    struct ThreadArguments* args = (struct ThreadArguments*) ptr;

    args->credit_card->issuer.id = args->value;

    /* TODO: check appropriate length too */

    switch(args->credit_card->issuer.id)
    {
        case 34:
        case 37:
            args->credit_card->issuer.name = "American Express";
            break;
        case 60 ... 69:
            args->credit_card->issuer.id = 6;
            args->credit_card->issuer.name = "Discover";
            break;
        case 51 ... 55:
            args->credit_card->issuer.name = "Master";
            break;
        case 40 ... 49:
            args->credit_card->issuer.id = 4;
            args->credit_card->issuer.name = "Visa";
            break;
        default:
            args->credit_card->issuer.name = "NONE";
            args->credit_card->issuer.id = 0;
    }

    return NULL;
}


void* set_number_length(void* ptr)
{
    printf("Thread1 running function set_number_length\n");

    struct ThreadArguments* args = (struct ThreadArguments*) ptr;

    const uint8_t MIN_NUMBER_LENGTH = 13;
    const uint8_t MAX_NUMBER_LENGTH = 19;

    args->credit_card->num_length = args->value;

    if (args->value < MIN_NUMBER_LENGTH ||
        args->value > MAX_NUMBER_LENGTH)
    {
        args->credit_card->valid = false;
    }

    return NULL;
}


void print_credit_cards(struct CreditCard* credit_cards_p)
{
    for (int i = 0; i < NUMBER_LIST_LENGTH; i++)
    {
        printf("%s", credit_cards_p[i].valid ? "true" : "false");
        printf(" | ");
        printf("%20"PRIu64" ", credit_cards_p[i].number);
        printf("%2u ",  credit_cards_p[i].num_length);
        printf("%2u ",  credit_cards_p[i].issuer.id);
        printf("%17s ",  credit_cards_p[i].issuer.name);
        printf("%3u ",  credit_cards_p[i].running_even_sum);
        printf("%3u ",  credit_cards_p[i].running_odd_sum);
        printf("\n");
    }
}


void set_credit_card_numbers(struct CreditCard* credit_cards_p)
{
    char filename[] = "list_of_numbers.txt";
    FILE* file_ptr = fopen(filename, "r+e");

    if (file_ptr == NULL)
    {
        printf("Could not open file: %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < NUMBER_LIST_LENGTH; i++)
    {
        fscanf(file_ptr, "%"PRIu64"", &credit_cards_p[i].number);
        credit_cards_p[i].num_length = 0;
        credit_cards_p[i].num_length = 0;
        credit_cards_p[i].running_even_sum = 0;
        credit_cards_p[i].running_odd_sum = 0;
        credit_cards_p[i].issuer.id = 0;
        credit_cards_p[i].issuer.name = "NONE";
        credit_cards_p[i].valid = true;
    }

    fclose(file_ptr);
}

