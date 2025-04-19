/**
* Assignment 4: Producer Consumer Problem
 * @file main.cpp
 * @author Ethan Aquilina (TODO: your name)
 * @brief The main program for the producer consumer problem.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <cstdlib>
#include "buffer.h"

using namespace std;
// global buffer object
Buffer buffer;


// Producer thread function
void *producer(void *param) {
    buffer_item item = *((int *) param);

    while (true) {
        usleep(rand() % 1000000); // sleep 0–999ms

        if (buffer.insert_item(item)) {
            cout << "Producer " << item << ": Inserted item " << item << endl;
            buffer.print_buffer();
        } else {
            cout << "Producer error condition" << endl;
        }
    }
}

// Consumer thread function
void *consumer(void *param) {
    buffer_item item;

    while (true) {
        usleep(rand() % 1000000); // sleep 0–999ms

        if (buffer.remove_item(&item)) {
            cout << "Consumer " << item << ": Removed item " << item << endl;
            buffer.print_buffer();
        } else {
            cout << "Consumer error condition" << endl;
        }
    }
}

int main(int argc, char *argv[]) {
    // 1. Get command line arguments
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <sleep time> <# of producers> <# of consumers>" << endl;
        return -1;
    }

    int sleep_time = atoi(argv[1]);
    int num_producers = atoi(argv[2]);
    int num_consumers = atoi(argv[3]);

    // 2. Seed the random number generator
    srand(time(NULL));

    // 3. Create producer threads
    pthread_t producers[num_producers];
    int producer_ids[num_producers]; // to uniquely identify each thread

    for (int i = 0; i < num_producers; ++i) {
        producer_ids[i] = i + 1;
        if (pthread_create(&producers[i], NULL, producer, &producer_ids[i]) != 0) {
            cerr << "Error creating producer thread " << i + 1 << endl;
            return -1;
        }
    }

    // 4. Create consumer threads
    pthread_t consumers[num_consumers];
    int consumer_ids[num_consumers]; // could be used for unique names if needed

    for (int i = 0; i < num_consumers; ++i) {
        consumer_ids[i] = i + 1;
        if (pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]) != 0) {
            cerr << "Error creating consumer thread " << i + 1 << endl;
            return -1;
        }
    }

    // 5. Main thread sleeps for specified time
    sleep(sleep_time);

    // 6. Exit the program
    cout << "Main thread exiting after " << sleep_time << " seconds." << endl;
    return 0;
}