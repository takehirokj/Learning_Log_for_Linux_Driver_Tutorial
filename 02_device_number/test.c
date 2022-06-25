#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
  int dev = open("/dev/my_device", O_RDONLY);

  if (dev == -1) {
    printf("failed to open\n");
    return -1;
  }

  printf("succeed to open\n");
  close(dev);

  return 0;
}
