#ifndef _LEDGER_H
#define _LEDGER_H

#include <bank.h>

using namespace std;

#define D 0
#define W 1
#define T 2

const int SEED_RANDOM = 377;

struct Ledger {
	int from;
	int to;
	int amount;
  	int mode;
	int ledgerID;
};

extern list<struct Ledger> ledger;

void InitBank(int num_workers, char *filename);
void load_ledger(char *filename);
void *worker(void *unused);

#endif