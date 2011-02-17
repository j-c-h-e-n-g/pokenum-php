/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2010 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Steeve Lennmark                                              |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_POKENUM_H
#define PHP_POKENUM_H

extern zend_module_entry pokenum_module_entry;
#define phpext_pokenum_ptr &pokenum_module_entry

#ifdef PHP_WIN32
#	define PHP_POKENUM_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_POKENUM_API __attribute__ ((visibility("default")))
#else
#	define PHP_POKENUM_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define PHP_POKENUM_EXTNAME "pokenum"
#define PHP_POKENUM_VERSION "0.1"

PHP_MINIT_FUNCTION(pokenum);
PHP_MSHUTDOWN_FUNCTION(pokenum);
PHP_RINIT_FUNCTION(pokenum);
PHP_RSHUTDOWN_FUNCTION(pokenum);
PHP_MINFO_FUNCTION(pokenum);

PHP_FUNCTION(pokenum);
PHP_FUNCTION(pokenum_error);
PHP_FUNCTION(pokenum_errno);

ZEND_BEGIN_MODULE_GLOBALS(pokenum)
	char *pokenum_err;
	int pokenum_errn;
ZEND_END_MODULE_GLOBALS(pokenum)

#ifdef ZTS
#define POKENUM_G(v) TSRMG(pokenum_globals_id, zend_pokenum_globals *, v)
#else
#define POKENUM_G(v) (pokenum_globals.v)
#endif

#define PN_ERR_TYPE 1
#define PN_ERR_CARD_UNKNOWN 2
#define PN_ERR_CARD_DUPLICATE 3
#define PN_ERR_BOARD_TOO_MANY 4
#define PN_ERR_BOARD_TOO_FEW 5
#define PN_ERR_PLAYERS_TOO_MANY 6

#endif	/* PHP_POKENUM_H */
