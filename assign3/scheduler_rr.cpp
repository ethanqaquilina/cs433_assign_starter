/**
 * Assignment 3: CPU Scheduler
 * @file scheduler_rr.h
 * @author Ethan Aquilina (TODO: your name)
 * @brief This Scheduler class implements the RoundRobin (RR) scheduling algorithm.
 * @version 0.1
 */
// You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
//  Remember to add sufficient and clear comments to your code
//

#include "scheduler_rr.h"
#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <algorithm>

/**
 * @brief Construct a new SchedulerRR object
 * @param time_quantum The time slice for each process
 */
SchedulerRR::SchedulerRR(int time_quantum)
    : time_quantum(time_quantum) {}

/**
 * @brief Destroy the SchedulerRR object
 */
SchedulerRR::~SchedulerRR() {}

/**
 * @brief This function is called once before the simulation starts.
 *        It is used to initialize the scheduler.
 * @param process_list The list of processes in the simulation.
 */
void SchedulerRR::init(std::vector<PCB> &process_list)
{
    for (auto &process : process_list)
    {
        process.og_BT = process.burst_time;
        ready_queue.push(process); // Push all processes to the ready queue
    }
}

/**
 * @brief This function simulates the scheduling of processes in the ready queue.
 *        It stops when all processes are finished.
 */
void SchedulerRR::simulate()
{
    int current_time = 0;

    // While there are still processes in the ready queue
    while (!ready_queue.empty())
    {
        PCB process = ready_queue.front(); // Get the first process
        ready_queue.pop();                 // Remove it from the queue

        // If the process arrives after the current time, we jump to the arrival time
        if (current_time < process.arrival_time)
        {
            current_time = process.arrival_time;
        }

        // Calculate the time used in this quantum
        int time_used = 0;
        if (time_quantum > process.burst_time)
        {
            time_used = process.burst_time;
        }
        else
        {
            time_used = time_quantum;
        }

        // Set the start time of the process
        process.start_time = current_time;

        // Calculate TAT
        process.turnaround_time = process.start_time + time_used - process.arrival_time;

        // Update the current time after the process finishes
        current_time = process.start_time + time_used;

        // Decrease the burst time of the process by the amount of time used
        process.burst_time -= time_used;

        // If the process still has remaining burst time, push it back into the queue
        if (process.burst_time > 0)
        {
            ready_queue.push(process);
        }
        else
        {
            process.waiting_time = process.turnaround_time - process.og_BT;
            completed_processes.push_back(process); // If done, add to completed
        }
    }
}

/**
 * @brief This function is called once after the simulation ends.
 *        It is used to print out the results of the simulation.
 */
void SchedulerRR::print_results()
{
    // Print the details for each process
    for (const auto &process : completed_processes)
    {
        std::cout << process.name << ": Turn-around time = " << process.turnaround_time
                  << ", Waiting time = " << process.waiting_time << std::endl;
    }

    // Calculate and print the average turn-around and waiting time
    float avg_turnaround_time = 0.0;
    float avg_waiting_time = 0.0;
    for (const auto &process : completed_processes)
    {
        avg_turnaround_time += process.turnaround_time;
        avg_waiting_time += process.waiting_time;
    }

    avg_turnaround_time /= completed_processes.size();
    avg_waiting_time /= completed_processes.size();

    std::cout << "Average turn-around time = " << avg_turnaround_time
              << ", Average waiting time = " << avg_waiting_time << std::endl;
}