/* File: startup_ARMCM4.S
 * Purpose: startup file for Cortex-M4 devices. Should use with
 *   GCC for ARM Embedded Processors
 * Version: V2.0
 * Date: 16 August 2013
 *
 * Copyright (c) 2011 - 2013 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/
	.syntax	unified
	.arch	armv7e-m

	.section .stack
	.align	3
#ifdef __STACK_SIZE
	.equ	Stack_Size, __STACK_SIZE
#else
	.equ	Stack_Size, 0xc00
#endif
	.globl	__StackTop
	.globl	__StackLimit
__StackLimit:
	.space	Stack_Size
	.size	__StackLimit, . - __StackLimit
__StackTop:
	.size	__StackTop, . - __StackTop

	.section .heap
	.align	3
#ifdef __HEAP_SIZE
	.equ	Heap_Size, __HEAP_SIZE
#else
	.equ	Heap_Size, 0
#endif
	.globl	__HeapBase
	.globl	__HeapLimit
__HeapBase:
	.if	Heap_Size
	.space	Heap_Size
	.endif
	.size	__HeapBase, . - __HeapBase
__HeapLimit:
	.size	__HeapLimit, . - __HeapLimit

	.section .isr_vector
	.align	2
	.globl	__isr_vector
__isr_vector:
	.long	__StackTop            /* Top of Stack */
	.long	Reset_Handler         /* Reset Handler */
	.long	NMI_Handler           /* NMI Handler */
	.long	HardFault_Handler     /* Hard Fault Handler */
	.long	MemManage_Handler     /* MPU Fault Handler */
	.long	BusFault_Handler      /* Bus Fault Handler */
	.long	UsageFault_Handler    /* Usage Fault Handler */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	SVC_Handler           /* SVCall Handler */
	.long	DebugMon_Handler      /* Debug Monitor Handler */
	.long	0                     /* Reserved */
	.long	PendSV_Handler        /* PendSV Handler */
	.long	SysTick_Handler       /* SysTick Handler */

	/* External interrupts */
	.word     WWDG_IRQHandler                   /* Window WatchDog              */                                        
	.word     PVD_IRQHandler                    /* PVD through EXTI Line detection */                        
	.word     TAMP_STAMP_IRQHandler             /* Tamper and TimeStamps through the EXTI line */            
	.word     RTC_WKUP_IRQHandler               /* RTC Wakeup through the EXTI line */                      
	.word     FLASH_IRQHandler                  /* FLASH                        */                                          
	.word     RCC_IRQHandler                    /* RCC                          */                                            
	.word     EXTI0_IRQHandler                  /* EXTI Line0                   */                        
	.word     EXTI1_IRQHandler                  /* EXTI Line1                   */                          
	.word     EXTI2_IRQHandler                  /* EXTI Line2                   */                          
	.word     EXTI3_IRQHandler                  /* EXTI Line3                   */                          
	.word     EXTI4_IRQHandler                  /* EXTI Line4                   */                          
	.word     DMA1_Stream0_IRQHandler           /* DMA1 Stream 0                */                  
	.word     DMA1_Stream1_IRQHandler           /* DMA1 Stream 1                */                   
	.word     DMA1_Stream2_IRQHandler           /* DMA1 Stream 2                */                   
	.word     DMA1_Stream3_IRQHandler           /* DMA1 Stream 3                */                   
	.word     DMA1_Stream4_IRQHandler           /* DMA1 Stream 4                */                   
	.word     DMA1_Stream5_IRQHandler           /* DMA1 Stream 5                */                   
	.word     DMA1_Stream6_IRQHandler           /* DMA1 Stream 6                */                   
	.word     ADC_IRQHandler                    /* ADC1, ADC2 and ADC3s         */                   
	.word     0               				  /* Reserved                      */                         
	.word     0              					  /* Reserved                     */                          
	.word     0                                 /* Reserved                     */                          
	.word     0                                 /* Reserved                     */                          
	.word     EXTI9_5_IRQHandler                /* External Line[9:5]s          */                          
	.word     TIM1_BRK_TIM9_IRQHandler          /* TIM1 Break and TIM9          */         
	.word     TIM1_UP_TIM10_IRQHandler          /* TIM1 Update and TIM10        */         
	.word     TIM1_TRG_COM_TIM11_IRQHandler     /* TIM1 Trigger and Commutation and TIM11 */
	.word     TIM1_CC_IRQHandler                /* TIM1 Capture Compare         */                          
	.word     TIM2_IRQHandler                   /* TIM2                         */                   
	.word     TIM3_IRQHandler                   /* TIM3                         */                   
	.word     TIM4_IRQHandler                   /* TIM4                         */                   
	.word     I2C1_EV_IRQHandler                /* I2C1 Event                   */                          
	.word     I2C1_ER_IRQHandler                /* I2C1 Error                   */                          
	.word     I2C2_EV_IRQHandler                /* I2C2 Event                   */                          
	.word     I2C2_ER_IRQHandler                /* I2C2 Error                   */                            
	.word     SPI1_IRQHandler                   /* SPI1                         */                   
	.word     SPI2_IRQHandler                   /* SPI2                         */                   
	.word     USART1_IRQHandler                 /* USART1                       */                   
	.word     USART2_IRQHandler                 /* USART2                       */                   
	.word     0               				  /* Reserved                       */                   
	.word     EXTI15_10_IRQHandler              /* External Line[15:10]s        */                          
	.word     RTC_Alarm_IRQHandler              /* RTC Alarm (A and B) through EXTI Line */                 
	.word     OTG_FS_WKUP_IRQHandler            /* USB OTG FS Wakeup through EXTI line */                       
	.word     0                                 /* Reserved     				  */         
	.word     0                                 /* Reserved       			  */         
	.word     0                                 /* Reserved 					  */
	.word     0                                 /* Reserved                     */                          
	.word     DMA1_Stream7_IRQHandler           /* DMA1 Stream7                 */                          
	.word     0                                 /* Reserved                     */                   
	.word     SDIO_IRQHandler                   /* SDIO                         */                   
	.word     TIM5_IRQHandler                   /* TIM5                         */                   
	.word     SPI3_IRQHandler                   /* SPI3                         */                   
	.word     0                                 /* Reserved                     */                   
	.word     0                                 /* Reserved                     */                   
	.word     0                                 /* Reserved                     */                   
	.word     0                                 /* Reserved                     */
	.word     DMA2_Stream0_IRQHandler           /* DMA2 Stream 0                */                   
	.word     DMA2_Stream1_IRQHandler           /* DMA2 Stream 1                */                   
	.word     DMA2_Stream2_IRQHandler           /* DMA2 Stream 2                */                   
	.word     DMA2_Stream3_IRQHandler           /* DMA2 Stream 3                */                   
	.word     DMA2_Stream4_IRQHandler           /* DMA2 Stream 4                */                   
	.word     0                    			  /* Reserved                     */                   
	.word     0              					  /* Reserved                     */                     
	.word     0              					  /* Reserved                     */                          
	.word     0             					  /* Reserved                     */                          
	.word     0              					  /* Reserved                     */                          
	.word     0              					  /* Reserved                     */                          
	.word     OTG_FS_IRQHandler                 /* USB OTG FS                   */                   
	.word     DMA2_Stream5_IRQHandler           /* DMA2 Stream 5                */                   
	.word     DMA2_Stream6_IRQHandler           /* DMA2 Stream 6                */                   
	.word     DMA2_Stream7_IRQHandler           /* DMA2 Stream 7                */                   
	.word     USART6_IRQHandler                 /* USART6                       */                    
	.word     I2C3_EV_IRQHandler                /* I2C3 event                   */                          
	.word     I2C3_ER_IRQHandler                /* I2C3 error                   */                          
	.word     0                                 /* Reserved                     */                   
	.word     0                                 /* Reserved                     */                   
	.word     0                                 /* Reserved                     */                         
	.word     0                                 /* Reserved                     */                   
	.word     0                                 /* Reserved                     */                   
	.word     0                                 /* Reserved                     */                   
	.word     0                                 /* Reserved                     */
	.word     FPU_IRQHandler                    /* FPU                          */
	.word     0                                 /* Reserved                     */                   
	.word     0                                 /* Reserved                     */
	.word     SPI4_IRQHandler                   /* SPI4                         */
	.word     SPI5_IRQHandler                   /* SPI5                         */  
					
	/*******************************************************************************
	*
	* Provide weak aliases for each Exception handler to the Default_Handler. 
	* As they are weak aliases, any function with the same name will override 
	* this definition.
	* 
	*******************************************************************************/
	.weak      NMI_Handler
	.thumb_set NMI_Handler,Default_Handler

	.weak      HardFault_Handler
	.thumb_set HardFault_Handler,Default_Handler

	.weak      MemManage_Handler
	.thumb_set MemManage_Handler,Default_Handler

	.weak      BusFault_Handler
	.thumb_set BusFault_Handler,Default_Handler

	.weak      UsageFault_Handler
	.thumb_set UsageFault_Handler,Default_Handler

	.weak      SVC_Handler
	.thumb_set SVC_Handler,Default_Handler

	.weak      DebugMon_Handler
	.thumb_set DebugMon_Handler,Default_Handler

	.weak      PendSV_Handler
	.thumb_set PendSV_Handler,Default_Handler

	.weak      SysTick_Handler
	.thumb_set SysTick_Handler,Default_Handler              

	.weak      WWDG_IRQHandler                   
	.thumb_set WWDG_IRQHandler,Default_Handler      
				  
	.weak      PVD_IRQHandler      
	.thumb_set PVD_IRQHandler,Default_Handler
			   
	.weak      TAMP_STAMP_IRQHandler            
	.thumb_set TAMP_STAMP_IRQHandler,Default_Handler
			
	.weak      RTC_WKUP_IRQHandler                  
	.thumb_set RTC_WKUP_IRQHandler,Default_Handler
			
	.weak      FLASH_IRQHandler         
	.thumb_set FLASH_IRQHandler,Default_Handler
				  
	.weak      RCC_IRQHandler      
	.thumb_set RCC_IRQHandler,Default_Handler
				  
	.weak      EXTI0_IRQHandler         
	.thumb_set EXTI0_IRQHandler,Default_Handler
				  
	.weak      EXTI1_IRQHandler         
	.thumb_set EXTI1_IRQHandler,Default_Handler
					 
	.weak      EXTI2_IRQHandler         
	.thumb_set EXTI2_IRQHandler,Default_Handler 
				 
	.weak      EXTI3_IRQHandler         
	.thumb_set EXTI3_IRQHandler,Default_Handler
						
	.weak      EXTI4_IRQHandler         
	.thumb_set EXTI4_IRQHandler,Default_Handler
				  
	.weak      DMA1_Stream0_IRQHandler               
	.thumb_set DMA1_Stream0_IRQHandler,Default_Handler
		 
	.weak      DMA1_Stream1_IRQHandler               
	.thumb_set DMA1_Stream1_IRQHandler,Default_Handler
				  
	.weak      DMA1_Stream2_IRQHandler               
	.thumb_set DMA1_Stream2_IRQHandler,Default_Handler
				  
	.weak      DMA1_Stream3_IRQHandler               
	.thumb_set DMA1_Stream3_IRQHandler,Default_Handler 
				 
	.weak      DMA1_Stream4_IRQHandler              
	.thumb_set DMA1_Stream4_IRQHandler,Default_Handler
				  
	.weak      DMA1_Stream5_IRQHandler               
	.thumb_set DMA1_Stream5_IRQHandler,Default_Handler
				  
	.weak      DMA1_Stream6_IRQHandler               
	.thumb_set DMA1_Stream6_IRQHandler,Default_Handler
				  
	.weak      ADC_IRQHandler      
	.thumb_set ADC_IRQHandler,Default_Handler
			
	.weak      EXTI9_5_IRQHandler   
	.thumb_set EXTI9_5_IRQHandler,Default_Handler
			
	.weak      TIM1_BRK_TIM9_IRQHandler            
	.thumb_set TIM1_BRK_TIM9_IRQHandler,Default_Handler
			
	.weak      TIM1_UP_TIM10_IRQHandler            
	.thumb_set TIM1_UP_TIM10_IRQHandler,Default_Handler
	  
	.weak      TIM1_TRG_COM_TIM11_IRQHandler      
	.thumb_set TIM1_TRG_COM_TIM11_IRQHandler,Default_Handler
	  
	.weak      TIM1_CC_IRQHandler   
	.thumb_set TIM1_CC_IRQHandler,Default_Handler
				  
	.weak      TIM2_IRQHandler            
	.thumb_set TIM2_IRQHandler,Default_Handler
				  
	.weak      TIM3_IRQHandler            
	.thumb_set TIM3_IRQHandler,Default_Handler
				  
	.weak      TIM4_IRQHandler            
	.thumb_set TIM4_IRQHandler,Default_Handler
				  
	.weak      I2C1_EV_IRQHandler   
	.thumb_set I2C1_EV_IRQHandler,Default_Handler
					 
	.weak      I2C1_ER_IRQHandler   
	.thumb_set I2C1_ER_IRQHandler,Default_Handler
					 
	.weak      I2C2_EV_IRQHandler   
	.thumb_set I2C2_EV_IRQHandler,Default_Handler
				  
	.weak      I2C2_ER_IRQHandler   
	.thumb_set I2C2_ER_IRQHandler,Default_Handler
						   
	.weak      SPI1_IRQHandler            
	.thumb_set SPI1_IRQHandler,Default_Handler
						
	.weak      SPI2_IRQHandler            
	.thumb_set SPI2_IRQHandler,Default_Handler
				  
	.weak      USART1_IRQHandler      
	.thumb_set USART1_IRQHandler,Default_Handler
					 
	.weak      USART2_IRQHandler      
	.thumb_set USART2_IRQHandler,Default_Handler
								  
	.weak      EXTI15_10_IRQHandler               
	.thumb_set EXTI15_10_IRQHandler,Default_Handler
			   
	.weak      RTC_Alarm_IRQHandler               
	.thumb_set RTC_Alarm_IRQHandler,Default_Handler
			
	.weak      OTG_FS_WKUP_IRQHandler         
	.thumb_set OTG_FS_WKUP_IRQHandler,Default_Handler
			
	.weak      DMA1_Stream7_IRQHandler               
	.thumb_set DMA1_Stream7_IRQHandler,Default_Handler
					 
	.weak      SDIO_IRQHandler            
	.thumb_set SDIO_IRQHandler,Default_Handler
					 
	.weak      TIM5_IRQHandler            
	.thumb_set TIM5_IRQHandler,Default_Handler
					 
	.weak      SPI3_IRQHandler            
	.thumb_set SPI3_IRQHandler,Default_Handler
					 
	.weak      DMA2_Stream0_IRQHandler               
	.thumb_set DMA2_Stream0_IRQHandler,Default_Handler
			   
	.weak      DMA2_Stream1_IRQHandler               
	.thumb_set DMA2_Stream1_IRQHandler,Default_Handler
				  
	.weak      DMA2_Stream2_IRQHandler               
	.thumb_set DMA2_Stream2_IRQHandler,Default_Handler
			
	.weak      DMA2_Stream3_IRQHandler               
	.thumb_set DMA2_Stream3_IRQHandler,Default_Handler
			
	.weak      DMA2_Stream4_IRQHandler               
	.thumb_set DMA2_Stream4_IRQHandler,Default_Handler
			
	.weak      OTG_FS_IRQHandler      
	.thumb_set OTG_FS_IRQHandler,Default_Handler
					 
	.weak      DMA2_Stream5_IRQHandler               
	.thumb_set DMA2_Stream5_IRQHandler,Default_Handler
				  
	.weak      DMA2_Stream6_IRQHandler               
	.thumb_set DMA2_Stream6_IRQHandler,Default_Handler
				  
	.weak      DMA2_Stream7_IRQHandler               
	.thumb_set DMA2_Stream7_IRQHandler,Default_Handler
				  
	.weak      USART6_IRQHandler      
	.thumb_set USART6_IRQHandler,Default_Handler
						
	.weak      I2C3_EV_IRQHandler   
	.thumb_set I2C3_EV_IRQHandler,Default_Handler
						
	.weak      I2C3_ER_IRQHandler   
	.thumb_set I2C3_ER_IRQHandler,Default_Handler
						
	.weak      FPU_IRQHandler                  
	.thumb_set FPU_IRQHandler,Default_Handler  

	.weak      SPI4_IRQHandler                  
	.thumb_set SPI4_IRQHandler,Default_Handler

	.weak      SPI5_IRQHandler                  
	.thumb_set SPI5_IRQHandler,Default_Handler    


	.text
	.thumb
	.thumb_func
	.align	2
	.globl	Reset_Handler
	.type	Reset_Handler, %function
