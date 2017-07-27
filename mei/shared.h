/* Definitions shared for the application
*/
#pragma once


// General
#define NT_DEVICE_NAME       L"\\Device\\MeiNt"
#define WIN32_DEVICE_NAME    L"\\DosDevices\\Mei"
#define DEVICE_NAME          L"\\\\.\\Mei"
#define DRIVER_NAME          L"Mei"
#define DRIVER_FILE          L"mei.sys"


// IOCTLs
#define IOCTL_MEI_AMTHI_INIT           (0x800 << 2) | (0x22 << 16)
#define IOCTL_MEI_AMTHI_SET_IPV4       (0x801 << 2) | (0x22 << 16)
#define IOCTL_MEI_AMTHI_UNCONFIGURE    (0x802 << 2) | (0x22 << 16)
