#ifndef NESEMULATOR_1_H
#define NESEMULATOR_1_H

#include <display.h>
#include <fstorage.h>
#include <utils.h>

#include "compositevideo.h"
#include "nes/nes.h"

//--------------------------------------------------------------------------------
/// TYPEDEFS:

// Define this if running on little-endian (x86) systems
#define HOST_LITTLE_ENDIAN  //!!! important for Arduino
#define ZERO_LENGTH 0

// quell stupid compiler warnings
#define UNUSED(x) ((x) = (x))

// AUDIO_SETUP
#define DEFAULT_SAMPLERATE 24000
#define DEFAULT_FRAGSIZE 60  // max.256, default 240

// VIDEO_SETUP
// todo: remove from here
#define NES_SCREEN_WIDTH 256

#define PAL

// NTSC = 60Hz, PAL = 50Hz
#ifdef PAL
#define NES_REFRESH_RATE 50
#else
#define NES_REFRESH_RATE 60
#endif

#define NES_CLOCK_DIVIDER 12  // default: 12
//#define  NES_MASTER_CLOCK     21477272.727272727272
#define NES_MASTER_CLOCK (236250000 / 11)
#define NES_SCANLINE_CYCLES (1364.0 / NES_CLOCK_DIVIDER)
#define NES_FIQ_PERIOD (NES_MASTER_CLOCK / NES_CLOCK_DIVIDER / 60)

#define NES_RAMSIZE 0x800
#define FILESPERPAGE 8

SdFile dirFile;
SdFile file;

uint8_t NES_POWER = 1;

void (*audio_callback)(void *buffer, int length) = NULL;

/* read counters */
int pad0_readcount, pad1_readcount, ppad_readcount, ark_readcount;

void input_strobe(void) {
  pad0_readcount = 0;
  pad1_readcount = 0;
  ppad_readcount = 0;
  ark_readcount = 0;
}

// moved from main cpp file, from setup() function, after initVideo()
// I2S0.conf.rx_start = 0;  /// stop DMA ADC
// I2S0.in_link.start = 0;

uint8_t get_pad0(void) {
  uint8_t value;
  value = 0;

  //  12, 33, 9, 10 pin not usable

  if (digitalRead(PIN_A) == 1) value |= 1;        // A
  if (digitalRead(PIN_B) == 1) value |= 2;        // B
  if (digitalRead(PIN_SELECT) == 1) value |= 4;   // SELECT
  if (digitalRead(PIN_START) == 1) value |= 8;    // START
  if (digitalRead(PIN_UP) == 1) value |= 16;      // UP
  if (digitalRead(PIN_DOWN) == 1) value |= 32;    // DOWN
  if (digitalRead(PIN_LEFT) == 1) value |= 64;    // LEFT
  if (digitalRead(PIN_RIGHT) == 1) value |= 128;  // RIGHT

  if (digitalRead(PIN_SELECT) == 1 && digitalRead(PIN_START) == 1) {
    /// osd_stopsound();
    /// audio_callback = NULL;

    NES_POWER = 0;  // EXIT
  }

  // EJECT EMULATOR
  if (JOY_SHARE == 1 && JOY_OPTIONS == 1) {
    /// osd_stopsound();

    JOY_SHARE = 0;
    JOY_OPTIONS = 0;
    NES_POWER = 0;
  }

  if (JOY_SHARE == 1) value |= 8;    // START
  if (JOY_OPTIONS == 1) value |= 4;  // SELECT

  if (JOY_UP == 1) value |= 16;      // UP
  if (JOY_DOWN == 1) value |= 32;    // DOWN
  if (JOY_LEFT == 1) value |= 64;    // LEFT
  if (JOY_RIGHT == 1) value |= 128;  // RIGHT
  if (JOY_CROSS == 1) value |= 1;    // A
  if (JOY_SQUARE == 1) value |= 2;   // B
  ///  if (JOY_CIRCLE==1) Serial.println("JOY_CIRCLE");
  ///  if (JOY_TRIANGLE==1) Serial.println("JOY_TRIANGLE");

  // mask out left/right simultaneous keypresses
  ///  if ((value & JOY_UP) && (value & JOY_DOWN)) value &= ~(JOY_UP |
  ///  JOY_DOWN); if ((value & JOY_LEFT) && (value & JOY_RIGHT)) value &=
  ///  ~(JOY_LEFT | JOY_RIGHT);
  // return (0x40 | value) due to bus conflicts
  /// return (0x40 | ((value >> pad0_readcount++) & 1));
  return (((value >> pad0_readcount++) & 1));
}
//********************************************************************************
//********************************************************************************
TimerHandle_t timer_1;

// Seemingly, this will be called only once. Should call func with a freq of
// frequency,
IRAM_ATTR int osd_installtimer_1(int frequency, void *func, int funcsize,
                                 void *counter, int countersize) {
  debug("Timer install, freq=%d", frequency);

  delay(1000);
  timer_1 = xTimerCreate("nes_hid", configTICK_RATE_HZ / frequency, pdTRUE,
                         NULL, (TimerCallbackFunction_t)func);
  xTimerStart(timer_1, 0);
  return 0;
}
//********************************************************************************
//................................................................................
// AUDIO SETUP
typedef struct sndinfo_s {
  int sample_rate;
  int bps;
} sndinfo_t;

#if SOUND_ENABLED

intr_handle_t i2s_interrupt;

i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_BCK_IO,
    .ws_io_num = I2S_WS_IO,
    .data_out_num = I2S_DO_IO,
    .data_in_num = I2S_DI_IO  // Not used
};
i2s_config_t audio_cfg = {
    .mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER |
                                    I2S_MODE_TX /*| I2S_MODE_DAC_BUILT_IN*/),
    .sample_rate = DEFAULT_SAMPLERATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    ///.channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    ///  .communication_format =
    ///  static_cast<i2s_comm_format_t>(I2S_COMM_FORMAT_PCM |
    ///  I2S_COMM_FORMAT_I2S_MSB),
    .communication_format = static_cast<i2s_comm_format_t>(
        I2S_COMM_FORMAT_STAND_PCM_SHORT | I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags =
        ESP_INTR_FLAG_LEVEL1 /*| ESP_INTR_FLAG_IRAM  | ESP_INTR_FLAG_SHARED*/,
    .dma_buf_count = 6,
    // .dma_buf_len = 2 * DEFAULT_FRAGSIZE,
    .dma_buf_len = 4 * DEFAULT_FRAGSIZE,
    .use_apll = false};
#endif
int init_sound(void) {
#if SOUND_ENABLED
  i2s_driver_install(I2S_NUM_1, &audio_cfg, 0, NULL);
  i2s_set_pin(I2S_NUM_1, &pin_config);
  i2s_set_sample_rates(I2S_NUM_1, DEFAULT_SAMPLERATE);
#endif
  audio_callback = NULL;
  return 0;
}

void osd_setsound(void (*playfunc)(void *buffer, int length)) {
  // Indicates we should call playfunc() to get more data.
  audio_callback = playfunc;
}

void osd_stopsound(void) { audio_callback = NULL; }

void osd_getsoundinfo(sndinfo_t *info) {
  info->sample_rate = DEFAULT_SAMPLERATE;
  info->bps = 16;
}
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#if SOUND_ENABLED
QueueHandle_t queue;
//  uint16_t audio_frame[DEFAULT_FRAGSIZE];
uint16_t audio_frame[2 * DEFAULT_FRAGSIZE];
#endif

