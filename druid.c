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

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/json/php_json.h"
#include "php_druid.h"
#include "ext/standard/php_rand.h"
#include "ext/standard/php_array.h"
#include "ext/standard/file.h"
#include "zend_exceptions.h"



#if PHP_VERSION_ID >= 70000

#define DRUID_ZVAL_STRING(z, s) ZVAL_STRING(z, s)
#define DRUID_ZVAL_STRINGL(z, s, l) ZVAL_STRINGL(z, s, l)
#define DRUID_RETURN_STRINGL(k, l) RETURN_STRINGL(k, l)
#define DRUID_ADD_INDEX_STRINGL(z, i, s, l) add_index_stringl(&z, i, s, l)
#define DRUID_ADD_INDEX_LONG(z, i, l) add_index_long(&z, i, l)
#define DRUID_ADD_INDEX_ZVAL(z, i, zn) add_index_zval(&z, i, &zn)
#define DRUID_ADD_ASSOC_LONG_EX(z, s, l, v) add_assoc_long_ex(z, s, l, (long) v)
#define DRUID_ADD_ASSOC_DOUBLE_EX(z, s, l, v) add_assoc_double_ex(z, s, l, (double) v)
#define DRUID_ADD_ASSOC_ZVAL_EX(z, s, l, zn) add_assoc_zval_ex(z, s, l, zn)
#define DRUID_ADD_ASSOC_STRING_EX(a, k, l, s) add_assoc_string_ex(&a, k, l, s)
#define DRUID_ADD_NEXT_INDEX_STRING(a, s) add_next_index_string(a, s)
#define DRUID_ADD_NEXT_INDEX_STRINGL(a, s, l) add_next_index_stringl(a, s, l)
#define DRUID_ZEND_HASH_GET_CURRENT_KEY(ht, key, idx) zend_hash_get_current_key(ht, key, idx)
#define DRUID_ZEND_HASH_INDEX_UPDATE(ht, h, pData, nDataSize, pDest)  zend_hash_index_update_ptr(ht, h, pData)

#else

#define DRUID_ZVAL_STRING(z, s) ZVAL_STRING(z, s, 1)
#define DRUID_ZVAL_STRINGL(z, s, l) ZVAL_STRING(z, s, l, 1)
#define DRUID_RETURN_STRINGL(k, l) RETURN_STRINGL(k, l, 1)
#define DRUID_ADD_INDEX_STRINGL(z, i, s, l) add_index_stringl(z, i, s, l, 1)
#define DRUID_ADD_INDEX_LONG(z, i, l) add_index_long(z, i, l)
#define DRUID_ADD_INDEX_ZVAL(z, i, zn) add_index_zval(z, i, zn)
#define DRUID_ADD_ASSOC_LONG_EX(z, s, l, v) add_assoc_long_ex(z, s, l, (long) v)
#define DRUID_ADD_ASSOC_DOUBLE_EX(z, s, l, v) add_assoc_double_ex(z, s, l, (double) v)
#define DRUID_ADD_ASSOC_ZVAL_EX(z, s, l, zn) add_assoc_zval_ex(z, s, l, zn)
#define DRUID_ADD_ASSOC_STRING_EX(a, k, l, s) add_assoc_string_ex(a, k, l, s, 1)
#define DRUID_ADD_NEXT_INDEX_STRING(a, s) add_next_index_string(a, s, 1)
#define DRUID_ADD_NEXT_INDEX_STRINGL(a, s, l) add_next_index_stringl(a, s, l, 1)
#define DRUID_ZEND_HASH_GET_CURRENT_KEY(ht, key, idx) zend_hash_get_current_key(ht, key, idx, 0)
#define DRUID_ZEND_HASH_INDEX_UPDATE(ht, h, pData, nDataSize, pDest)  zend_hash_index_update(ht, h, pData, nDataSize, pDest)

#endif


ZEND_DECLARE_MODULE_GLOBALS(druid)
#if ZEND_MODULE_API_NO >= 20050922
zend_module_dep druid_deps[] =
{
    ZEND_MOD_REQUIRED("json")
    {
        NULL, NULL, NULL
    }
};
#endif

static int le_druid;

const zend_function_entry druid_functions[] =
{
    {NULL, NULL, NULL}
};

