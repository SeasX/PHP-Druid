<?php
/**
 * @author neeke@php.net
 * Date: 16/12/16 下午4:52
 */


try {

    $request_json = '{"queryType":"timeseries","dataSource":"agentTopic-1m","granularity":{"type":"duration","duration":"300000"},"intervals":["2016-12-10T14:06:00.000Z/2016-12-18T14:36:00.000Z"],"filter":{"type":"and","fields":[{"type":"selector","dimension":"accountId","value":"107"},{"type":"selector","dimension":"hostId","value":"10675435376182376"},{"type":"selector","dimension":"serviceType","value":"101"}]},"aggregations":[{"type":"doubleSum","name":"oneMinute","fieldName":"oneMinute"},{"type":"doubleSum","name":"fiveMinute","fieldName":"fiveMinute"},{"type":"doubleSum","name":"fifteenMinute","fieldName":"fifteenMinute"},{"type":"longSum","name":"count","fieldName":"count"}],"postAggregations":[{"type":"arithmetic","name":"oneMinuteAvg","fn":"/","fields":[{"type":"fieldAccess","name":"oneMinute","fieldName":"oneMinute"},{"type":"fieldAccess","name":"count","fieldName":"count"}]},{"type":"arithmetic","name":"fiveMinuteAvg","fn":"/","fields":[{"type":"fieldAccess","name":"fiveMinute","fieldName":"fiveMinute"},{"type":"fieldAccess","name":"count","fieldName":"count"}]},{"type":"arithmetic","name":"fifteenMinuteAvg","fn":"/","fields":[{"type":"fieldAccess","name":"fifteenMinute","fieldName":"fifteenMinute"},{"type":"fieldAccess","name":"count","fieldName":"count"}]}]}';

    $druid = Druid::getInstance(Druid::DRUID_INSTANCE_DEFAULT);

    $array = array("bbbbb", "cccccc", "dddddd", "eeeeee");

    $druid->setDruidHosts($array);

    var_dump($druid);

    var_dump($druid->getData($request_json, array('dataSource' => 'aaa')));

    var_dump($druid);

    var_dump($druid->getDebugInfo());
} catch (Exception $e) {
    echo '<pre>';
    var_dump($druid->getDebugInfo());
    exit(var_dump($e->getCode(), $e->getMessage()));
}
