#include "main.h"
#include "shared.h" 
#include "mei.h"


// Global storage for the MEI context
PMEI_CONTEXT g_MeiContext;


NTSTATUS CtlCreate(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp)
{
	UNREFERENCED_PARAMETER(pDeviceObject);

	// Complete the request
	pIrp->IoStatus.Information = 0;
	pIrp->IoStatus.Status = STATUS_SUCCESS;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);


	return STATUS_SUCCESS;
}



NTSTATUS CtlClose(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp)
{
	UNREFERENCED_PARAMETER(pDeviceObject);

	// Complete the request
	pIrp->IoStatus.Information = 0;
	pIrp->IoStatus.Status = STATUS_SUCCESS;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);


	return STATUS_SUCCESS;
}



VOID UnloadOperation(IN PDRIVER_OBJECT pDriverObject)
{
	// Delete the symbolic link to the driver's device
	UNICODE_STRING Win32NameString;

	RtlInitUnicodeString(&Win32NameString, WIN32_DEVICE_NAME);
	IoDeleteSymbolicLink(&Win32NameString);


	// Delete the device
	PDEVICE_OBJECT pDeviceObject = pDriverObject->DeviceObject;
	IoDeleteDevice(pDeviceObject);


	// Delete the MEI context
	MeiClose(g_MeiContext);
}


// AMT request arguments
#define MAX_STR_LENGTH 17

typedef struct REQUEST_AMTHI_INIT
{
	UCHAR password[MAX_STR_LENGTH];
	ULONG status;
} *PREQUEST_AMTHI_INIT;

typedef struct REQUEST_AMTHI_SET_IPV4
{
	WCHAR hostname[MAX_STR_LENGTH];
	WCHAR ip_address[MAX_STR_LENGTH];
	WCHAR mask[MAX_STR_LENGTH];
	ULONG status;
} *PREQUEST_AMTHI_SET_IPV4;

typedef struct REQUEST_AMTHI_UNCONFIGURE
{
	ULONG status;
} *PREQUEST_AMTHI_UNCONFIGURE;


NTSTATUS CtlDispatch(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp)
{
	UNREFERENCED_PARAMETER(pDeviceObject);

	// Retrieve the stack locaton of current request
	PIO_STACK_LOCATION pIrpStack = IoGetCurrentIrpStackLocation(pIrp);

	// Process the request
	switch (pIrpStack->Parameters.DeviceIoControl.IoControlCode)
	{

	case IOCTL_MEI_AMTHI_SET_IPV4:
	{
		if (g_MeiContext != NULL)
		{
			// Retrieve the location of the system buffer
			PREQUEST_AMTHI_SET_IPV4 AmtSetIpv4 = (PREQUEST_AMTHI_SET_IPV4) pIrp->AssociatedIrp.SystemBuffer;

			UNREFERENCED_PARAMETER(AmtSetIpv4);

			// Specify the bytes count to be returned
			pIrp->IoStatus.Information = sizeof(REQUEST_AMTHI_SET_IPV4);
		}

		break; // IOCTL_MEI_AMTHI_SET_IPV4
	}

	case IOCTL_MEI_AMTHI_INIT:
	{
		if (g_MeiContext != NULL)
		{
			// Retrieve the location of the system buffer
			PREQUEST_AMTHI_INIT AmtInit = (PREQUEST_AMTHI_INIT) pIrp->AssociatedIrp.SystemBuffer;

			UNREFERENCED_PARAMETER(AmtInit);

			// Specify the bytes count to be returned
			pIrp->IoStatus.Information = sizeof(REQUEST_AMTHI_INIT);
		}

		break; // IOCTL_MEI_AMTHI_INIT
	}

	case IOCTL_MEI_AMTHI_UNCONFIGURE:
	{
		if (g_MeiContext != NULL)
		{
			// Retrieve the location of the system buffer
			PREQUEST_AMTHI_UNCONFIGURE AmtUnconfigure = (PREQUEST_AMTHI_UNCONFIGURE)pIrp->AssociatedIrp.SystemBuffer;

			UNREFERENCED_PARAMETER(AmtUnconfigure);

			// Specify the bytes count to be returned
			pIrp->IoStatus.Information = sizeof(REQUEST_AMTHI_UNCONFIGURE);
		}

		break; // IOCTL_MEI_AMTHI_UNCONFIGURE
	}

	default:
		break;

	}


	// Complete the request
	pIrp->IoStatus.Status = STATUS_SUCCESS;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);


	return STATUS_SUCCESS;
}



extern "C" DRIVER_INITIALIZE DriverEntry;
extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath)
{
	UNREFERENCED_PARAMETER(pRegistryPath);


	// Create a symbolic link to the device
	UNICODE_STRING NtDeviceName;
	UNICODE_STRING Win32DeviceName;

	RtlInitUnicodeString(&NtDeviceName, NT_DEVICE_NAME);
	RtlInitUnicodeString(&Win32DeviceName, WIN32_DEVICE_NAME);
	IoCreateSymbolicLink(&Win32DeviceName, &NtDeviceName);


	// Create the device
	PDEVICE_OBJECT pDeviceObject;

	IoCreateDevice(pDriverObject, 0, &NtDeviceName, FILE_DEVICE_UNKNOWN, 0, FALSE, &pDeviceObject);


	// Specify the main driver's control routines
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = CtlCreate;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = CtlClose;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = CtlDispatch;

	pDriverObject->DriverUnload = UnloadOperation;


	// Initialize the MEI context
	g_MeiContext = NULL;
	g_MeiContext = MeiInit();


	return STATUS_SUCCESS;
}
