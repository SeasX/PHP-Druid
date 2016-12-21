# PHP-Druid
A Druid driver for PHP with PECL extension

## Install

### Make Install PHP-Druid
```sh
$ /path/to/phpize
$ ./configure --with-php-config=/path/to/php-config
$ make && make install
```

### PECL Install PHP-Druid
```sh
$ pecl install Druid
```
    

## Config

    Druid.ini
    
    extension=druid.so
    druid.base_auth_passport = ""
    druid.base_auth_user = ""
    druid.debug = 1
    druid.host = "http://10.0.3.46:9082/druid/v2/"
    druid.tpl_path = "/data/php-druid/tpl"
    
## Demo

```
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
    
        /**
         * demo 1
         *
         * use tpl build request json
         */
        $Druid->setTplPath(__DIR__ . '/tpl');
        $result_1 = $Druid->getDataByTpl('request_tpl_demo.json', array('@startTimeToEndTime@' => '["2016-12-10T14:06:00.000Z/2016-12-20T14:36:00.000Z"]'));
    
    
        /**
         * demo 2
         *
         * use full request json
         */
        $result_2 = $Druid->getData(file_get_contents(__DIR__ . '/tpl/request_full_demo.json'));
    
        var_dump($result_1, $result_2);
    
    } catch (Exception $e) {
        var_dump($e->getCode(), $e->getMessage(), $Druid->getDebugInfo());
    }
```
    