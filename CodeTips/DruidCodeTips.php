<?php

/**
 * @author neeke@php.net
 * Date: 16/12/13 下午9:23
 */
class Druid
{
    /**
     * @var Druid
     */
    static private $Druid = NULL;

    /**
     * @return Druid
     */
    static public function getInstance()
    {
        if (self::$Druid == NULL) {
            self::$Druid = new self();
        }

        return self::$Druid;
    }

    private function __construct()
    {

    }

    public function __destruct()
    {

    }

    private function __clone()
    {

    }

    private function __sleep()
    {

    }

    private function __wakeup()
    {

    }

    public function debugWitch($bool)
    {
        return TRUE;
    }

    public function setDruidHosts(array $array)
    {
        return TRUE;
    }

    public function setTplPath($sTplPath)
    {
        return TRUE;
    }

    public function getData($request_json, array $array = array())
    {
        return array();
    }

    public function getDataByTpl($tpl, array $array = array())
    {
        return array();
    }

    public function getDebugInfo()
    {
        return array();
    }

}