#ifndef APU_H
#define APU_H

#include <stdint.h>

// define this for realtime generated noise
#define REALTIME_NOISE

#define APU_WRA0 0x4000
#define APU_WRA1 0x4001
#define APU_WRA2 0x4002
#define APU_WRA3 0x4003
#define APU_WRB0 0x4004
#define APU_WRB1 0x4005
#define APU_WRB2 0x4006
#define APU_WRB3 0x4007
#define APU_WRC0 0x4008
#define APU_WRC2 0x400A
#define APU_WRC3 0x400B
#define APU_WRD0 0x400C
#define APU_WRD2 0x400E
#define APU_WRD3 0x400F
#define APU_WRE0 0x4010
#define APU_WRE1 0x4011
#define APU_WRE2 0x4012
#define APU_WRE3 0x4013

#define APU_SMASK 0x4015

// length of generated noise
#define APU_NOISE_32K 0x7FFF
#define APU_NOISE_93 93

#define APU_BASEFREQ 1789772.7272727272727272

// channel structures:  As much data as possible is precalculated, to keep the
// sample processing as lean as possible

typedef struct rectangle_s {
  uint8_t regs[4];

  bool enabled;

  float accum;
  int32_t freq;
  int32_t output_vol;
  bool fixed_envelope;
  bool holdnote;
  uint8_t volume;

  int32_t sweep_phase;
  int32_t sweep_delay;
  bool sweep_on;
  uint8_t sweep_shifts;
  uint8_t sweep_length;
  bool sweep_inc;

  // this may not be necessary in the future
  int32_t freq_limit;
  int32_t env_phase;
  int32_t env_delay;
  uint8_t env_vol;

  int vbl_length;
  uint8_t adder;
  int duty_flip;
} rectangle_t;

typedef struct triangle_s {
  uint8_t regs[3];

  bool enabled;

  float accum;
  int32_t freq;
  int32_t output_vol;

  uint8_t adder;

  bool holdnote;
  bool counter_started;
  // quasi-hack
  int write_latency;

  int vbl_length;
  int linear_length;
} triangle_t;

typedef struct noise_s {
  uint8_t regs[3];

  bool enabled;

  float accum;
  int32_t freq;
  int32_t output_vol;

  int32_t env_phase;
  int32_t env_delay;
  uint8_t env_vol;
  bool fixed_envelope;
  bool holdnote;

  uint8_t volume;

  int vbl_length;

#ifdef REALTIME_NOISE
  uint8_t xor_tap;
#else
  bool short_sample;
  int cur_pos;
#endif  // REALTIME_NOISE
} noise_t;

typedef struct dmc_s {
  uint8_t regs[4];

  // bodge for timestamp queue
  bool enabled;

  float accum;
  int32_t freq;
  int32_t output_vol;

  uint32_t address;
  uint32_t cached_addr;
  int dma_length;
  int cached_dmalength;
  uint8_t cur_byte;

  bool looping;
  bool irq_gen;
  bool irq_occurred;

} dmc_t;

enum { APU_FILTER_NONE, APU_FILTER_LOWPASS, APU_FILTER_WEIGHTED };

typedef struct {
  uint32_t min_range, max_range;
  uint8_t (*read_func)(uint32_t address);
} apu_memread;

typedef struct {
  uint32_t min_range, max_range;
  void (*write_func)(uint32_t address, uint8_t value);
} apu_memwrite;

// external sound chip stuff
typedef struct apuext_s {
  int (*init)(void);
  void (*shutdown)(void);
  void (*reset)(void);
  int32_t (*process)(void);
  apu_memread *mem_read;
  apu_memwrite *mem_write;
} apuext_t;

typedef struct apu_s {
  rectangle_t rectangle[2];
  triangle_t triangle;
  noise_t noise;
  dmc_t dmc;
  uint8_t enable_reg;

  void *buffer;  // pointer to output buffer
  int num_samples;

  uint8_t mix_enable;
  int filter_type;

  double base_freq;
  float cycle_rate;

  int sample_rate;
  int sample_bits;
  int refresh_rate;

  void (*process)(void *buffer, int num_samples);
  void (*irq_callback)(void);
  uint8_t (*irqclear_callback)(void);

  // external sound chip
  apuext_t *ext;
} apu_t;

#endif  // APU_H