const zend_function_entry druid_methods[] =
{
    PHP_ME(DRUID_NAME, __construct, NULL, ZEND_ACC_CTOR|ZEND_ACC_PRIVATE)
    PHP_ME(DRUID_NAME, __clone, NULL, ZEND_ACC_CLONE|ZEND_ACC_PRIVATE)
    PHP_ME(DRUID_NAME, __sleep, NULL, ZEND_ACC_PRIVATE)
    PHP_ME(DRUID_NAME, __wakeup, NULL, ZEND_ACC_PRIVATE)
    PHP_ME(DRUID_NAME, __destruct,               NULL, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
    PHP_ME(DRUID_NAME, getInstance,    	         NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(DRUID_NAME, debugWitch,    	         NULL, ZEND_ACC_PUBLIC)
    PHP_ME(DRUID_NAME, setDruidHosts,    	     NULL, ZEND_ACC_PUBLIC)
    PHP_ME(DRUID_NAME, setTplPath,    	         NULL, ZEND_ACC_PUBLIC)
    PHP_ME(DRUID_NAME, getData,    	             NULL, ZEND_ACC_PUBLIC)
    PHP_ME(DRUID_NAME, getDataByTpl,    	     NULL, ZEND_ACC_PUBLIC)
    PHP_ME(DRUID_NAME, getDebugInfo,    	     NULL, ZEND_ACC_PUBLIC)
    {
        NULL, NULL, NULL
    }
};

zend_module_entry druid_module_entry =
{
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    DRUID_NAME,
    druid_functions,
    PHP_MINIT(druid),
    PHP_MSHUTDOWN(druid),
    PHP_RINIT(druid),
    PHP_RSHUTDOWN(druid),
    PHP_MINFO(druid),
#if ZEND_MODULE_API_NO >= 20010901
    PHP_DRUID_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_DRUID
ZEND_GET_MODULE(druid)
#endif

PHP_INI_BEGIN()
STD_PHP_INI_ENTRY("druid.host", "", PHP_INI_SYSTEM, OnUpdateString, host, zend_druid_globals, druid_globals)
STD_PHP_INI_ENTRY("druid.base_auth_user",  "", PHP_INI_PERDIR, OnUpdateString, base_auth_user, zend_druid_globals, druid_globals)
STD_PHP_INI_ENTRY("druid.base_auth_passport", "", PHP_INI_PERDIR, OnUpdateString, base_auth_passport, zend_druid_globals, druid_globals)
STD_PHP_INI_ENTRY("druid.tpl_path", "/data/php-druid/tpl", PHP_INI_PERDIR, OnUpdateString, tpl_path, zend_druid_globals, druid_globals)

STD_PHP_INI_BOOLEAN("druid.debug", "0", PHP_INI_ALL, OnUpdateBool, debug, zend_druid_globals, druid_globals)

PHP_INI_END()

static void php_druid_init_globals(zend_druid_globals *druid_globals)
{

}

PHP_MINIT_FUNCTION(druid)
{

    ZEND_INIT_MODULE_GLOBALS(druid, php_druid_init_globals, NULL);
    REGISTER_INI_ENTRIES();
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, DRUID_NAME, druid_methods);


    druid_ce=zend_register_internal_class(&ce TSRMLS_CC);

    zend_declare_class_constant_stringl(druid_ce,ZEND_STRL("DRUID_CONTENT_TYPE"),ZEND_STRL(DRUID_CONTENT_TYPE) TSRMLS_CC);

    zend_declare_property_null(druid_ce, ZEND_STRL(DRUID_NAME), ZEND_ACC_STATIC | ZEND_ACC_PROTECTED TSRMLS_CC);

    zend_declare_property_null(druid_ce, ZEND_STRL(DRUID_PROPERTY_RESPONSE_INFO), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_long(druid_ce, ZEND_STRL(DRUID_PROPERTY_RESPONSE_CODE),0, ZEND_ACC_PROTECTED TSRMLS_CC);

    zend_declare_property_long(druid_ce, ZEND_STRL(DRUID_PROPERTY_CURL_ERR_NO),0, ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_null(druid_ce, ZEND_STRL(DRUID_PROPERTY_CURL_ERR_STR), ZEND_ACC_PROTECTED TSRMLS_CC);

    zend_declare_property_null(druid_ce, ZEND_STRL(DRUID_PROPERTY_HOSTS), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_bool(druid_ce, ZEND_STRL(DRUID_PROPERTY_HOST_RAND), 0, ZEND_ACC_PROTECTED TSRMLS_CC);

    return SUCCESS;
}
/* }}} */

PHP_MSHUTDOWN_FUNCTION(druid)
{

    UNREGISTER_INI_ENTRIES();

    return SUCCESS;
}

PHP_RINIT_FUNCTION(druid)
{
    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(druid)
{
    return SUCCESS;
}

PHP_MINFO_FUNCTION(druid)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "druid support", "enabled");
    php_info_print_table_row(2, "druid version", PHP_DRUID_VERSION);
    php_info_print_table_row(2, "druid credits", DRUID_CREDITS);
    php_info_print_table_end();

    DISPLAY_INI_ENTRIES();

}

PHP_METHOD(DRUID_NAME, getInstance)
{

    zval *instance;

    instance = zend_read_static_property(druid_ce, ZEND_STRL(DRUID_NAME), 1 TSRMLS_CC);

    if (IS_OBJECT == Z_TYPE_P(instance)
            && instanceof_function(Z_OBJCE_P(instance), druid_ce TSRMLS_CC))
    {
        //Do Nothing
    }
    else
    {
        MAKE_STD_ZVAL(instance);
        object_init_ex(instance, druid_ce);

        zend_update_property_long(druid_ce, instance, ZEND_STRL(DRUID_PROPERTY_CURL_ERR_NO), 0 TSRMLS_CC);
        zend_update_property_string(druid_ce, instance, ZEND_STRL(DRUID_PROPERTY_CURL_ERR_STR), "" TSRMLS_CC);

        zend_update_property_long(druid_ce, instance, ZEND_STRL(DRUID_PROPERTY_RESPONSE_CODE), 0 TSRMLS_CC);
        zend_update_property_null(druid_ce, instance, ZEND_STRL(DRUID_PROPERTY_RESPONSE_INFO) TSRMLS_CC);

        zend_update_property_null(druid_ce, instance, ZEND_STRL(DRUID_PROPERTY_HOSTS) TSRMLS_CC);
        zend_update_property_bool(druid_ce, instance, ZEND_STRL(DRUID_PROPERTY_HOST_RAND), 0 TSRMLS_CC);

        zend_update_static_property(druid_ce, ZEND_STRL(DRUID_NAME), instance TSRMLS_CC);
    }

    RETURN_ZVAL(instance, 1, 0);
}


PHP_METHOD(DRUID_NAME, __construct)
{
}

PHP_METHOD(DRUID_NAME,__destruct)
{
    php_printf("__destruct   \n");
    zend_declare_property_null(druid_ce, ZEND_STRL(DRUID_PROPERTY_RESPONSE_INFO), ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_update_static_property_null(druid_ce, ZEND_STRL(DRUID_NAME) TSRMLS_CC);
}

PHP_METHOD(DRUID_NAME, __sleep)
{
}

PHP_METHOD(DRUID_NAME, __wakeup)
{
}

PHP_METHOD(DRUID_NAME, __clone)
{
}

PHP_METHOD(DRUID_NAME, debugWitch)
{
    zend_bool debug = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &debug) == FAILURE) {
        RETURN_FALSE;
    }

    DRUID_G(debug) = debug;

    RETURN_TRUE;
}

PHP_METHOD(DRUID_NAME, setDruidHosts)
{
    int argc = ZEND_NUM_ARGS();

    zval *hosts;

    if (zend_parse_parameters(argc TSRMLS_CC, "a", &hosts) == FAILURE)
    {
        RETURN_FALSE;
    }

    if (Z_TYPE_P(hosts) != IS_ARRAY)
    {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "The argument is not an array");
        RETURN_FALSE;
    }

	zend_update_property(druid_ce, getThis(), ZEND_STRL(DRUID_PROPERTY_HOSTS), hosts TSRMLS_CC);
	zend_update_property_bool(druid_ce, getThis(), ZEND_STRL(DRUID_PROPERTY_HOST_RAND), 1 TSRMLS_CC);

    RETURN_TRUE;
}

PHP_METHOD(DRUID_NAME, setTplPath)
{
#if PHP_VERSION_ID >= 70000

    zend_string *tpl_path;

    if (zend_parse_parameters(argc TSRMLS_CC, "S", &tpl_path) == FAILURE)
    {
       RETURN_FALSE;
    }

    DRUID_G(tpl_path) = estrdup(ZSTR_VAL(tpl_path));

    RETURN_TRUE;

#else

    char *tpl_path;
    int tpl_path_len = 0;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &tpl_path, &tpl_path_len) == FAILURE) {
        RETURN_FALSE;
    }

    DRUID_G(tpl_path) = estrdup(tpl_path);

    RETURN_TRUE;

#endif

}

