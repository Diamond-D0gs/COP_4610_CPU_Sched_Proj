// Gabriel Garcia - Z23643475
// COP4610 - Prof. Borko Furht 
// CPU Scheduler Project
// 10/18/23

#pragma once

#include "CPUScheduler.hpp"
#include "ProcessEntry.hpp"

#include <list>
#include <vector>
#include <functional>

class FirstComeFirstServe : public CPUScheduler {
protected:
	std::list<ProcessEntry*> ready_queue;

	ProcessEntry* get_next_process();

	void display_current_process_stats(ProcessEntry* process_entry);

	bool queues_empty();

	int32_t get_current_process_burst_time(ProcessEntry* process_entry);

	void process_current_burst(ProcessEntry* process_entry, const int32_t time);

	void update_response_and_wait_times(const int32_t time);

	void send_to_ready_queue(ProcessEntry* process_entry);

public:
	FirstComeFirstServe(const std::vector<ProcessEntry::ProcessData>& process_data);

	~FirstComeFirstServe() = default;
};