void do_audio_frame() {
  /// printf("do_audio_frame\n");

#if SOUND_ENABLED
  int left = DEFAULT_SAMPLERATE / NES_REFRESH_RATE;
  while (left) {
    int n = DEFAULT_FRAGSIZE;
    if (n > left) n = left;
    audio_callback(audio_frame, n);  // get more data

    // 16 bit mono -> 32-bit (16 bit r+l)
    for (int i = n - 1; i >= 0; i--) {
      //      audio_frame[i] = audio_frame[i] + 0x8000;
      ///      uint16_t a = (audio_frame[i] >> 3); //VEEERY BAD!
      uint16_t a = (audio_frame[i] >> 0);
      audio_frame[i * 2 + 1] = 0x8000 + a;
      audio_frame[i * 2] = 0x8000 - a;

      /// Serial.print(audio_frame[i]);
    }
    size_t i2s_bytes_write;
    // i2s_write(I2S_NUM_0, (const char *)audio_frame, 2 * n, &i2s_bytes_write,
    // portMAX_DELAY); left -= i2s_bytes_write / 2;
    i2s_write(I2S_NUM_1, (const char *)audio_frame, 4 * n, &i2s_bytes_write,
              portMAX_DELAY);
    left -= i2s_bytes_write / 4;
  }

#endif
}
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// HID timer ISR
volatile int time_ticks = 0;
IRAM_ATTR static void timer_isr(void) {
  if (SOUND_ENABLED)
    if (audio_callback && NES_POWER == 1) do_audio_frame();
  time_ticks++;
}
IRAM_ATTR static void timer_isr_end(void) {}
IRAM_ATTR int install_timer(int hertz) {
  return osd_installtimer_1(hertz, (void *)timer_isr,
                            (int)timer_isr_end - (int)timer_isr,
                            (void *)&time_ticks, sizeof(time_ticks));
}
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
char *NESEXPLORE(char *path) {
  uint8_t num = 0;
  uint8_t loadedFileNames = 0;

  // clear memory variables
  for (uint16_t tmp = 0; tmp < MAXFILES; tmp++)
    memset(filename[tmp], 0, sizeof(filename[tmp]));
  fileext[0] = 0;
  fileext[1] = 0;
  fileext[2] = 0;
  fileext[3] = 0;

  num = 0;
  loadedFileNames = 0;

  // LOAD FILENAMES INTO MEMORY...

  // Load List files in root directory.
  /// if (!dirFile.open("/", O_READ)) {
  if (!dirFile.open(path, O_READ)) {
    while (1) {
    };
  }
  while (num < MAXFILES && file.openNext(&dirFile, O_READ)) {
    // Skip hidden files.
    if (!file.isHidden()) {
      for (uint8_t i = strlen(filename[num]); i > 3; i--) filename[num][i] = 0;
      file.getName(filename[num], MAXFILENAME_LENGTH);
      if (file.isSubDir()) {
        sprintf(filename[num], "%s/", filename[num]);
        num++;
      } else {
        for (uint8_t i = strlen(filename[num]); i > 3; i--) {
          if (filename[num][i] != 0) {
            fileext[3] = '\0';
            fileext[2] = filename[num][i];
            fileext[1] = filename[num][i - 1];
            fileext[0] = filename[num][i - 2];
            break;
          }
        }
      }

      if (DEBUG) {
        /// Serial.println(fileext[num]);
        /// Serial.println(strlen(filename[num]));
      }

      // check NES File extension, then increase index
      if ((fileext[0] == 'N' || fileext[0] == 'n') &&
          (fileext[1] == 'E' || fileext[1] == 'e') &&
          (fileext[2] == 'S' || fileext[2] == 's')) {
        num++;
      }
    }
    loadedFileNames = num;
    file.close();
  }

  dirFile.close();

  debug("--------------------------------------");
  debug("Count of loaded File Names: %d\n", loadedFileNames);

  sortStrings(filename, loadedFileNames);

  // DRAW FILENAMES INTO BUFFER
  uint8_t CURSOR = 0;
  uint8_t PAGE = 0;
  bool NamesDisplayed = false;

  while (1) {
    PAGE = CURSOR / FILESPERPAGE;
    if (!NamesDisplayed) {
      nescreen::fillScreen();
      nescreen::setTextPosition(16, 24);
      nescreen::drawText(path);
      nescreen::update();

      for (num = PAGE * FILESPERPAGE;
           num < ((PAGE + 1) * FILESPERPAGE) && num < loadedFileNames; num++) {
        nescreen::setTextPosition(40, 48 + 20 * (num % FILESPERPAGE));

        if (filename[num][strlen(filename[num]) - 1] == '/')
          nescreen::drawText(filename[num], 23);
        else
          nescreen::drawText(filename[num], 48);

        delay(1);
      }
      NamesDisplayed = true;
    }

    // Draw Cursor
    nescreen::setTextPosition(16, 48 + (20 * (CURSOR % FILESPERPAGE)));
    nescreen::drawText("->", 48);
    delay(200);

    // todo: recheck and remove
    //  // PROCESS CURSOR SELECTION
    //  while (JOY_CROSS == 0 && JOY_SQUARE == 0 && JOY_OPTIONS == 0 &&
    //         JOY_SHARE == 0 && JOY_UP == 0 && JOY_DOWN == 0 && JOY_LEFT == 0
    //         && JOY_RIGHT == 0) {
    //    if (digitalRead(PIN_A) == 1) {
    //      JOY_CROSS = 1;  // A
    //      delay(25);
    //    }
    //    if (digitalRead(PIN_B) == 1) {
    //      JOY_SQUARE = 1;  // B
    //      delay(25);
    //    }
    //    if (digitalRead(PIN_SELECT) == 1) {
    //      JOY_OPTIONS = 1;  // SELECT
    //      delay(25);
    //    }
    //    if (digitalRead(PIN_START) == 1) {
    //      JOY_SHARE = 1;  // START
    //      delay(25);
    //    }
    //    if (digitalRead(PIN_UP) == 1) {
    //      JOY_UP = 1;
    //      delay(25);
    //    }
    //    if (digitalRead(PIN_DOWN) == 1) {
    //      JOY_DOWN = 1;  // DOWN
    //      delay(25);
    //    }
    //    if (digitalRead(PIN_LEFT) == 1) {
    //      JOY_LEFT = 1;  // LEFT
    //      delay(25);
    //    }
    //    if (digitalRead(PIN_RIGHT) == 1) {
    //      JOY_RIGHT = 1;  // RIGHT
    //      delay(25);
    //    }
    //  }

    // Empty Cursor
    nescreen::setTextPosition(16, 48 + (20 * (CURSOR % FILESPERPAGE)));
    nescreen::drawText("  ", 48);

    if (JOY_SHARE == 1 && JOY_OPTIONS == 1) {
      JOY_SHARE = 0;
      JOY_OPTIONS = 0;
      EXIT = true;
      ///         PLAYING=false;
      NES_POWER = 0;

      return MAINPATH;
    }

    if (JOY_UP == 1) {
      if (CURSOR % FILESPERPAGE == 0) NamesDisplayed = false;  // changed page
      if (CURSOR == 0 && loadedFileNames > 0)
        CURSOR = loadedFileNames - 1;
      else if (CURSOR > 0 && loadedFileNames > 0)
        CURSOR--;
      JOY_UP = 0;
    }
    if (JOY_DOWN == 1) {
      if (CURSOR % FILESPERPAGE == FILESPERPAGE - 1 ||
          CURSOR == loadedFileNames - 1)
        NamesDisplayed = false;  // changed page
      if (CURSOR == loadedFileNames - 1 && loadedFileNames > 0)
        CURSOR = 0;
      else if (CURSOR < loadedFileNames - 1 && loadedFileNames > 0)
        CURSOR++;
      JOY_DOWN = 0;
    }
    if (JOY_LEFT == 1) {
      if (CURSOR > FILESPERPAGE - 1) CURSOR -= FILESPERPAGE;
      NamesDisplayed = false;
      JOY_LEFT = 0;
    }
    if (JOY_RIGHT == 1) {
      if (CURSOR / FILESPERPAGE < loadedFileNames / FILESPERPAGE)
        CURSOR += FILESPERPAGE;
      if (CURSOR > loadedFileNames - 1) CURSOR = loadedFileNames - 1;
      NamesDisplayed = false;
      JOY_RIGHT = 0;
    }
    if (JOY_OPTIONS == 1) {
      // do nothing  = unused
      JOY_OPTIONS = 0;
    }
    if ((JOY_CROSS == 1 || JOY_SHARE == 1) && JOY_OPTIONS == 0) {
      dirFile.close();
      JOY_CROSS = 0;
      JOY_SHARE = 0;
      JOY_OPTIONS = 0;
      delay(25);

      ///         PLAYINGFILE=CURSOR;
      ///         TOTALFILES=loadedFileNames;

      sprintf(MAINPATH, "%s%s", path, filename[CURSOR]);
      debug(MAINPATH);
      return MAINPATH;  // START //A
    }
    if ((JOY_SQUARE == 1) && JOY_OPTIONS == 0) {
      dirFile.close();
      JOY_SQUARE = 0;
      JOY_SHARE = 0;
      JOY_OPTIONS = 0;
      delay(25);

      if (DEBUG) Serial.println(path);
      if (DEBUG) Serial.println(strlen(path));

      sprintf(MAINPATH, "%s", path);

      if (strlen(MAINPATH) > 1) {
        MAINPATH[strlen(MAINPATH) - 1] = '\0';
        for (uint8_t strpos = strlen(MAINPATH) - 1; strpos > 0; strpos--) {
          if (MAINPATH[strpos] == '/') break;
          MAINPATH[strpos] = '\0';
        }
      }

      if (DEBUG) Serial.println(MAINPATH);
      if (DEBUG) Serial.println(strlen(MAINPATH));
      return MAINPATH;
    }
  };
}
//################################################################################
//********************************************************************************
char *NESBrowse(char *path) {
  if (path[strlen(path) - 1] != '/')
    if (strlen(path) > 1) {
      path[strlen(path) - 1] = '\0';
      for (uint8_t strpos = strlen(path) - 1; strpos > 0; strpos--) {
        if (path[strpos] == '/') break;
        path[strpos] = '\0';
      }
    }

  EXIT = false;
  //................................................................................
  while (EXIT == false && path[strlen(path) - 1] == '/') {
    path = NESEXPLORE(path);
    Serial.println(path);
  }
  return path;
}
//________________________________________________________________________________

//--------------------------------------------------------------------------------
ppu_t ppu;
apu_t apu;
mmc_t mmc;

nes_t nes;
nes_t *NESmachine;

//--------------------------------------------------------------------------------
//
//   EMULATOR CORE CODES:
//
//--------------------------------------------------------------------------------
// NES.C part
void nes_setfiq(uint8_t value) {
  nes.fiq_state = value;
  nes.fiq_cycles = (int)NES_FIQ_PERIOD;
}
void nes_nmi(void) { nes6502_nmi(); }
//--------------------------------------------------------------------------------
// PPU.C:

// PPU access
#define PPU_MEM(x) ppu.page[(x) >> 10][(x)]

// Background (color 0) and solid sprite pixel flags
#define BG_TRANS 0x80
#define SP_PIXEL 0x40
#define BG_CLEAR(V) ((V)&BG_TRANS)
#define BG_SOLID(V) (0 == BG_CLEAR(V))
#define SP_CLEAR(V) (0 == ((V)&SP_PIXEL))

// Full BG color
#define FULLBG (ppu.palette[0] | BG_TRANS)

void ppu_displaysprites(bool display) { ppu.drawsprites = display; }

void ppu_getcontext(ppu_t *dest_ppu) {
  int nametab[4];

  *dest_ppu = ppu;

  // we can't just copy contexts here, because more than likely,
  // the top 8 pages of the ppu are pointing to internal PPU memory,
  // which means we need to recalculate the page pointers.
  // TODO: we can either get rid of the page pointing in the code,
  // or add more robust checks to make sure that pages 8-15 are
  // definitely pointing to internal PPU RAM, not just something
  // that some crazy mapper paged in.

  nametab[0] = (ppu.page[8] - ppu.nametab + 0x2000) >> 10;
  nametab[1] = (ppu.page[9] - ppu.nametab + 0x2400) >> 10;
  nametab[2] = (ppu.page[10] - ppu.nametab + 0x2800) >> 10;
  nametab[3] = (ppu.page[11] - ppu.nametab + 0x2C00) >> 10;

  dest_ppu->page[8] = dest_ppu->nametab + (nametab[0] << 10) - 0x2000;
  dest_ppu->page[9] = dest_ppu->nametab + (nametab[1] << 10) - 0x2400;
  dest_ppu->page[10] = dest_ppu->nametab + (nametab[2] << 10) - 0x2800;
  dest_ppu->page[11] = dest_ppu->nametab + (nametab[3] << 10) - 0x2C00;
  dest_ppu->page[12] = dest_ppu->page[8] - 0x1000;
  dest_ppu->page[13] = dest_ppu->page[9] - 0x1000;
  dest_ppu->page[14] = dest_ppu->page[10] - 0x1000;
  dest_ppu->page[15] = dest_ppu->page[11] - 0x1000;
}

void ppu_setcontext(ppu_t *src_ppu) {
  int nametab[4];

  ppu = *src_ppu;

  // we can't just copy contexts here, because more than likely,
  // the top 8 pages of the ppu are pointing to internal PPU memory,
  // which means we need to recalculate the page pointers.
  // TODO: we can either get rid of the page pointing in the code,
  // or add more robust checks to make sure that pages 8-15 are
  // definitely pointing to internal PPU RAM, not just something
  // that some crazy mapper paged in.

  nametab[0] = (src_ppu->page[8] - src_ppu->nametab + 0x2000) >> 10;
  nametab[1] = (src_ppu->page[9] - src_ppu->nametab + 0x2400) >> 10;
  nametab[2] = (src_ppu->page[10] - src_ppu->nametab + 0x2800) >> 10;
  nametab[3] = (src_ppu->page[11] - src_ppu->nametab + 0x2C00) >> 10;

  ppu.page[8] = ppu.nametab + (nametab[0] << 10) - 0x2000;
  ppu.page[9] = ppu.nametab + (nametab[1] << 10) - 0x2400;
  ppu.page[10] = ppu.nametab + (nametab[2] << 10) - 0x2800;
  ppu.page[11] = ppu.nametab + (nametab[3] << 10) - 0x2C00;
  ppu.page[12] = ppu.page[8] - 0x1000;
  ppu.page[13] = ppu.page[9] - 0x1000;
  ppu.page[14] = ppu.page[10] - 0x1000;
  ppu.page[15] = ppu.page[11] - 0x1000;
}

ppu_t *temp;

