#ifndef  _HANDLE_SIGNALS_H_
#define _HANDLE_SIGNALS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ec_simple.h"
#include <signal.h>
#include <stdbool.h>

typedef void (* sighandler_t) (int);
extern bool handle_signals(int(*cleanup)(int));
extern sighandler_t set_sig_handler(int signo, sighandler_t func);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //_HANDLE_SIGNALS_H_
