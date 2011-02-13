dnl $Id$
dnl config.m4 for extension pokenum

PHP_ARG_WITH(pokenum, for pokenum support,
[  --with-pokenum             Include pokenum support])

if test "$PHP_POKENUM" != "no"; then
  dnl # --with-pokenum -> check with-path
  SEARCH_PATH="/usr/local/include/poker-eval /usr/include/poker-eval"
  SEARCH_FOR="enumdefs.h"  # you most likely want to change this
  if test -r $PHP_POKENUM/$SEARCH_FOR; then # path given as parameter
     POKENUM_DIR=$PHP_POKENUM
  else # search default path list
     AC_MSG_CHECKING([for pokenum files in default path])
     for i in $SEARCH_PATH ; do
       if test -r $i/$SEARCH_FOR; then
         POKENUM_DIR=$i
         AC_MSG_RESULT(found in $i)
       fi
     done
   fi

  if test -z "$POKENUM_DIR"; then
     AC_MSG_RESULT([not found])
     AC_MSG_ERROR([Please reinstall the pokenum distribution])
  fi

	PHP_ADD_INCLUDE($POKENUM_DIR)

	PHP_CHECK_LIBRARY(poker-eval, enumExhaustive,
	[
		AC_DEFINE(HAVE_POKENUMLIB,1,[ ])
	], [], [
		-lpoker-eval
  ])

	PHP_ADD_LIBRARY_WITH_PATH(poker-eval, /usr/lib, POKENUM_SHARED_LIBADD)
	PHP_SUBST(POKENUM_SHARED_LIBADD)

	PHP_NEW_EXTENSION(pokenum, pokenum.c, $ext_shared)
fi