ppu_t *ppu_create(void) {
  static bool pal_generated = false;

  if (NULL == temp) temp = (ppu_t *)malloc(sizeof(ppu_t));
  if (NULL == temp) return NULL;

  memset(temp, 0, sizeof(ppu_t));

  temp->latchfunc = NULL;
  temp->vromswitch = NULL;
  temp->vram_present = false;
  temp->drawsprites = true;

  // TODO: probably a better way to do this...
  if (false == pal_generated) {
    /// pal_generate();
    pal_generated = true;
  }

  /// ppu_setdefaultpal(temp);

  return temp;
}

void ppu_setpage(int size, int page_num, uint8_t *location) {
  // deliberately fall through
  switch (size) {
    case 8:
      ppu.page[page_num++] = location;
      ppu.page[page_num++] = location;
      ppu.page[page_num++] = location;
      ppu.page[page_num++] = location;
    case 4:
      ppu.page[page_num++] = location;
      ppu.page[page_num++] = location;
    case 2:
      ppu.page[page_num++] = location;
    case 1:
      ppu.page[page_num++] = location;
      break;
  }
}

// make sure $3000-$3F00 mirrors $2000-$2F00
void ppu_mirrorhipages(void) {
  ppu.page[12] = ppu.page[8] - 0x1000;
  ppu.page[13] = ppu.page[9] - 0x1000;
  ppu.page[14] = ppu.page[10] - 0x1000;
  ppu.page[15] = ppu.page[11] - 0x1000;
}

void ppu_mirror(int nt1, int nt2, int nt3, int nt4) {
  ppu.page[8] = ppu.nametab + (nt1 << 10) - 0x2000;
  ppu.page[9] = ppu.nametab + (nt2 << 10) - 0x2400;
  ppu.page[10] = ppu.nametab + (nt3 << 10) - 0x2800;
  ppu.page[11] = ppu.nametab + (nt4 << 10) - 0x2C00;
  ppu.page[12] = ppu.page[8] - 0x1000;
  ppu.page[13] = ppu.page[9] - 0x1000;
  ppu.page[14] = ppu.page[10] - 0x1000;
  ppu.page[15] = ppu.page[11] - 0x1000;
}

// bleh, for snss
uint8_t *ppu_getpage(int page) { return ppu.page[page]; }

void mem_trash(uint8_t *buffer, int length) {
  int i;

  for (i = 0; i < length; i++) buffer[i] = (uint8_t)rand();
}

// reset state of ppu
void ppu_reset() {
  /// if (HARD_RESET == reset_type)
  mem_trash(ppu.oam, 256);
  /// memset(ppu.oam, 0, 256);

  ppu.ctrl0 = 0;
  ppu.ctrl1 = PPU_CTRL1F_OBJON | PPU_CTRL1F_BGON;
  ppu.stat = 0;
  ppu.flipflop = 0;
  ppu.vaddr_latch = 0x2000;
  ppu.vaddr = ppu.vaddr_latch = 0x2000;

  ppu.oam_addr = 0;
  ppu.tile_xofs = 0;

  ppu.latch = 0;
  ppu.vram_accessible = true;
}

// we render a scanline of graphics first so we know exactly
// where the sprite 0 strike is going to occur (in terms of
// cpu cycles), using the relation that 3 pixels == 1 cpu cycle
void ppu_setstrike(int x_loc) {
  if (false == ppu.strikeflag) {
    ppu.strikeflag = true;

    // 3 pixels per cpu cycle
    ppu.strike_cycle = nes6502_getcycles(false) + (x_loc / 3);
  }
}

void ppu_oamdma(uint8_t value) {
  uint32_t cpu_address;
  uint8_t oam_loc;

  cpu_address = (uint32_t)(value << 8);

  // Sprite DMA starts at the current SPRRAM address
  oam_loc = ppu.oam_addr;
  do {
    ppu.oam[oam_loc++] = nes6502_getbyte(cpu_address++);
  } while (oam_loc != ppu.oam_addr);

  // TODO: enough with houdini
  cpu_address -= 256;
  // Odd address in $2003
  if ((ppu.oam_addr >> 2) & 1) {
    for (oam_loc = 4; oam_loc < 8; oam_loc++)
      ppu.oam[oam_loc] = nes6502_getbyte(cpu_address++);
    cpu_address += 248;
    for (oam_loc = 0; oam_loc < 4; oam_loc++)
      ppu.oam[oam_loc] = nes6502_getbyte(cpu_address++);
  } else {  // Even address in $2003
    for (oam_loc = 0; oam_loc < 8; oam_loc++)
      ppu.oam[oam_loc] = nes6502_getbyte(cpu_address++);
  }

  // make the CPU spin for DMA cycles
  nes6502_burn(513);
  nes6502_release();
}

void ppu_writehigh(uint32_t address, uint8_t value) {
  switch (address) {
    case PPU_OAMDMA:
      ppu_oamdma(value);
      break;

    case PPU_JOY0:
      // VS system VROM switching - bleh!
      if (ppu.vromswitch) ppu.vromswitch(value);

      // see if we need to strobe them joypads
      value &= 1;

      if (0 == value && ppu.strobe) input_strobe();

      ppu.strobe = value;

      // NES Controller strobe
      ///  digitalWrite(NESCTRL_Latch, HIGH);
      ///  digitalWrite(NESCTRL_Latch, LOW);
      //----------------------

      break;

    case PPU_JOY1:  // frame IRQ control
      nes_setfiq(value);
      break;

    default:
      break;
  }
}

uint8_t ppu_readhigh(uint32_t address) {
  uint8_t value;
  switch (address) {
    case PPU_JOY0:
      value = get_pad0();

      ///      if (digitalRead(NESCTRL_Data) == LOW) value=255;
      ///      digitalWrite(NESCTRL_Clock, HIGH);
      ///      digitalWrite(NESCTRL_Clock, LOW);
      /// pad0_readcount++;

      break;

    case PPU_JOY1:
      value = 0;
      break;

    default:
      value = 0xFF;
      break;
  }

  return value;
}

// Read from $2000-$2007
uint8_t ppu_read(uint32_t address) {
  uint8_t value;
  // handle mirrored reads up to $3FFF
  switch (address & 0x2007) {
    case PPU_STAT:
      value = (ppu.stat & 0xE0) | (ppu.latch & 0x1F);

      if (ppu.strikeflag) {
        if (nes6502_getcycles(false) >= ppu.strike_cycle)
          value |= PPU_STATF_STRIKE;
      }

      // clear both vblank flag and vram address flipflop
      ppu.stat &= ~PPU_STATF_VBLANK;
      ppu.flipflop = 0;
      break;

    case PPU_VDATA:
      // buffered VRAM reads
      value = ppu.latch = ppu.vdata_latch;

      // VRAM only accessible during VBL
      if ((ppu.bg_on || ppu.obj_on) && !ppu.vram_accessible) {
        ppu.vdata_latch = 0xFF;
        /// nes_log_printf("VRAM read at $%04X, scanline %d\n", ppu.vaddr,
        /// nes_getcontextptr()->scanline);
      } else {
        uint32_t addr = ppu.vaddr;
        if (addr >= 0x3000) addr -= 0x1000;
        ppu.vdata_latch = PPU_MEM(addr);
      }
      ppu.vaddr += ppu.vaddr_inc;
      ppu.vaddr &= 0x3FFF;
      break;

    case PPU_OAMDATA:
    case PPU_CTRL0:
    case PPU_CTRL1:
    case PPU_OAMADDR:
    case PPU_SCROLL:
    case PPU_VADDR:
    default:
      value = ppu.latch;
      break;
  }

  return value;
}

// Write to $2000-$2007
void ppu_write(uint32_t address, uint8_t value) {
  // write goes into ppu latch...
  ppu.latch = value;

  switch (address & 0x2007) {
    case PPU_CTRL0:
      ppu.ctrl0 = value;
      ppu.obj_height = (value & PPU_CTRL0F_OBJ16) ? 16 : 8;
      ppu.bg_base = (value & PPU_CTRL0F_BGADDR) ? 0x1000 : 0;
      ppu.obj_base = (value & PPU_CTRL0F_OBJADDR) ? 0x1000 : 0;
      ppu.vaddr_inc = (value & PPU_CTRL0F_ADDRINC) ? 32 : 1;
      ppu.tile_nametab = value & PPU_CTRL0F_NAMETAB;

      // Mask out bits 10 & 11 in the ppu latch
      ppu.vaddr_latch &= ~0x0C00;
      ppu.vaddr_latch |= ((value & 3) << 10);
      break;
    case PPU_CTRL1:
      ppu.ctrl1 = value;
      ppu.obj_on = (value & PPU_CTRL1F_OBJON) ? true : false;
      ppu.bg_on = (value & PPU_CTRL1F_BGON) ? true : false;
      ppu.obj_mask = (value & PPU_CTRL1F_OBJMASK) ? false : true;
      ppu.bg_mask = (value & PPU_CTRL1F_BGMASK) ? false : true;
      break;
    case PPU_OAMADDR:
      ppu.oam_addr = value;
      break;
    case PPU_OAMDATA:
      ppu.oam[ppu.oam_addr++] = value;
      break;
    case PPU_SCROLL:
      if (0 == ppu.flipflop) {
        // Mask out bits 4 - 0 in the ppu latch
        ppu.vaddr_latch &= ~0x001F;
        ppu.vaddr_latch |= (value >> 3);  // Tile number
        ppu.tile_xofs = (value & 7);      // Tile offset (0-7 pix)
      } else {
        // Mask out bits 14-12 and 9-5 in the ppu latch
        ppu.vaddr_latch &= ~0x73E0;
        ppu.vaddr_latch |= ((value & 0xF8) << 2);  // Tile number
        ppu.vaddr_latch |= ((value & 7) << 12);    // Tile offset (0-7 pix)
      }
      ppu.flipflop ^= 1;
      break;
    case PPU_VADDR:
      if (0 == ppu.flipflop) {
        // Mask out bits 15-8 in ppu latch
        ppu.vaddr_latch &= ~0xFF00;
        ppu.vaddr_latch |= ((value & 0x3F) << 8);
      } else {
        // Mask out bits 7-0 in ppu latch
        ppu.vaddr_latch &= ~0x00FF;
        ppu.vaddr_latch |= value;
        ppu.vaddr = ppu.vaddr_latch;
      }
      ppu.flipflop ^= 1;
      break;
    case PPU_VDATA:
      if (ppu.vaddr < 0x3F00) {
        // VRAM only accessible during scanlines 241-260
        if ((ppu.bg_on || ppu.obj_on) && !ppu.vram_accessible) {
          /// nes_log_printf("VRAM write to $%04X, scanline %d\n", ppu.vaddr,
          /// nes_getcontextptr()->scanline);
          PPU_MEM(ppu.vaddr) = 0xFF;  // corrupt
        } else {
          uint32_t addr = ppu.vaddr;
          if (false == ppu.vram_present && addr >= 0x3000) ppu.vaddr -= 0x1000;
          PPU_MEM(addr) = value;
        }
      } else {
        if (0 == (ppu.vaddr & 0x0F)) {
          int i;
          for (i = 0; i < 8; i++)
            ppu.palette[i << 2] = (value & 0x3F) | BG_TRANS;
        } else if (ppu.vaddr & 3) {
          ppu.palette[ppu.vaddr & 0x1F] = value & 0x3F;
        }
      }
      ppu.vaddr += ppu.vaddr_inc;
      ppu.vaddr &= 0x3FFF;
      break;
    default:
      break;
  }
}

