/**
 * Basic minimal, default signal handling setup.
 * This is nearly a complete copy of Rochkind's code available here: http://basepath.com/aup/ex/sigmin_8c-source.html
 * @AuthorMarc J. Rochkind, Samuel Pinkus.
 * @see Advanced Unix Programming, M.J. Rochkind
 */

#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include "handle_signals.h"

// Pointer to client callback function for handled signals.
// The handled signals are always terminal.
static int (*cleanup)(int) = NULL;

// Most sigs.
struct sig_to_string {
  int sig;
  char * str;
  unsigned int len;
} sigmap[] = {
  {SIGBUS, "SIGBUS", 6},
  {SIGFPE, "SIGFPE", 6},
  {SIGILL, "SIGILL", 6},
  {SIGPIPE, "SIGPIPE", 7},
  {SIGSEGV, "SIGSEGV", 7},
  {SIGSYS, "SIGSYS", 7},
  {SIGXCPU, "SIGXCPU", 7},
  {SIGXFSZ, "SIGXFSZ", 7},
  {SIGABRT, "SIGABRT", 7},
  {SIGHUP, "SIGHUP", 6},
  {SIGINT, "SIGINT", 6},
  {SIGKILL, "SIGKILL", 7},
  {SIGQUIT, "SIGQUIT", 7},
  {SIGTERM, "SIGTERM", 7},
  {SIGUSR1, "SIGUSR1", 7},
  {SIGUSR2, "SIGUSR2", 7},
  {SIGCHLD, "SIGCHLD", 7},
  {SIGCONT, "SIGCONT", 7},
  {SIGSTOP, "SIGSTOP", 7},
  {SIGTSTP, "SIGTSTP", 7},
  {SIGTTIN, "SIGTTIN", 7},
  {SIGTTOU, "SIGTTOU", 7},
  {SIGALRM, "SIGALRM", 7},
  {SIGPROF, "SIGPROF", 7},
  {SIGPOLL, "SIGPOLL", 7},
  {SIGTRAP, "SIGTRAP", 7},
  {SIGURG, "SIGURG", 6},
  { 0, NULL, 0}
};


/**
 * Default handler.
 */
static void handler(int signum)
{
  int i = 0;
  int cont = 0;

  write(STDERR_FILENO, "Terminal signal (", 17);
  for(i = 0; sigmap[i].sig > 0; i++)
  {
    if (sigmap[i].sig == signum) {
      write(STDERR_FILENO, sigmap[i].str, sigmap[i].len);
      break;
    }
  }
  write(STDERR_FILENO, ").\n", 3);

  if(cleanup != NULL)
  {
    cont = cleanup(signum);
  }
  if(!cont)
  {
    kill(0, SIGINT);
    _exit(EXIT_FAILURE);
  }
}


/**
 * Setup generic signal handling. This is just a convenience.
 * @input option pointer to a user defined clean up function.
 * @returns 0 on success -1 on error
 */
extern bool handle_signals(int (*cleanup_in)(int))
{
  sigset_t set;
  cleanup = cleanup_in;
  struct sigaction act;
  //ignore all signals. to ensure dont get signals while setting up signal policy.
  ec_neg1(sigfillset(&set))
  ec_neg1(sigprocmask(SIG_SETMASK, &set, NULL))
  memset(&act, 0, sizeof(act));
  ec_neg1(sigfillset(&act.sa_mask))
  act.sa_handler = SIG_IGN;
  //ignore these.
  ec_neg1(sigaction(SIGHUP, &act, NULL))
  ec_neg1(sigaction(SIGQUIT, &act, NULL))
  ec_neg1(sigaction(SIGPIPE, &act, NULL))
  //setup a handler for these.
  act.sa_handler = handler;
  ec_neg1(sigaction(SIGINT, &act, NULL))
  ec_neg1(sigaction(SIGTERM, &act, NULL))
  ec_neg1(sigaction(SIGBUS, &act, NULL))
  ec_neg1(sigaction(SIGFPE, &act, NULL))
  ec_neg1(sigaction(SIGILL, &act, NULL))
  ec_neg1(sigaction(SIGSEGV, &act, NULL))
  ec_neg1(sigaction(SIGSYS, &act, NULL))
  ec_neg1(sigaction(SIGXCPU, &act, NULL))
  ec_neg1(sigaction(SIGXFSZ, &act, NULL))
  //block some signals.
  ec_neg1(sigemptyset(&set))
  ec_neg1(sigaddset(&set, SIGUSR1))
  ec_neg1(sigaddset(&set, SIGUSR2))
  ec_neg1(sigprocmask(SIG_SETMASK, &set, NULL))
  return true;
  EC_CLEANUP_BGN
  return false;  //functions dont exit. always return, -1 on error if appropriate.
  EC_CLEANUP_END
}


/**
 * Setup a signal handler function for a given signal. replaces signal() function from std C.
 * User must make sure signal mask unblocked.
 * @input signal number
 * @input pointer to function.
 * @returns old signal handler or SIG_ERR on error.
 */
sighandler_t set_sig_handler(int signo, sighandler_t func)
{
  struct sigaction  act, oact;
  act.sa_handler = func;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  if (signo == SIGALRM)
  {
    #ifdef  SA_INTERRUPT
    act.sa_flags |= SA_INTERRUPT;  /* SunOS 4.x */
    #endif
  }
  else
  {
    #ifdef  SA_RESTART
    act.sa_flags |= SA_RESTART;  /* SVR4, 44BSD */
    #endif
  }
  ec_neg1(sigaction(signo, &act, &oact));
  return(oact.sa_handler);
  EC_CLEANUP_BGN
  return SIG_ERR;
  EC_CLEANUP_END
}
