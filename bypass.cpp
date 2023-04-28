#include "utils.h"
#include "bypass.h"

#include <iostream>
#include <windows.h>

namespace bypass {
	uint32_t LDR_LOAD_DLL = 0xFCDC0;
	uint32_t CHEAT_ENGINE = 0xF9940;

	uint64_t base_address = 0;

	bool check() {
		uint64_t checksum = 0;
			
		checksum ^= utils::read<uint64_t>(base_address + LDR_LOAD_DLL);
		checksum ^= utils::read<uint64_t>(base_address + CHEAT_ENGINE);

		if (checksum < 0x75000000ull || checksum > 0x76000000ull) {
			printf("Unsupported SRB version -> 0x%I64X\n", checksum);
			return FALSE;
		}

		return TRUE;
	}

	void init() {

		while ((base_address = reinterpret_cast<uint64_t>(GetModuleHandleA("starrailbase.dll"))) == 0);

		utils::suspend_other_threads();

		if (check()) {
			utils::write<uint64_t>(base_address + LDR_LOAD_DLL, 0xCCCCC300000000B8ull); // bypass for dll injections (speedhack in cheat engine / debuggers)
			utils::write<uint8_t>(base_address + LDR_LOAD_DLL + 9, 0xC3); // bypass for dll injections (speedhack in cheat engine / debuggers)
			utils::write<uint8_t>(base_address + CHEAT_ENGINE, 0xC3); // bypass for cheat engine
		}
		else {
			printf("Failed to init bypass\n");

			Sleep(5000);
			TerminateProcess(GetCurrentProcess(), 0);
		}

		utils::resume_other_threads();
	}
}
