#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_COFFEE 100
#define MIN_COFFEE 3
#define PLAYERS_THREAD 5

int coffee_available, coffee_drunk[PLAYERS_THREAD];
int winnerPos = 0, winner[PLAYERS_THREAD];
sem_t mutex;

void printCoffee() {
  sem_wait(&mutex);
  for (int i = 0; i < PLAYERS_THREAD; i++) {
    printf("Players %d drinks %d\n", i+1, coffee_drunk[i]);
  }
  printf("-------------------------------\n\n");
  sem_post(&mutex);
}

void drink_coffee(int *player) {
  if (coffee_available < 0) return;
  sem_wait(&mutex);
  coffee_drunk[*player] += 1;
  printf("Player %d drinks a coffee\n", *player);
  coffee_available -= 1;
  sem_post(&mutex);
}

void *player(void *drunkArr) {
  int *playerNum = drunkArr;
  while (coffee_available > 0) {
    drink_coffee(playerNum);
    printCoffee();
    usleep(50000);
  }
  winner[winnerPos] = *playerNum;
  winnerPos++;
  return NULL;
}

int main() {
  pthread_t players[PLAYERS_THREAD];
  int i, a[PLAYERS_THREAD];
  sem_init(&mutex, 0, 1);
  printf("How many coffee available?\n");
  scanf("%d", &coffee_available);

  for (i = 0; i < PLAYERS_THREAD; i++) {
    a[i] = i;
    pthread_create(&players[i], NULL, player, (void *) &a[i]);
  }
  for (i = 0; i < PLAYERS_THREAD; i++) {
    void *playerNum;
    pthread_join(players[i], &playerNum);
  }
  
  printf("\nWinners:\n");
  for (i = 0; i < PLAYERS_THREAD; i++) {
    printf("%d%s place: Player %d\n", i+1,
            i == 0 ? "st" : i == 1 ? "nd" : i == 3 ? "rd" : "th",
            winner[i]+1);
  }
  
  return 0;
}