// rendering routines
inline void draw_bgtile(uint8_t *surface, uint8_t pat1, uint8_t pat2,
                        const uint8_t *colors) {
  uint32_t pattern = ((pat2 & 0xAA) << 8) | ((pat2 & 0x55) << 1) |
                     ((pat1 & 0xAA) << 7) | (pat1 & 0x55);
  *surface++ = colors[(pattern >> 14) & 3];
  *surface++ = colors[(pattern >> 6) & 3];
  *surface++ = colors[(pattern >> 12) & 3];
  *surface++ = colors[(pattern >> 4) & 3];
  *surface++ = colors[(pattern >> 10) & 3];
  *surface++ = colors[(pattern >> 2) & 3];
  *surface++ = colors[(pattern >> 8) & 3];
  *surface = colors[pattern & 3];
}

inline int draw_oamtile(uint8_t *surface, uint8_t attrib, uint8_t pat1,
                        uint8_t pat2, const uint8_t *col_tbl,
                        bool check_strike) {
  int strike_pixel = -1;
  uint32_t color = ((pat2 & 0xAA) << 8) | ((pat2 & 0x55) << 1) |
                   ((pat1 & 0xAA) << 7) | (pat1 & 0x55);
  // sprite is not 100% transparent
  if (color) {
    uint8_t colors[8];
    // swap pixels around if our tile is flipped
    if (0 == (attrib & OAMF_HFLIP)) {
      colors[0] = (color >> 14) & 3;
      colors[1] = (color >> 6) & 3;
      colors[2] = (color >> 12) & 3;
      colors[3] = (color >> 4) & 3;
      colors[4] = (color >> 10) & 3;
      colors[5] = (color >> 2) & 3;
      colors[6] = (color >> 8) & 3;
      colors[7] = color & 3;
    } else {
      colors[7] = (color >> 14) & 3;
      colors[6] = (color >> 6) & 3;
      colors[5] = (color >> 12) & 3;
      colors[4] = (color >> 4) & 3;
      colors[3] = (color >> 10) & 3;
      colors[2] = (color >> 2) & 3;
      colors[1] = (color >> 8) & 3;
      colors[0] = color & 3;
    }

    // check for solid sprite pixel overlapping solid bg pixel
    if (check_strike) {
      if (colors[0] && BG_SOLID(surface[0]))
        strike_pixel = 0;
      else if (colors[1] && BG_SOLID(surface[1]))
        strike_pixel = 1;
      else if (colors[2] && BG_SOLID(surface[2]))
        strike_pixel = 2;
      else if (colors[3] && BG_SOLID(surface[3]))
        strike_pixel = 3;
      else if (colors[4] && BG_SOLID(surface[4]))
        strike_pixel = 4;
      else if (colors[5] && BG_SOLID(surface[5]))
        strike_pixel = 5;
      else if (colors[6] && BG_SOLID(surface[6]))
        strike_pixel = 6;
      else if (colors[7] && BG_SOLID(surface[7]))
        strike_pixel = 7;
    }

    // draw the character
    if (attrib & OAMF_BEHIND) {
      if (colors[0])
        surface[0] =
            SP_PIXEL | (BG_CLEAR(surface[0]) ? col_tbl[colors[0]] : surface[0]);
      if (colors[1])
        surface[1] =
            SP_PIXEL | (BG_CLEAR(surface[1]) ? col_tbl[colors[1]] : surface[1]);
      if (colors[2])
        surface[2] =
            SP_PIXEL | (BG_CLEAR(surface[2]) ? col_tbl[colors[2]] : surface[2]);
      if (colors[3])
        surface[3] =
            SP_PIXEL | (BG_CLEAR(surface[3]) ? col_tbl[colors[3]] : surface[3]);
      if (colors[4])
        surface[4] =
            SP_PIXEL | (BG_CLEAR(surface[4]) ? col_tbl[colors[4]] : surface[4]);
      if (colors[5])
        surface[5] =
            SP_PIXEL | (BG_CLEAR(surface[5]) ? col_tbl[colors[5]] : surface[5]);
      if (colors[6])
        surface[6] =
            SP_PIXEL | (BG_CLEAR(surface[6]) ? col_tbl[colors[6]] : surface[6]);
      if (colors[7])
        surface[7] =
            SP_PIXEL | (BG_CLEAR(surface[7]) ? col_tbl[colors[7]] : surface[7]);
    } else {
      if (colors[0] && SP_CLEAR(surface[0]))
        surface[0] = SP_PIXEL | col_tbl[colors[0]];
      if (colors[1] && SP_CLEAR(surface[1]))
        surface[1] = SP_PIXEL | col_tbl[colors[1]];
      if (colors[2] && SP_CLEAR(surface[2]))
        surface[2] = SP_PIXEL | col_tbl[colors[2]];
      if (colors[3] && SP_CLEAR(surface[3]))
        surface[3] = SP_PIXEL | col_tbl[colors[3]];
      if (colors[4] && SP_CLEAR(surface[4]))
        surface[4] = SP_PIXEL | col_tbl[colors[4]];
      if (colors[5] && SP_CLEAR(surface[5]))
        surface[5] = SP_PIXEL | col_tbl[colors[5]];
      if (colors[6] && SP_CLEAR(surface[6]))
        surface[6] = SP_PIXEL | col_tbl[colors[6]];
      if (colors[7] && SP_CLEAR(surface[7]))
        surface[7] = SP_PIXEL | col_tbl[colors[7]];
    }
  }
  return strike_pixel;
}

void ppu_renderbg(uint8_t *vidbuf) {
  uint8_t *bmp_ptr, *data_ptr, *tile_ptr, *attrib_ptr;
  uint32_t refresh_vaddr, bg_offset, attrib_base;
  int tile_count;
  uint8_t tile_index, x_tile, y_tile;
  uint8_t col_high, attrib, attrib_shift;

  // draw a line of transparent background color if bg is disabled
  if (false == ppu.bg_on) {
    memset(vidbuf, FULLBG, NES_SCREEN_WIDTH);
    return;
  }

  bmp_ptr = vidbuf - ppu.tile_xofs;               // scroll x
  refresh_vaddr = 0x2000 + (ppu.vaddr & 0x0FE0);  // mask out x tile
  x_tile = ppu.vaddr & 0x1F;
  y_tile = (ppu.vaddr >> 5) & 0x1F;  // to simplify calculations
  bg_offset = ((ppu.vaddr >> 12) & 7) + ppu.bg_base;  // offset in y tile

  // calculate initial values
  tile_ptr = &PPU_MEM(refresh_vaddr + x_tile);  // pointer to tile index
  attrib_base = (refresh_vaddr & 0x2C00) + 0x3C0 + ((y_tile & 0x1C) << 1);
  attrib_ptr = &PPU_MEM(attrib_base + (x_tile >> 2));
  attrib = *attrib_ptr++;
  attrib_shift = (x_tile & 2) + ((y_tile & 2) << 1);
  col_high = ((attrib >> attrib_shift) & 3) << 2;
  // ppu fetches 33 tiles
  tile_count = 33;
  while (tile_count--) {
    // Tile number from nametable
    tile_index = *tile_ptr++;
    data_ptr = &PPU_MEM(bg_offset + (tile_index << 4));
    // Handle $FD/$FE tile VROM switching (PunchOut)
    if (ppu.latchfunc) ppu.latchfunc(ppu.bg_base, tile_index);

    if (tile_count > 1)
      draw_bgtile(bmp_ptr, data_ptr[0], data_ptr[8], ppu.palette + col_high);

    bmp_ptr += 8;

    x_tile++;
    if (0 == (x_tile & 1)) {    // check every 2 tiles
      if (0 == (x_tile & 3)) {  // check every 4 tiles
        if (32 == x_tile) {     // check every 32 tiles
          x_tile = 0;
          refresh_vaddr ^= (1 << 10);  // switch nametable
          attrib_base ^= (1 << 10);
          // recalculate pointers
          tile_ptr = &PPU_MEM(refresh_vaddr);
          attrib_ptr = &PPU_MEM(attrib_base);
        }
        // Get the attribute byte
        attrib = *attrib_ptr++;
      }
      attrib_shift ^= 2;
      col_high = ((attrib >> attrib_shift) & 3) << 2;
    }
  }
  // Blank left hand column if need be
  if (ppu.bg_mask) {
    uint32_t *buf_ptr = (uint32_t *)vidbuf;
    uint32_t bg_clear = FULLBG | FULLBG << 8 | FULLBG << 16 | FULLBG << 24;
    ((uint32_t *)buf_ptr)[0] = bg_clear;
    ((uint32_t *)buf_ptr)[1] = bg_clear;
  }
}

// OAM entry
typedef struct obj_s {
  uint8_t y_loc;
  uint8_t tile;
  uint8_t atr;
  uint8_t x_loc;
} obj_t;

// TODO: fetch valid OAM a scanline before, like the Real Thing
void ppu_renderoam(uint8_t *vidbuf, int scanline) {
  uint8_t *buf_ptr;
  uint32_t vram_offset, savecol[2] = {0};
  int sprite_num, spritecount;
  obj_t *sprite_ptr;
  uint8_t sprite_height;
  if (false == ppu.obj_on) return;
  // Get our buffer pointer
  buf_ptr = vidbuf;
  // Save left hand column?
  if (ppu.obj_mask) {
    savecol[0] = ((uint32_t *)buf_ptr)[0];
    savecol[1] = ((uint32_t *)buf_ptr)[1];
  }
  sprite_height = ppu.obj_height;
  vram_offset = ppu.obj_base;
  spritecount = 0;
  sprite_ptr = (obj_t *)ppu.oam;
  for (sprite_num = 0; sprite_num < 64; sprite_num++, sprite_ptr++) {
    uint8_t *data_ptr, *bmp_ptr;
    uint32_t vram_adr;
    int y_offset;
    uint8_t tile_index, attrib, col_high;
    uint8_t sprite_y, sprite_x;
    bool check_strike;
    int strike_pixel;
    sprite_y = sprite_ptr->y_loc + 1;
    // Check to see if sprite is out of range
    if ((sprite_y > scanline) || (sprite_y <= (scanline - sprite_height)) ||
        (0 == sprite_y) || (sprite_y >= 240))
      continue;
    sprite_x = sprite_ptr->x_loc;
    tile_index = sprite_ptr->tile;
    attrib = sprite_ptr->atr;
    bmp_ptr = buf_ptr + sprite_x;
    // Handle $FD/$FE tile VROM switching (PunchOut)
    if (ppu.latchfunc) ppu.latchfunc(vram_offset, tile_index);
    // Get upper two bits of color
    col_high = ((attrib & 3) << 2);
    // 8x16 even sprites use $0000, odd use $1000
    if (16 == ppu.obj_height)
      vram_adr = ((tile_index & 1) << 12) | ((tile_index & 0xFE) << 4);
    else
      vram_adr = vram_offset + (tile_index << 4);
    // Get the address of the tile
    data_ptr = &PPU_MEM(vram_adr);
    // Calculate offset (line within the sprite)
    y_offset = scanline - sprite_y;
    if (y_offset > 7) y_offset += 8;
    // Account for vertical flippage
    if (attrib & OAMF_VFLIP) {
      if (16 == ppu.obj_height)
        y_offset -= 23;
      else
        y_offset -= 7;
      data_ptr -= y_offset;
    } else {
      data_ptr += y_offset;
    }

    // if we're on sprite 0 and sprite 0 strike flag isn't set, check for a
    // strike
    check_strike = (0 == sprite_num) && (false == ppu.strikeflag);
    strike_pixel = draw_oamtile(bmp_ptr, attrib, data_ptr[0], data_ptr[8],
                                ppu.palette + 16 + col_high, check_strike);
    if (strike_pixel >= 0) ppu_setstrike(strike_pixel);

    // maximum of 8 sprites per scanline
    if (++spritecount == PPU_MAXSPRITE) {
      ppu.stat |= PPU_STATF_MAXSPRITE;
      break;
    }
  }
  // Restore lefthand column
  if (ppu.obj_mask) {
    ((uint32_t *)buf_ptr)[0] = savecol[0];
    ((uint32_t *)buf_ptr)[1] = savecol[1];
  }
}

