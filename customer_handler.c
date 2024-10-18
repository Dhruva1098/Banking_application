
#include "headers/customer_handler.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/file.h>

#define DB_PATH "database/customer_db"
#define BUFFER_SIZE 1024

int fetch_account_balance(int account_id, int* balance) {  // helper
  FILE *file = fopen(DB_PATH, "r");
  if (file == NULL) {
    perror("Error opening customer database");
    return -1;
  }
  int fd = fileno(file);
  if (flock(fd, LOCK_SH) != 0) {
    perror("error locking the file for reading");
    fclose(file);
    return -1;
  }
  char line[BUFFER_SIZE];
  while (fgets(line, sizeof(line), file)) {
    int file_account_id;
    int file_account_balance;
    sscanf(line, "%d, %d", &file_account_id, &file_account_balance);
    if (file_account_id == account_id) {
      *balance = file_account_balance;
      flock(fd, LOCK_UN);
      fclose(file);
      return 0;
    }
  }
  flock(fd, LOCK_UN);
  fclose(file);
  return -1;  // account not found
}

void view_account_balance(int connfd) {
  char account_id_buff[BUFFER_SIZE] = {0};
  char request[] = "please enter your account ID:\n";

