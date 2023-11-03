// Gabriel Garcia - Z23643475
// COP4610 - Prof. Borko Furht 
// CPU Scheduler Project
// 10/18/23

#include "ProcessEntry.hpp"

ProcessEntry::ProcessEntry(std::vector<int32_t> process_bursts, uint32_t process_ID) : process_ID(process_ID) {
	int32_t total_time = 0, cpu_time = 0;
	for (uint32_t i = 0; i < process_bursts.size(); ++i) {
		// Populate the burst list.
		bursts.push_back(process_bursts[i]);
		
		// Get the sum of all the bursts.
		total_time += process_bursts[i];

		// Gather the expected CPU time by only adding the even bursts.
		if ((i % 2) == 0)
			cpu_time += process_bursts[i];
	}

	// Create the result object with the total & CPU time.
	process_result = ProcessResult(total_time, cpu_time);
}

ProcessEntry::ProcessEntry(const ProcessData& process_data) : process_ID(process_data.process_ID) {
	int32_t total_time = 0, cpu_time = 0;
	for (uint32_t i = 0; i < process_data.bursts.size(); ++i) {
		// Populate the burst list.
		bursts.push_back(process_data.bursts[i]);

		// Get the sum of all the bursts.
		total_time += process_data.bursts[i];

		// Gather the expected CPU time by only adding the even bursts.
		if ((i % 2) == 0)
			cpu_time += process_data.bursts[i];
	}

	// Create the result object with the total & CPU time.
	process_result = ProcessResult(total_time, cpu_time);
}

int32_t ProcessEntry::process_current_burst(const int32_t time) {
	assert(!bursts.empty());

	if (!start)
		start = true;

	int32_t burst = bursts.front() - time;

	if (burst <= 0) {
		bursts.pop_front();

		if (!bursts.empty())
			if (current_state == CPU_BURST)
				current_state = ProcessState::IO_BURST;
			else
				current_state = ProcessState::CPU_BURST;
	}
	else
		bursts.front() = burst;

	return burst;
}