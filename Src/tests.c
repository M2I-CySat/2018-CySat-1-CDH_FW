#include <stdint.h>
#include <string.h>

#include "tests.h"
#include "uart2.h"
#include "mem.h"
#include "utilities.h"
#include "radio.h"
#include "eps.h"


#define TEST_FLAG_1 0x0f0f0f0f
#define TEST_FLAG_2 0xf0f0f0f0
#define TEST_FLAG_3 0x55aa55aa

#define HELLO "Hello, world!\n"

static uint32_t read_test_flag(void);
static void write_test_flag(uint32_t flag);

int Test_EPS()
{
	Debug_Printf("Sending blocking update request to EPS task");
	if (EPS_UpdateTelemetry(EPS_BLOCK)) {
		Debug_Printf("!!! TEST FAILED !!! Error from EPS_UpdateTelemetry");
	}
	Debug_Printf("UpdateTelemetry returned");

	Debug_Printf("Reading telemetry values from EPS");
	struct eps_telemetry telemetry;
	EPS_GetTelemetry(&telemetry);
	Debug_Printf("Battery voltage should be nonzero: %2.3f",
			telemetry.battery_voltage);
	
	if (telemetry.battery_voltage == 0.0) {
		Debug_Printf("!!! TEST FAILED !!! EPS did not actually update!");
	}

	return 0;
}

int Test_RadioTransmit()
{
	Radio_Transmit((uint8_t *) HELLO, strlen(HELLO));

	return 0;
}

int Test_Mem()
{
	uint32_t test_flag;
	int test_failure = 0;

	Debug_Printf("Memory test running");

	test_flag = read_test_flag();
	Debug_Printf("Initial contents of test flag: 0x%x");

	Debug_Printf("Writing flag 1 (0x%x)...", TEST_FLAG_1);
	write_test_flag(TEST_FLAG_1);
	Debug_Printf("Read back test flag 1: (0x%x)", test_flag);
	if (test_flag != TEST_FLAG_1) {
		Debug_Printf("!!! TEST FAILED !!!");
		test_failure++;
	}

	Debug_Printf("Writing flag 2 (0x%x)...", TEST_FLAG_2);
	write_test_flag(TEST_FLAG_2);
	Debug_Printf("Read back test flag 2: (0x%x)", test_flag);
	if (test_flag != TEST_FLAG_2) {
		Debug_Printf("!!! TEST FAILED !!!");
		test_failure++;
	}

	Debug_Printf("Writing flag 3 (0x%x)...", TEST_FLAG_3);
	write_test_flag(TEST_FLAG_3);
	Debug_Printf("Read back test flag 3: (0x%x)", test_flag);
	if (test_flag != TEST_FLAG_3) {
		Debug_Printf("!!! TEST FAILED !!!");
		test_failure++;
	}

	return test_failure;
}

static uint32_t read_test_flag()
{
	uint8_t buf[TEST_FLAG_LENGTH];
	uint32_t flag;

	MEM_Read(buf, TEST_FLAG_ADDRESS, TEST_FLAG_LENGTH);

	flag = Unpack32(buf);

	return flag;
}

static void write_test_flag(uint32_t flag)
{
	uint8_t buf[TEST_FLAG_LENGTH];
	Pack32(buf, flag);

	MEM_Write(buf, TEST_FLAG_ADDRESS, TEST_FLAG_LENGTH);
}
