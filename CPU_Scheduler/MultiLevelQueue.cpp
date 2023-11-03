// Gabriel Garcia - Z23643475
// COP4610 - Prof. Borko Furht 
// CPU Scheduler Project
// 10/18/23

#include "MultiLevelQueue.hpp"

#include <iostream>

MultiLevelQueue::MultiLevelQueue(const std::vector<ProcessEntry::ProcessData>& process_data) {
	// Populate the 1st round robin queue and the working set.
	for (const ProcessEntry::ProcessData& process_data_entry : process_data) {
		ProcessEntry* process_entry = new ProcessEntryEx(process_data_entry);
		process_working_set.push_back(process_entry);
		round_robin_1.push_back(process_entry);
	}
}

bool MultiLevelQueue::queues_empty() {
	return round_robin_1.empty() && round_robin_2.empty() && fcfs.empty() && io_processes.empty();
}

ProcessEntry* MultiLevelQueue::get_next_process() {
	ProcessEntry* process_entry = nullptr;

	// Grab from the queues based off their respective priorities.
	if (!round_robin_1.empty()) {
		process_entry = round_robin_1.front();
		round_robin_1.pop_front();
	}
	else if (!round_robin_2.empty()) {
		process_entry = round_robin_2.front();
		round_robin_2.pop_front();
	}
	else if (!fcfs.empty()) {
		process_entry = fcfs.front();
		fcfs.pop_front();
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

void MultiLevelQueue::display_current_process_stats(ProcessEntry* process_entry) {
	std::cout << "\nCurrent Execution Time: " << execution_time << "\n";
	
	std::cout << "Process belongs to the " << static_cast<ProcessEntryEx*>(process_entry)->get_current_queue_name() << " queue.\n";
	
	std::cout << "Processing a" << ((process_entry->get_current_state() == ProcessEntry::CPU_BURST) ? " CPU Burst" : "n I/O Burst") << ".\n";
	std::cout << "Running Process ID: " << process_entry->get_process_ID() << "\n\n";

	std::cout << "Processes in Round Robin 1 queue:\n";
	if (!round_robin_1.empty())
		for (const ProcessEntry* process_entry : round_robin_1)
			std::cout << "\tProcess ID: " << process_entry->get_process_ID() << ", Next CPU Burst: " << process_entry->get_current_burst_time() << ".\n";
	else
		std::cout << "\tQueue Empty.\n";
	std::cout << "Processes in Round Robin 2 queue:\n";
	if (!round_robin_1.empty())
		for (const ProcessEntry* process_entry : round_robin_2)
			std::cout << "\tProcess ID: " << process_entry->get_process_ID() << ", Next CPU Burst: " << process_entry->get_current_burst_time() << ".\n";
	else
		std::cout << "\tQueue Empty.\n";
	std::cout << "Processes in FCFS queue:\n";
	if (!fcfs.empty())
		for (const ProcessEntry* process_entry : fcfs)
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

int32_t MultiLevelQueue::get_current_process_burst_time(ProcessEntry* process_entry) {
	// Time is based off of which queue the process belongs to.
	if (process_entry->get_current_state() == ProcessEntry::CPU_BURST)
		switch (static_cast<ProcessEntryEx*>(process_entry)->get_current_queue()) {
		case ProcessEntryEx::ROUND_ROBIN_1:
			return ROUND_ROBIN_1_QUANTUM;
		case ProcessEntryEx::ROUND_ROBIN_2:
			return ROUND_ROBIN_2_QUANTUM;
		case ProcessEntryEx::FCFS:
			return process_entry->get_current_burst_time();
		default:
			assert(false); // Shouldn't be possible to get here, so we assert.
			return 0; // Return something so the compiler doesn't complain.
		}
	else // Bypass if currently in an I/O burst.
		return process_entry->get_current_burst_time();
}

void MultiLevelQueue::process_current_burst(ProcessEntry* process_entry, const int32_t time) {
	int32_t remaining_time = process_entry->process_current_burst(time);
	// If there is still remaining time, and we're in a CPU burst, demote the process' queue.
	if ((remaining_time > 0) && (process_entry->get_current_state() == ProcessEntry::CPU_BURST))
		static_cast<ProcessEntryEx*>(process_entry)->downgrade_queue();
}

void MultiLevelQueue::update_response_and_wait_times(const int32_t time) {
	for (ProcessEntry* process_entry : round_robin_1) {
		ProcessResult& process_result = process_entry->get_process_result();
		// Update the waiting times of processes sitting in the 1st Round Robin queue.
		process_result.add_to_waiting_time(time);
		// Update the response times of the processes that haven't started yet.
		if (!process_entry->started())
			process_result.add_to_response_time(time);
	}

	// Update the waiting times of processes sitting in the 2nd Round Robin queue.
	for (ProcessEntry* process_entry : round_robin_2)
		process_entry->get_process_result().add_to_waiting_time(time);

	// Update the waiting times of processes sitting in the FCFS queue.
	for (ProcessEntry* process_entry : fcfs)
		process_entry->get_process_result().add_to_waiting_time(time);
}

void MultiLevelQueue::send_to_ready_queue(ProcessEntry* process_entry) {
	switch (static_cast<ProcessEntryEx*>(process_entry)->get_current_queue()) {
	case ProcessEntryEx::ROUND_ROBIN_1:
		round_robin_1.push_back(process_entry);
		break;
	case ProcessEntryEx::ROUND_ROBIN_2:
		round_robin_2.push_back(process_entry);
		break;
	case ProcessEntryEx::FCFS:
		fcfs.push_back(process_entry);
		break;
	default:
		// Shouldn't be possible to get here, so we assert.
		assert(false);
	}
}