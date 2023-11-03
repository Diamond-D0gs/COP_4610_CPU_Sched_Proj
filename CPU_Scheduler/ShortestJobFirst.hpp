// Gabriel Garcia - Z23643475
// COP4610 - Prof. Borko Furht 
// CPU Scheduler Project
// 10/18/23

#pragma once

#include "FirstComeFirstServe.hpp"

class ShortestJobFirst : public FirstComeFirstServe {
public:
	ShortestJobFirst(const std::vector<ProcessEntry::ProcessData>& process_data)
		: FirstComeFirstServe(process_data) {}

	ProcessEntry* get_next_process();
};