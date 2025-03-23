/**
 * Assignment 3: CPU Scheduler
 * @file scheduler_priority.cpp
 * @author Ethan Aquilina (TODO: your name)
 * @brief This Scheduler class implements the Priority scheduling algorithm.
 * @version 0.1
 */
// You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
//  Remember to add sufficient and clear comments to your code

#include "scheduler_priority.h"
#include <iostream>
#include <vector>
#include <queue>
#include <functional>

/**
 * @brief Construct a new SchedulerPriority object
 */
SchedulerPriority::SchedulerPriority()
    : ready_queue([](const PCB &a, const PCB &b)
                  {
                      return a.priority > b.priority; // Min-heap based on priority (lower priority number means higher priority)
                  })
{
}

/**
 * @brief Destroy the SchedulerPriority object
 */
SchedulerPriority::~SchedulerPriority() {}

/**
 * @brief Initialize the scheduler with the process list
 * @param process_list The list of processes in the simulation.
 */
void SchedulerPriority::init(std::vector<PCB> &process_list)
{
    for (const auto &process : process_list)
    {
        ready_queue.push(process); // Push all processes to the priority queue
    }
}

/**
 * @brief Simulate the Priority scheduling algorithm
 */
void SchedulerPriority::simulate()
{
    int current_time = 0;
    float total_TAT = 0;
    float total_WT = 0;

    // Process all the jobs in the ready_queue
    while (!ready_queue.empty())
    {
        // Get the process with the highest priority (lowest priority number)
        PCB process = ready_queue.top();
        ready_queue.pop();

        // If the process arrives after the current time, wait for its arrival
        if (current_time < process.arrival_time)
        {
            current_time = process.arrival_time;
        }

        // Calculate the waiting time for the process
        process.waiting_time = current_time - process.arrival_time;
        total_WT += process.waiting_time;

        // Set the start and turnaround times
        process.start_time = current_time;
        process.turnaround_time = process.start_time + process.burst_time - process.arrival_time;

        // Update the current time after the process finishes
        current_time = process.start_time + process.burst_time;
        total_TAT += process.turnaround_time;

        // Push the completed process to the completed list after calculation
        completed_processes.push_back(process);
    }

    // Calculate average turnaround time and waiting time
    this->avg_turnaroundtime = total_TAT / completed_processes.size();
    this->avg_waitingtime = total_WT / completed_processes.size();
}

/**
 * @brief Print the results of the simulation in original order
 */
void SchedulerPriority::print_results()
{
    for (const auto &process : completed_processes)
    {
        std::cout << process.name << ": Turn-around time = " << process.turnaround_time
                  << ", Waiting time = " << process.waiting_time << std::endl;
    }

    // Print average turn-around time and waiting time
    std::cout << "Average turn-around time = " << avg_turnaroundtime
              << ", Average waiting time = " << avg_waitingtime << std::endl;
}