<?php
/**
 * @author neeke@php.net
 * Date: 17/1/9 下午3:47
 */


$Druid_1 = Druid::getInstance('druid_1');
$Druid_2 = Druid::getInstance('druid_2');


$Druid_1->aa = TRUE;
$Druid_1->bb = TRUE;

$Druid_2->cc = TRUE;
$Druid_2->dd = TRUE;

echo '<pre>';
exit(var_dump($Druid_1, $Druid_2));