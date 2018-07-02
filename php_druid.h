/*
+----------------------------------------------------------------------+
| PHP-Druid                                                            |
+----------------------------------------------------------------------+
| This source file is subject to version 2.0 of the Apache license,    |
| that is bundled with this package in the file LICENSE, and is        |
| available through the world-wide-web at the following url:           |
| http://www.apache.org/licenses/LICENSE-2.0.html                      |
| If you did not receive a copy of the Apache2.0 license and are unable|
| to obtain it through the world-wide-web, please send a note to       |
| license@php.net so we can mail you a copy immediately.               |
+----------------------------------------------------------------------+
| Author: Neeke.Gao  <neeke@php.net>                                   |
+----------------------------------------------------------------------+
*/

#ifndef PHP_DRUID_H
#define PHP_DRUID_H

#define DRUID_NAME "Druid"
#define DRUID_CREDITS "neeke@php.net"
#define PHP_DRUID_VERSION "1.0.0"

#define DRUID_CONTENT_TYPE                  "Content-Type:application/json"
#define DRUID_PROPERTY_CURL_ERR_NO          "_curl_error_no"
#define DRUID_PROPERTY_CURL_ERR_STR         "_curl_error_str"

#define DRUID_PROPERTY_DEBUG                "debug"

#define DRUID_PROPERTY_RESPONSE_CODE        "response_code"
#define DRUID_PROPERTY_RESPONSE_INFO        "response_debug_info"

#define DRUID_PROPERTY_TPL_PATH             "tpl_path"

#define DRUID_PROPERTY_HOSTS                "hosts"
#define DRUID_PROPERTY_HOST_RAND            "host_rand"
#define DRUID_PROPERTY_HOST_RAND_BAR        (double)0.5

#define DRUID_INSTANCE_DEFAULT              "default"
#define DRUID_INSTANCE_DEFAULT_LEN          strlen(DRUID_INSTANCE_DEFAULT)

#define DRUID_RESPONSE_CODE_ERROR_BAR       399

extern zend_module_entry druid_module_entry;
#define phpext_druid_ptr &druid_module_entry



#ifdef PHP_WIN32
#	define PHP_DRUID_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_DRUID_API __attribute__ ((visibility("default")))
#else
#	define PHP_DRUID_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#include <curl/curl.h>
#include <curl/multi.h>

#ifdef PHP_WIN32
#include "win32/time.h"
#elif defined(NETWARE)
#include <sys/timeval.h>
#include <sys/time.h>
#else
#include <sys/time.h>
#endif

struct druidCurlResult {
  char *memory;
  size_t size;
};

PHP_MINIT_FUNCTION(druid);
PHP_MSHUTDOWN_FUNCTION(druid);
PHP_RINIT_FUNCTION(druid);
PHP_RSHUTDOWN_FUNCTION(druid);
PHP_MINFO_FUNCTION(druid);

zend_class_entry *druid_ce,*php_com_exception_class_entry;


PHP_METHOD(DRUID_NAME, __construct);
PHP_METHOD(DRUID_NAME, __destruct);
PHP_METHOD(DRUID_NAME, __clone);
PHP_METHOD(DRUID_NAME, __sleep);
PHP_METHOD(DRUID_NAME, __wakeup);
PHP_METHOD(DRUID_NAME, getInstance);
PHP_METHOD(DRUID_NAME, debugWitch);
PHP_METHOD(DRUID_NAME, getDebugWitch);
PHP_METHOD(DRUID_NAME, setDruidHosts);
PHP_METHOD(DRUID_NAME, getDruidHosts);
PHP_METHOD(DRUID_NAME, setTplPath);
PHP_METHOD(DRUID_NAME, getTplPath);
PHP_METHOD(DRUID_NAME, getData);
PHP_METHOD(DRUID_NAME, getDataByTpl);
PHP_METHOD(DRUID_NAME, getDebugInfo);


ZEND_BEGIN_MODULE_GLOBALS(druid)
	char *base_auth_user;
	char *base_auth_passport;
	char *host;
	char *tpl_path;
	zend_bool debug;

    int curl_dns_cache_timeout;
    int curl_connect_timeout;
    int curl_timeout;
ZEND_END_MODULE_GLOBALS(druid)

extern ZEND_DECLARE_MODULE_GLOBALS(druid);

#ifdef ZTS
#define DRUID_G(v) TSRMG(druid_globals_id, zend_druid_globals *, v)
#else
#define DRUID_G(v) (druid_globals.v)
#endif

#endif

int druid_php_rand(TSRMLS_D);
char *druid_get_host(zval *druid TSRMLS_DC);
static void druid_getApi(zval *return_value, zval *druid, char *request_json TSRMLS_DC);
int druid_get_debug_info(zval *druid,CURL *curl_handle,char *request_json TSRMLS_DC);
int druid_get_contents(zval *druid,char *request_json, struct druidCurlResult *resultStr TSRMLS_DC);
char *druid_file_get_contents_by_tpl(char *filename TSRMLS_DC);


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

