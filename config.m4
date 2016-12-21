dnl $Id$
dnl config.m4 for extension druid

dnl If your extension references something external, use with:

PHP_ARG_WITH(druid, for druid support,
Make sure that the comment is aligned:
[  --with-druid             Include druid support])

PHP_ARG_WITH(curl, for curl protocol support,
[  --with-curl[=DIR]       Include curl protocol support])


AC_DEFUN([AC_SMARTAGENT_EPOLL],
[
	AC_MSG_CHECKING([for epoll])

	AC_TRY_COMPILE(
	[
		#include <sys/epoll.h>
	], [
		int epollfd;
		struct epoll_event e;

		epollfd = epoll_create(1);
		if (epollfd < 0) {
			return 1;
		}

		e.events = EPOLLIN | EPOLLET;
		e.data.fd = 0;

		if (epoll_ctl(epollfd, EPOLL_CTL_ADD, 0, &e) == -1) {
			return 1;
		}

		e.events = 0;
		if (epoll_wait(epollfd, &e, 1, 1) < 0) {
			return 1;
		}
	], [
		AC_DEFINE([HAVE_EPOLL], 1, [do we have epoll?])
		AC_MSG_RESULT([yes])
	], [
		AC_MSG_RESULT([no])
	])
])
dnl }}}


dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(druid, whether to enable druid support,
dnl Make sure that the comment is aligned:
dnl [  --enable-druid           Enable druid support])

if test "$PHP_druid" != "no"; then
  if test -r $PHP_CURL/include/curl/easy.h; then
      CURL_DIR=$PHP_CURL
    else
      AC_MSG_CHECKING(for cURL in default path)
      for i in /usr/local /usr; do
        if test -r $i/include/curl/easy.h; then
          CURL_DIR=$i
          AC_MSG_RESULT(found in $i)
          break
        fi
      done
    fi

    if test -z "$CURL_DIR"; then
      AC_MSG_RESULT(not found)
      AC_MSG_ERROR(Please reinstall the libcurl distribution - easy.h should be in <curl-dir>/include/curl/)
    fi

    PHP_ADD_INCLUDE($CURL_DIR/include)
    PHP_EVAL_LIBLINE($CURL_LIBS, druid_SHARED_LIBADD)
    PHP_ADD_LIBRARY_WITH_PATH(curl, $CURL_DIR/$PHP_LIBDIR, druid_SHARED_LIBADD)

    AC_SMARTAGENT_EPOLL()


  PHP_SUBST(druid_SHARED_LIBADD)

  PHP_NEW_EXTENSION(druid, druid.c, $ext_shared)
fi