#include "driver.h"



DWORD LoadDriver(SC_HANDLE* scManager, SC_HANDLE* scService, PHANDLE device, PWCHAR driverPath, PWCHAR driverName, PWCHAR deviceName)
{
	*scManager = NULL;
	*scService = NULL;
	*device = NULL;


	// Gain access to the Service Control Manager
	*scManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (*scManager == NULL)
		return GetLastError();


	// Create the service as a kernel mode driver (if exists, open it, unload, then try to load again)
	*scService = CreateServiceW(*scManager, driverName, driverName, SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, driverPath, NULL, NULL, NULL, NULL, NULL);
	if (*scService == NULL)
	{
		if (GetLastError() == ERROR_SERVICE_EXISTS)
		{
			*scService = OpenServiceW(*scManager, driverName, SERVICE_ALL_ACCESS);
			UnloadDriver(scManager, scService, device);

			return LoadDriver(scManager, scService, device, driverPath, driverName, deviceName);
		}
		else
			return GetLastError();
	}


	// Start the service (driver)
	if (!StartServiceW(*scService, 0, NULL))
		return GetLastError();


	// Create the driver's device
	*device = CreateFileW(deviceName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (*device == INVALID_HANDLE_VALUE)
		return GetLastError();


	return 0;
}



DWORD UnloadDriver(SC_HANDLE* scManager, SC_HANDLE* scService, PHANDLE device)
{
	// Close the driver's device handle
	if ((*device != NULL) && (*device != INVALID_HANDLE_VALUE))
		CloseHandle(*device);


	// Stop the service
	SERVICE_STATUS ServiceStatus;

	ZeroMemory(&ServiceStatus, sizeof(SERVICE_STATUS));

	ControlService(*scService, SERVICE_CONTROL_STOP, &ServiceStatus);


	// Delete the service
	DWORD errorCode = 0;

	if (!DeleteService(*scService))
		errorCode = GetLastError();


	// Close service and Service Control Manager handles
	if (*scService != NULL)
		CloseServiceHandle(*scService);
	if (*scManager != NULL)
		CloseServiceHandle(*scManager);


	return errorCode;
}
