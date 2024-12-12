#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>

#define NUMBER_OF_PIZZAS 120
#define PIZZA_ARRAY_SIZE 5
#define N_OF_PIZZAS_IN_A_QUEUE 100

struct pizza
{
    char current_Pizza[PIZZA_ARRAY_SIZE];
    char pizza_queue[N_OF_PIZZAS_IN_A_QUEUE][PIZZA_ARRAY_SIZE];
    char *pizza_message;
    int last_pizza_num;
    int total_amount_of_pizzas_produced;
    pthread_mutex_t pizza_creation;
    pthread_mutex_t pizza_queue_managing;
    pthread_mutex_t pizza_consuming;

    pthread_cond_t d_ready;
    pthread_cond_t o_ready;
    pthread_cond_t full;
    pthread_cond_t empty;
    pthread_cond_t pizza_delivered;
};

// int hand_pizza_to_the_customer;
void send_pizza_to_customer(char *buf_pizza, void *p)
{
    struct pizza *buf = (struct pizza *)p;
    buf->pizza_message = buf_pizza;
    pthread_cond_signal(&(buf->pizza_delivered));
    if(buf_pizza != NULL)
        pthread_cond_wait(&(buf->pizza_delivered), &(buf->pizza_consuming));
    else
        pthread_mutex_unlock(&(buf->pizza_consuming));
    return;
}

char* get_pizza_from_courier(void *p)
{
    struct pizza *buf = (struct pizza *)p;
    pthread_cond_wait(&(buf->pizza_delivered), &(buf->pizza_consuming));
    return buf->pizza_message;
}

int check_last_pizza_num(void *p)
{
    struct pizza *buf = (struct pizza *)p;
    return buf->last_pizza_num;
}

int is_producing_needed(void *p)
{
    struct pizza *buf = (struct pizza *)p;
    if (buf->total_amount_of_pizzas_produced == NUMBER_OF_PIZZAS)
        return 0;
    return 1;
}

void pizza_put_d(void *p)
{
    struct pizza *buf = (struct pizza *)p;
    strncat(buf->current_Pizza, "d", 1);
    return;
}

void pizza_put_o(void *p)
{
    struct pizza *buf = (struct pizza *)p;
    strncat(buf->current_Pizza, "o", 1);
    return;
}

void pizza_put_in_a_queue(void *p)
{
    struct pizza *buf = (struct pizza *)p;
    buf->last_pizza_num++;
    if (buf->last_pizza_num > N_OF_PIZZAS_IN_A_QUEUE - 1)
        pthread_cond_wait(&(buf->empty), &(buf->pizza_queue_managing));

    strncpy(buf->pizza_queue[buf->last_pizza_num], buf->current_Pizza, 5);
    for (int i = 0; i < PIZZA_ARRAY_SIZE; i++)
        buf->current_Pizza[i] = 0;
    pthread_cond_signal(&(buf->full));
    return;
}

char *pizza_get_from_queue(void *p)
{
    struct pizza *buf = (struct pizza *)p;
    if(!(is_producing_needed(buf) || check_last_pizza_num(buf)))
        return NULL;
    char *pizza_buf = (char *)calloc(PIZZA_ARRAY_SIZE, sizeof(char));
    if (buf->last_pizza_num == 0)
        pthread_cond_wait(&(buf->full), &(buf->pizza_queue_managing));
    strncpy(pizza_buf, buf->pizza_queue[buf->last_pizza_num], PIZZA_ARRAY_SIZE);
    for (int i = 0; i < PIZZA_ARRAY_SIZE; i++)
        (buf->pizza_queue[buf->last_pizza_num])[i] = 0;
    buf->last_pizza_num--;
    pthread_cond_signal(&(buf->empty));
    return pizza_buf;
}

void *povar_d(void *p);
void *povar_o(void *p);
void *courier(void *p);
void *consumer(void *p);

void check_for_dodo(char *buf);


