/**
 * Assignment 3: CPU Scheduler
 * @file scheduler_sjf.cpp
 * @author Ethan Aquilina (TODO: your name)
 * @brief This Scheduler class implements the SJF scheduling algorithm.
 * @version 0.1
 */
// You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
//  Remember to add sufficient and clear comments to your code

#include "scheduler_sjf.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <unordered_map>

/**
 * @brief Construct a new SchedulerSJF object
 */
SchedulerSJF::SchedulerSJF()
    : ready_queue([](const PCB &a, const PCB &b)
                  {
                      return a.burst_time > b.burst_time; // Min-heap based on burst time
                  })
{
}

/**
 * @brief Destroy the SchedulerSJF object
 */
SchedulerSJF::~SchedulerSJF() {}

/**
 * @brief Initialize the scheduler with the process list
 * @param process_list The list of processes in the simulation.
 */
void SchedulerSJF::init(std::vector<PCB> &process_list)
{
    // Store processes in original order
    for (const auto &process : process_list)
    {
        original_processes.push_back(process); // Save original order
        ready_queue.push(process);             // Insert into the priority queue
    }
}

/**
 * @brief Simulate the SJF scheduling algorithm
 */
void SchedulerSJF::simulate()
{
    int current_time = 0;
    float total_TAT = 0;
    float total_WT = 0;

    // Process all the jobs in the ready_queue
    while (!ready_queue.empty())
    {
        // Get the process with the shortest burst time
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

        // Push the completed process to the completed list
        completed_processes.push_back(process);
    }

    // Calculate average turnaround time and waiting time
    this->avg_turnaroundtime = total_TAT / completed_processes.size();
    this->avg_waitingtime = total_WT / completed_processes.size();
}

/**
 * @brief Print the results of the simulation in the original order
 */
void SchedulerSJF::print_results()
{
    // Map each process to its turnaround and waiting times
    std::unordered_map<int, std::pair<float, float>> process_times;

    for (const auto &process : completed_processes)
    {
        process_times[process.id] = {process.turnaround_time, process.waiting_time};
    }

    // Print the results for each process in its original arrival order
    for (const auto &process : original_processes)
    {
        int id = process.id;
        std::cout << "Process " << process.name << ": Turn-around time = "
                  << process_times[id].first << ", Waiting time = "
                  << process_times[id].second << std::endl;
    }

    // Print average turn-around time and waiting time
    std::cout << "Average turn-around time = " << avg_turnaroundtime
              << ", Average waiting time = " << avg_waitingtime << std::endl;
}