// Fake rendering a line - This is needed for sprite 0 hits when we're skipping
// drawing a frame
void ppu_fakeoam(int scanline) {
  uint8_t *data_ptr;
  obj_t *sprite_ptr;
  uint32_t vram_adr, color;
  int y_offset;
  uint8_t pat1, pat2;
  uint8_t tile_index, attrib;
  uint8_t sprite_height, sprite_y, sprite_x;

  // we don't need to be here if strike flag is set
  if (false == ppu.obj_on || ppu.strikeflag) return;

  sprite_height = ppu.obj_height;
  sprite_ptr = (obj_t *)ppu.oam;
  sprite_y = sprite_ptr->y_loc + 1;
  // Check to see if sprite is out of range
  if ((sprite_y > scanline) || (sprite_y <= (scanline - sprite_height)) ||
      (0 == sprite_y) || (sprite_y > 240))
    return;
  sprite_x = sprite_ptr->x_loc;
  tile_index = sprite_ptr->tile;
  attrib = sprite_ptr->atr;
  // 8x16 even sprites use $0000, odd use $1000
  if (16 == ppu.obj_height)
    vram_adr = ((tile_index & 1) << 12) | ((tile_index & 0xFE) << 4);
  else
    vram_adr = ppu.obj_base + (tile_index << 4);
  data_ptr = &PPU_MEM(vram_adr);
  // Calculate offset (line within the sprite)
  y_offset = scanline - sprite_y;
  if (y_offset > 7) y_offset += 8;

  // Account for vertical flippage
  if (attrib & OAMF_VFLIP) {
    if (16 == ppu.obj_height)
      y_offset -= 23;
    else
      y_offset -= 7;
    data_ptr -= y_offset;
  } else {
    data_ptr += y_offset;
  }

  // check for a solid sprite 0 pixel
  pat1 = data_ptr[0];
  pat2 = data_ptr[8];
  color = ((pat2 & 0xAA) << 8) | ((pat2 & 0x55) << 1) | ((pat1 & 0xAA) << 7) |
          (pat1 & 0x55);

  if (color) {
    uint8_t colors[8];

    // buckle up, it's going to get ugly...
    if (0 == (attrib & OAMF_HFLIP)) {
      colors[0] = (color >> 14) & 3;
      colors[1] = (color >> 6) & 3;
      colors[2] = (color >> 12) & 3;
      colors[3] = (color >> 4) & 3;
      colors[4] = (color >> 10) & 3;
      colors[5] = (color >> 2) & 3;
      colors[6] = (color >> 8) & 3;
      colors[7] = color & 3;
    } else {
      colors[7] = (color >> 14) & 3;
      colors[6] = (color >> 6) & 3;
      colors[5] = (color >> 12) & 3;
      colors[4] = (color >> 4) & 3;
      colors[3] = (color >> 10) & 3;
      colors[2] = (color >> 2) & 3;
      colors[1] = (color >> 8) & 3;
      colors[0] = color & 3;
    }

    if (colors[0])
      ppu_setstrike(sprite_x + 0);
    else if (colors[1])
      ppu_setstrike(sprite_x + 1);
    else if (colors[2])
      ppu_setstrike(sprite_x + 2);
    else if (colors[3])
      ppu_setstrike(sprite_x + 3);
    else if (colors[4])
      ppu_setstrike(sprite_x + 4);
    else if (colors[5])
      ppu_setstrike(sprite_x + 5);
    else if (colors[6])
      ppu_setstrike(sprite_x + 6);
    else if (colors[7])
      ppu_setstrike(sprite_x + 7);
  }
}

bool ppu_enabled(void) { return (ppu.bg_on || ppu.obj_on); }

void ppu_renderscanline(int scanline, bool draw_flag) {
  uint8_t *buf = screenMemory[scanline];
  // start scanline - transfer ppu latch into vaddr
  if (ppu.bg_on || ppu.obj_on) {
    if (0 == scanline) {
      ppu.vaddr = ppu.vaddr_latch;
    } else {
      ppu.vaddr &= ~0x041F;
      ppu.vaddr |= (ppu.vaddr_latch & 0x041F);
    }
  }

  if (draw_flag) ppu_renderbg(buf);

  // TODO: fetch obj data 1 scanline before
  if (true == ppu.drawsprites && true == draw_flag)
    ppu_renderoam(buf, scanline);
  else
    ppu_fakeoam(scanline);
}

void ppu_endscanline(int scanline) {
  // modify vram address at end of scanline
  if (scanline < 240 && (ppu.bg_on || ppu.obj_on)) {
    int ytile;

    // check for max 3 bit y tile offset
    if (7 == (ppu.vaddr >> 12)) {
      ppu.vaddr &= ~0x7000;  // clear y tile offset
      ytile = (ppu.vaddr >> 5) & 0x1F;

      if (29 == ytile) {
        ppu.vaddr &= ~0x03E0;  // clear y tile
        ppu.vaddr ^= 0x0800;   // toggle nametable
      } else if (31 == ytile) {
        ppu.vaddr &= ~0x03E0;  // clear y tile
      } else {
        ppu.vaddr += 0x20;  // increment y tile
      }
    } else {
      ppu.vaddr += 0x1000;  // increment tile y offset
    }
  }
}

void ppu_checknmi(void) {
  if (ppu.ctrl0 & PPU_CTRL0F_NMI) nes_nmi();
}

void ppu_scanline(int scanline, bool draw_flag) {
  if (scanline < 240) {
    // Lower the Max Sprite per scanline flag
    ppu.stat &= ~PPU_STATF_MAXSPRITE;
    ppu_renderscanline(scanline, draw_flag);
  } else if (241 == scanline) {
    ppu.stat |= PPU_STATF_VBLANK;
    ppu.vram_accessible = true;
  } else if (261 == scanline) {
    ppu.stat &= ~PPU_STATF_VBLANK;
    ppu.strikeflag = false;
    ppu.strike_cycle = (uint32_t)-1;
    ppu.vram_accessible = false;
  }

  nescreen::update();
}

void ppu_destroy(ppu_t **src_ppu) {
  if (*src_ppu) {
    free(*src_ppu);
    *src_ppu = NULL;
  }
}

//--------------------------------------------------------------------------------
// APU.C
#define APU_OVERSAMPLE
#define APU_VOLUME_DECAY(x) ((x) -= ((x) >> 7))

#define APU_RECTANGLE_OUTPUT(channel) (apu.rectangle[channel].output_vol)
#define APU_TRIANGLE_OUTPUT \
  (apu.triangle.output_vol + (apu.triangle.output_vol >> 2))
#define APU_NOISE_OUTPUT \
  ((apu.noise.output_vol + apu.noise.output_vol + apu.noise.output_vol) >> 2)
#define APU_DMC_OUTPUT \
  ((apu.dmc.output_vol + apu.dmc.output_vol + apu.dmc.output_vol) >> 2)

// look up table madness
int32_t decay_lut[16];
int vbl_lut[32];
int trilength_lut[128];

// noise lookups for both modes
#ifndef REALTIME_NOISE
int8_t noise_long_lut[APU_NOISE_32K];
int8_t noise_short_lut[APU_NOISE_93];
#endif  // !REALTIME_NOISE

// vblank length table used for rectangles, triangle, noise
const uint8_t vbl_length[32] = {5,  127, 10, 1,  19, 2,  40, 3,  80, 4,  30,
                                5,  7,   6,  13, 7,  6,  8,  12, 9,  24, 10,
                                48, 11,  96, 12, 36, 13, 8,  14, 16, 15};

// frequency limit of rectangle channels
const int freq_limit[8] = {0x3FF, 0x555, 0x666, 0x71C,
                           0x787, 0x7C1, 0x7E0, 0x7F0};

// noise frequency lookup table
const int noise_freq[16] = {4,   8,   16,  32,  64,  96,   128,  160,
                            202, 254, 380, 508, 762, 1016, 2034, 4068};

// DMC transfer freqs
const int dmc_clocks[16] = {428, 380, 340, 320, 286, 254, 226, 214,
                            190, 160, 142, 128, 106, 85,  72,  54};

// ratios of pos/neg pulse for rectangle waves
const int duty_flip[4] = {2, 4, 8, 12};

void apu_setcontext(apu_t *src_apu) { apu = *src_apu; }

void apu_getcontext(apu_t *dest_apu) { *dest_apu = apu; }

void apu_setchan(int chan, bool enabled) {
  if (enabled)
    apu.mix_enable |= (1 << chan);
  else
    apu.mix_enable &= ~(1 << chan);
}

// emulation of the 15-bit shift register the
// NES uses to generate pseudo-random series
// for the white noise channel
#ifdef REALTIME_NOISE
int8_t shift_register15(uint8_t xor_tap) {
  int sreg = 0x4000;
  int bit0, tap, bit14;

  bit0 = sreg & 1;
  tap = (sreg & xor_tap) ? 1 : 0;
  bit14 = (bit0 ^ tap);
  sreg >>= 1;
  sreg |= (bit14 << 14);
  return (bit0 ^ 1);
}
#else   // !REALTIME_NOISE
void shift_register15(int8_t *buf, int count) {
  int sreg = 0x4000;
  int bit0, bit1, bit6, bit14;

  if (count == APU_NOISE_93) {
    while (count--) {
      bit0 = sreg & 1;
      bit6 = (sreg & 0x40) >> 6;
      bit14 = (bit0 ^ bit6);
      sreg >>= 1;
      sreg |= (bit14 << 14);
      *buf++ = bit0 ^ 1;
    }
  } else {  // 32K noise
    while (count--) {
      bit0 = sreg & 1;
      bit1 = (sreg & 2) >> 1;
      bit14 = (bit0 ^ bit1);
      sreg >>= 1;
      sreg |= (bit14 << 14);
      *buf++ = bit0 ^ 1;
    }
  }
}
#endif  // !REALTIME_NOISE