int main()
{
    struct pizza buf = {{}, {}, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    pthread_mutex_init(&(buf.pizza_creation), 0);
    pthread_mutex_init(&(buf.pizza_queue_managing), 0);

    pthread_t pov_d, pov_o, cour, cons;

    pthread_create(&pov_d, NULL, povar_d, &buf);
    pthread_create(&pov_o, NULL, povar_o, &buf);
    pthread_create(&cour, NULL, courier, &buf);
    pthread_create(&cons, NULL, consumer, &buf);    

    pthread_join(pov_d, NULL);
    pthread_join(pov_o, NULL);
    pthread_join(cour, NULL);
    pthread_join(cons, NULL);

    pthread_mutex_destroy(&(buf.pizza_creation));
    pthread_mutex_destroy(&(buf.pizza_queue_managing));
    return 0;
}

void *povar_d(void *p)
{
    struct pizza *buf = (struct pizza *)p;

    while (is_producing_needed(buf))
    {
        pizza_put_d(buf);
        // printf("managed to put 1st d\n");
        pthread_cond_signal(&(buf->d_ready));
        pthread_cond_wait(&(buf->o_ready), &(buf->pizza_creation));
        // pthread_mutex_lock(&(buf->pizza_creation));
        pizza_put_d(buf);
        // printf("managed to put 2nd d\n");
        pthread_cond_signal(&(buf->d_ready));
        // printf("imma, alive(povar_d)\n");
        pthread_cond_wait(&(buf->o_ready), &(buf->pizza_creation));
        // printf("imma, alive(povar_d)\n");
    }
    printf("povar_d finished his work\n");
    return NULL;
}

void *povar_o(void *p)
{
    struct pizza *buf = (struct pizza *)p;

    while (is_producing_needed(buf))
    {
        pthread_cond_wait(&(buf->d_ready), &(buf->pizza_creation));
        // pthread_mutex_lock(&(buf->pizza_creation));
        pizza_put_o(buf);
        // printf("managed to put 1st o\n");
        pthread_cond_signal(&(buf->o_ready));
        // printf("line %d: sent siganl to povar_d\n", __LINE__);
        pthread_cond_wait(&(buf->d_ready), &(buf->pizza_creation));

        pizza_put_o(buf);
        // printf("managed to put 2nd o\n");
        // printf("current pizza == %s\n", buf->current_Pizza);

        pthread_mutex_lock(&(buf->pizza_queue_managing));
        pizza_put_in_a_queue(buf);
        // printf("i'm alive\n");
        buf->total_amount_of_pizzas_produced++;
        // printf("i'm alive %d and total amount of produced pizza is %d\n", __LINE__, buf->total_amount_of_pizzas_produced);
        pthread_mutex_unlock(&(buf->pizza_queue_managing));

        pthread_cond_signal(&(buf->o_ready));
    }
    pthread_mutex_unlock(&(buf->pizza_creation));
    printf("povar_o finished his work\n");
    return NULL;
}

void *courier(void *p)
{
    struct pizza *buf = (struct pizza *)p;
    char* pizza_buf;
    while ((pizza_buf = pizza_get_from_queue(buf)) != NULL)
    {
        sleep(0.2); // getting pizza to the consumer
        send_pizza_to_customer(pizza_buf, buf);
    }
    send_pizza_to_customer(pizza_buf, buf);
    printf("courier finished his work\n");
    return NULL;
}

void *consumer(void *p)
{
    struct pizza *buf = (struct pizza *)p;
    char* pizza_to_eat;
    while((pizza_to_eat = get_pizza_from_courier(buf)) != NULL)
    {
        check_for_dodo(pizza_to_eat);
        free(pizza_to_eat);             //consuming pizza
        pthread_cond_signal(&(buf->pizza_delivered));
    }
    printf("consumer finished his work\n");
    return NULL;
}

void check_for_dodo(char *buf)
{
    if (strncmp(buf, "dodo", PIZZA_ARRAY_SIZE) == 0)
        printf("dodo approved\n");
    else
    {
        printf("dodo disapproved: ");
        fwrite(buf, 1, PIZZA_ARRAY_SIZE, stdout);
        printf("\n");
    }
    return;
}