Reset_Handler:
/*  Firstly it copies data from read only memory to RAM. There are two schemes
 *  to copy. One can copy more than one sections. Another can only copy
 *  one section.  The former scheme needs more instructions and read-only
 *  data to implement than the latter.
 *  Macro __STARTUP_COPY_MULTIPLE is used to choose between two schemes.  */

#ifdef __STARTUP_COPY_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of triplets, each of which specify:
 *    offset 0: LMA of start of a section to copy from
 *    offset 4: VMA of start of a section to copy to
 *    offset 8: size of the section to copy. Must be multiply of 4
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
	ldr	r4, =__copy_table_start__
	ldr	r5, =__copy_table_end__

.L_loop0:
	cmp	r4, r5
	bge	.L_loop0_done
	ldr	r1, [r4]
	ldr	r2, [r4, #4]
	ldr	r3, [r4, #8]

.L_loop0_0:
	subs	r3, #4
	ittt	ge
	ldrge	r0, [r1, r3]
	strge	r0, [r2, r3]
	bge	.L_loop0_0

	adds	r4, #12
	b	.L_loop0

.L_loop0_done:
#else
/*  Single section scheme.
 *
 *  The ranges of copy from/to are specified by following symbols
 *    __etext: LMA of start of the section to copy from. Usually end of text
 *    __data_start__: VMA of start of the section to copy to
 *    __data_end__: VMA of end of the section to copy to
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
	ldr	r1, =__etext
	ldr	r2, =__data_start__
	ldr	r3, =__data_end__

.L_loop1:
	cmp	r2, r3
	ittt	lt
	ldrlt	r0, [r1], #4
	strlt	r0, [r2], #4
	blt	.L_loop1
#endif /*__STARTUP_COPY_MULTIPLE */

/*  This part of work usually is done in C library startup code. Otherwise,
 *  define this macro to enable it in this startup.
 *
 *  There are two schemes too. One can clear multiple BSS sections. Another
 *  can only clear one section. The former is more size expensive than the
 *  latter.
 *
 *  Define macro __STARTUP_CLEAR_BSS_MULTIPLE to choose the former.
 *  Otherwise efine macro __STARTUP_CLEAR_BSS to choose the later.
 */
#ifdef __STARTUP_CLEAR_BSS_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of tuples specifying:
 *    offset 0: Start of a BSS section
 *    offset 4: Size of this BSS section. Must be multiply of 4
 */
	ldr	r3, =__zero_table_start__
	ldr	r4, =__zero_table_end__

.L_loop2:
	cmp	r3, r4
	bge	.L_loop2_done
	ldr	r1, [r3]
	ldr	r2, [r3, #4]
	movs	r0, 0

.L_loop2_0:
	subs	r2, #4
	itt	ge
	strge	r0, [r1, r2]
	bge	.L_loop2_0

	adds	r3, #8
	b	.L_loop2
.L_loop2_done:
#elif defined (__STARTUP_CLEAR_BSS)
/*  Single BSS section scheme.
 *
 *  The BSS section is specified by following symbols
 *    __bss_start__: start of the BSS section.
 *    __bss_end__: end of the BSS section.
 *
 *  Both addresses must be aligned to 4 bytes boundary.
 */
	ldr	r1, =__bss_start__
	ldr	r2, =__bss_end__

	movs	r0, 0
.L_loop3:
	cmp	r1, r2
	itt	lt
	strlt	r0, [r1], #4
	blt	.L_loop3
#endif /* __STARTUP_CLEAR_BSS_MULTIPLE || __STARTUP_CLEAR_BSS */

#ifndef __NO_SYSTEM_INIT
	bl	SystemInit
#endif

#ifndef __START
#define __START _start
#endif
	bl	__START

	.pool
	.size	Reset_Handler, . - Reset_Handler

	.align	1
	.thumb_func
	.weak	Default_Handler
	.type	Default_Handler, %function
Default_Handler:
	b	.
	.size	Default_Handler, . - Default_Handler

	.end
