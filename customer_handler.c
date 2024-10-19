
#include "headers/customer_handler.h"

#include <stdio.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/file.h>

#define DB_PATH "database/customer_db"
#define BUFFER_SIZE 1024

unsigned long hashed_password(const char *password) {
  unsigned long hash = 5381;
  int c;
  while ((c = *password++)) {
    hash = ((hash << 5) + hash) + c;
  }
  return hash;
}
void lock_customer(int fd, int account_id) {
  struct flock lock = {0};
  lock.l_type = F_WRLCK;
  lock.l_whence = SEEK_SET;
  lock.l_start = sizeof(customerRecord) * account_id;
  lock.l_len = sizeof(customerRecord);
  fcntl(fd, F_SETLKW, &lock);
}
void unlock_customer(int fd, int account_id) {
  struct flock lock = {0};
  lock.l_type = F_UNLCK;
  lock.l_whence = SEEK_SET;
  lock.l_start = sizeof(customerRecord) * account_id;
  lock.l_len = sizeof(customerRecord);
  fcntl(fd, F_SETLK, &lock);
}
int fetch_account_balance(int account_id, int* balance) {  // helper
  int fd = open(DB_PATH, O_RDONLY);
  if (fd < 0) { return -1; }
  customerRecord record;
  lseek(fd, sizeof(customerRecord) * account_id, SEEK_SET);
  read(fd, &record, sizeof(customerRecord));
  close(fd);
  *balance = record.balance;
  return 0;
}

void view_account_balance(int connfd, int account_id) {
  int balance;
  if (fetch_account_balance(account_id, &balance) == 0) {
    char response[100];
    snprintf(response, sizeof(response), "Your balance is: %d\n", balance);
    write(connfd, response, strlen(response));
  } else {
    char response[] = "Unable to fetch your balance \n";
    write(connfd, response, strlen(response));
  }
}
