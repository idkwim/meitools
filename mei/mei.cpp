#include "mei.h"
#include "pci.h"



PMEI_CONTEXT MeiInit()
{
	PMEI_CONTEXT MeiContext = (PMEI_CONTEXT) MmAllocateNonCachedMemory(sizeof(MEI_CONTEXT));
	if (MeiContext == NULL)
		return NULL;

	RtlZeroMemory(MeiContext, sizeof(MEI_CONTEXT));


	MeiContext->PciBase = PciGetDeviceBase(MEI_PCI_CLASSCODE);
	if (MeiContext->PciBase == 0)
	{
		MeiClose(MeiContext);
		return NULL;
	}


	MeiContext->MmioBase = PciRead(MeiContext->PciBase + MEI_PCI_CFG_HECIMBAR_OFFSET) & 0xFFFFFFF0;
	if (MeiContext->MmioBase == 0)
	{
		MeiClose(MeiContext);
		return NULL;
	}


	return MeiContext;
}



VOID MeiClose(PMEI_CONTEXT MeiContext)
{
	if (MeiContext != NULL)
		MmFreeNonCachedMemory(MeiContext, sizeof(MEI_CONTEXT));
}
