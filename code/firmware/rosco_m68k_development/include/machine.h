/*
 *------------------------------------------------------------
 *                                  ___ ___ _   
 *  ___ ___ ___ ___ ___       _____|  _| . | |_ 
 * |  _| . |_ -|  _| . |     |     | . | . | '_|
 * |_| |___|___|___|___|_____|_|_|_|___|___|_,_| 
 *                     |_____|       firmware v2
 * ------------------------------------------------------------
 * Copyright (c)2019-2021 Ross Bamford and contributors
 * See top-level LICENSE.md for licence information.
 *
 * C prototypes for system routines implemented in assembly.
 * ------------------------------------------------------------
 */
#ifndef _ROSCOM68K_MACHINE_H
#define _ROSCOM68K_MACHINE_H

#include <stdnoreturn.h>
#include <stdint.h>

#define MFP_GPDR      0xf80001
#define MFP_AER       0xf80003
#define MFP_DDR       0xf80005
#define MFP_IERA      0xf80007
#define MFP_IERB      0xf80009
#define MFP_IPRA      0xf8000B
#define MFP_IPRB      0xf8000D
#define MFP_ISRA      0xf8000F
#define MFP_ISRB      0xf80011
#define MFP_IMRA      0xf80013
#define MFP_IMRB      0xf80015
#define MFP_VR        0xf80017
#define MFP_TACR      0xf80019
#define MFP_TBCR      0xf8001B
#define MFP_TCDCR     0xf8001D
#define MFP_TADR      0xf8001F
#define MFP_TBDR      0xf80021
#define MFP_TCDR      0xf80023
#define MFP_TDDR      0xf80025
#define MFP_SCR       0xf80027
#define MFP_UCR       0xf80029
#define MFP_RSR       0xf8002B
#define MFP_TSR       0xf8002D
#define MFP_UDR       0xf8002F

// Define addresses used by the temporary bus error handler.
// N.B. These are duplicated in equates.asm, and must be kept in sync!
#define BERR_SAVED                  0x1180
#define BERR_FLAG                   0x1184

/* 
 * Idle the processor.
 *
 * Issues a STOP instruction, keeping the CPU in supervisor mode
 * and setting interrupt priority to 3.
 *
 * Does not stop the system tick.
 *
 * Requires the CPU to be in supervisor mode.
 */
noreturn void IDLE();

/*
 * Halt the processor.
 *
 * Issues a STOP instruction, keeping the CPU in supervisor mode,
 * masking all interrupts (priority to 7) and disabling the MFP interrupts.
 *
 * The only way to come back from this is via the wetware!
 *
 * Requires the CPU to be in supervisor mode.
 */
noreturn void HALT();

/*
 * Start the system tick (MFP Timer C).
 *
 * Note that this cannot be done until we've finished using polled
 * (synchronous) IO on the UART (i.e. the EARLY_PRINT routines).
 * Otherwise, timing issues will cause garbage on the serial port. 
 */
void START_HEART();

/*
 * Stop the system tick (MFP Timer C)
 */
void STOP_HEART();

/*
 * Set m68k interrupt priority level (0-7).
 *
 * Requires the CPU to be in supervisor mode.
 */
void SET_INTR(uint8_t priority);

/*
 * Early print null-terminated string.
 *
 * Don't use this after START_HEART has been called. 
 */
void EARLY_PRINT_C(char *str);

/*
 * Firmware PRINT function (uses pointer at $414)
 */
void FW_PRINT_C(char *str);

/*
 * Firmware PRINTLN function (uses pointer at $418)
 */
void FW_PRINTLN_C(char *str);

/*
 * Busywait for a while. The actual time is wholly dependent
 * on CPU (i.e. clock) speed!
 */
void BUSYWAIT_C(uint32_t ticks);

/*
 * Install temporary bus error handler that will set a flag if an error
 * occurs, and not retry the instruction.
 *
 * Saves the existing handler for use by a subsequent RESTORE_BERR_HANDLER.
 *
 * Supports various m68k models.
 *
 * The flag will be set at BERR_FLAG (defined above). The flag will
 * be zeroed when this function is called.
 */
extern void INSTALL_TEMP_BERR_HANDLER(void);

/*
 * Restore original bus error handler, saved by a prior call to
 * INSTALL_TEMP_BERR_HANDLER.
 */
extern void RESTORE_BERR_HANDLER(void);


#endif

