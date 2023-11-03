// Gabriel Garcia - Z23643475
// COP4610 - Prof. Borko Furht 
// CPU Scheduler Project
// 10/18/23

#include "FirstComeFirstServe.hpp"

#include <iostream>

FirstComeFirstServe::FirstComeFirstServe(const std::vector<ProcessEntry::ProcessData>& process_data) {
	for (const ProcessEntry::ProcessData& process_data_entry : process_data) {
		ProcessEntry* process_entry = new ProcessEntry(process_data_entry);
		process_working_set.push_back(process_entry);
		ready_queue.push_back(process_entry);
	}
}

bool FirstComeFirstServe::queues_empty() {
	return ready_queue.empty() && io_processes.empty();
}

int32_t FirstComeFirstServe::get_current_process_burst_time(ProcessEntry* process_entry) {
	return process_entry->get_current_burst_time();
}

void FirstComeFirstServe::process_current_burst(ProcessEntry* process_entry, const int32_t time) {
	process_entry->process_current_burst(time);
}

ProcessEntry* FirstComeFirstServe::get_next_process() {
	ProcessEntry* process_entry = nullptr;
	
	if (!ready_queue.empty()) {
		// Grab the first process off the front of the ready queue.
		process_entry = ready_queue.front();
		ready_queue.pop_front();
	}
	else {
		// To simulate the concurrent nature of I/O, the shortest I/O burst is grabbed.
		io_processes.sort(shortest_process_sort);
		process_entry = io_processes.front();
		io_processes.pop_front();
	}

	assert(process_entry != nullptr);

	return process_entry;
}

void FirstComeFirstServe::display_current_process_stats(ProcessEntry* process_entry) {
	std::cout << "\nCurrent Execution Time: " << execution_time << "\n";
	std::cout << "Processing a" << ((process_entry->get_current_state() == ProcessEntry::CPU_BURST) ? " CPU Burst" : "n I/O Burst") << ".\n";
	std::cout << "Running Process ID: " << process_entry->get_process_ID() << "\n\n";
	
	std::cout << "Processes in Ready Queue:\n";
	if (!ready_queue.empty())
		for (const ProcessEntry* process_entry : ready_queue)
			std::cout << "\tProcess ID: " << process_entry->get_process_ID() << ", Next CPU Burst: " << process_entry->get_current_burst_time() << ".\n";
	else
		std::cout << "\tQueue Empty.\n";
	std::cout << "Processes waiting on I/O:\n";
	if (!io_processes.empty())
		for (const ProcessEntry* process_entry : io_processes)
			std::cout << "\tProcess ID: " << process_entry->get_process_ID() << ", Remaining I/O Time: " << process_entry->get_current_burst_time() << ".\n";
	else
		std::cout << "\tNo processes waiting on I/O.\n";
}

void FirstComeFirstServe::update_response_and_wait_times(const int32_t time) {
	for (ProcessEntry* process_entry : ready_queue) {
		ProcessResult& process_result = process_entry->get_process_result();
		// Increment the waiting time of processes sitting in the queue.
		process_result.add_to_waiting_time(time);
		// Increment the response time of those that haven't been started yet.
		if (!process_entry->started())
			process_result.add_to_response_time(time);
	}
}

void FirstComeFirstServe::send_to_ready_queue(ProcessEntry* process_entry) {
	ready_queue.push_back(process_entry);
}