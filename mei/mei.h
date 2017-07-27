/* The MEI interface
*/
#pragma once

#include "main.h"


// Intel MEI identification value
#define MEI_PCI_CLASSCODE    0x078000


// Intel MEI PCI CFG specific registers
#define MEI_PCI_CFG_HECIMBAR_OFFSET    0x10
#define MEI_PCI_CFG_HFS_OFFSET         0x40
#define MEI_PCI_CFG_MEUMA_OFFSET       0x44
#define MEI_PCI_CFG_GMES_OFFSET        0x48
#define MEI_PCI_CFG_HGS_OFFSET         0x4C
#define MEI_PCI_CFG_PID_PC_OFFSET      0x50
#define MEI_PCI_CFG_PMCS_OFFSET        0x54
#define MEI_PCI_CFG_MID_MC_OFFSET      0x8C
#define MEI_PCI_CFG_MA_OFFSET          0x90
#define MEI_PCI_CFG_MUA_OFFSET         0x94
#define MEI_PCI_CFG_MD_OFFSET          0x98
#define MEI_PCI_CFG_HIDM_OFFSET        0xA0
#define MEI_PCI_CFG_HERS_OFFSET        0xBC
#define MEI_PCI_CFG_HER1_OFFSET        0xC0
#define MEI_PCI_CFG_HER2_OFFSET        0xC4
#define MEI_PCI_CFG_HER3_OFFSET        0xC8
#define MEI_PCI_CFG_HER4_OFFSET        0xCC
#define MEI_PCI_CFG_HER5_OFFSET        0xD0
#define MEI_PCI_CFG_HER6_OFFSET        0xD4
#define MEI_PCI_CFG_HER7_OFFSET        0xD8
#define MEI_PCI_CFG_HER8_OFFSET        0xDC


// Intel MEI MMIO registers
#define MEI_MMIO_H_CB_WW_OFFSET      0x00
#define MEI_MMIO_H_CSR_OFFSET        0x04
#define MEI_MMIO_ME_CB_RW_OFFSET     0x08
#define MEI_MMIO_ME_CSR_HA_OFFSET    0x0C


typedef struct MEI_CONTEXT
{
	ULONG PciBase;
	ULONG MmioBase;
} *PMEI_CONTEXT;



typedef struct MEI_CSR
{
	ULONG InterruptEnable : 1;
	ULONG InterruptStatus : 1;
	ULONG InterruptGenerate : 1;
	ULONG Ready : 1;
	ULONG Reset : 1;
	ULONG Reserved : 3;
	ULONG BufferReadPtr : 8;
	ULONG BufferWritePtr : 8;
	ULONG BufferDepth : 8;
};


#define MEI_CLIENT_AMTHI     0x02
#define MEI_CLIENT_WDT       0x04
#define MEI_CLIENT_POLICY    0x05
#define MEI_CLIENT_MKHI      0x07
#define MEI_CLIENT_ICC       0x08

#define MEI_HOST    0

typedef struct MEI_MSG_HEADER
{
	ULONG ClientAddress : 8;
	ULONG HostAddress : 8;
	ULONG Length : 9;
	ULONG Reserved : 6;
	ULONG IsComplete : 1;
};


#define MKHI_GROUP_ID_CBM       0x00
#define MKHI_GROUP_ID_PM        0x01
#define MKHI_GROUP_ID_PWD       0x02
#define MKHI_GROUP_ID_FWCAPS    0x03

typedef struct MEI_MKHI_MESSAGE_HEADER
{
	ULONG GroupId : 8;
	ULONG Command : 7;
	ULONG IsResponse : 1;
	ULONG Reserved : 8;
	ULONG Result : 8;
};

#define AMTHI_GROUP_ID_CFG       0x12

#define AMTHI_CMD_SET_IPV4       0x0C
#define AMTHI_CMD_INIT           0x05
#define AMTHI_CMD_UNCONFIGURE    0x06

typedef struct MEI_AMTHI_MESSAGE_HEADER
{
	ULONG GroupId : 8;
	ULONG Command : 7;
	ULONG IsResponse : 1;
	ULONG Reserved : 8;
	ULONG Result : 8;
};


PMEI_CONTEXT MeiInit();
VOID MeiClose(PMEI_CONTEXT MeiContext);
