--TEST--
Check for PHP-Druid debugWitch function and getDebugWitch function
--SKIPIF--
<?php if (!extension_loaded("druid")) print "skip";?>
--FILE--
<?php
$oDruidDefault = Druid::getInstance(Druid::DRUID_INSTANCE_DEFAULT);
var_dump(is_object($oDruidDefault));
var_dump(is_callable(array($oDruidDefault,'debugWitch')));
var_dump($oDruidDefault->debugWitch(false));
var_dump($oDruidDefault->getDebugWitch());
var_dump($oDruidDefault->debugWitch(true));
var_dump($oDruidDefault->getDebugWitch());

$oDruidMy = Druid::getInstance('my');
var_dump(is_object($oDruidMy));
var_dump(is_callable(array($oDruidMy,'debugWitch')));
var_dump($oDruidMy->debugWitch(false));
var_dump($oDruidMy->getDebugWitch());
var_dump($oDruidMy->debugWitch(true));
var_dump($oDruidMy->getDebugWitch());

?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(false)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(false)
bool(true)
bool(true)

