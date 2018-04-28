<?php
/**
 * @author neeke@php.net
 * Date: 16/12/21 下午8:38
 */

try {

    $Druid_1 = Druid::getInstance(Druid::DRUID_INSTANCE_DEFAULT);
    $Druid_1->debugWitch(true);

    /**
     * you can also use default host with druid.host in php.ini/druid.ini
     */
    $aHosts = array("http://10.0.3.46:9082/druid/v2/", "http://10.0.3.46:9082/druid/v2/");
    $Druid_1->setDruidHosts($aHosts);

    /**
     * demo 1
     *
     * use tpl build request json
     */
    $Druid_1->setTplPath(__DIR__ . '/tpl');
    $result_1 = $Druid_1->getDataByTpl('request_tpl_demo.json', array('@startTimeToEndTime@' => '["2016-12-10T14:06:00.000Z/2016-12-27T14:36:00.000Z"]'));


    $Druid_2 = Druid::getInstance('druid_2_instance');
    /**
     * demo 2
     *
     * use full request json
     */
    $result_2 = $Druid_2->getData(file_get_contents(__DIR__ . '/tpl/request_full_demo.json'));

    var_dump($result_1, $result_2);
    var_dump($Druid_1, $Druid_2);
} catch (Exception $e) {
    var_dump($e->getCode(), $e->getMessage(), $Druid_1->getDebugInfo(), $Druid_2->getDebugInfo());
}