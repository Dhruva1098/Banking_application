#include "headers/customer_handler.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>

void view_account_balance(int connfd, char* customer_id){
	FILE *file = fopen("database/customer_db", "r");
	if (file == NULL){
		char* error_msg = "Error: Could not open customer data";

