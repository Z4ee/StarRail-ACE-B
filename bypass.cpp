#include "utils.h"
#include "bypass.h"

#include <iostream>
#include <windows.h>

namespace bypass {

#define LDR_LOAD_DLL 0x9AB611
#define CHEAT_ENGINE 0xF09A55

	uint64_t base_address = 0;

	bool check() {
		uint64_t checksum = 0;
			
		checksum ^= utils::read<uint64_t>(base_address + LDR_LOAD_DLL);
		checksum ^= utils::read<uint64_t>(base_address + CHEAT_ENGINE);

		if (checksum != 0xE48D20006BE94C00ull) {
			printf("Unsupported SRB version -> 0x%I64X\n", checksum);

			return FALSE;
		}

		return TRUE;
	}

	void init() {

		while ((base_address = reinterpret_cast<uint64_t>(GetModuleHandleA("starrailbase.dll"))) == 0);

		if (check()) {
			utils::write<uint64_t>(base_address + LDR_LOAD_DLL, 0x40006800000000B8ull); // bypass for dll injections (speedhack in cheat engine / debuggers)
			utils::write<uint16_t>(base_address + CHEAT_ENGINE, 0x03EB); // bypass for cheat engine
		}
		else {
			printf("Failed to init bypass\n");

			Sleep(5000);
			TerminateProcess(GetCurrentProcess(), 0);
		}
	}
}