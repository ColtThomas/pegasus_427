##############################################################################
## Filename:          C:\Users\superman\Desktop\pegasus_427\lab8\lab6hw/drivers/dma_control_v1_00_a/data/dma_control_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Wed Dec 07 10:03:15 2016 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "dma_control" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}
