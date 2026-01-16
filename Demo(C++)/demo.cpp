#include "TSMaster.h"
#include <iostream>
using namespace std;

void _stdcall OnPreCAN(const pnative_int AObj, const PLIBCAN AData)
{
	AData->FData[0] = 0x00;  // Set the first data byte of the CAN message
}

void _stdcall OnCAN(const pnative_int AObj, const PLIBCAN AData) {
	if (AData->is_tx == true) return; // If the message is a transmission, return immediately
	int chn = AData->FIdxChn;
	cout << "Channel£º" << chn << endl;  // print CAN Channel
}

int main() {
	initialize_lib_tsmaster("TSMaster");  // Initialize the TSMaster library with the application name "TSMaster"
	tsapp_set_can_channel_count(2);  // Set the CAN channel count to 2
	tsapp_set_mapping_verbose("TSMaster", APP_CAN, CH1, "TC1034", TS_USB_DEVICE, 0, 0, CH1, true);  // Map the first CAN channel
	tsapp_set_mapping_verbose("TSMaster", APP_CAN, CH2, "TC1034", TS_USB_DEVICE, 0, 1, CH1, true);  // Map the second CAN channel
	tsapp_configure_baudrate_canfd(CH1, 500, 2000, lfdtISOCAN, lfdmNormal, true);  // Configure the first CAN channel with specified parameters
	tsapp_configure_baudrate_canfd(CH2, 500, 2000, lfdtISOCAN, lfdmNormal, true);  // Configure the second CAN channel with specified parameters
	tsapp_connect();  // Connect to the CAN interface
	TLIBCAN f0 = { CH1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };  // Create a TLIBCAN object with all fields set to 0xFF
	tsapp_add_cyclic_msg_can(&f0, 50);  // Add a cyclic CAN message on the first channel with a period of 1000 ms
	pnative_int h = 0;
	tsapp_register_pretx_event_can(h, &OnPreCAN);  // Register a pre-transmission event callback for CAN messages
	pnative_int h2 = 0;
	tsapp_register_event_can(h2, &OnCAN);  // Register another pre-transmission event callback for CAN messages
	while (1);
	finalize_lib_tsmaster();  // Finalize the TSMaster library
	return 0;
}