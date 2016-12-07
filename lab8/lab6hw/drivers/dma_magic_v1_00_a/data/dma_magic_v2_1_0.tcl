##############################################################################
## Filename:          C:\Users\superman\Desktop\pegasus_427\lab8\lab6hw/drivers/dma_magic_v1_00_a/data/dma_magic_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Tue Dec 06 17:42:58 2016 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "dma_magic" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}
