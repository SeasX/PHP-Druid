--TEST--
Check for PHP-Druid setTplPath function and getTplPath function
--SKIPIF--
<?php if (!extension_loaded("druid")) print "skip";?>
--FILE--
<?php
$sTplPathDefault = ini_get('druid.tpl_path');
$sTplPathMy = '/path/to/tpl_path';

$oDruidDefault = Druid::getInstance(Druid::DRUID_INSTANCE_DEFAULT);
var_dump(is_object($oDruidDefault));
var_dump(is_callable(array($oDruidDefault,'setTplPath')));
var_dump($oDruidDefault->getTplPath() == $sTplPathDefault);
var_dump($oDruidDefault->setTplPath($sTplPathMy));
var_dump($oDruidDefault->getTplPath() == $sTplPathMy);

$oDruidMy = Druid::getInstance('my');
var_dump(is_object($oDruidMy));
var_dump(is_callable(array($oDruidMy,'setTplPath')));
var_dump($oDruidMy->getTplPath() == $sTplPathDefault);
var_dump($oDruidMy->setTplPath($sTplPathMy));
var_dump($oDruidMy->getTplPath() == $sTplPathMy);

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

