#pragma once
#include <Windows.h>


DWORD LoadDriver(SC_HANDLE* scManager, SC_HANDLE* scService, PHANDLE device, PWCHAR driverPath, PWCHAR driverName, PWCHAR deviceName);
DWORD UnloadDriver(SC_HANDLE* scManager, SC_HANDLE* scService, PHANDLE device);
