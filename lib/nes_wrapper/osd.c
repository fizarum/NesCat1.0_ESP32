#include "nes/osd.h"

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "nes/log.h"
#include "nes/nofconfig.h"
#include "nes/nofrendo.h"
#include "nes/noftypes.h"
#include "nes/version.h"

char configfilename[] = "na";

/* This is os-specific part of main() */
int osd_main(int argc, char *argv[]) {
  config.filename = configfilename;

  return main_loop("rom", system_autodetect);
}

/* File system interface */
void osd_fullname(char *fullname, const char *shortname) {
  strncpy(fullname, shortname, PATH_MAX);
}

/* This gives filenames for storage of saves */
char *osd_newextension(char *string, char *ext) { return string; }

/* This gives filenames for storage of PCX snapshots */
int osd_makesnapname(char *filename, int len) { return -1; }
