#include "ProcessEntryEx.hpp"

void ProcessEntryEx::downgrade_queue() {
	if (current_queue != FCFS)
		current_queue = static_cast<ProcessQueue>(static_cast<uint32_t>(current_queue) + 1);
}

const char* ProcessEntryEx::get_current_queue_name() const {
	switch (current_queue) {
	case ProcessEntryEx::ROUND_ROBIN_1:
		return "Round Robin 1";
	case ProcessEntryEx::ROUND_ROBIN_2:
		return "Round Robin 2";
	case ProcessEntryEx::FCFS:
		return "FCFS";
	default:
		return "Error!";
	}
}