// RECTANGLE WAVE
// ==============
// reg0: 0-3=volume, 4=envelope, 5=hold, 6-7=duty cycle
// reg1: 0-2=sweep shifts, 3=sweep inc/dec, 4-6=sweep length, 7=sweep on
// reg2: 8 bits of freq
// reg3: 0-2=high freq, 7-4=vbl length counter
//
#ifdef APU_OVERSAMPLE

#define APU_MAKE_RECTANGLE(ch)                                               \
  int32_t apu_rectangle_##ch(void) {                                         \
    int32_t output, total;                                                   \
    int num_times;                                                           \
                                                                             \
    APU_VOLUME_DECAY(apu.rectangle[ch].output_vol);                          \
                                                                             \
    if (false == apu.rectangle[ch].enabled ||                                \
        0 == apu.rectangle[ch].vbl_length)                                   \
      return APU_RECTANGLE_OUTPUT(ch);                                       \
                                                                             \
    if (false == apu.rectangle[ch].holdnote) apu.rectangle[ch].vbl_length--; \
                                                                             \
    apu.rectangle[ch].env_phase -= 4;                                        \
    while (apu.rectangle[ch].env_phase < 0) {                                \
      apu.rectangle[ch].env_phase += apu.rectangle[ch].env_delay;            \
                                                                             \
      if (apu.rectangle[ch].holdnote)                                        \
        apu.rectangle[ch].env_vol = (apu.rectangle[ch].env_vol + 1) & 0x0F;  \
      else if (apu.rectangle[ch].env_vol < 0x0F)                             \
        apu.rectangle[ch].env_vol++;                                         \
    }                                                                        \
                                                                             \
    if (apu.rectangle[ch].freq < 8 ||                                        \
        (false == apu.rectangle[ch].sweep_inc &&                             \
         apu.rectangle[ch].freq > apu.rectangle[ch].freq_limit))             \
      return APU_RECTANGLE_OUTPUT(ch);                                       \
                                                                             \
    if (apu.rectangle[ch].sweep_on && apu.rectangle[ch].sweep_shifts) {      \
      apu.rectangle[ch].sweep_phase -= 2;                                    \
      while (apu.rectangle[ch].sweep_phase < 0) {                            \
        apu.rectangle[ch].sweep_phase += apu.rectangle[ch].sweep_delay;      \
                                                                             \
        if (apu.rectangle[ch].sweep_inc) {                                   \
          if (0 == ch)                                                       \
            apu.rectangle[ch].freq +=                                        \
                ~(apu.rectangle[ch].freq >> apu.rectangle[ch].sweep_shifts); \
          else                                                               \
            apu.rectangle[ch].freq -=                                        \
                (apu.rectangle[ch].freq >> apu.rectangle[ch].sweep_shifts);  \
        } else {                                                             \
          apu.rectangle[ch].freq +=                                          \
              (apu.rectangle[ch].freq >> apu.rectangle[ch].sweep_shifts);    \
        }                                                                    \
      }                                                                      \
    }                                                                        \
                                                                             \
    apu.rectangle[ch].accum -= apu.cycle_rate;                               \
    if (apu.rectangle[ch].accum >= 0) return APU_RECTANGLE_OUTPUT(ch);       \
                                                                             \
    if (apu.rectangle[ch].fixed_envelope)                                    \
      output = apu.rectangle[ch].volume << 8;                                \
    else                                                                     \
      output = (apu.rectangle[ch].env_vol ^ 0x0F) << 8;                      \
                                                                             \
    num_times = total = 0;                                                   \
                                                                             \
    while (apu.rectangle[ch].accum < 0) {                                    \
      apu.rectangle[ch].accum += apu.rectangle[ch].freq + 1;                 \
      apu.rectangle[ch].adder = (apu.rectangle[ch].adder + 1) & 0x0F;        \
                                                                             \
      if (apu.rectangle[ch].adder < apu.rectangle[ch].duty_flip)             \
        total += output;                                                     \
      else                                                                   \
        total -= output;                                                     \
                                                                             \
      num_times++;                                                           \
    }                                                                        \
                                                                             \
    apu.rectangle[ch].output_vol = total / num_times;                        \
    return APU_RECTANGLE_OUTPUT(ch);                                         \
  }

#else  // !APU_OVERSAMPLE
#define APU_MAKE_RECTANGLE(ch)                                               \
  int32_t apu_rectangle_##ch(void) {                                         \
    int32_t output;                                                          \
                                                                             \
    APU_VOLUME_DECAY(apu.rectangle[ch].output_vol);                          \
                                                                             \
    if (false == apu.rectangle[ch].enabled ||                                \
        0 == apu.rectangle[ch].vbl_length)                                   \
      return APU_RECTANGLE_OUTPUT(ch);                                       \
                                                                             \
    if (false == apu.rectangle[ch].holdnote) apu.rectangle[ch].vbl_length--; \
                                                                             \
    apu.rectangle[ch].env_phase -= 4;                                        \
    while (apu.rectangle[ch].env_phase < 0) {                                \
      apu.rectangle[ch].env_phase += apu.rectangle[ch].env_delay;            \
                                                                             \
      if (apu.rectangle[ch].holdnote)                                        \
        apu.rectangle[ch].env_vol = (apu.rectangle[ch].env_vol + 1) & 0x0F;  \
      else if (apu.rectangle[ch].env_vol < 0x0F)                             \
        apu.rectangle[ch].env_vol++;                                         \
    }                                                                        \
                                                                             \
    if (apu.rectangle[ch].freq < 8 ||                                        \
        (false == apu.rectangle[ch].sweep_inc &&                             \
         apu.rectangle[ch].freq > apu.rectangle[ch].freq_limit))             \
      return APU_RECTANGLE_OUTPUT(ch);                                       \
                                                                             \
    if (apu.rectangle[ch].sweep_on && apu.rectangle[ch].sweep_shifts) {      \
      apu.rectangle[ch].sweep_phase -= 2;                                    \
      while (apu.rectangle[ch].sweep_phase < 0) {                            \
        apu.rectangle[ch].sweep_phase += apu.rectangle[ch].sweep_delay;      \
                                                                             \
        if (apu.rectangle[ch].sweep_inc) {                                   \
          if (0 == ch)                                                       \
            apu.rectangle[ch].freq +=                                        \
                ~(apu.rectangle[ch].freq >> apu.rectangle[ch].sweep_shifts); \
          else                                                               \
            apu.rectangle[ch].freq -=                                        \
                (apu.rectangle[ch].freq >> apu.rectangle[ch].sweep_shifts);  \
        } else {                                                             \
          apu.rectangle[ch].freq +=                                          \
              (apu.rectangle[ch].freq >> apu.rectangle[ch].sweep_shifts);    \
        }                                                                    \
      }                                                                      \
    }                                                                        \
                                                                             \
    apu.rectangle[ch].accum -= apu.cycle_rate;                               \
    if (apu.rectangle[ch].accum >= 0) return APU_RECTANGLE_OUTPUT(ch);       \
                                                                             \
    while (apu.rectangle[ch].accum < 0) {                                    \
      apu.rectangle[ch].accum += (apu.rectangle[ch].freq + 1);               \
      apu.rectangle[ch].adder = (apu.rectangle[ch].adder + 1) & 0x0F;        \
    }                                                                        \
                                                                             \
    if (apu.rectangle[ch].fixed_envelope)                                    \
      output = apu.rectangle[ch].volume << 8;                                \
    else                                                                     \
      output = (apu.rectangle[ch].env_vol ^ 0x0F) << 8;                      \
                                                                             \
    if (0 == apu.rectangle[ch].adder)                                        \
      apu.rectangle[ch].output_vol = output;                                 \
    else if (apu.rectangle[ch].adder == apu.rectangle[ch].duty_flip)         \
      apu.rectangle[ch].output_vol = -output;                                \
                                                                             \
    return APU_RECTANGLE_OUTPUT(ch);                                         \
  }

#endif  // !APU_OVERSAMPLE

// generate the functions
APU_MAKE_RECTANGLE(0)
APU_MAKE_RECTANGLE(1)

// TRIANGLE WAVE
// =============
// reg0: 7=holdnote, 6-0=linear length counter
// reg2: low 8 bits of frequency
// reg3: 7-3=length counter, 2-0=high 3 bits of frequency
//
int32_t apu_triangle(void) {
  APU_VOLUME_DECAY(apu.triangle.output_vol);

  if (false == apu.triangle.enabled || 0 == apu.triangle.vbl_length)
    return APU_TRIANGLE_OUTPUT;

  if (apu.triangle.counter_started) {
    if (apu.triangle.linear_length > 0) apu.triangle.linear_length--;
    if (apu.triangle.vbl_length && false == apu.triangle.holdnote)
      apu.triangle.vbl_length--;
  } else if (false == apu.triangle.holdnote && apu.triangle.write_latency) {
    if (--apu.triangle.write_latency == 0) apu.triangle.counter_started = true;
  }

  if (0 == apu.triangle.linear_length || apu.triangle.freq < 4)  // inaudible
    return APU_TRIANGLE_OUTPUT;

  apu.triangle.accum -= apu.cycle_rate;
  while (apu.triangle.accum < 0) {
    apu.triangle.accum += apu.triangle.freq;
    apu.triangle.adder = (apu.triangle.adder + 1) & 0x1F;

    if (apu.triangle.adder & 0x10)
      apu.triangle.output_vol -= (2 << 8);
    else
      apu.triangle.output_vol += (2 << 8);
  }

  return APU_TRIANGLE_OUTPUT;
}

// WHITE NOISE CHANNEL
// ===================
// reg0: 0-3=volume, 4=envelope, 5=hold
// reg2: 7=small(93 byte) sample,3-0=freq lookup
// reg3: 7-4=vbl length counter
//
// TODO: AAAAAAAAAAAAAAAAAAAAAAAA!  #ifdef MADNESS!

