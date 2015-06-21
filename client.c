#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "buffer.h"

void *producer(void *);
void *consumer(void *);

char texto[] = "Texto a ser escrito no buffer para testes...";
FILE *fr;

#define check(A,msg) \
  if ((A)) { \
    printf("##msg"); \
    exit(1); \
  }
    
int main()
{
  buffer *b = malloc(sizeof(buffer));
  pthread_t pthread_consumidor, pthread_produtor;
  initBuffer(b);

  check(pthread_create(&pthread_produtor, NULL, producer, (void *)b), "ERROR creating thread producer!!\n");
  check(pthread_create(&pthread_consumidor, NULL, consumer, (void *)b), "ERROR creating thread consumer!!\n")
  check(pthread_join(pthread_produtor, NULL), "ERROR joining thread producer")
  check(pthread_join(pthread_consumidor, NULL), "ERROR joining thread consumer")

  destructBuffer(b);

  pthread_exit(NULL);
  return 1;
}

void *producer(void * d)
{
  char ch;
  buffer *pb = (buffer *)d;
  fr = fopen ("disco", "rt");
  ch = fgetc(fr);
  while( ch != EOF )
  {
    printf("Producer: %c\n", ch);
    pb->insert(pb, ch);
    ch = fgetc(fr); 
  }
  pb->insert(pb, ch);
  fclose(fr);  /* close the file prior to exiting the routine */
  return 0;
}
void *consumer(void *d)
{
  char ch;
  buffer *pb = (buffer *)d;
  ch = pb->get(pb);
  while( ch != EOF )
  {
    printf("\t\tConsumer: %c\n", ch);
    ch = pb->get(pb);
  }
  return 0;
}
