// Gabriel Garcia - Z23643475
// COP4610 - Prof. Borko Furht 
// CPU Scheduler Project
// 10/18/23

#pragma once

#include <cstdint>

class ProcessResult {
private:
	int32_t response_time = 0;
	int32_t waiting_time = 0;
	int32_t total_time = 0;
	int32_t cpu_time = 0;

public:
	ProcessResult() = default;

	ProcessResult(const int32_t total_time, const int32_t cpu_time)
		: total_time(total_time), cpu_time(cpu_time) {}

	inline int32_t get_response_time() const {
		return response_time;
	}

	inline int32_t get_waiting_time() const {
		return waiting_time;
	}

	inline int32_t get_turn_around_time() const {
		return (response_time + waiting_time + total_time);
	}

	inline int32_t get_cpu_time() const {
		return cpu_time;
	}

	inline void add_to_response_time(const int32_t time) {
		response_time += time;
	}

	inline void add_to_waiting_time(const int32_t time) {
		waiting_time += time;
	}
};