int32_t apu_noise(void) {
  int32_t outvol;

#if defined(APU_OVERSAMPLE) && defined(REALTIME_NOISE)
#else   // !(APU_OVERSAMPLE && REALTIME_NOISE)
  int32_t noise_bit;
#endif  // !(APU_OVERSAMPLE && REALTIME_NOISE)
#ifdef APU_OVERSAMPLE
  int num_times;
  int32_t total;
#endif  // APU_OVERSAMPLE

  APU_VOLUME_DECAY(apu.noise.output_vol);

  if (false == apu.noise.enabled || 0 == apu.noise.vbl_length)
    return APU_NOISE_OUTPUT;

  // vbl length counter
  if (false == apu.noise.holdnote) apu.noise.vbl_length--;

  // envelope decay at a rate of (env_delay + 1) / 240 secs
  apu.noise.env_phase -= 4;  // 240/60
  while (apu.noise.env_phase < 0) {
    apu.noise.env_phase += apu.noise.env_delay;

    if (apu.noise.holdnote)
      apu.noise.env_vol = (apu.noise.env_vol + 1) & 0x0F;
    else if (apu.noise.env_vol < 0x0F)
      apu.noise.env_vol++;
  }

  apu.noise.accum -= apu.cycle_rate;
  if (apu.noise.accum >= 0) return APU_NOISE_OUTPUT;

#ifdef APU_OVERSAMPLE
  if (apu.noise.fixed_envelope)
    outvol = apu.noise.volume << 8;  // fixed volume
  else
    outvol = (apu.noise.env_vol ^ 0x0F) << 8;

  num_times = total = 0;
#endif  // APU_OVERSAMPLE

  while (apu.noise.accum < 0) {
    apu.noise.accum += apu.noise.freq;

#ifdef REALTIME_NOISE

#ifdef APU_OVERSAMPLE
    if (shift_register15(apu.noise.xor_tap))
      total += outvol;
    else
      total -= outvol;

    num_times++;
#else   // !APU_OVERSAMPLE
    noise_bit = shift_register15(apu.noise.xor_tap);
#endif  // !APU_OVERSAMPLE

#else  // !REALTIME_NOISE
    apu.noise.cur_pos++;

    if (apu.noise.short_sample) {
      if (APU_NOISE_93 == apu.noise.cur_pos) apu.noise.cur_pos = 0;
    } else {
      if (APU_NOISE_32K == apu.noise.cur_pos) apu.noise.cur_pos = 0;
    }

#ifdef APU_OVERSAMPLE
    if (apu.noise.short_sample)
      noise_bit = noise_short_lut[apu.noise.cur_pos];
    else
      noise_bit = noise_long_lut[apu.noise.cur_pos];

    if (noise_bit)
      total += outvol;
    else
      total -= outvol;

    num_times++;
#endif  // APU_OVERSAMPLE
#endif  // !REALTIME_NOISE
  }

#ifdef APU_OVERSAMPLE
  apu.noise.output_vol = total / num_times;
#else  // !APU_OVERSAMPLE
  if (apu.noise.fixed_envelope)
    outvol = apu.noise.volume << 8;  // fixed volume
  else
    outvol = (apu.noise.env_vol ^ 0x0F) << 8;

#ifndef REALTIME_NOISE
  if (apu.noise.short_sample)
    noise_bit = noise_short_lut[apu.noise.cur_pos];
  else
    noise_bit = noise_long_lut[apu.noise.cur_pos];
#endif  // !REALTIME_NOISE

  if (noise_bit)
    apu.noise.output_vol = outvol;
  else
    apu.noise.output_vol = -outvol;
#endif  // !APU_OVERSAMPLE
  return APU_NOISE_OUTPUT;
}

inline void apu_dmcreload(void) {
  apu.dmc.address = apu.dmc.cached_addr;
  apu.dmc.dma_length = apu.dmc.cached_dmalength;
  apu.dmc.irq_occurred = false;
}

// DELTA MODULATION CHANNEL
// =========================
// reg0: 7=irq gen, 6=looping, 3-0=pointer to clock table
// reg1: output dc level, 6 bits unsigned
// reg2: 8 bits of 64-byte aligned address offset : $C000 + (value * 64)
// reg3: length, (value * 16) + 1
//
static int32_t apu_dmc(void) {
  int delta_bit;

  APU_VOLUME_DECAY(apu.dmc.output_vol);

  // only process when channel is alive
  if (apu.dmc.dma_length) {
    apu.dmc.accum -= apu.cycle_rate;

    while (apu.dmc.accum < 0) {
      apu.dmc.accum += apu.dmc.freq;

      delta_bit = (apu.dmc.dma_length & 7) ^ 7;

      if (7 == delta_bit) {
        apu.dmc.cur_byte = nes6502_getbyte(apu.dmc.address);

        // steal a cycle from CPU
        nes6502_burn(1);

        // prevent wraparound
        if (0xFFFF == apu.dmc.address)
          apu.dmc.address = 0x8000;
        else
          apu.dmc.address++;
      }

      if (--apu.dmc.dma_length == 0) {
        // if loop bit set, we're cool to retrigger sample
        if (apu.dmc.looping) {
          apu_dmcreload();
        } else {
          // check to see if we should generate an irq
          if (apu.dmc.irq_gen) {
            apu.dmc.irq_occurred = true;
            if (apu.irq_callback) apu.irq_callback();
          }

          // bodge for timestamp queue
          apu.dmc.enabled = false;
          break;
        }
      }

      // positive delta
      if (apu.dmc.cur_byte & (1 << delta_bit)) {
        if (apu.dmc.regs[1] < 0x7D) {
          apu.dmc.regs[1] += 2;
          apu.dmc.output_vol += (2 << 8);
        }
      } else {  // negative delta
        if (apu.dmc.regs[1] > 1) {
          apu.dmc.regs[1] -= 2;
          apu.dmc.output_vol -= (2 << 8);
        }
      }
    }
  }
  return APU_DMC_OUTPUT;
}

void apu_write(uint32_t address, uint8_t value) {
  if (SOUND_ENABLED) {
    int chan;
    switch (address) {
      // rectangles
      case APU_WRA0:
      case APU_WRB0:
        chan = (address & 4) >> 2;
        apu.rectangle[chan].regs[0] = value;
        apu.rectangle[chan].volume = value & 0x0F;
        apu.rectangle[chan].env_delay = decay_lut[value & 0x0F];
        apu.rectangle[chan].holdnote = (value & 0x20) ? true : false;
        apu.rectangle[chan].fixed_envelope = (value & 0x10) ? true : false;
        apu.rectangle[chan].duty_flip = duty_flip[value >> 6];
        break;
      case APU_WRA1:
      case APU_WRB1:
        chan = (address & 4) >> 2;
        apu.rectangle[chan].regs[1] = value;
        apu.rectangle[chan].sweep_on = (value & 0x80) ? true : false;
        apu.rectangle[chan].sweep_shifts = value & 7;
        apu.rectangle[chan].sweep_delay = decay_lut[(value >> 4) & 7];
        apu.rectangle[chan].sweep_inc = (value & 0x08) ? true : false;
        apu.rectangle[chan].freq_limit = freq_limit[value & 7];
        break;
      case APU_WRA2:
      case APU_WRB2:
        chan = (address & 4) >> 2;
        apu.rectangle[chan].regs[2] = value;
        apu.rectangle[chan].freq = (apu.rectangle[chan].freq & ~0xFF) | value;
        break;
      case APU_WRA3:
      case APU_WRB3:
        chan = (address & 4) >> 2;
        apu.rectangle[chan].regs[3] = value;
        apu.rectangle[chan].vbl_length = vbl_lut[value >> 3];
        apu.rectangle[chan].env_vol = 0;
        apu.rectangle[chan].freq =
            ((value & 7) << 8) | (apu.rectangle[chan].freq & 0xFF);
        apu.rectangle[chan].adder = 0;
        break;
      // triangle
      case APU_WRC0:
        apu.triangle.regs[0] = value;
        apu.triangle.holdnote = (value & 0x80) ? true : false;

        if (false == apu.triangle.counter_started && apu.triangle.vbl_length)
          apu.triangle.linear_length = trilength_lut[value & 0x7F];
        break;
      case APU_WRC2:
        apu.triangle.regs[1] = value;
        apu.triangle.freq = (((apu.triangle.regs[2] & 7) << 8) + value) + 1;
        break;
      case APU_WRC3:
        apu.triangle.regs[2] = value;
        // this is somewhat of a hack.  there appears to be some latency on
        // the Real Thing between when trireg0 is written to and when the
        // linear length counter actually begins its countdown.  we want to
        // prevent the case where the program writes to the freq regs first,
        // then to reg 0, and the counter accidentally starts running because
        // of the sound queue's timestamp processing.
        //
        // set latency to a couple hundred cycles -- should be plenty of time
        // for the 6502 code to do a couple of table dereferences and load up
        // the other triregs

        apu.triangle.write_latency = (int)(228 / apu.cycle_rate);
        apu.triangle.freq = (((value & 7) << 8) + apu.triangle.regs[1]) + 1;
        apu.triangle.vbl_length = vbl_lut[value >> 3];
        apu.triangle.counter_started = false;
        apu.triangle.linear_length = trilength_lut[apu.triangle.regs[0] & 0x7F];
        break;
      // noise
      case APU_WRD0:
        apu.noise.regs[0] = value;
        apu.noise.env_delay = decay_lut[value & 0x0F];
        apu.noise.holdnote = (value & 0x20) ? true : false;
        apu.noise.fixed_envelope = (value & 0x10) ? true : false;
        apu.noise.volume = value & 0x0F;
        break;
      case APU_WRD2:
        apu.noise.regs[1] = value;
        apu.noise.freq = noise_freq[value & 0x0F];
#ifdef REALTIME_NOISE
        apu.noise.xor_tap = (value & 0x80) ? 0x40 : 0x02;
#else   // !REALTIME_NOISE
        // detect transition from long->short sample
        if ((value & 0x80) && false == apu.noise.short_sample) {
          // recalculate short noise buffer
          shift_register15(noise_short_lut, APU_NOISE_93);
          apu.noise.cur_pos = 0;
        }
        apu.noise.short_sample = (value & 0x80) ? true : false;
#endif  // !REALTIME_NOISE
        break;
      case APU_WRD3:
        apu.noise.regs[2] = value;
        apu.noise.vbl_length = vbl_lut[value >> 3];
        apu.noise.env_vol = 0;  // reset envelope
        break;
      // DMC
      case APU_WRE0:
        apu.dmc.regs[0] = value;
        apu.dmc.freq = dmc_clocks[value & 0x0F];
        apu.dmc.looping = (value & 0x40) ? true : false;
        if (value & 0x80) {
          apu.dmc.irq_gen = true;
        } else {
          apu.dmc.irq_gen = false;
          apu.dmc.irq_occurred = false;
        }
        break;
      case APU_WRE1:  // 7-bit DAC
        // add the _delta_ between written value and
        // current output level of the volume reg
        value &= 0x7F;  // bit 7 ignored
        apu.dmc.output_vol += ((value - apu.dmc.regs[1]) << 8);
        apu.dmc.regs[1] = value;
        break;
      case APU_WRE2:
        apu.dmc.regs[2] = value;
        apu.dmc.cached_addr = 0xC000 + (uint16_t)(value << 6);
        break;
      case APU_WRE3:
        apu.dmc.regs[3] = value;
        apu.dmc.cached_dmalength = ((value << 4) + 1) << 3;
        break;
      case APU_SMASK:
        // bodge for timestamp queue
        apu.dmc.enabled = (value & 0x10) ? true : false;
        apu.enable_reg = value;

        for (chan = 0; chan < 2; chan++) {
          if (value & (1 << chan)) {
            apu.rectangle[chan].enabled = true;
          } else {
            apu.rectangle[chan].enabled = false;
            apu.rectangle[chan].vbl_length = 0;
          }
        }

        if (value & 0x04) {
          apu.triangle.enabled = true;
        } else {
          apu.triangle.enabled = false;
          apu.triangle.vbl_length = 0;
          apu.triangle.linear_length = 0;
          apu.triangle.counter_started = false;
          apu.triangle.write_latency = 0;
        }

        if (value & 0x08) {
          apu.noise.enabled = true;
        } else {
          apu.noise.enabled = false;
          apu.noise.vbl_length = 0;
        }

        if (value & 0x10) {
          if (0 == apu.dmc.dma_length) apu_dmcreload();
        } else {
          apu.dmc.dma_length = 0;
        }
        apu.dmc.irq_occurred = false;
        break;
      // unused, but they get hit in some mem-clear loops
      case 0x4009:
      case 0x400D:
        break;
      default:
        break;
    }
  }
}

