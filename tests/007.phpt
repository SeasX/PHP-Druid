--TEST--
Check for PHP-Druid getData function
--SKIPIF--
<?php if (!extension_loaded("druid")) print "skip";?>
--FILE--
<?php
$aHostsDefault = array('http://php-druid-test-get-data-1.php-druid-test-get-data.com','http://php-druid-test-get-data-2.php-druid-test-get-data.com');

$request_json = '{"queryType":"timeseries","dataSource":"agentTopic-1m","granularity":{"type":"duration","duration":"300000"},"intervals":["2016-12-10T14:06:00.000Z/2016-12-18T14:36:00.000Z"],"filter":{"type":"and","fields":[{"type":"selector","dimension":"accountId","value":"107"},{"type":"selector","dimension":"hostId","value":"10675435376182376"},{"type":"selector","dimension":"serviceType","value":"101"}]},"aggregations":[{"type":"doubleSum","name":"oneMinute","fieldName":"oneMinute"},{"type":"doubleSum","name":"fiveMinute","fieldName":"fiveMinute"},{"type":"doubleSum","name":"fifteenMinute","fieldName":"fifteenMinute"},{"type":"longSum","name":"count","fieldName":"count"}],"postAggregations":[{"type":"arithmetic","name":"oneMinuteAvg","fn":"/","fields":[{"type":"fieldAccess","name":"oneMinute","fieldName":"oneMinute"},{"type":"fieldAccess","name":"count","fieldName":"count"}]},{"type":"arithmetic","name":"fiveMinuteAvg","fn":"/","fields":[{"type":"fieldAccess","name":"fiveMinute","fieldName":"fiveMinute"},{"type":"fieldAccess","name":"count","fieldName":"count"}]},{"type":"arithmetic","name":"fifteenMinuteAvg","fn":"/","fields":[{"type":"fieldAccess","name":"fifteenMinute","fieldName":"fifteenMinute"},{"type":"fieldAccess","name":"count","fieldName":"count"}]}]}';

$oDruidDefault = Druid::getInstance(Druid::DRUID_INSTANCE_DEFAULT);
var_dump(is_object($oDruidDefault));
var_dump(is_callable(array($oDruidDefault,'getData')));
var_dump($oDruidDefault->setDruidHosts($aHostsDefault));

try {
    $result = $oDruidDefault->getData($request_json, array('dataSource'=>'aaa'));
    var_dump($result == NULL || is_array($result));
    var_dump($oDruidDefault->getDebugInfo() == NULL || is_array($oDruidDefault->getDebugInfo()));
} catch (Exception $e) {
    var_dump(is_string($e->getMessage()));
    var_dump($e->getCode() > 0);
}

?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
