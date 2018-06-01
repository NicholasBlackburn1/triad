#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "triad.h"

/* A very simple HTTP server which responds to a number of paths with different
 * information. See httpserv2.c for an example of an HTTP server which responds
 * with files. */

static int count = 0;

static void onLine(triad_Event *e) {
  char path[128];
  if (sscanf(e->data, "GET %127s", path) == 1) {
    /* Print request */
    printf("%s %s\n", triad_getAddress(e->stream), path);
    /* Send header */
    triad_writef(e->stream, "HTTP/1.1 200 OK\r\n\r\n");
    /* Handle request */
    if (!strcmp(path, "/")) {
      triad_writef(e->stream, "<html><body><pre>"
                             "<a href='/date'>date</a><br>"
                             "<a href='/count'>count</a><br>"
                             "<a href='/ip'>ip</a>"
                             "</pre></html></body>" );

    } else if (!strcmp(path, "/date")) {
      time_t t = time(0);
      triad_writef(e->stream, "%s", ctime(&t));

    } else if (!strcmp(path, "/count")) {
      triad_writef(e->stream, "%d", ++count);

    } else if (!strcmp(path, "/ip")) {
      triad_writef(e->stream, "%s", triad_getAddress(e->stream));

    } else {
      triad_writef(e->stream, "bad request '%s'", path);
    }
    /* Close stream when all data has been sent */
    triad_end(e->stream);
  }
}

static void onAccept(triad_Event *e) {
  triad_addListener(e->remote, triad_EVENT_LINE, onLine, NULL);
}

static void onListen(triad_Event *e) {
  printf("server listening: http://localhost:%d\n", triad_getPort(e->stream));
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
  triad_addListener(s, triad_EVENT_LISTEN, onListen, NULL);
  triad_listen(s, 8000);

  while (triad_getStreamCount() > 0) {
    triad_update();
  }

  return 0;
}
