#include "main.h"
#include "driver.h"
#include <stdio.h>



void print_welcome()
{
	puts("AMTactivator v1.0");
}



void print_usage()
{
	puts("\r\nUSAGE: AMTactivator [option]");
	puts("options:");
	puts("\tenable \tEnable Intel AMT on this system");
	puts("\tdisable \tDisable Intel AMT on this system");
}


#define MAX_LENGTH 17

typedef struct REQUEST_AMTHI_INIT
{
	UCHAR password[MAX_LENGTH];
	ULONG status;
} *PREQUEST_AMTHI_INIT;

typedef struct REQUEST_AMTHI_SET_IPV4
{
	WCHAR hostname[MAX_LENGTH];
	WCHAR ip_address[MAX_LENGTH];
	WCHAR mask[MAX_LENGTH];
	ULONG status;
} *PREQUEST_AMTHI_SET_IPV4;

void AmtEnable(HANDLE MeiDevice)
{
	REQUEST_AMTHI_INIT AmtInit = { 0 };
	REQUEST_AMTHI_SET_IPV4 AmtSetIpv4 = { 0 };

	puts("Applying network configuration :");
	printf("Host name:\t\t%s\r\n", AMT_CONFIG_HOSTNAME);
	printf("IPv4 address:\t\t%s\r\n", AMT_CONFIG_IPV4);
	printf("Subnet mask address:\t\t%s\r\n", AMT_CONFIG_MASK);

	memcpy(&AmtSetIpv4.hostname, AMT_CONFIG_HOSTNAME, sizeof(AMT_CONFIG_HOSTNAME));
	memcpy(&AmtSetIpv4.ip_address, AMT_CONFIG_IPV4, sizeof(AMT_CONFIG_IPV4));
	memcpy(&AmtSetIpv4.mask, AMT_CONFIG_MASK, sizeof(AMT_CONFIG_MASK));
	memcpy(&AmtInit.password, AMT_CONFIG_PASSWORD, sizeof(AMT_CONFIG_PASSWORD));
	

	DWORD ret = 0;

	if (!DeviceIoControl(MeiDevice, IOCTL_MEI_AMTHI_INIT, &AmtInit, sizeof(AmtInit), &AmtInit, sizeof(AmtInit), &ret, NULL))
		printf("Request to the driver failed: %Xh\r\n", GetLastError());


	ret = 0;

	if (!DeviceIoControl(MeiDevice, IOCTL_MEI_AMTHI_SET_IPV4, &AmtSetIpv4, sizeof(AmtSetIpv4), &AmtSetIpv4, sizeof(AmtSetIpv4), &ret, NULL))
		printf("Request to the driver failed: %Xh\r\n", GetLastError());


	if ((AmtInit.status == 0) && (AmtSetIpv4.status == 0))
	{
		puts("Configuration applied.");
		puts("\r\nIntel AMT activated.");
	}
	else
		puts("\r\nThe activation is not supported on you system.");
}


typedef struct REQUEST_AMTHI_UNCONFIGURE
{
	ULONG status;
} *PREQUEST_AMTHI_UNCONFIGURE;

void AmtDisable(HANDLE MeiDevice)
{
	REQUEST_AMTHI_UNCONFIGURE AmtUnconfigure = { 0 };

	DWORD ret = 0;

	if (!DeviceIoControl(MeiDevice, IOCTL_MEI_AMTHI_UNCONFIGURE, &AmtUnconfigure, sizeof(AmtUnconfigure), &AmtUnconfigure, sizeof(AmtUnconfigure), &ret, NULL))
		printf("Request to the driver failed: %Xh\r\n", GetLastError());


	if (AmtUnconfigure.status == 0)
		puts("\r\nIntel AMT deactivated. Reboot you system.");
	else
		puts("\r\nThe deactivation went wrong. Try reflashing Intel ME firmware.");
}



int main(int argc, char *argv[])
{
	int exitCode = EXIT_SUCCESS;

	print_welcome();


	// Parse arguments
	if (argc < 2)
	{
		print_usage();
		return EXIT_FAILURE;
	}


	bool enableFlag = false;
	bool disableFlag = false;

	if (!memcmp(argv[1], ARGUMENT_ENABLE, sizeof(ARGUMENT_ENABLE)))
		enableFlag = true;

	if (!memcmp(argv[1], ARGUMENT_DISABLE, sizeof(ARGUMENT_DISABLE)))
		disableFlag = true;

	if (!enableFlag & !disableFlag)
	{
		print_usage();
		return EXIT_FAILURE;
	}


	// Get the driver file path
	DWORD  pathLen = GetFullPathNameW(DRIVER_FILE, 0, NULL, NULL);
	PWCHAR driverPath = new WCHAR[pathLen];

	GetFullPathNameW(DRIVER_FILE, pathLen, driverPath, NULL);


	// Load the driver
	puts("Loading the HECI driver...");

	SC_HANDLE scManager, scService;
	HANDLE    device;

	DWORD errorCode = LoadDriver(&scManager, &scService, &device, driverPath, DRIVER_NAME, DEVICE_NAME);
	if (errorCode)
	{
		printf("Driver load failed: %Xh\r\n", errorCode);
		exitCode = EXIT_FAILURE;
	}
	else
	{
		if (enableFlag)
			AmtEnable(device);
		if (disableFlag)
			AmtDisable(device);
	}

	delete[] driverPath;


	// Unload the driver
	UnloadDriver(&scManager, &scService, &device);


	return exitCode;
}
