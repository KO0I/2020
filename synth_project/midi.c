#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <soundcard.h>

#define DEVICE "/dev/midi"

  int
main ()
{
  int fd;

  unsigned char note_on[] = { 0xc0, 0,  /* Program change */
    0x90, 60, 60
  };        /* Note on */
  unsigned char note_off[] = { 0x80, 60, 60 };  /* Note off */

  if ((fd = open (DEVICE, O_WRONLY, 0)) == -1)
  {
    perror ("open " DEVICE);
    exit (-1);
  }

  if (write (fd, note_on, sizeof (note_on)) == -1)
  {
    perror ("write " DEVICE);
    exit (-1);
  }

  sleep (1);      /* Delay one second */

  if (write (fd, note_off, sizeof (note_off)) == -1)
  {
    perror ("write " DEVICE);
    exit (-1);
  }

  close (fd);
  exit (0);
}
