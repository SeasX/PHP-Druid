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
    static private $Druid = null;

    /**
     * @return Druid
     */
    static public function getInstance($instance_name)
    {
        if (self::$Druid == null) {
            self::$Druid = array();
        }

        if (!array_key_exists($instance_name, self::$Druid)) {
            self::$Druid[$instance_name] = new self();
        }

        return self::$Druid[$instance_name];
    }

    /**
     * Druid constructor.
     */
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

    /**
     * @param $bool
     *
     * @return bool
     */
    public function debugWitch($bool)
    {
        return true;
    }

    /**
     * @return bool
     */
    public function getDebugWitch()
    {
        return true;
    }

    /**
     * @param array $array
     *
     * @return bool
     */
    public function setDruidHosts(array $array)
    {
        return true;
    }

    /**
     * @return array
     */
    public function getDruidHosts()
    {
        return array();
    }

    /**
     * @param $tpl_path
     *
     * @return bool
     */
    public function setTplPath($tpl_path)
    {
        return true;
    }

    /**
     * @return string
     */
    public function getTplPath()
    {
        return '';
    }

    /**
     * @param       $request_json
     * @param array $array
     *
     * @return array
     */
    public function getData($request_json, array $array = array())
    {
        return array();
    }

    /**
     * @param       $request_json_tpl
     * @param array $array
     *
     * @return array
     */
    public function getDataByTpl($request_json_tpl, array $array = array())
    {
        return array();
    }

    /**
     * @return array
     */
    public function getDebugInfo()
    {
        return array();
    }

}