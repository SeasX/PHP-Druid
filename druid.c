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
#define DRUID_ADD_ASSOC_LONG_EX(z, s, l, v) add_assoc_long_ex(&z, s, l, (long) v)
#define DRUID_ADD_ASSOC_DOUBLE_EX(z, s, l, v) add_assoc_double_ex(&z, s, l, (double) v)
#define DRUID_ADD_ASSOC_ZVAL_EX(z, s, l, zn) add_assoc_zval_ex(z, s, l, zn)
#define DRUID_ADD_ASSOC_ZVAL_EX_AND(z, s, l, zn) add_assoc_zval_ex(&z, s, l, zn)
#define DRUID_ADD_ASSOC_STRING_EX(a, k, l, s) add_assoc_string_ex(&a, k, l, s)
#define DRUID_ADD_NEXT_INDEX_STRING(a, s) add_next_index_string(a, s)
#define DRUID_ADD_NEXT_INDEX_STRINGL(a, s, l) add_next_index_stringl(a, s, l)
#define DRUID_ZEND_HASH_GET_CURRENT_KEY(ht, key, idx) zend_hash_get_current_key(ht, key, idx)
#define DRUID_ZEND_HASH_INDEX_UPDATE(ht, h, pData, nDataSize, pDest)  zend_hash_index_update_ptr(ht, h, pData)

#define DRUID_ZEND_READ_PROPERTY(ce,z,zl) zend_read_property(ce, z, zl, 1, NULL)
#define DRUID_ZEND_UPDATE_PROPERTY(ce,z,zl,zn) zend_update_property(ce, z, zl, zn)
#define DRUID_ZEND_UPDATE_STATIC_PROPERTY(ce,zl,zn) zend_update_static_property(ce, zl, &zn)
#define DRUID_ZEND_UPDATE_PROPERTY_LONG(ce,z,zl,zn) zend_update_property_long(ce,z,zl,zn)

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
#define DRUID_ADD_ASSOC_ZVAL_EX_AND(z, s, l, zn) add_assoc_zval_ex(z, s, l, zn)
#define DRUID_ADD_ASSOC_STRING_EX(a, k, l, s) add_assoc_string_ex(a, k, l, s, 1)
#define DRUID_ADD_NEXT_INDEX_STRING(a, s) add_next_index_string(a, s, 1)
#define DRUID_ADD_NEXT_INDEX_STRINGL(a, s, l) add_next_index_stringl(a, s, l, 1)
#define DRUID_ZEND_HASH_GET_CURRENT_KEY(ht, key, idx) zend_hash_get_current_key(ht, key, idx, 0)
#define DRUID_ZEND_HASH_INDEX_UPDATE(ht, h, pData, nDataSize, pDest)  zend_hash_index_update(ht, h, pData, nDataSize, pDest)

#define DRUID_ZEND_READ_PROPERTY(ce,z,zl) zend_read_property(ce, z, zl, 1 TSRMLS_CC)
#define DRUID_ZEND_UPDATE_PROPERTY(ce,z,zl,zn) zend_update_property(ce, z, zl, zn TSRMLS_CC)
#define DRUID_ZEND_UPDATE_STATIC_PROPERTY(ce,zl,zn) zend_update_static_property(ce, zl, zn TSRMLS_CC)
#define DRUID_ZEND_UPDATE_PROPERTY_LONG(ce,z,zl,zn) zend_update_property_long(ce,z,zl,zn TSRMLS_CC)
#endif


ZEND_DECLARE_MODULE_GLOBALS(druid)

static zend_module_dep druid_deps[] =
{
    ZEND_MOD_REQUIRED("json")
#ifdef ZEND_MOD_END
    ZEND_MOD_END
#else
    {NULL, NULL, NULL}
#endif
};

static int le_druid;

const zend_function_entry druid_functions[] =
{
#ifdef PHP_FE_END
    PHP_FE_END
#else
    {NULL, NULL, NULL}
#endif
};

