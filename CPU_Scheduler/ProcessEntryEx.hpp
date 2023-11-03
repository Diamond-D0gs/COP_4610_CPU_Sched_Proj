#include "ProcessEntry.hpp"

class ProcessEntryEx : public ProcessEntry {
public:
	enum ProcessQueue {
		ROUND_ROBIN_1,
		ROUND_ROBIN_2,
		FCFS
	};

private:
	ProcessQueue current_queue = ROUND_ROBIN_1;
	
public:
	ProcessEntryEx(const ProcessData& process_data)
		: ProcessEntry(process_data) {}


	inline ProcessQueue get_current_queue() const {
		return current_queue;
	}

	const char* get_current_queue_name() const;

	void downgrade_queue();
};