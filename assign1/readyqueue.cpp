#include <iostream>
#include "readyqueue.h"

using namespace std;

//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient comments to your code


/**
 * @brief Constructor for the ReadyQueue class.
 */
 ReadyQueue::ReadyQueue()  {
     //TODO: add your code here
     head = nullptr;
     count = 0;
 }

/**
 *@brief Destructor
*/
ReadyQueue::~ReadyQueue() {
    //TODO: add your code to release dynamically allocate memory
    while(head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

/**
 * @brief Add a PCB representing a process into the ready queue.
 *
 * @param pcbPtr: the pointer to the PCB to be added
 */
void ReadyQueue::addPCB(PCB *pcbPtr) {
    //TODO: add your code here

    // When adding a PCB to the queue, you must change its state to READY.
    Node* newNode = new Node(pcbPtr);
    newNode->pcb->setState(ProcState::READY);

    // Check if queue is empty or if the head is less important than the pcb added
    if (count == 0 || pcbPtr->priority >= head->pcb->priority) {
        newNode->next = head;
        head = newNode;
    } else {
        Node* current = head;
        // Find the next spot in queue for PCB
        while(current->next != nullptr && pcbPtr->priority <= current->next->pcb->priority) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
    count++;
}

/**
 * @brief Remove and return the PCB with the highest priority from the queue
 *
 * @return PCB*: the pointer to the PCB with the highest priority
 */
PCB* ReadyQueue::removePCB() {
    //TODO: add your code here
    // When removing a PCB from the queue, you must change its state to RUNNING.

    // If empty queue return nullpter
    if(count == 0) {

        return nullptr;

    } else {

        // Remove the head node, has highest priority
        Node* removedNode = head;
        head = head->next;
        count--;

        // Save a pointer to the PCB to return
        PCB* removedPCB = removedNode->pcb;

        // Set state to RUNNING
        removedPCB->setState(ProcState::RUNNING);

        // Delete node that was removed
        delete removedNode;

        // Return pointer to PCB
        return removedPCB;
    }
}

/**
 * @brief Returns the number of elements in the queue.
 *
 * @return int: the number of PCBs in the queue
 */
int ReadyQueue::size() {
    //TODO: add your code here
    return count;
}

/**
 * @brief Display the PCBs in the queue.
 */
void ReadyQueue::displayAll() {
    //TODO: add your code here

    // If queue is empty
    if (count == 0) {
        cout << "Empty Queue\n";

    } else {

        // Point to first node and display each until the end
        Node* current = head;
        while(current != nullptr) {
            current->pcb->display();
            current = current->next;
        }
    }
}