/*Just used by PHP7*/
// We asure the src is on heap, so every call we can safe free than alloc.
static char *strreplace(char *src, const char *oldstr, const char *newstr, size_t len)
{
    char *needle;
    char *tmp;

    if(strcmp(oldstr, newstr)==0)
    {
        return src;
    }

    while((needle = strstr(src, oldstr)) && (needle - src <= len))
    {
        tmp = (char*)emalloc(strlen(src) + (strlen(newstr) - strlen(oldstr)) + 1);

        strncpy(tmp, src, needle-src);

        tmp[needle-src]='\0';
        strcat(tmp, newstr);
        strcat(tmp, needle+strlen(oldstr));

        efree(src);
        src = tmp;
        len = strlen(src);
    }

    return src;
}

#if PHP_VERSION_ID >= 70000
static char *php_strtr_array(char *str, int slen, HashTable *pats)
{
    zend_ulong num_key;
    zend_string *str_key;
    zval *entry;
    char *tmp = estrdup(str);

    ZEND_HASH_FOREACH_KEY_VAL(pats, num_key, str_key, entry)
    {
        if (UNEXPECTED(!str_key))
        {

        }
        else
        {
            zend_string *s = zval_get_string(entry);

            if (strstr(str,ZSTR_VAL(str_key)))
            {
                tmp = strreplace(tmp, ZSTR_VAL(str_key), ZSTR_VAL(s), strlen(str));
            }

            zend_string_release(s);
        }
    }
    ZEND_HASH_FOREACH_END();

    return tmp;
}

