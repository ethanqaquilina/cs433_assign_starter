/**
* Assignment 3: CPU Scheduler
 * @file scheduler_fcfs.cpp
 * @author Ethan Aquilina (TODO: your name)
 * @brief This Scheduler class implements the FCSF scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "scheduler_fcfs.h"
#include <iostream>
#include <queue>

// TODO: add implementation of SchedulerFCFS constructor, destrcutor and 
// member functions init, print_results, and simulate here

SchedulerFCFS::SchedulerFCFS(){}

SchedulerFCFS::~SchedulerFCFS(){}

void SchedulerFCFS::init(std::vector<PCB>& process_list){
    this->ready_queue = std::queue<PCB>(); // Clear any existing data
    for (const auto& process : process_list) {
        this->ready_queue.push(process); // Enqueue processes in order
    }
}


void SchedulerFCFS::print_results(){
    for (const auto& process : completed_processes) {
        std::cout << process.name << " turn-around time = " << process.turnaround_time
                  << ", waiting time = " << process.waiting_time << std::endl;
    }
    // Average turn-around time = 94.375, Average waiting time = 73.125
    cout << "Average turn-around time = " << avg_turnaroundtime << ", Average waiting time = " << avg_waitingtime << endl;
    
}

void SchedulerFCFS::simulate(){
    int current_time = 0;
    float total_TAT = 0;
    float total_WT = 0;

    while (!ready_queue.empty()) {
        PCB process = ready_queue.front();
        ready_queue.pop();
        
        process.waiting_time = current_time;
        total_WT += process.waiting_time;

        process.start_time = current_time;
        process.turnaround_time = current_time + process.burst_time;

        current_time = process.turnaround_time;
        total_TAT += process.turnaround_time;
        
        completed_processes.push_back(process);
    }

    this->avg_turnaroundtime = total_TAT/completed_processes.size();
    this->avg_waitingtime = total_WT/completed_processes.size();

}