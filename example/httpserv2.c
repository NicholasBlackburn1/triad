#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "triad.h"

/* An example of a simple HTTP server which serves up files. We make use of
 * `udata` and the triad_EVENT_READY event to send files in chunks as needed
 * instead of loading the entire file into the stream's write buffer in one go.
 * This allows us to send large files without any issues */

typedef struct {
  FILE *fp;
} Client;


static void client_onReady(triad_Event *e) {
  Client *client = e->udata;
  int c;
  int count = 32000;
  while (count--) {
    if ((c = fgetc(client->fp)) != EOF) {
      triad_write(e->stream, &c, 1);
    } else {
      triad_end(e->stream);
      break;
    }
  }
}

static void client_onLine(triad_Event *e) {
  Client *client = e->udata;
  char path[128];
  if (sscanf(e->data, "GET %127s", path) == 1) {
    /* Print request */
    printf("%s %s\n", triad_getAddress(e->stream), path);
    /* Send header */
    triad_writef(e->stream, "HTTP/1.1 200 OK\r\n\r\n");
    /* Check request */
    if (strstr(path, "..") || path[1] == '/') {
      /* Handle bad request */
      triad_writef(e->stream, "bad request '%s'", path);
      triad_end(e->stream);
    } else {
      /* Handle good request */
      client->fp = fopen(path + 1, "rb");
      if (client->fp) {
        /* Remove this event handler (we don't care about anything else the
         * client has to say) */
        triad_removeListener(e->stream, triad_EVENT_LINE, client_onLine, client);
        /* Add the event handler for sending the file */
        triad_addListener(e->stream, triad_EVENT_READY, client_onReady, client);
      } else {
        triad_writef(e->stream, "not found '%s'\n", path);
        triad_end(e->stream);
      }
    }
  }
}

static void client_onClose(triad_Event *e) {
  Client *client = e->udata;
  if (client->fp) fclose(client->fp);
  free(client);
}


static void server_onAccept(triad_Event *e) {
  Client *client = calloc(1, sizeof(*client));
  triad_addListener(e->remote, triad_EVENT_LINE,  client_onLine,  client);
  triad_addListener(e->remote, triad_EVENT_CLOSE, client_onClose, client);
}

static void server_onListen(triad_Event *e) {
  printf("server listening: http://localhost:%d\n", triad_getPort(e->stream));
}

static void server_onError(triad_Event *e) {
  printf("server error: %s\n", e->msg);
}


int main(void) {
  triad_Stream *serv;
  triad_init();

  serv = triad_newStream();
  triad_addListener(serv, triad_EVENT_ERROR,  server_onError,  NULL);
  triad_addListener(serv, triad_EVENT_ACCEPT, server_onAccept, NULL);
  triad_addListener(serv, triad_EVENT_LISTEN, server_onListen, NULL);
  triad_listen(serv, 8000);

  while (triad_getStreamCount() > 0) {
    triad_update();
  }

  triad_shutdown();
  return 0;
}
