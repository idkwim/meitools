/* The PCI interface
*/
#pragma once


// I/O space
#define IO_PCI_ADDR    0xCF8
#define IO_PCI_DATA    0xCFC


// PCI space
#define PCI_START       0x80000000
#define PCI_END         0x81000000
#define PCI_CFG_SIZE    0x100


// PCI CFG registers
#define PCI_CFG_VID_DID_OFFSET       0x00
#define PCI_CFG_CMD_STS_OFFSET       0x04
#define PCI_CFG_RID_CLASS_OFFSET     0x08
#define PCI_CFG_CLS_PLT_HT_OFFSET    0x0C
#define PCI_CFG_BAR0_OFFSET          0x10
#define PCI_CFG_BAR1_OFFSET          0x14
#define PCI_CFG_BAR2_OFFSET          0x18
#define PCI_CFG_BAR3_OFFSET          0x1C
#define PCI_CFG_BAR4_OFFSET          0x20
#define PCI_CFG_BAR5_OFFSET          0x24
#define PCI_CFG_CISP_OFFSET          0x28
#define PCI_CFG_SVID_SID_OFFSET      0x2C
#define PCI_CFG_EXPROMBASE_OFFSET    0x30
#define PCI_CFG_CAPP_OFFSET          0x34
#define PCI_CFG_INTR_MLMG_OFFSET     0x3C


ULONG PciRead(ULONG address);
VOID PciWrite(ULONG address, ULONG value);
ULONG PciGetDeviceBase(ULONG classCode);
BOOLEAN PciGetDeviceCfg(ULONG pciBase, PVOID out, ULONG outSize);
