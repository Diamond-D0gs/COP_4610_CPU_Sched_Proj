// Gabriel Garcia - Z23643475
// COP4610 - Prof. Borko Furht 
// CPU Scheduler Project
// 10/18/23

#pragma once

#include "ProcessEntry.hpp"

#include <string>

class CPUScheduler {
private:
	// Toggles whether or not to display context switch info.
	static const bool DISPLAY_CONTEXT_SWITCH_INFO = false;

	int32_t total_turn_around_time = 0;
	double average_turn_around_time = 0.0f;
	double average_response_time = 0.0f;
	double average_waiting_time = 0.0f;
	double cpu_utilization = 0.0f;

	void update_io_queue(const int32_t time);

	void retire_process(ProcessEntry* process_entry);

	void display_results();

protected:
	int32_t execution_time = 0;

	// Holds a copy of the original data set that all the queues store references to.
	std::vector<ProcessEntry*> process_working_set;
	// Holds all the processes currently in their I/O burst.
	std::list<ProcessEntry*> io_processes;

	virtual bool queues_empty() = 0;

	virtual ProcessEntry* get_next_process() = 0;

	virtual void display_current_process_stats(ProcessEntry* process_entry) = 0;

	virtual int32_t get_current_process_burst_time(ProcessEntry* process_entry) = 0;

	virtual void process_current_burst(ProcessEntry* process_entry, const int32_t time) = 0;

	virtual void update_response_and_wait_times(const int32_t time) = 0;

	virtual void send_to_ready_queue(ProcessEntry* process_entry) = 0;

	static bool shortest_process_sort(const ProcessEntry* a, const ProcessEntry* b);

public:
	virtual ~CPUScheduler() = default;

	void run();
};