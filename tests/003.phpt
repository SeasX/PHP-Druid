--TEST--
Check for PHP-Druid getInstance function
--SKIPIF--
<?php if (!extension_loaded("druid")) print "skip";?>
--FILE--
<?php
$oDruidDefault = Druid::getInstance(Druid::DRUID_INSTANCE_DEFAULT);
var_dump(is_object($oDruidDefault));

$oDruidMy = Druid::getInstance('my');
var_dump(is_object($oDruidMy));

?>
--EXPECT--
bool(true)
bool(true)