// Read from $4000-$4017
uint8_t apu_read(uint32_t address) {
  uint8_t value;
  if (SOUND_ENABLED) {
    switch (address) {
      case APU_SMASK:
        value = 0;
        // Return 1 in 0-5 bit pos if a channel is playing
        if (apu.rectangle[0].enabled && apu.rectangle[0].vbl_length)
          value |= 0x01;
        if (apu.rectangle[1].enabled && apu.rectangle[1].vbl_length)
          value |= 0x02;
        if (apu.triangle.enabled && apu.triangle.vbl_length) value |= 0x04;
        if (apu.noise.enabled && apu.noise.vbl_length) value |= 0x08;

        // bodge for timestamp queue
        if (apu.dmc.enabled) value |= 0x10;
        if (apu.dmc.irq_occurred) value |= 0x80;
        if (apu.irqclear_callback) value |= apu.irqclear_callback();
        break;
      default:
        value = (address >> 8);  // heavy capacitance on data bus
        break;
    }
  }
  return value;
}

#define CLIP_OUTPUT16(out)  \
  {                         \
    if (out > 0x7FFF)       \
      out = 0x7FFF;         \
    else if (out < -0x8000) \
      out = -0x8000;        \
  }

void apu_process(void *buffer, int num_samples) {
  static int32_t prev_sample = 0;
  uint16_t *buf16;
  uint8_t *buf8;

  if (NULL != buffer) {
    // bleh
    apu.buffer = buffer;

    buf16 = (uint16_t *)buffer;
    buf8 = (uint8_t *)buffer;

    while (num_samples--) {
      int32_t next_sample, accum = 0;
      if (apu.mix_enable & 0x01) accum += apu_rectangle_0();
      if (apu.mix_enable & 0x02) accum += apu_rectangle_1();
      if (apu.mix_enable & 0x04) accum += apu_triangle();
      if (apu.mix_enable & 0x08) accum += apu_noise();
      if (apu.mix_enable & 0x10) accum += apu_dmc();
      if (apu.ext && (apu.mix_enable & 0x20)) accum += apu.ext->process();

      // do any filtering
      if (APU_FILTER_NONE != apu.filter_type) {
        next_sample = accum;

        if (APU_FILTER_LOWPASS == apu.filter_type) {
          accum += prev_sample;
          accum >>= 1;
        } else
          accum = (accum + accum + accum + prev_sample) >> 2;

        prev_sample = next_sample;
      }

      // do clipping
      CLIP_OUTPUT16(accum);

      // signed 16-bit output, unsigned 8-bit
      if (16 == apu.sample_bits)
        *buf16++ = (uint16_t)accum;
      else
        *buf8++ = (accum >> 8) ^ 0x80;
    }
  }
}

// set the filter type
void apu_setfilter(int filter_type) { apu.filter_type = filter_type; }

void apu_reset(void) {
  uint32_t address;

  // initialize all channel members
  for (address = 0x4000; address <= 0x4013; address++) apu_write(address, 0);
  apu_write(0x4015, 0);
  if (apu.ext && NULL != apu.ext->reset) apu.ext->reset();
}

void apu_build_luts(int num_samples) {
  int i;

  // lut used for enveloping and frequency sweeps
  for (i = 0; i < 16; i++) decay_lut[i] = num_samples * (i + 1);

  // used for note length, based on vblanks and size of audio buffer
  for (i = 0; i < 32; i++) vbl_lut[i] = vbl_length[i] * num_samples;

  // triangle wave channel's linear length table
  for (i = 0; i < 128; i++) trilength_lut[i] = (int)(0.25 * i * num_samples);

#ifndef REALTIME_NOISE
  // generate noise samples
  shift_register15(noise_long_lut, APU_NOISE_32K);
  shift_register15(noise_short_lut, APU_NOISE_93);
#endif  // !REALTIME_NOISE
}

void apu_setparams(double base_freq, int sample_rate, int refresh_rate,
                   int sample_bits) {
  apu.sample_rate = sample_rate;
  apu.refresh_rate = refresh_rate;
  apu.sample_bits = sample_bits;
  apu.num_samples = sample_rate / refresh_rate;
  if (0 == base_freq)
    apu.base_freq = APU_BASEFREQ;
  else
    apu.base_freq = base_freq;
  apu.cycle_rate = (float)(apu.base_freq / sample_rate);

  // build various lookup tables for apu
  apu_build_luts(apu.num_samples);

  apu_reset();
}

apu_t *temp_apu;

// Initializes emulated sound hardware, creates waveforms/voices
apu_t *apu_create(double base_freq, int sample_rate, int refresh_rate,
                  int sample_bits) {
  int channel;

  if (NULL == temp_apu) temp_apu = (apu_t *)malloc(sizeof(apu_t));
  if (NULL == temp_apu) return NULL;

  memset(temp_apu, 0, sizeof(apu_t));

  // set the update routine
  temp_apu->process = apu_process;
  temp_apu->ext = NULL;

  // clear the callbacks
  temp_apu->irq_callback = NULL;
  temp_apu->irqclear_callback = NULL;

  apu_setcontext(temp_apu);
  apu_setparams(base_freq, sample_rate, refresh_rate, sample_bits);
  for (channel = 0; channel < 6; channel++) apu_setchan(channel, true);
  apu_setfilter(APU_FILTER_WEIGHTED);
  apu_getcontext(temp_apu);
  return temp_apu;
}

void apu_destroy(apu_t **src_apu) {
  if (*src_apu) {
    ///    if ((*src_apu)->ext && NULL != (*src_apu)->ext->shutdown)
    ///    (*src_apu)->ext->shutdown();
    free(*src_apu);
    *src_apu = NULL;
  }
}

void apu_setext(apu_t *src_apu, apuext_t *ext) {
  src_apu->ext = ext;

  // initialize it
  if (src_apu->ext && NULL != src_apu->ext->init) src_apu->ext->init();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

nes_t *nes_getcontextptr(void) { return &nes; }

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
///(MMC.C) part
rominfo_t *mmc_getinfo(void) { return mmc.cart; }
void ppu_setlatchfunc(ppulatchfunc_t func) { ppu.latchfunc = func; }
void ppu_setvromswitch(ppuvromswitch_t func) { ppu.vromswitch = func; }

//--------------------------------------------------------------------------------
// MMC.C
#define MMC_8KROM (mmc.cart->rom_banks * 2)
#define MMC_16KROM (mmc.cart->rom_banks)
#define MMC_32KROM (mmc.cart->rom_banks / 2)
#define MMC_8KVROM (mmc.cart->vrom_banks)
#define MMC_4KVROM (mmc.cart->vrom_banks * 2)
#define MMC_2KVROM (mmc.cart->vrom_banks * 4)
#define MMC_1KVROM (mmc.cart->vrom_banks * 8)

#define MMC_LAST8KROM (MMC_8KROM - 1)
#define MMC_LAST16KROM (MMC_16KROM - 1)
#define MMC_LAST32KROM (MMC_32KROM - 1)
#define MMC_LAST8KVROM (MMC_8KVROM - 1)
#define MMC_LAST4KVROM (MMC_4KVROM - 1)
#define MMC_LAST2KVROM (MMC_2KVROM - 1)
#define MMC_LAST1KVROM (MMC_1KVROM - 1)

void mmc_setcontext(mmc_t *src_mmc) { mmc = *src_mmc; }

// raise an IRQ
void nes_irq(void) { nes6502_irq(); }

void mmc_getcontext(mmc_t *dest_mmc) { *dest_mmc = mmc; }

// VROM bankswitching
void mmc_bankvrom(int size, uint32_t address, int bank) {
  if (0 == mmc.cart->vrom_banks) return;

  switch (size) {
    case 1:
      if (bank == MMC_LASTBANK) bank = MMC_LAST1KVROM;
      ppu_setpage(1, address >> 10,
                  &mmc.cart->vrom[(bank % MMC_1KVROM) << 10] - address);
      break;

    case 2:
      if (bank == MMC_LASTBANK) bank = MMC_LAST2KVROM;
      ppu_setpage(2, address >> 10,
                  &mmc.cart->vrom[(bank % MMC_2KVROM) << 11] - address);
      break;

    case 4:
      if (bank == MMC_LASTBANK) bank = MMC_LAST4KVROM;
      ppu_setpage(4, address >> 10,
                  &mmc.cart->vrom[(bank % MMC_4KVROM) << 12] - address);
      break;

    case 8:
      if (bank == MMC_LASTBANK) bank = MMC_LAST8KVROM;
      ppu_setpage(8, 0, &mmc.cart->vrom[(bank % MMC_8KVROM) << 13]);
      break;

    default:
      break;
      /// nes_log_printf("invalid VROM bank size %d\n", size);
  }
}

// ROM bankswitching
void mmc_bankrom(int size, uint32_t address, int bank) {
  nes6502_context mmc_cpu;
  nes6502_getcontext(&mmc_cpu);

  switch (size) {
    case 8:
      if (bank == MMC_LASTBANK) bank = MMC_LAST8KROM;
      {
        int page = address >> NES6502_BANKSHIFT;
        mmc_cpu.mem_page[page] = &mmc.cart->rom[(bank % MMC_8KROM) << 13];
        mmc_cpu.mem_page[page + 1] = mmc_cpu.mem_page[page] + 0x1000;
      }

      break;

    case 16:
      if (bank == MMC_LASTBANK) bank = MMC_LAST16KROM;
      {
        int page = address >> NES6502_BANKSHIFT;
        mmc_cpu.mem_page[page] = &mmc.cart->rom[(bank % MMC_16KROM) << 14];
        mmc_cpu.mem_page[page + 1] = mmc_cpu.mem_page[page] + 0x1000;
        mmc_cpu.mem_page[page + 2] = mmc_cpu.mem_page[page] + 0x2000;
        mmc_cpu.mem_page[page + 3] = mmc_cpu.mem_page[page] + 0x3000;
      }
      break;

    case 32:
      if (bank == MMC_LASTBANK) bank = MMC_LAST32KROM;

      mmc_cpu.mem_page[8] = &mmc.cart->rom[(bank % MMC_32KROM) << 15];
      mmc_cpu.mem_page[9] = mmc_cpu.mem_page[8] + 0x1000;
      mmc_cpu.mem_page[10] = mmc_cpu.mem_page[8] + 0x2000;
      mmc_cpu.mem_page[11] = mmc_cpu.mem_page[8] + 0x3000;
      mmc_cpu.mem_page[12] = mmc_cpu.mem_page[8] + 0x4000;
      mmc_cpu.mem_page[13] = mmc_cpu.mem_page[8] + 0x5000;
      mmc_cpu.mem_page[14] = mmc_cpu.mem_page[8] + 0x6000;
      mmc_cpu.mem_page[15] = mmc_cpu.mem_page[8] + 0x7000;
      break;

    default:
      /// nes_log_printf("invalid ROM bank size %d\n", size);
      break;
  }

  nes6502_setcontext(&mmc_cpu);
}

#endif  // NESEMULATOR_1_H