#include <stdio.h>
#include <string.h>
#include "triad.h"

/* A simple IRC bot. Connects to an IRC network, joins a channel then sits
 * idle, responding to the server's PING messges and printing everything the
 * server sends it. */

static char *server  = "irc.afternet.org";
static char *channel = "#triadbots";
static char nick[32];
static int  isRegistered = 0;


static void onConnect(triad_Event *e) {
  /* Generate a random nick name */
  sprintf(nick, "testbot%04x", (int)(triad_getTime()) % 0xFFFF);
  /* Introduce ourselves to the server */
  triad_writef(e->stream, "NICK %s\r\n", nick);
  triad_writef(e->stream, "USER %s %s bla :%s\r\n", nick, nick, nick);
}

static void onError(triad_Event *e) {
  printf("error: %s\n", e->msg);
}

static void onLine(triad_Event *e) {
  printf("%s\n", e->data);
  /* Handle PING */
  if (!memcmp(e->data, "PING", 4)) {
    triad_writef(e->stream, "PONG%s\r\n", e->data + 4);
  }
  /* Handle RPL_WELCOME */
  if (!isRegistered && strstr(e->data, "001")) {
    /* Join channel */
    triad_writef(e->stream, "JOIN %s\r\n", channel);
    isRegistered = 1;
  }
}

int main(void) {
  triad_Stream *s;
  triad_init();

  s = triad_newStream();
  triad_addListener(s, triad_EVENT_CONNECT, onConnect, NULL);
  triad_addListener(s, triad_EVENT_ERROR,   onError,   NULL);
  triad_addListener(s, triad_EVENT_LINE,    onLine,    NULL);
  triad_connect(s, server, 6667);

  while (triad_getStreamCount() > 0) {
    triad_update();
  }
  
  triad_shutdown();
  return 0;
}
