#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "triad.h"

/* An echo server: Echos any data received by a client back to the client */

static void onData(triad_Event *e) {
  triad_write(e->stream, e->data, e->size);
}

static void onAccept(triad_Event *e) {
  triad_addListener(e->remote, triad_EVENT_DATA, onData, NULL);
  triad_writef(e->remote, "echo server\r\n");
}

static void onError(triad_Event *e) {
  printf("server error: %s\n", e->msg);
}

int main(void) {
  triad_Stream *s;
  triad_init();

  s = triad_newStream();
  triad_addListener(s, triad_EVENT_ERROR,  onError,  NULL);
  triad_addListener(s, triad_EVENT_ACCEPT, onAccept, NULL);
  triad_listen(s, 8000);

  while (triad_getStreamCount() > 0) {
    triad_update();
  }

  return 0;
}