#else
static char *php_strtr_array(char *str, int slen, HashTable *hash)
{
    zval **entry;
    char  *string_key;
    uint   string_key_len;
    zval **trans;
    zval   ctmp;
    ulong  num_key;
    int    minlen = 128 * 1024;
    int    maxlen = 0, pos, len, found;
    char  *key;
    HashPosition hpos;
    smart_str result = {0};
    char  *result_str;
    HashTable tmp_hash;

    zend_hash_init(&tmp_hash, zend_hash_num_elements(hash), NULL, NULL, 0);
    zend_hash_internal_pointer_reset_ex(hash, &hpos);

    while (zend_hash_get_current_data_ex(hash, (void **)&entry, &hpos) == SUCCESS)
    {
        switch (zend_hash_get_current_key_ex(hash, &string_key, &string_key_len, &num_key, 0, &hpos))
        {
        case HASH_KEY_IS_STRING:
            len = string_key_len - 1;
            if (len < 1)
            {
                zend_hash_destroy(&tmp_hash);
            }
            else
            {
                zend_hash_add(&tmp_hash, string_key, string_key_len, entry, sizeof(zval*), NULL);
                if (len > maxlen)
                {
                    maxlen = len;
                }
                if (len < minlen)
                {
                    minlen = len;
                }
            }
            break;

        case HASH_KEY_IS_LONG:
            Z_TYPE(ctmp) = IS_LONG;
            Z_LVAL(ctmp) = num_key;

            convert_to_string(&ctmp);
            len = Z_STRLEN(ctmp);
            zend_hash_add(&tmp_hash, Z_STRVAL(ctmp), len + 1, entry, sizeof(zval*), NULL);
            zval_dtor(&ctmp);

            if (len > maxlen)
            {
                maxlen = len;
            }
            if (len < minlen)
            {
                minlen = len;
            }
            break;
        }
        zend_hash_move_forward_ex(hash, &hpos);
    }

    key = emalloc(maxlen + 1);
    pos = 0;

    while (pos < slen)
    {
        if ((pos + maxlen) > slen)
        {
            maxlen = slen - pos;
        }

        found = 0;
        memcpy(key, str + pos, maxlen);

        for (len = maxlen; len >= minlen; len--)
        {
            key[len] = 0;

            if (zend_hash_find(&tmp_hash, key, len + 1, (void**)&trans) == SUCCESS)
            {
                char *tval;
                int tlen;
                zval tmp;

                if (Z_TYPE_PP(trans) != IS_STRING)
                {
                    tmp = **trans;
                    zval_copy_ctor(&tmp);
                    convert_to_string(&tmp);
                    tval = Z_STRVAL(tmp);
                    tlen = Z_STRLEN(tmp);
                }
                else
                {
                    tval = Z_STRVAL_PP(trans);
                    tlen = Z_STRLEN_PP(trans);
                }

                smart_str_appendl(&result, tval, tlen);
                pos += len;
                found = 1;

                if (Z_TYPE_PP(trans) != IS_STRING)
                {
                    zval_dtor(&tmp);
                }

                break;
            }
        }

        if (! found)
        {
            smart_str_appendc(&result, str[pos++]);
        }
    }

    zend_hash_destroy(&tmp_hash);
    result_str = estrndup(result.c, result.len);
    efree(key);
    smart_str_free(&result);

    return result_str;
}
#endif

