--TEST--
Check for PHP-Druid setDruidHosts function and getDruidHosts function
--SKIPIF--
<?php if (!extension_loaded("druid")) print "skip";?>
--FILE--
<?php
$aHosts = array('http://192.168.0.100:18082','http://10.0.1.15:18082');

$oDruidDefault = Druid::getInstance(Druid::DRUID_INSTANCE_DEFAULT);
var_dump(is_object($oDruidDefault));
var_dump(is_callable(array($oDruidDefault,'setDruidHosts')));
var_dump($oDruidDefault->getDruidHosts() == NULL);
var_dump($oDruidDefault->setDruidHosts($aHosts));
var_dump($oDruidDefault->getDruidHosts() == $aHosts);


$oDruidMy = Druid::getInstance('my');
var_dump(is_object($oDruidMy));
var_dump(is_callable(array($oDruidMy,'setDruidHosts')));
var_dump($oDruidMy->getDruidHosts() == NULL);
var_dump($oDruidMy->setDruidHosts($aHosts));
var_dump($oDruidMy->getDruidHosts() == $aHosts);

?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)

