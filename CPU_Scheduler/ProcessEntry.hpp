// Gabriel Garcia - Z23643475
// COP4610 - Prof. Borko Furht 
// CPU Scheduler Project
// 10/18/23

#pragma once

#include "ProcessResult.hpp"

#include <list>
#include <vector>
#include <cstdint>
#include <cassert>

class ProcessEntry {
public:
	struct ProcessData {
		const std::vector<int32_t> bursts;
		const uint32_t process_ID;

		ProcessData(const std::vector<int32_t> bursts, const uint32_t process_ID)
			: bursts(bursts), process_ID(process_ID) {}
	};

	enum ProcessState {
		CPU_BURST,
		IO_BURST,
		DONE
	};

private:
	bool start = false;
	std::list<int32_t> bursts;
	ProcessResult process_result;
	ProcessState current_state = CPU_BURST;
	uint32_t process_ID = 0;

public:
	ProcessEntry() = default;

	ProcessEntry(std::vector<int32_t> process_bursts, uint32_t process_ID);

	ProcessEntry(const ProcessData& process_data);

	inline uint32_t get_process_ID() const {
		return process_ID;
	}

	inline int32_t get_current_burst_time() const {
		assert(!bursts.empty());
		return bursts.front();
	}

	inline ProcessState get_current_state() const {
		return current_state;
	}

	inline bool started() const {
		return start;
	}

	inline bool done() const {
		return bursts.empty();
	}

	inline const ProcessResult& get_process_result() const {
		return process_result;
	}

	inline ProcessResult& get_process_result() {
		return process_result;
	}

	int32_t process_current_burst(const int32_t time);
};