PHP_METHOD(DRUID_NAME,getData)
{
    int argc = ZEND_NUM_ARGS();

    zval *result;
    char *request,*request_json;
    int  request_len;
    zval **content;

    if (zend_parse_parameters(argc TSRMLS_CC, "s|Z", &request, &request_len, &content) == FAILURE)
    {
        RETURN_FALSE;
    }

    if (argc > 1 && Z_TYPE_PP(content) != IS_ARRAY)
    {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "The second argument is not an array");
        RETURN_FALSE;
    }

    if (argc > 1)
    {
        request_json = php_strtr_array(request,request_len,HASH_OF(*content));
    }
    else
    {
        request_json = request;
    }

    zval *druid = zend_read_static_property(druid_ce, ZEND_STRL(DRUID_NAME), 1 TSRMLS_CC);

    result = druid_getApi(druid,request_json TSRMLS_CC);         //CURL查询

    if (result == NULL)
    {
        RETURN_FALSE;
    }
    else
    {
        RETURN_ZVAL( result,1,0);
    }
}

PHP_METHOD(DRUID_NAME,getDataByTpl)
{
    int argc = ZEND_NUM_ARGS();

    zval *result;
    char *tpl,*request,*request_json,*filename;
    int  tpl_len,filename_len;
    zval **content;

    if (zend_parse_parameters(argc TSRMLS_CC, "s|Z", &tpl, &tpl_len, &content) == FAILURE)
    {
        RETURN_FALSE;
    }

    if (argc > 1 && Z_TYPE_PP(content) != IS_ARRAY)
    {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "The second argument is not an array");
        RETURN_FALSE;
    }

    filename_len = spprintf(&filename, 0, "%s/%s", DRUID_G(tpl_path),tpl);
    request = druid_file_get_contents_by_tpl(filename TSRMLS_CC);

    if (argc > 1)
    {
        request_json = php_strtr_array(request,strlen(request),HASH_OF(*content));
    }
    else
    {
        request_json = tpl;
    }

    zval *druid = zend_read_static_property(druid_ce, ZEND_STRL(DRUID_NAME), 1 TSRMLS_CC);

    result = druid_getApi(druid,request_json TSRMLS_CC);         //CURL查询

    if (result == NULL)
    {
        RETURN_FALSE;
    }
    else
    {
        RETURN_ZVAL( result,1,0);
    }
}

PHP_METHOD(DRUID_NAME,getDebugInfo)
{
    zval *info = zend_read_property(druid_ce, getThis(), ZEND_STRL(DRUID_PROPERTY_RESPONSE_INFO), 1 TSRMLS_CC);
    RETVAL_ZVAL(info, 1, 0);
}


