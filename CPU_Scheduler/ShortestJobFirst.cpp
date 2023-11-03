// Gabriel Garcia - Z23643475
// COP4610 - Prof. Borko Furht 
// CPU Scheduler Project
// 10/18/23

#include "ShortestJobFirst.hpp"

ProcessEntry* ShortestJobFirst::get_next_process() {
	// Sort the ready queue so the first process is the smallest.
	// Otherwise logic is identical to the first come first serve queue.
	ready_queue.sort(shortest_process_sort);
	return FirstComeFirstServe::get_next_process();
}