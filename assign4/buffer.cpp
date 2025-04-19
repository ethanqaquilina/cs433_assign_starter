/**
* Assignment 4: Producer Consumer Problem
 * @file buffer.cpp
 * @author Ethan Aquilina (TODO: your name)
 * @brief Implementation file for the buffer class
 * @version 0.1
 */

// You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code
#include "buffer.h"
#include <iostream>
#include <semaphore.h>
#include <vector>

using namespace std;

Buffer::Buffer(int size)
    : size(size), count(0), in(0), out(0), buffer(size) {
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&not_full, nullptr);
    pthread_cond_init(&not_empty, nullptr);
}

Buffer::~Buffer() {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);
}

bool Buffer::insert_item(buffer_item item) {
    pthread_mutex_lock(&mutex);

    while (count == size) {
        pthread_cond_wait(&not_full, &mutex);
    }

    buffer[in] = item;
    in = (in + 1) % size;
    count++;

    pthread_cond_signal(&not_empty);
    pthread_mutex_unlock(&mutex);
    return true;
}

bool Buffer::remove_item(buffer_item *item) {
    pthread_mutex_lock(&mutex);

    while (count == 0) {
        pthread_cond_wait(&not_empty, &mutex);
    }

    *item = buffer[out];
    out = (out + 1) % size;
    count--;

    pthread_cond_signal(&not_full);
    pthread_mutex_unlock(&mutex);
    return true;
}

int Buffer::get_size() {
    return size;
}

int Buffer::get_count() {
    return count;
}

bool Buffer::is_empty() {
    return count == 0;
}

bool Buffer::is_full() {
    return count == size;
}

void Buffer::print_buffer() {
    pthread_mutex_lock(&mutex);
    cout << "Buffer: [";
    for (int i = 0; i < count; ++i) {
        int index = (out + i) % size;
        cout << buffer[index];
        if (i < count - 1) cout << ", ";
    }
    cout << "]" << endl;
    pthread_mutex_unlock(&mutex);
}
