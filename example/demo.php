<?php
/**
 * @author neeke@php.net
 * Date: 16/12/21 下午8:38
 */

try {

    $Druid = Druid::getInstance();
    $Druid->debugWitch(TRUE);

    /**
     * you can also use default host with druid.host in php.ini/druid.ini
     */
    $aHosts = array("http://10.0.3.46:9082/druid/v2/", "http://10.0.3.46:9082/druid/v2/");
    $Druid->setDruidHosts($aHosts);
var_dump($Druid);
    /**
     * demo 1
     *
     * use tpl build request json
     */
    $Druid->setTplPath(__DIR__ . '/tpl');
    $result_1 = $Druid->getDataByTpl('request_tpl_demo.json', array('@startTimeToEndTime@' => '["2016-12-10T14:06:00.000Z/2016-12-22T14:36:00.000Z"]'));


    /**
     * demo 2
     *
     * use full request json
     */
    $result_2 = $Druid->getData(file_get_contents(__DIR__ . '/tpl/request_full_demo.json'));

    var_dump($result_1);

} catch (Exception $e) {
    var_dump($e->getCode(), $e->getMessage(), $Druid->getDebugInfo());
}