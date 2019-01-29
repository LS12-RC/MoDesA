/*
* --------------------------------------------------------------------------     
* Copyright (c) 2017 Hardware-Software-Co-Design, Friedrich-                     
* Alexander-Universitaet Erlangen-Nuernberg (FAU), Germany.                      
* All rights reserved.                                                           
*                                                                                
*                                                                                
* This code and any associated documentation is provided "as is"                 
*                                                                                
* IN NO EVENT SHALL HARDWARE-SOFTWARE-CO-DESIGN, FRIEDRICH-ALEXANDER-            
* UNIVERSITAET ERLANGEN-NUERNBERG (FAU) BE LIABLE TO ANY PARTY FOR DIRECT,       
* INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT            
* OF THE USE OF THIS CODE AND ITS DOCUMENTATION, EVEN IF HARDWARE-               
* SOFTWARE-CO-DESIGN, FRIEDRICH-ALEXANDER-UNIVERSITAET ERLANGEN-NUERNBERG        
* (FAU) HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. THE                  
* AFOREMENTIONED EXCLUSIONS OF LIABILITY DO NOT APPLY IN CASE OF INTENT          
* BY HARDWARE-SOFTWARE-CO-DESIGN, FRIEDRICH-ALEXANDER-UNIVERSITAET               
* ERLANGEN-NUERNBERG (FAU).                                                      
*                                                                                
* HARDWARE-SOFTWARE-CO-DESIGN, FRIEDRICH-ALEXANDER-UNIVERSITAET ERLANGEN-        
* NUERNBERG (FAU), SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT         
* NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          
* FOR A PARTICULAR PURPOSE.                                                      
*                                                                                
* THE CODE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND HARDWARE-              
* SOFTWARE-CO-DESIGN, FRIEDRICH-ALEXANDER-UNIVERSITAET ERLANGEN-                 
* NUERNBERG (FAU) HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT,             
* UPDATES, ENHANCEMENTS, OR MODIFICATIONS.                                       
* -------------------------------------------------------------------------      
*                                                                                
*  @author Streit Franz-Josef                                                    
*  @date   05 Mai 2018                                                      
*  @version 0.1                                                                  
*  @brief baremetal software driver for the write adapter ipb 
*                                                                                
*                                                                                
**/


#ifndef WRITE_ADAPTER_H
#define WRITE_ADAPTER_H

#ifdef __cplusplus                                                               
extern "C" {                                                                     
#endif

#define BAREMETAL

#ifdef BAREMETAL
  /****************** Include Files ********************/
#include "xil_types.h"
#include "xil_printf.h"
#include "xaxidma.h"
#else
  /****************** Include Files ********************/
#include <linux/types.h>

  typedef u8 __u8;
  typedef s8 __s8;
  typedef u16 __u16;
  typedef s16 __s16;
  typedef u32 __u32;
  typedef s32 __s32;

#define WRITE_ADAPTER_S00_AXI_SLV_REG0_OFFSET 0
#define WRITE_ADAPTER_S00_AXI_SLV_REG1_OFFSET 4
#define WRITE_ADAPTER_S00_AXI_SLV_REG2_OFFSET 8
#define WRITE_ADAPTER_S00_AXI_SLV_REG3_OFFSET 12

#define REG_WRITE(base,reg,val) iowrite32((u32)(val),((u32*)(base))+(reg))
#define REG_READ(base,reg) ioread32(((u32*)(base))+(reg))

#endif
  typedef enum {                                                                 
    pop_32_bit,                                                                 
    pop_64_bit                                                                  
  }pop_size;

  typedef enum {                                                                 
    reg_en_write_adapt = 0x0001,                                                   
    reg_nen_write_adapt = 0x0000,                                                  
  }reg_ctrl_write_adapt; 

  typedef struct {                                                               
    volatile u32 lsb_pop;                                                       
    volatile u32 msb_pop;                                                       
    volatile u16 use_reg;
    volatile u32 pkg_lng;
  }write_adapter; 

  /** @brief Initialization of write_adapter driver
   *
   *  @param set hw address of device.
   *  @return pointer to write_adapter struct.
   */
  write_adapter * init_write_adapter(u32 address);

  /** @brief Use write_adapter to read values from the system
   *
   *  @param control struct of write_adapter
   *  @param pointer to DMA instance
   *  @param generic void pointer to data
   *  @param distinguish between <= 32 and 64 bit data types 
   *  @return Return if succeeded.
   */
  u32 pop_data(write_adapter *control, XAxiDma * InstancePtr, void* data, pop_size size);

  /** @brief Use write_adapter to enable register or DMA interface                
   *                                                                             
   *  @param control struct of write_adapter                                      
   *  @param hw address of read DMA
   *  @param pointer to DMA instance
   *  @param set register or DMA interface
   *  @return Status if succeeded.
   */                                                                            
  u32 set_reg_write_adapt(write_adapter *control, XAxiDma * InstancePtr, u32 dma_id, reg_ctrl_write_adapt use_reg);

  /** @brief Use write_adapter to read interface settings                          
   *                                                                             
   * @param control struct of write_adapter                                       
   * @return register content                                                    
   */                                                                           
  reg_ctrl_write_adapt get_reg_write_adapt(write_adapter *control); 

  /** @brief Use write_adapter to set package length for DMA transfers                
   *                                                                             
   *  @param control struct of write_adapter                                      
   *  @param set number of packages                                       
   *  @return void.                                                              
   */                                                                            
  void set_pkg_lng(write_adapter *control, u32 lng);                         

  /** @brief Use write_adapter to get expected package length from DMA                
   *                                                                             
   *  @param control struct of write_adapter                                      
   *  @return number of packages.                                                             
   */                                                                            
  u32 get_pkg_lng(write_adapter *control);                         

#ifdef __cplusplus                                                               
}                                                                                
#endif 

#endif // WRITE_ADAPTER_H



