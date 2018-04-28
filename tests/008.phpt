--TEST--
Check for PHP-Druid getDataByTpl function and getDebugInfo function
--SKIPIF--
<?php if (!extension_loaded("druid")) print "skip";?>
--FILE--
<?php
$aHostsDefault = array('http://php.net','http://pecl.php.net');
$sTplPath = __DIR__ . '/../example/tpl';

$oDruidDefault = Druid::getInstance(Druid::DRUID_INSTANCE_DEFAULT);
var_dump(is_object($oDruidDefault));
var_dump(is_callable(array($oDruidDefault,'getDataByTpl')));
var_dump($oDruidDefault->debugWitch(true));
var_dump($oDruidDefault->setDruidHosts($aHostsDefault));
var_dump($oDruidDefault->setTplPath($sTplPath));

$result = NULL;
$debugInfo = NULL;

try {

    $result = $oDruidDefault->getDataByTpl('request_tpl_demo.json', array('@startTimeToEndTime@' => '["2016-12-10T14:06:00.000Z/2016-12-27T14:36:00.000Z"]'));
    $debugInfo = $oDruidDefault->getDebugInfo();

    var_dump($result == NULL || is_array($result));
    var_dump($debugInfo == NULL || is_array($debugInfo));
} catch (Exception $e) {
    var_dump(is_string($e->getMessage()));
    var_dump($e->getCode() > 0);
}


if (is_array($debugInfo))
{
    var_dump(array_key_exists("url",$debugInfo) && in_array($debugInfo["url"],$aHostsDefault));
}
else
{
    var_dump(true);
}

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

