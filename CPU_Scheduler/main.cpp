// Gabriel Garcia - Z23643475
// COP4610 - Prof. Borko Furht 
// CPU Scheduler Project
// 10/18/23

#include "CPUScheduler.hpp"
#include "FirstComeFirstServe.hpp"
#include "ShortestJobFirst.hpp"
#include "MultiLevelQueue.hpp"

#include <vector>
#include <cstdint>
#include <iostream>

int main(void) {
	// Create the data set, process IDs and arrays of ints representing bursts.
	uint32_t process_ID = 1;
	const std::vector<ProcessEntry::ProcessData> process_entries {
		{{5, 27, 3, 31, 5, 43, 4, 18, 6, 22, 4, 26, 3, 24, 4}, process_ID++},
		{{4, 48, 5, 44, 7, 42, 12, 37, 9, 76, 4, 41, 9, 31, 7, 43, 8}, process_ID++},
		{{8, 33, 12, 41, 18, 65, 14, 21, 4, 61, 15, 18, 14, 26, 5, 31, 6}, process_ID++},
		{{3, 35, 4, 41, 5, 45, 3, 51, 4, 61, 5, 54, 6, 82, 5, 77, 3}, process_ID++},
		{{16, 24, 17, 21, 5, 36, 16, 26, 7, 31, 13, 28, 11, 21, 6, 13, 3, 11, 4}, process_ID++},
		{{11, 22, 4, 8, 5, 10, 6, 12, 7, 14, 9, 18, 12, 24, 15, 30, 8}, process_ID++},
		{{14, 46, 17, 41, 11, 42, 15, 21, 4, 32, 7, 19, 16, 33, 10}, process_ID++},
		{{4, 14, 5, 33, 6, 51, 14, 73, 16, 87, 6}, process_ID++}
	};

	CPUScheduler* first_come_first_serve = new FirstComeFirstServe(process_entries);

	std::cout << "First Come First Serve:\n";
	first_come_first_serve->run();

	delete first_come_first_serve;

	CPUScheduler* shortest_job_first = new ShortestJobFirst(process_entries);

	std::cout << "\nShortest Job First:\n";
	shortest_job_first->run();

	delete shortest_job_first;

	CPUScheduler* multi_level_queue = new MultiLevelQueue(process_entries);

	std::cout << "\nMulti Level Queue:\n";
	multi_level_queue->run();

	delete multi_level_queue;
	
	return 0;
}