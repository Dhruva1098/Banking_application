#ifndef SS_PROJECT_HEADERS_CUSTOMER_HANDLER_H_
#define SS_PROJECT_HEADERS_CUSTOMER_HANDLER_H_

int fetch_account_balance(int account_id, int* balance);
void view_account_balance(int connfd);
void deposit_money(int connfd);
void withdraw_money(int connfd);
void transfer_funds(int connfd);
void apply_for_loan(int connfd);
void change_password(int connfd);
void add_feedback(int connfd);
void view_transaction_history(int connfd);
void customer_handler(int connfd);

#endif  // SS_PROJECT_HEADERS_CUSTOMER_HANDLER_H_