char *druid_file_get_contents_by_tpl(char *filename TSRMLS_DC)
{
    char *contents;
    php_stream *stream;
    int len;
    zval *zcontext = NULL;
    php_stream_context *context = NULL;

    context = php_stream_context_from_zval(zcontext, 0);

    stream = php_stream_open_wrapper_ex(filename, "rb",0 | REPORT_ERRORS, NULL, context);
    if (!stream) {
        return "";
    }

    if ((len = php_stream_copy_to_mem(stream, &contents, PHP_STREAM_COPY_ALL, 0)) > 0) {
        return contents;
    } else if (len == 0) {
        return "";
    } else {
        return "";
    }

    php_stream_close(stream);
}

static zval *druid_getApi(zval *druid, char *request_json TSRMLS_DC)
{
    zval *err_str,*err_no;
    zval *response_code;
    zval *resultZval;
    MAKE_STD_ZVAL(resultZval);

    struct druidCurlResult curlResult;

    if(druid_get_contents(druid,request_json,&curlResult TSRMLS_CC) != SUCCESS)
    {
        err_str = zend_read_property(druid_ce, druid, ZEND_STRL(DRUID_PROPERTY_CURL_ERR_STR), 1 TSRMLS_CC);
        err_no = zend_read_property(druid_ce, druid, ZEND_STRL(DRUID_PROPERTY_CURL_ERR_NO), 1 TSRMLS_CC);

        zend_throw_exception(php_com_exception_class_entry,Z_STRVAL_P(err_str),Z_LVAL_P(err_no) TSRMLS_CC);
        return NULL;
    }

    response_code = zend_read_property(druid_ce, druid, ZEND_STRL(DRUID_PROPERTY_RESPONSE_CODE), 1 TSRMLS_CC);
    if (Z_LVAL_P(response_code) > DRUID_RESPONSE_CODE_ERROR_BAR)
    {
        zend_throw_exception(php_com_exception_class_entry,curlResult.memory,Z_LVAL_P(response_code) TSRMLS_CC);
        return NULL;
    }

    php_json_decode(resultZval, curlResult.memory, (long)curlResult.size, 1, 512 TSRMLS_CC);
    free(curlResult.memory);

    return resultZval;
}


static size_t druid_curl_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct druidCurlResult *mem = (struct druidCurlResult *)userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL)
    {
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

int druid_php_rand(TSRMLS_D)
{
    int rnd_idx;
    rnd_idx = php_rand(TSRMLS_C);

    if ((double) (rnd_idx / (PHP_RAND_MAX + 1.0)) < DRUID_PROPERTY_HOST_RAND_BAR)
    {
        return SUCCESS;
    }

    return FAILURE;
}

char *druid_get_host(zval *druid TSRMLS_DC)
{
    zval *host_rand,*hosts;
    host_rand = zend_read_property(druid_ce, druid, ZEND_STRL(DRUID_PROPERTY_HOST_RAND), 1 TSRMLS_CC);

#if PHP_VERSION_ID >= 70000

    zend_ulong num_key;
    zend_string *str_key;
    zval *entry;

    if (Z_LVAL_P(host_rand) == 1)
    {
        ZEND_HASH_FOREACH_KEY_VAL(pats, num_key, str_key, entry)
        {
            if (UNEXPECTED(!str_key))
            {

            }
            else
            {
                zend_string *s = zval_get_string(entry);

                if (druid_php_rand(TSRMLS_C) == SUCCESS)
                {
                    return ZSTR_VAL(str_key);
                }
                zend_string_release(s);
            }
        }
        ZEND_HASH_FOREACH_END();
    }

#else

    if (Z_LVAL_P(host_rand) == 1) {

        hosts = zend_read_property(druid_ce, druid, ZEND_STRL(DRUID_PROPERTY_HOSTS), 1 TSRMLS_CC);

        zval **entry;
        HashPosition pos;

        for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(hosts), &pos);
            zend_hash_get_current_data_ex(Z_ARRVAL_P(hosts), (void **)&entry, &pos) == SUCCESS;
            zend_hash_move_forward_ex(Z_ARRVAL_P(hosts), &pos)
        ) {
            if (Z_TYPE_PP(entry) == IS_ARRAY || Z_TYPE_PP(entry) == IS_OBJECT || Z_TYPE_PP(entry) == IS_LONG) {
                continue;
            }

            if (druid_php_rand(TSRMLS_C) == SUCCESS)
            {
                return Z_STRVAL_PP(entry);
            }
        }
    }
#endif

    return DRUID_G(host);
}

