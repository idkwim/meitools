#include <ntddk.h>
#include "pci.h"



ULONG PciRead(ULONG address)
{
	WRITE_PORT_ULONG((PULONG) IO_PCI_ADDR, address);
	return READ_PORT_ULONG((PULONG) IO_PCI_DATA);
}



VOID PciWrite(ULONG address, ULONG value)
{
	WRITE_PORT_ULONG((PULONG) IO_PCI_ADDR, address);
	WRITE_PORT_ULONG((PULONG) IO_PCI_DATA, value);
}



ULONG PciGetDeviceBase(ULONG classCode)
{
	for (ULONG pciBase = PCI_START; pciBase < PCI_END; pciBase += PCI_CFG_SIZE)
		if (PciRead(pciBase + PCI_CFG_RID_CLASS_OFFSET) >> 8 == classCode)
			return pciBase;

	return 0;
}



BOOLEAN PciGetDeviceCfg(ULONG pciBase, PVOID out, ULONG outSize)
{
	BOOLEAN isPciBaseOk = (pciBase >= PCI_START) && (pciBase <= PCI_END - PCI_CFG_SIZE);
	BOOLEAN isOutOk = (out != NULL) && (outSize >= PCI_CFG_SIZE);

	if (isPciBaseOk && isOutOk)
		for (ULONG i = 0; i < PCI_CFG_SIZE; i += sizeof(ULONG))
			*((PULONG) out + i / sizeof(ULONG)) = PciRead(pciBase + i);
	else
		return FALSE;


	return TRUE;
}
