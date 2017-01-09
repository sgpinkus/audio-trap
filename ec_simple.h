/**
 * Very simple error checking macros - just a convenient goto
 */
#ifndef _EC_SIMPLE_H_
#define _EC_SIMPLE_H_
#include <errno.h>
#include <assert.h>

//This is needed to reset the value of ec_in_cleanup after return from inside EC_CLEANUP section.
static const int ec_in_cleanup = 0;

#define EC_CLEANUP_BGN\
  ec_cleanup_bgn:\
  {\
    int ec_in_cleanup = 1;

  #define EC_CLEANUP_END\
  }

//used for error checking where error return is any single unique value.
#define ec_cmp(var, errrtn)\
  {\
    assert(!ec_in_cleanup);\
    if ((intptr_t)(var) == (intptr_t)(errrtn)) {\
      goto ec_cleanup_bgn;\
    }\
  }

//for non zero error return.
//e.g. pthread thread library: 0 => OK, else return errno (> 0).
#define ec_rv(var)\
  {\
    int errrtn;\
    assert(!ec_in_cleanup);\
    if ((errrtn = (var)) != 0) {\
      goto ec_cleanup_bgn;\
    }\
  }

//actually no diff from above here. See Rochkind's.
#define ec_ai(var)\
  {\
    int errrtn;\
    assert(!ec_in_cleanup);\
    if ((errrtn = (var)) != 0) {\
      goto ec_cleanup_bgn;\
    }\
  }

#define ec_neg1(x) errno = 0; ec_cmp((x), -1)
#define ec_null(x) errno = 0; ec_cmp((x), NULL)
#define ec_zero(x) errno = 0; ec_cmp((x), 0) /* not in book */
#define ec_false(x) errno = 0; ec_cmp((x), false)
#define ec_eof(x) errno = 0; ec_cmp((x), EOF)
#define ec_nzero(x)\
  {\
    if ((x) != 0)  { errno = 0; ec_cmp(0, 0); }\
  }
#define EC_FAIL errno = 0; ec_cmp(0, 0)

#endif //_EC_SIMPLE_H_
