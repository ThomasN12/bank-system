#ifndef _BANK_H
#define _BANK_H

#include <stdlib.h>
#include <fstream>
#include <string>
#include <sys/wait.h>   /* for wait() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <sys/mman.h>   /* for mmap() ) */
#include <semaphore.h>  /* for sem */
#include <assert.h>		/* for assert */
#include <iostream>     /* for cout */
#include <list>
#include <array>
#include <pthread.h> 

using namespace std;

struct Account {
  unsigned int accountID; 
  long balance;
  pthread_mutex_t lock;
};


class Bank {
  private:
    int num;
    int num_succ;
    int num_fail;
    
  public:
    Bank(int N);
    ~Bank(); // destructor
    
    int deposit(int workerID, int ledgerID, int accountID, int amount);
    int withdraw(int workerID, int ledgerID, int accountID, int amount);
    int transfer(int workerID, int ledgerID, int src_id, int dest_id, unsigned int amount);
    
    void print_account();
    void recordSucc(char* message);
    void recordFail(char* message);

    pthread_mutex_t bank_lock;
    struct Account *accounts;
};

#endif