// Gabriel Garcia - Z23643475
// COP4610 - Prof. Borko Furht 
// CPU Scheduler Project
// 10/18/23

#include "CPUScheduler.hpp"

#include <iostream>

void CPUScheduler::update_io_queue(const int32_t time) {
	std::list<ProcessEntry*> new_io_processes;
	
	// Forward all processes on the I/O queue by "time".
	for (ProcessEntry* process_entry : io_processes) {
		int32_t ready_time = process_entry->process_current_burst(time);
		// All processes that go zero or less are placed back onto the ready queue.
		if (ready_time <= 0)
			send_to_ready_queue(process_entry);
		else
			new_io_processes.push_back(process_entry);
	}

	// Replace old I/O processes with the new one.
	io_processes = std::move(new_io_processes);
}

void CPUScheduler::retire_process(ProcessEntry* process_entry) {
	if (!process_entry->done()) { // Let the process reference be lost if it's done.
		if (process_entry->get_current_state() == ProcessEntry::CPU_BURST)
			send_to_ready_queue(process_entry); // Else commit it to the ready queue.
		else
			io_processes.push_back(process_entry); // Or the I/O process list.
	}
	else // Display that the process has completed.
		std::cout << "\nProcess ID: " << process_entry->get_process_ID() << " completed.\n";
}

void CPUScheduler::run() {
	while (!queues_empty()) { // Keep running the loop while there are still processes in queues.
		ProcessEntry* current_process = get_next_process();

		if (DISPLAY_CONTEXT_SWITCH_INFO)
			display_current_process_stats(current_process);

		int32_t time = get_current_process_burst_time(current_process);

		process_current_burst(current_process, time);

		update_response_and_wait_times(time);

		update_io_queue(time);

		retire_process(current_process);

		execution_time += time;
	}

	// Get the sum of each process' times.
	for (const ProcessEntry* process_entry : process_working_set) {
		const ProcessResult& process_result = process_entry->get_process_result();
		total_turn_around_time += process_result.get_turn_around_time();
		average_response_time += process_result.get_response_time();
		average_waiting_time += process_result.get_waiting_time();
		cpu_utilization += process_result.get_cpu_time();
	}

	// Divide by the number of processes to get the average.
	average_turn_around_time = static_cast<double>(total_turn_around_time) / process_working_set.size();
	average_response_time /= process_working_set.size();
	average_waiting_time /= process_working_set.size();

	// Calculate the % CPU utilization.
	cpu_utilization = 100.0f - ((cpu_utilization / total_turn_around_time) * 100.0f);

	display_results();
}

void CPUScheduler::display_results() {
	// Iterate through the processes and generate a list of their individual results.
	std::cout << "\nProcess Results:\n";
	for (uint32_t i = 0; i < process_working_set.size(); ++i) {
		const ProcessResult& process_result = process_working_set[i]->get_process_result();
		std::cout << "Process ID: " << (i + 1) << ") Waiting Time: " << process_result.get_waiting_time() << ", Response Time: "
			<< process_result.get_response_time() << ", Turn Around Time: " << process_result.get_turn_around_time() << ".\n";
	}

	// Append the total turn around time.
	std::cout << "\nTotal Turn Around Time: " << total_turn_around_time << "\n\n";

	// Apppend the averages of their results.
	std::cout << "Average Waiting Time: " << average_waiting_time << "\n";
	std::cout << "Average Response Time: " << average_response_time << "\n";
	std::cout << "Average Turn Around Time: " << average_turn_around_time << "\n\n";

	// Append the CPU utilization.
	std::cout << "CPU Utilization: %" << cpu_utilization << "\n\n";
}

bool CPUScheduler::shortest_process_sort(const ProcessEntry* a, const ProcessEntry* b) {
	return a->get_current_burst_time() < b->get_current_burst_time();
}