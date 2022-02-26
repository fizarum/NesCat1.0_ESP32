#ifndef CPU_H
#define CPU_H

#include <stdint.h>

// Define this to enable decimal mode in ADC / SBC (not needed in NES)
//#define  NES6502_DECIMAL

#define NES6502_NUMBANKS 16

// P (flag) register bitmasks
#define N_FLAG 0x80
#define V_FLAG 0x40
#define R_FLAG 0x20  // Reserved, always 1
#define B_FLAG 0x10
#define D_FLAG 0x08
#define I_FLAG 0x04
#define Z_FLAG 0x02
#define C_FLAG 0x01

// Vector addresses
#define NMI_VECTOR 0xFFFA
#define RESET_VECTOR 0xFFFC
#define IRQ_VECTOR 0xFFFE

// cycle counts for interrupts
#define INT_CYCLES 7
#define RESET_CYCLES 6

#define NMI_MASK 0x01
#define IRQ_MASK 0x02

// Stack is located on 6502 page 1
#define STACK_OFFSET 0x0100

typedef struct {
  uint32_t min_range, max_range;
  uint8_t (*read_func)(uint32_t address);
} nes6502_memread;

typedef struct {
  uint32_t min_range, max_range;
  void (*write_func)(uint32_t address, uint8_t value);
} nes6502_memwrite;

typedef struct {
  uint8_t *mem_page[NES6502_NUMBANKS];  // memory page pointers
  nes6502_memread *read_handler;
  nes6502_memwrite *write_handler;
  uint32_t pc_reg;
  uint8_t a_reg, p_reg;
  uint8_t x_reg, y_reg;
  uint8_t s_reg;
  uint8_t jammed;  // is processor jammed?
  uint8_t int_pending, int_latency;
  int32_t total_cycles, burn_cycles;
} nes6502_context;

#endif  // CPU_H