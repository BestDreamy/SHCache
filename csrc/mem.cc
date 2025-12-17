#include "cpu/cpu.h"
#include "mem.h"

Memory::Memory() {
    mem.clear();
}

// Read from memory
bool Memory::read_memory(const paddr_t &addr, uint32_t& data) {
	auto it = mem.find(addr);
	if (it == mem.end()) {
		mem[addr] = 0x44332211; // Test Mem Data
		// mem[addr] = 0;
	}
	data = mem[addr];
	return true;
}

// Write to memory
void Memory::write_memory(const paddr_t &addr, const uint32_t &data) {
	mem[addr] = data;
}

void Memory::chi_read_memory_with_DMT(const reqflit_t &req) {
	printReqFlit(req);
	reqflit_t req2snf =  createReadNoSnp(req);
	datflit_t dat = createCompData_UC(req2snf);

	cpu[req2snf.StashNID_ReturnNID].cache.dat_channel.push(dat);

	devLog("RN[%d] dat channel push dat", req2snf.StashNID_ReturnNID);
}