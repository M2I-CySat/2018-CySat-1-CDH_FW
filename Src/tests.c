#include <stdint.h>

#include "tests.h"
#include "uart2.h"
#include "mem.h"

#define TEST_FLAG_1 0x0f0f0f0f
#define TEST_FLAG_2 0xf0f0f0f0
#define TEST_FLAG_3 0x55aa55aa

static uint32_t read_test_flag(void);
static void write_test_flag(uint32_t flag);

int Test_Mem(void)
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

static uint32_t read_test_flag(void)
{
	uint8_t buf[TEST_FLAG_LENGTH];
	uint32_t flag;

	MEM_Read(buf, TEST_FLAG_ADDRESS, TEST_FLAG_LENGTH);

	flag = buf[0] | (buf[1] << 8) | 
		(buf[2] << 16) | (buf[3] << 24);

	return flag;
}

static void write_test_flag(uint32_t flag)
{
	uint8_t buf[TEST_FLAG_LENGTH];
	buf[0] = flag & 0xff;
	buf[1] = (flag >> 8) & 0xff;
	buf[2] = (flag >> 16) & 0xff;
	buf[3] = (flag >> 24) & 0xff;

	MEM_Write(buf, TEST_FLAG_ADDRESS, TEST_FLAG_LENGTH);
}
