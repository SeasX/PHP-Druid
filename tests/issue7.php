<?php
/**
 * @author neeke@php.net
 * Date: 17/1/9 下午3:47
 */
$Druid_1 = Druid::getInstance(Druid::DRUID_INSTANCE_DEFAULT);
$Druid_2 = Druid::getInstance('druid_2');

$Druid_1->aa = TRUE;
$Druid_1->debugWitch(TRUE);

$Druid_2->debugWitch(FALSE);
$Druid_2->bb = TRUE;

$Druid_3 = Druid::getInstance(Druid::DRUID_INSTANCE_DEFAULT);
$Druid_4 = Druid::getInstance('druid_2');

var_dump($Druid_1 == $Druid_3, $Druid_2 == $Druid_4);