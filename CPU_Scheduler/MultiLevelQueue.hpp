// Gabriel Garcia - Z23643475
// COP4610 - Prof. Borko Furht 
// CPU Scheduler Project
// 10/18/23

#pragma once

#include "CPUScheduler.hpp"
#include "ProcessEntryEx.hpp"

#include <list>
#include <vector>
#include <functional>

class MultiLevelQueue : public CPUScheduler {
private:
	static const int32_t ROUND_ROBIN_1_QUANTUM = 5;
	static const int32_t ROUND_ROBIN_2_QUANTUM = 10;

	std::list<ProcessEntry*> round_robin_1;
	std::list<ProcessEntry*> round_robin_2;
	std::list<ProcessEntry*> fcfs;

	void send_to_ready_queue(ProcessEntry* process_entry);

public:
	MultiLevelQueue(const std::vector<ProcessEntry::ProcessData>& process_data);

	bool queues_empty();

	ProcessEntry* get_next_process();

	void display_current_process_stats(ProcessEntry* process_entry);

	int32_t get_current_process_burst_time(ProcessEntry* process_entry);

	void process_current_burst(ProcessEntry* process_entry, const int32_t time);

	void update_response_and_wait_times(const int32_t time);
};