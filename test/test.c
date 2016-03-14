/***************************************************
 X-Keys Test Program

 The code may be used by anyone for any purpose,
 and can serve as a starting point for developing
 applications using the X-Keys libary.
 
 Alan Ott
 Signal 11 Software
***************************************************/

#include "PieHid32.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


void print_buf(char *data, int data_length)
{
	int data_index;
	for (data_index = 0; data_index < data_length; data_index++) {
		//printf("%08hh", data[data_index]);
		printf("%02hhx ", data[data_index]);
		if ((data_index+1) % 8 == 0)
			printf("  ");
		if ((data_index+1) % 16 == 0)
			printf("\n");
	}
	printf("\n\n");
}

int main(void)
{
	TEnumHIDInfo info[128];
	long number_of_hids;
	long hid_handle = -1;
	
	unsigned hids = EnumeratePIE(PI_VID, info, &number_of_hids);
	
	if (number_of_hids > 0) {
		TEnumHIDInfo *hid = &info[0];
		printf("Found XKeys Device:\n");
		printf("\tPID: %04x\n", hid->PID);
		printf("\tUsage Page: %04x\n", hid->UP);
		printf("\tUsage:      %04x\n", hid->Usage);
		printf("\tVersion: %d\n\n", hid->Version);
		
		hid_handle = hid->Handle;
		unsigned int hids = SetupInterfaceEx(hid_handle);
		if (hids != 0) {
			printf("Unabe to open device. err: %d\n", hids);
		}
	}
	
	if (hid_handle < 0) {
		printf("Unable to open device\n");
		exit(1);
	}

	char data[80];
	while (1) {
		
		unsigned int hid_data_chunk = 0;
		
		hid_data_chunk = ReadLast(hid_handle, data);
		if (hid_data_chunk == 0) {
			printf("LAST: \n");
			print_buf(data, 33);
			printf("ENDLAST\n\n");
		}
		
		hid_data_chunk = 0;
		
		while (hid_data_chunk == 0) {
			hid_data_chunk = BlockingReadData(hid_handle, data, 20);
			if (hid_data_chunk == 0) {
				print_buf(data, 33);
			}
			else if (hid_data_chunk == PIE_HID_READ_INSUFFICIENT_DATA) {
				printf(".");
				fflush(stdout);
			}	
			else {
				printf("Error Reading\n");
			}
		}
		
		printf("Sleeping\n");
		#if 1
		if (hid_data_chunk != 0) {
			//usleep(10*1000); //Sleep 10 milliseconds.
			sleep(1); //seconds
		}
		#endif
		
		//ClearBuffer(hid_handle);
	}
	
	return 0;
}