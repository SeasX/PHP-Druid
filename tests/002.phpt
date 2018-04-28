--TEST--
Check for PHP-Druid constant property
--SKIPIF--
<?php if (!extension_loaded("druid")) print "skip";?>
--FILE--
<?php
var_dump(is_string(Druid::DRUID_VERSION) && strlen(Druid::DRUID_VERSION) > 1);
var_dump(is_string(Druid::DRUID_CONTENT_TYPE) && strlen(Druid::DRUID_CONTENT_TYPE) > 1);
var_dump(is_string(Druid::DRUID_INSTANCE_DEFAULT) && strlen(Druid::DRUID_INSTANCE_DEFAULT) > 1);
?>
--EXPECT--
bool(true)
bool(true)
bool(true)

