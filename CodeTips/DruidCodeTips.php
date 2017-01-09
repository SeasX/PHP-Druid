<?php

/**
 * @author neeke@php.net
 * Date: 16/12/13 下午9:23
 */
class Druid
{
    const DRUID_INSTANCE_DEFAULT = 'default';

    /**
     * @var Druid
     */
    static private $Druid = NULL;

    /**
     * @return Druid
     */
    static public function getInstance($instance_name)
    {
        if (self::$Druid == NULL) {
            self::$Druid = array();
        }

        if (!array_key_exists($instance_name, self::$Druid)) {
            self::$Druid[$instance_name] = new self();
        }

        return self::$Druid[$instance_name];
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

    public function setTplPath($tpl_path)
    {
        return TRUE;
    }

    public function getData($request_json, array $array = array())
    {
        return array();
    }

    public function getDataByTpl($request_json_tpl, array $array = array())
    {
        return array();
    }

    public function getDebugInfo()
    {
        return array();
    }

}