int druid_get_debug_info(zval *druid,CURL *curl_handle,char *request_json TSRMLS_DC)
{
    char   *s_code;
    long    l_code;
    double  d_code;

    zval *debug_info;
    MAKE_STD_ZVAL(debug_info);
    array_init(debug_info);

    if (curl_easy_getinfo(curl_handle, CURLINFO_EFFECTIVE_URL, &s_code) == CURLE_OK) {
        DRUID_ADD_ASSOC_STRING_EX(debug_info,"url",4,s_code);
    }
    if (curl_easy_getinfo(curl_handle, CURLINFO_HTTP_CODE, &l_code) == CURLE_OK)
    {
         DRUID_ADD_ASSOC_LONG_EX(debug_info,"http_code",10,l_code);
    }
    if (curl_easy_getinfo(curl_handle, CURLINFO_HEADER_SIZE, &l_code) == CURLE_OK)
    {
        DRUID_ADD_ASSOC_LONG_EX(debug_info,"header_size",12,l_code);
    }
    if (curl_easy_getinfo(curl_handle, CURLINFO_REQUEST_SIZE, &l_code) == CURLE_OK)
    {
        DRUID_ADD_ASSOC_LONG_EX(debug_info,"request_size",13,l_code);
    }
    if (curl_easy_getinfo(curl_handle, CURLINFO_FILETIME, &l_code) == CURLE_OK)
    {
        DRUID_ADD_ASSOC_LONG_EX(debug_info,"filetime",9,l_code);
    }
    if (curl_easy_getinfo(curl_handle, CURLINFO_SSL_VERIFYRESULT, &l_code) == CURLE_OK)
    {
        DRUID_ADD_ASSOC_LONG_EX(debug_info,"ssl_verify_result",18,l_code);
    }
    if (curl_easy_getinfo(curl_handle, CURLINFO_REDIRECT_COUNT, &l_code) == CURLE_OK)
    {
        DRUID_ADD_ASSOC_LONG_EX(debug_info,"redirect_count",15,l_code);
    }
    if (curl_easy_getinfo(curl_handle, CURLINFO_TOTAL_TIME, &d_code) == CURLE_OK)
    {
        DRUID_ADD_ASSOC_DOUBLE_EX(debug_info,"total_time",11,d_code);
    }
    if (curl_easy_getinfo(curl_handle, CURLINFO_NAMELOOKUP_TIME, &d_code) == CURLE_OK)
    {
        DRUID_ADD_ASSOC_DOUBLE_EX(debug_info,"namelookup_time",16,d_code);
    }
    if (curl_easy_getinfo(curl_handle, CURLINFO_CONNECT_TIME, &d_code) == CURLE_OK)
    {
        DRUID_ADD_ASSOC_DOUBLE_EX(debug_info,"connect_time",13,d_code);
    }
    if (curl_easy_getinfo(curl_handle, CURLINFO_PRETRANSFER_TIME, &d_code) == CURLE_OK)
    {
        DRUID_ADD_ASSOC_DOUBLE_EX(debug_info,"pretransfer_time",17,d_code);
    }

    if (curl_easy_getinfo(curl_handle, CURLINFO_SIZE_UPLOAD, &d_code) == CURLE_OK)
    {
        DRUID_ADD_ASSOC_DOUBLE_EX(debug_info,"size_upload",13,d_code);
    }
    if (curl_easy_getinfo(curl_handle, CURLINFO_SIZE_DOWNLOAD, &d_code) == CURLE_OK)
    {
        DRUID_ADD_ASSOC_DOUBLE_EX(debug_info,"size_download",14,d_code);
    }
    if (curl_easy_getinfo(curl_handle, CURLINFO_SPEED_DOWNLOAD, &d_code) == CURLE_OK)
    {
        DRUID_ADD_ASSOC_DOUBLE_EX(debug_info,"speed_download",15,d_code);
    }
    if (curl_easy_getinfo(curl_handle, CURLINFO_SPEED_UPLOAD, &d_code) == CURLE_OK)
    {
        DRUID_ADD_ASSOC_DOUBLE_EX(debug_info,"speed_upload",13,d_code);
    }
    if (curl_easy_getinfo(curl_handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &d_code) == CURLE_OK)
    {
        DRUID_ADD_ASSOC_DOUBLE_EX(debug_info,"download_content_length",24,d_code);
    }
    if (curl_easy_getinfo(curl_handle, CURLINFO_CONTENT_LENGTH_UPLOAD, &d_code) == CURLE_OK)
    {
        DRUID_ADD_ASSOC_DOUBLE_EX(debug_info,"upload_content_length",22,d_code);
    }
    if (curl_easy_getinfo(curl_handle, CURLINFO_STARTTRANSFER_TIME, &d_code) == CURLE_OK)
    {
        DRUID_ADD_ASSOC_DOUBLE_EX(debug_info,"starttransfer_time",19,d_code);
    }
    if (curl_easy_getinfo(curl_handle, CURLINFO_REDIRECT_TIME, &d_code) == CURLE_OK)
    {
        DRUID_ADD_ASSOC_DOUBLE_EX(debug_info,"redirect_time",14,d_code);
    }

    DRUID_ADD_ASSOC_STRING_EX(debug_info,"request_json",13,request_json);

    zend_update_property(druid_ce, druid, ZEND_STRL(DRUID_PROPERTY_RESPONSE_INFO), debug_info TSRMLS_CC);

    return SUCCESS;
}