ZEND_BEGIN_ARG_INFO_EX(druid_void_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(druid_getinstance_arginfo, 0, 0, 1)
ZEND_ARG_INFO(0, instance_name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(druid_debug_arginfo, 0, 0, 1)
ZEND_ARG_INFO(0, debug)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(druid_setdruidhosts_arginfo, 0, 0, 1)
ZEND_ARG_INFO(0, hosts)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(druid_settplpath_arginfo, 0, 0, 1)
ZEND_ARG_INFO(0, tpl_path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(druid_getdata_arginfo, 0, 0, 1)
ZEND_ARG_INFO(0, request_json)
ZEND_ARG_INFO(0, content_array)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(druid_getdatabytpl_arginfo, 0, 0, 1)
ZEND_ARG_INFO(0, request_json_tpl)
ZEND_ARG_INFO(0, content_array)
ZEND_END_ARG_INFO()


const zend_function_entry druid_methods[] =
{
    PHP_ME(DRUID_NAME, __construct,             druid_void_arginfo, ZEND_ACC_CTOR|ZEND_ACC_PRIVATE)
    PHP_ME(DRUID_NAME, __clone,                 NULL, ZEND_ACC_PRIVATE)
    PHP_ME(DRUID_NAME, __sleep,                 NULL, ZEND_ACC_PRIVATE)
    PHP_ME(DRUID_NAME, __wakeup,                NULL, ZEND_ACC_PRIVATE)
    PHP_ME(DRUID_NAME, __destruct,              NULL, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
    PHP_ME(DRUID_NAME, getInstance,    	        druid_getinstance_arginfo, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(DRUID_NAME, debugWitch,    	        druid_debug_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(DRUID_NAME, setDruidHosts,    	    druid_setdruidhosts_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(DRUID_NAME, setTplPath,    	        druid_settplpath_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(DRUID_NAME, getData,    	            druid_getdata_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(DRUID_NAME, getDataByTpl,    	    druid_getdatabytpl_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(DRUID_NAME, getDebugInfo,    	    NULL, ZEND_ACC_PUBLIC)
    {
        NULL, NULL, NULL
    }
};

zend_module_entry druid_module_entry =
{
    STANDARD_MODULE_HEADER_EX,
    NULL,
    druid_deps,
    DRUID_NAME,
    druid_functions,
    PHP_MINIT(druid),
    PHP_MSHUTDOWN(druid),
    PHP_RINIT(druid),
    PHP_RSHUTDOWN(druid),
    PHP_MINFO(druid),
    PHP_DRUID_VERSION,
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

STD_PHP_INI_ENTRY("druid.curl_dns_cache_timeout", "1", PHP_INI_ALL, OnUpdateLongGEZero, curl_dns_cache_timeout, zend_druid_globals, druid_globals)
STD_PHP_INI_ENTRY("druid.curl_connect_timeout", "3", PHP_INI_ALL, OnUpdateLongGEZero, curl_connect_timeout, zend_druid_globals, druid_globals)
STD_PHP_INI_ENTRY("druid.curl_timeout", "5", PHP_INI_ALL, OnUpdateLongGEZero, curl_timeout, zend_druid_globals, druid_globals)
PHP_INI_END()

static void php_druid_init_globals(zend_druid_globals *druid_globals)
{

}

PHP_MINIT_FUNCTION(druid)
{
    zend_class_entry ce;

    ZEND_INIT_MODULE_GLOBALS(druid, php_druid_init_globals, NULL);
    REGISTER_INI_ENTRIES();

    INIT_CLASS_ENTRY(ce, DRUID_NAME, druid_methods);

#if PHP_VERSION_ID >= 70000
    druid_ce = zend_register_internal_class_ex(&ce, NULL);
#else
    druid_ce = zend_register_internal_class_ex(&ce, NULL, NULL TSRMLS_CC);
#endif

    zend_declare_class_constant_stringl(druid_ce,ZEND_STRL("DRUID_CONTENT_TYPE"),ZEND_STRL(DRUID_CONTENT_TYPE) TSRMLS_CC);
    zend_declare_class_constant_stringl(druid_ce,ZEND_STRL("DRUID_INSTANCE_DEFAULT"),ZEND_STRL(DRUID_INSTANCE_DEFAULT) TSRMLS_CC);

    zend_declare_property_null(druid_ce, ZEND_STRL(DRUID_NAME), ZEND_ACC_STATIC | ZEND_ACC_PROTECTED TSRMLS_CC);

    zend_declare_property_null(druid_ce, ZEND_STRL(DRUID_PROPERTY_TPL_PATH), ZEND_ACC_PROTECTED TSRMLS_CC);

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
    int argc = ZEND_NUM_ARGS();
    char *instance_name;
    zval *get_instance_array = NULL;

#if PHP_VERSION_ID >= 70000
    size_t  instance_name_len;
    zval *ppzval = NULL;
    zval set_instance_array;
#else
    int  instance_name_len;
    zval **ppzval = NULL;
    zval *set_instance_array;
#endif

    zval *instance;

//In php7 , this params instance_name can not be empty.
#if PHP_VERSION_ID >= 70000
    if (zend_parse_parameters(argc TSRMLS_CC, "s", &instance_name, &instance_name_len) == FAILURE)
    {
        zend_throw_exception(php_com_exception_class_entry,"The instance_name can not be empty,you can use Druid::DRUID_INSTANCE_DEFAULT",999 TSRMLS_CC);
        RETURN_FALSE;
    }
#else
    if (zend_parse_parameters(argc TSRMLS_CC, "|s", &instance_name, &instance_name_len) == FAILURE)
    {
        RETURN_FALSE;
    }
#endif

    if (argc < 1)
    {
        instance_name     = DRUID_INSTANCE_DEFAULT;
        instance_name_len = DRUID_INSTANCE_DEFAULT_LEN;
    }

    get_instance_array = zend_read_static_property(druid_ce, ZEND_STRL(DRUID_NAME), 1 TSRMLS_CC);

    if (get_instance_array && Z_TYPE_P(get_instance_array) == IS_ARRAY)
    {

#if PHP_VERSION_ID >= 70000
        if ((ppzval = zend_hash_str_find(Z_ARRVAL_P(get_instance_array),instance_name,instance_name_len)) != NULL)
        {
            RETURN_ZVAL(ppzval, 1, 0);
        }
        else
        {
            goto initInstance;
        }
#else
        if (zend_hash_find(Z_ARRVAL_P(get_instance_array), ZEND_STRL(instance_name), (void **)&ppzval) == SUCCESS )
        {
            RETURN_ZVAL(*ppzval, 1, 0);
        }
        else
        {
            goto initInstance;
        }
#endif
    }
    else
    {
#if PHP_VERSION_ID >= 70000
        array_init(&set_instance_array);
#else
        MAKE_STD_ZVAL(set_instance_array);
        array_init(set_instance_array);
#endif
        goto initInstance;
    }

initInstance:
#if PHP_VERSION_ID >= 70000
    instance = getThis();
    zval re_instance;

    if (!instance) {
        ZVAL_NULL(&re_instance);
        instance = &re_instance;
    }
#else
    MAKE_STD_ZVAL(instance);
#endif

    object_init_ex(instance, druid_ce);

    DRUID_ZEND_UPDATE_PROPERTY_LONG(druid_ce, instance, ZEND_STRL(DRUID_PROPERTY_CURL_ERR_NO), 0);
    zend_update_property_string(druid_ce, instance, ZEND_STRL(DRUID_PROPERTY_CURL_ERR_STR), "" TSRMLS_CC);

    zend_update_property_string(druid_ce, instance, ZEND_STRL(DRUID_PROPERTY_TPL_PATH), DRUID_G(tpl_path) TSRMLS_CC);

    DRUID_ZEND_UPDATE_PROPERTY_LONG(druid_ce, instance, ZEND_STRL(DRUID_PROPERTY_RESPONSE_CODE), 0);
    zend_update_property_null(druid_ce, instance, ZEND_STRL(DRUID_PROPERTY_RESPONSE_INFO) TSRMLS_CC);

    zend_update_property_null(druid_ce, instance, ZEND_STRL(DRUID_PROPERTY_HOSTS) TSRMLS_CC);
    zend_update_property_bool(druid_ce, instance, ZEND_STRL(DRUID_PROPERTY_HOST_RAND), 0 TSRMLS_CC);

    if (get_instance_array && IS_ARRAY == Z_TYPE_P(get_instance_array))
    {
        DRUID_ADD_ASSOC_ZVAL_EX(get_instance_array,instance_name,instance_name_len,instance);
        zend_update_static_property(druid_ce, ZEND_STRL(DRUID_NAME), get_instance_array TSRMLS_CC);
    }
    else
    {
        DRUID_ADD_ASSOC_ZVAL_EX_AND(set_instance_array,instance_name,instance_name_len,instance);
        DRUID_ZEND_UPDATE_STATIC_PROPERTY(druid_ce, ZEND_STRL(DRUID_NAME), set_instance_array);

        zval_ptr_dtor(&set_instance_array);
    }

    RETURN_ZVAL(instance, 1, 0);
}


PHP_METHOD(DRUID_NAME, __construct)
{
}

PHP_METHOD(DRUID_NAME,__destruct)
{
    zend_update_property_null(druid_ce, getThis(), ZEND_STRL(DRUID_PROPERTY_RESPONSE_INFO) TSRMLS_CC);
    zend_update_property_null(druid_ce, getThis(), ZEND_STRL(DRUID_PROPERTY_TPL_PATH) TSRMLS_CC);
    zend_update_property_null(druid_ce, getThis(), ZEND_STRL(DRUID_PROPERTY_HOSTS) TSRMLS_CC);

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
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &debug) == FAILURE)
    {
        RETURN_FALSE;
    }

    DRUID_G(debug) = debug;

    RETURN_TRUE;
}

PHP_METHOD(DRUID_NAME, setDruidHosts)
{
    int argc = ZEND_NUM_ARGS();
    zval *hosts;

    if (zend_parse_parameters(argc TSRMLS_CC, "z", &hosts) == FAILURE)
    {
        RETURN_FALSE;
    }

    if (Z_TYPE_P(hosts) != IS_ARRAY)
    {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "The argument is not an array");
        RETURN_FALSE;
    }

    DRUID_ZEND_UPDATE_PROPERTY(druid_ce, getThis(), ZEND_STRL(DRUID_PROPERTY_HOSTS), hosts);
    zend_update_property_bool(druid_ce, getThis(), ZEND_STRL(DRUID_PROPERTY_HOST_RAND), 1 TSRMLS_CC);

    RETURN_TRUE;
}

PHP_METHOD(DRUID_NAME, setTplPath)
{
    char *tpl_path;

#if PHP_VERSION_ID >= 70000
    size_t  tpl_path_len;
#else
    int  tpl_path_len;
#endif

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &tpl_path, &tpl_path_len) == FAILURE)
    {
        RETURN_FALSE;
    }

    zend_update_property_string(druid_ce, getThis(), ZEND_STRL(DRUID_PROPERTY_TPL_PATH), tpl_path TSRMLS_CC);

    RETURN_TRUE;
}

/*Just used by PHP7*/
#if PHP_VERSION_ID >= 70000
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
            (void)num_key;
        }
        else
        {
            zend_string *s = zval_get_string(entry);

            if (strstr(str,ZSTR_VAL(str_key)))
            {
                tmp = strreplace(tmp, ZSTR_VAL(str_key), ZSTR_VAL(s), strlen(str));
            }

            zend_string_release(s);
            zend_string_release(str_key);
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

    char *request,*request_json;
#if PHP_VERSION_ID >= 70000
    size_t  request_len;
#else
    int  request_len;
#endif
    zval *content;

    if (zend_parse_parameters(argc TSRMLS_CC, "s|z", &request, &request_len, &content) == FAILURE)
    {
        RETURN_FALSE;
    }

    if (argc > 1 && Z_TYPE_P(content) != IS_ARRAY)
    {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "The second argument is not an array");
        RETURN_FALSE;
    }

    if (argc > 1)
    {
        request_json = php_strtr_array(request,request_len,HASH_OF(content));
    }
    else
    {
        request_json = request;
    }

    druid_getApi(return_value, getThis(), request_json TSRMLS_CC);

    if (argc > 1)
    {
        efree(request_json);
    }
}

PHP_METHOD(DRUID_NAME,getDataByTpl)
{
    int argc = ZEND_NUM_ARGS();

    char *tpl,*request,*request_json,*filename;
    zval *tpl_path;
    zval *content;

#if PHP_VERSION_ID >= 70000
    size_t  tpl_len;
#else
    int  tpl_len;
#endif

    if (zend_parse_parameters(argc TSRMLS_CC, "s|z", &tpl, &tpl_len, &content) == FAILURE)
    {
        RETURN_FALSE;
    }

    if (argc > 1 && Z_TYPE_P(content) != IS_ARRAY)
    {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "The second argument is not an array");
        RETURN_FALSE;
    }

    tpl_path = DRUID_ZEND_READ_PROPERTY(druid_ce, getThis(), ZEND_STRL(DRUID_PROPERTY_TPL_PATH));

    spprintf(&filename, 0, "%s/%s", Z_STRVAL_P(tpl_path),tpl);
    request = druid_file_get_contents_by_tpl(filename TSRMLS_CC);
    efree(filename);

    if (request == NULL)
    {
        RETURN_FALSE;
    }

    if (argc > 1)
    {
        request_json = php_strtr_array(request,strlen(request),HASH_OF(content));
    }
    else
    {
        request_json = request;
    }

    druid_getApi(return_value, getThis(), request_json TSRMLS_CC);

    efree(request);
    if (argc > 1)
    {
        efree(request_json);
    }
}

PHP_METHOD(DRUID_NAME,getDebugInfo)
{
    zval *info = DRUID_ZEND_READ_PROPERTY(druid_ce, getThis(), ZEND_STRL(DRUID_PROPERTY_RESPONSE_INFO));
    RETVAL_ZVAL(info, 1, 0);
}


char *druid_file_get_contents_by_tpl(char *filename TSRMLS_DC)
{
    php_stream *stream;
    zval *zcontext = NULL;
    php_stream_context *context = NULL;

#if PHP_VERSION_ID >= 70000
    zend_string *contents;
#else
    char *contents;
    int len;
#endif

    context = php_stream_context_from_zval(zcontext, 0);

    stream = php_stream_open_wrapper_ex(filename, "rb",0 | REPORT_ERRORS, NULL, context);
    if (!stream)
    {
        return NULL;
    }

#if PHP_VERSION_ID >= 70000
    if ((contents = php_stream_copy_to_mem(stream, PHP_STREAM_COPY_ALL, 0)) != NULL)
    {
        php_stream_close(stream);
        return ZSTR_VAL(contents);
#else
    if ((len = php_stream_copy_to_mem(stream, &contents, PHP_STREAM_COPY_ALL, 0)) > 0)
    {
        php_stream_close(stream);
        return contents;
    }
    else if (len == 0)
    {
        php_stream_close(stream);
        return NULL;
#endif
    }
    else
    {
        php_stream_close(stream);
        return NULL;
    }

    php_stream_close(stream);
}

static void druid_getApi(zval *return_value, zval *druid, char *request_json TSRMLS_DC)
{
    zval *err_str,*err_no;
    zval *response_code;

    struct druidCurlResult curlResult;

    if(druid_get_contents(druid,request_json,&curlResult TSRMLS_CC) != SUCCESS)
    {
        err_str = DRUID_ZEND_READ_PROPERTY(druid_ce, druid, ZEND_STRL(DRUID_PROPERTY_CURL_ERR_STR));
        err_no = DRUID_ZEND_READ_PROPERTY(druid_ce, druid, ZEND_STRL(DRUID_PROPERTY_CURL_ERR_NO));

        free(curlResult.memory);
        zend_throw_exception(php_com_exception_class_entry,Z_STRVAL_P(err_str),Z_LVAL_P(err_no) TSRMLS_CC);
        RETURN_FALSE;
    }

    response_code = DRUID_ZEND_READ_PROPERTY(druid_ce, druid, ZEND_STRL(DRUID_PROPERTY_RESPONSE_CODE));
    if (Z_LVAL_P(response_code) > DRUID_RESPONSE_CODE_ERROR_BAR)
    {
        zend_throw_exception(php_com_exception_class_entry,curlResult.memory,Z_LVAL_P(response_code) TSRMLS_CC);
        free(curlResult.memory);
        RETURN_FALSE;
    }

    php_json_decode(return_value, curlResult.memory, (long)curlResult.size, 1, 512 TSRMLS_CC);
    free(curlResult.memory);
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
    int hash_sum = 0;
    int step = 0;
    zval *host_rand,*hosts;
    char *host_result;
#if PHP_VERSION_ID >= 70000

    zend_ulong num_key;
    zend_string *str_key;
    zval *entry;

#else

    zval **entry;
    HashPosition pos;

#endif

    host_rand = DRUID_ZEND_READ_PROPERTY(druid_ce, druid, ZEND_STRL(DRUID_PROPERTY_HOST_RAND));

#if PHP_VERSION_ID >= 70000

    if (Z_TYPE_P(host_rand) == IS_TRUE)
    {
        hosts = DRUID_ZEND_READ_PROPERTY(druid_ce, druid, ZEND_STRL(DRUID_PROPERTY_HOSTS));
        hash_sum = zend_hash_num_elements(HASH_OF(hosts));
        ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(hosts), num_key, str_key, entry)
        {
            (void)num_key;
            (void)str_key;
            step++;
            zend_string *s = zval_get_string(entry);

            if (druid_php_rand(TSRMLS_C) == SUCCESS || step == hash_sum)
            {
                host_result = estrdup(ZSTR_VAL(s));
                zend_string_release(s);
                return host_result;
            }
            else
            {
                zend_string_release(s);
            }
        }
        ZEND_HASH_FOREACH_END();
    }

#else

    if (Z_LVAL_P(host_rand) == 1)
    {

        hosts = DRUID_ZEND_READ_PROPERTY(druid_ce, druid, ZEND_STRL(DRUID_PROPERTY_HOSTS));

        hash_sum = zend_hash_num_elements(HASH_OF(hosts));

        for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(hosts), &pos);
                zend_hash_get_current_data_ex(Z_ARRVAL_P(hosts), (void **)&entry, &pos) == SUCCESS;
                zend_hash_move_forward_ex(Z_ARRVAL_P(hosts), &pos)
            )
        {
            step++;

            if (Z_TYPE_PP(entry) == IS_ARRAY || Z_TYPE_PP(entry) == IS_OBJECT || Z_TYPE_PP(entry) == IS_LONG)
            {
                continue;
            }

            if (druid_php_rand(TSRMLS_C) == SUCCESS || step == hash_sum)
            {
                host_result = estrdup(Z_STRVAL_PP(entry));

                return host_result;
            }
        }
    }
#endif

    host_result = estrdup(DRUID_G(host));
    return host_result;
}

int druid_get_debug_info(zval *druid,CURL *curl_handle,char *request_json TSRMLS_DC)
{
    char   *s_code;
    long    l_code;
    double  d_code;
    curl_version_info_data *info;

#if PHP_VERSION_ID >= 70000

    zval debug_info;
    array_init(&debug_info);

#else

    zval *debug_info;
    MAKE_STD_ZVAL(debug_info);
    array_init(debug_info);

#endif

    info = curl_version_info(CURLVERSION_NOW);
    DRUID_ADD_ASSOC_STRING_EX(debug_info, "version", 8, (char *)info->version);
    DRUID_ADD_ASSOC_STRING_EX(debug_info,"ssl_version",12,(char *)info->ssl_version);

    if (curl_easy_getinfo(curl_handle, CURLINFO_EFFECTIVE_URL, &s_code) == CURLE_OK)
    {
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
#if PHP_VERSION_ID >= 70000
    DRUID_ZEND_UPDATE_PROPERTY(druid_ce, druid, ZEND_STRL(DRUID_PROPERTY_RESPONSE_INFO), &debug_info);
#else
    DRUID_ZEND_UPDATE_PROPERTY(druid_ce, druid, ZEND_STRL(DRUID_PROPERTY_RESPONSE_INFO), debug_info);
#endif

    zval_ptr_dtor(&debug_info);

    return SUCCESS;
}

int druid_get_contents(zval *druid, char *request_json, struct druidCurlResult *result TSRMLS_DC)
{
    CURL *curl_handle;
    CURLcode res;
    char *url;
    long    l_code;
    struct curl_slist  *slist = NULL;

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

    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION,1);

    curl_easy_setopt(curl_handle, CURLOPT_ERRORBUFFER,       err_str);

    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS,        1);
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE,           0);
    curl_easy_setopt(curl_handle, CURLOPT_MAXREDIRS, 5);
    curl_easy_setopt(curl_handle, CURLOPT_DNS_USE_GLOBAL_CACHE, 1);

    curl_easy_setopt(curl_handle, CURLOPT_DNS_CACHE_TIMEOUT, DRUID_G(curl_dns_cache_timeout));
    curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, DRUID_G(curl_connect_timeout));
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, DRUID_G(curl_timeout));

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

        DRUID_ZEND_UPDATE_PROPERTY_LONG(druid_ce, druid, ZEND_STRL(DRUID_PROPERTY_CURL_ERR_NO), res);
        zend_update_property_stringl(druid_ce, druid, ZEND_STRL(DRUID_PROPERTY_CURL_ERR_STR), err_str, CURL_ERROR_SIZE TSRMLS_CC);

        curl_easy_cleanup(curl_handle);
        curl_global_cleanup();
        efree(url);

        return FAILURE;
    }

    result->size = tmp.size;
    result->memory = malloc(tmp.size +1);
    strcpy(result->memory,tmp.memory);

    if (curl_easy_getinfo(curl_handle, CURLINFO_HTTP_CODE, &l_code) == CURLE_OK)
    {
        DRUID_ZEND_UPDATE_PROPERTY_LONG(druid_ce, druid, ZEND_STRL(DRUID_PROPERTY_RESPONSE_CODE), l_code);
    }

    if (DRUID_G(debug))
    {
        druid_get_debug_info(druid,curl_handle,request_json TSRMLS_CC);
    }

    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
    efree(url);

    return SUCCESS;
}
