/**
 * Copyright (c) 2016 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#ifndef triad_H
#define triad_H

#include <stdarg.h>
#include "vector.h"

#ifdef _WIN32
  #include <windows.h> /* For SOCKET */
#endif


#ifdef __cplusplus
extern "C" {
#endif

#if _WIN32
typedef SOCKET triad_Socket;
#else
typedef int triad_Socket;
#endif

struct triad_Stream;
typedef struct triad_Stream triad_Stream;

typedef struct {
  int type;
  void *udata;
  triad_Stream *stream;
  triad_Stream *remote;
  const char *msg;
  char *data;
  int size;
} triad_Event;

typedef void (*triad_Callback)(triad_Event*);
typedef void (*triad_PanicCallback)(const char*);

enum {
  triad_EVENT_NULL,
  triad_EVENT_DESTROY,
  triad_EVENT_ACCEPT,
  triad_EVENT_LISTEN,
  triad_EVENT_CONNECT,
  triad_EVENT_CLOSE,
  triad_EVENT_READY,
  triad_EVENT_DATA,
  triad_EVENT_LINE,
  triad_EVENT_ERROR,
  triad_EVENT_TIMEOUT,
  triad_EVENT_TICK
};

enum {
  triad_STATE_CLOSED,
  triad_STATE_CLOSING,
  triad_STATE_CONNECTING,
  triad_STATE_CONNECTED,
  triad_STATE_LISTENING
};


void triad_init(void);
void triad_update(void);
void triad_shutdown(void);
const char *triad_getVersion(void);
double triad_getTime(void);
int  triad_getStreamCount(void);
void triad_setTickInterval(double seconds);
void triad_setUpdateTimeout(double seconds);
triad_PanicCallback triad_atPanic(triad_PanicCallback func);

triad_Stream *triad_newStream(void);
int  triad_listen(triad_Stream *stream, int port);
int  triad_listenEx(triad_Stream *stream, const char *host, int port,
                   int backlog);
int  triad_connect(triad_Stream *stream, const char *host, int port);
void triad_addListener(triad_Stream *stream, int event,
                      triad_Callback callback, void *udata);
void triad_removeListener(triad_Stream *stream, int event,
                         triad_Callback callback, void *udata);
void triad_removeAllListeners(triad_Stream *stream, int event);
void triad_end(triad_Stream *stream);
void triad_close(triad_Stream *stream);
void triad_write(triad_Stream *stream, const void *data, int size);
void triad_vwritef(triad_Stream *stream, const char *fmt, va_list args);
void triad_writef(triad_Stream *stream, const char *fmt, ...);
void triad_setTimeout(triad_Stream *stream, double seconds);
void triad_setNoDelay(triad_Stream *stream, int opt);
int  triad_getState(triad_Stream *stream);
const char *triad_getAddress(triad_Stream *stream);
int  triad_getPort(triad_Stream *stream);
int  triad_getBytesSent(triad_Stream *stream);
int  triad_getBytesReceived(triad_Stream *stream);
triad_Socket triad_getSocket(triad_Stream *stream);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