int druid_get_contents(zval *druid, char *request_json, struct druidCurlResult *result TSRMLS_DC)
{
    CURL *curl_handle;
    CURLcode res;
    char *url;
    char   *s_code;
    long    l_code;
    double  d_code;

    char err_str[CURL_ERROR_SIZE + 1];
    struct druidCurlResult tmp;

    curl_global_init(CURL_GLOBAL_ALL);

    curl_handle = curl_easy_init();

    if (!curl_handle)
    {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "curl init failed\n");
        return FAILURE;
    }

    result->size = 0;
    result->memory =malloc(1);

    url = druid_get_host(druid TSRMLS_CC);

    memset(err_str, 0, CURL_ERROR_SIZE + 1);

    tmp.memory = malloc(1);
    tmp.size = 0;


    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, druid_curl_callback);

    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA,(void *) &tmp);

    curl_easy_setopt(curl_handle,CURLOPT_FOLLOWLOCATION,1);

    curl_easy_setopt(curl_handle, CURLOPT_ERRORBUFFER,       err_str);

    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS,        1);
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE,           0);

    curl_easy_setopt(curl_handle, CURLOPT_DNS_USE_GLOBAL_CACHE, 1);
    curl_easy_setopt(curl_handle, CURLOPT_DNS_CACHE_TIMEOUT, 5);
    curl_easy_setopt(curl_handle, CURLOPT_MAXREDIRS, 20);
    curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, 10);
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 30);

    struct curl_slist  *slist = NULL;
    slist = curl_slist_append(slist, DRUID_CONTENT_TYPE);
    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, slist);

    curl_easy_setopt(curl_handle,CURLOPT_URL,url);
    curl_easy_setopt(curl_handle,CURLOPT_POST,1);

    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, request_json);
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, strlen(request_json));

    res = curl_easy_perform(curl_handle);

    if(res != CURLE_OK)
    {
        err_str[CURL_ERROR_SIZE] = 0;

        zend_update_property_long(druid_ce, druid, ZEND_STRL(DRUID_PROPERTY_CURL_ERR_NO), res TSRMLS_CC);
        zend_update_property_stringl(druid_ce, druid, ZEND_STRL(DRUID_PROPERTY_CURL_ERR_STR), err_str, CURL_ERROR_SIZE TSRMLS_CC);

        curl_easy_cleanup(curl_handle);
        curl_global_cleanup();
        return FAILURE;
    }

    result->size = tmp.size;
    result->memory = malloc(tmp.size +1);
    strcpy(result->memory,tmp.memory);

    if (curl_easy_getinfo(curl_handle, CURLINFO_HTTP_CODE, &l_code) == CURLE_OK)
    {
        zend_update_property_long(druid_ce, druid, ZEND_STRL(DRUID_PROPERTY_RESPONSE_CODE), l_code TSRMLS_CC);
    }

    if (DRUID_G(debug))
    {
        druid_get_debug_info(druid,curl_handle,request_json TSRMLS_CC);
    }

    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();

    return SUCCESS;
}