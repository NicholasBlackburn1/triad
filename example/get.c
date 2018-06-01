#include <stdio.h>
#include <string.h>
#include "triad.h"

/* Connects to a daytime server and prints the response */

static void onConnect(triad_Event *e) {
  printf("connected: %s\n", e->msg);
}

static void onError(triad_Event *e) {
  printf("error: %s\n", e->msg);
}

static void onData(triad_Event *e) {
  printf("%s", e->data);
  triad_close(e->stream);
}

int main(void) {
  triad_Stream *s;
  triad_init();

  s = triad_newStream();
  triad_addListener(s, triad_EVENT_CONNECT, onConnect, NULL);
  triad_addListener(s, triad_EVENT_ERROR,   onError,   NULL);
  triad_addListener(s, triad_EVENT_DATA,    onData,    NULL);
  triad_connect(s, "127.0.0.1", 8080);

  char *header = "GET /index.html HTTP/1.1\r\nHost: www.example.com\r\n\r\n";

  triad_write(s, header, strlen(header));

  while (triad_getStreamCount() > 0) {
    triad_update();
  }

  triad_shutdown();
  return 0;
}
