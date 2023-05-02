#include <ledger.h>

using namespace std;


// pthread_mutex_t ledger_lock = NULL;
pthread_mutex_t ledger_lock;

list<struct Ledger> ledger;
Bank *bank;

/**
 * @brief creates a new bank object and sets up workers
 * 
 * Requirements:
 *  - Create a new Bank object class with 10 accounts.
 *  - Load the ledger into a list
 *  - Set up the worker threads.  
 *  
 * @param num_workers 
 * @param filename 
 */
void InitBank(int num_workers, char *filename) {
	// Create a new Bank object with 10 accounts.
	bank = new Bank(10);
	bank->print_account();
	// Load the ledger into a list.
	pthread_mutex_init(&ledger_lock, NULL);
	load_ledger(filename);

	// Set up the worker threads.
	pthread_t workers[num_workers];
	int workerIDs[num_workers];

	for (int i = 0; i < num_workers; i++) {
		workerIDs[i] = i;
		pthread_create(&workers[i], NULL, worker, (void*)&workerIDs[i]);
	}

	// Wait for worker threads to finish.
	for (int i = 0; i < num_workers; i++) {
		pthread_join(workers[i], NULL);
	}
	bank->print_account();
	// Free the Bank object.
	delete bank;
}

/**
 * @brief Parse a ledger file and store each line into a list
 * 
 * @param filename 
 */
void load_ledger(char *filename){
	
	ifstream infile(filename);
	int f, t, a, m, ledgerID=0;
	while (infile >> f >> t >> a >> m) {
		struct Ledger l;
		l.from = f;
		l.to = t;
		l.amount = a;
		l.mode = m;
		l.ledgerID = ledgerID++;
		ledger.push_back(l);
	}
}

/**
 * @brief Remove items from the list and execute the instruction.
 * 
 * @param workerID 
 * @return void* 
 */
void *worker(void *workerID) {
	int id = *((int*)workerID);
    while (!ledger.empty()) {
        pthread_mutex_lock(&ledger_lock);
        if (!ledger.empty()) {
            struct Ledger foundLedger = ledger.front();
            ledger.pop_front();
            pthread_mutex_unlock(&ledger_lock);
            if (foundLedger.mode == D) {
				bank->deposit(id, foundLedger.ledgerID, foundLedger.from, foundLedger.amount);
			} else if (foundLedger.mode == W) {
				bank->withdraw(id, foundLedger.ledgerID, foundLedger.from, foundLedger.amount);
			} else if (foundLedger.mode == T) {
				bank->transfer(id, foundLedger.ledgerID, foundLedger.from, foundLedger.to, foundLedger.amount);
			} else {
				cerr << "Unknown mode: " << foundLedger.mode << endl;
			}
        } else {
            pthread_mutex_unlock(&ledger_lock);
        }
    }
    return NULL;
}