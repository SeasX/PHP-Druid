# PHP-Druid
[![Build Status](https://travis-ci.org/Neeke/PHP-Druid.svg?branch=master)](https://travis-ci.org/Neeke/PHP-Druid)

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
    druid.curl_dns_cache_timeout = 1
    druid.curl_connect_timeout = 3
    druid.curl_timeout = 5
    
    
## Demo

```
    <?php
    /**
     * @author neeke@php.net
     * Date: 16/12/21 下午8:38
     */
    
    try {
    
        $Druid_1 = Druid::getInstance(Druid::DRUID_INSTANCE_DEFAULT);
        $Druid_1->debugWitch(TRUE);
    
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
        var_dump($Druid_1,$Druid_2);
    } catch (Exception $e) {
        var_dump($e->getCode(), $e->getMessage(), $Druid_1->getDebugInfo(), $Druid_2->getDebugInfo());
    }
```

## PHP --re
```
Extension [ <persistent> extension #30 Druid version 0.9.3 ] {

  - Dependencies {
    Dependency [ curl (Required) ]
    Dependency [ json (Required) ]
  }

  - INI {
    Entry [ druid.host <SYSTEM> ]
      Current = ''
    }
    Entry [ druid.base_auth_user <PERDIR> ]
      Current = ''
    }
    Entry [ druid.base_auth_passport <PERDIR> ]
      Current = ''
    }
    Entry [ druid.tpl_path <PERDIR> ]
      Current = '/data/php-druid/tpl'
    }
    Entry [ druid.debug <ALL> ]
      Current = '0'
    }
    Entry [ druid.curl_dns_cache_timeout <ALL> ]
      Current = '1'
    }
    Entry [ druid.curl_connect_timeout <ALL> ]
      Current = '3'
    }
    Entry [ druid.curl_timeout <ALL> ]
      Current = '5'
    }
  }

  - Classes [1] {
    Class [ <internal:Druid> class Druid ] {

      - Constants [2] {
        Constant [ public string DRUID_CONTENT_TYPE ] { Content-Type:application/json }
        Constant [ public string DRUID_INSTANCE_DEFAULT ] { default }
      }

      - Static properties [1] {
        Property [ protected static $Druid ]
      }

      - Static methods [1] {
        Method [ <internal:Druid> static public method getInstance ] {

          - Parameters [1] {
            Parameter #0 [ <required> $instance_name ]
          }
        }
      }

      - Properties [7] {
        Property [ <default> protected $tpl_path ]
        Property [ <default> protected $response_debug_info ]
        Property [ <default> protected $response_code ]
        Property [ <default> protected $_curl_error_no ]
        Property [ <default> protected $_curl_error_str ]
        Property [ <default> protected $hosts ]
        Property [ <default> protected $host_rand ]
      }

      - Methods [11] {
        Method [ <internal:Druid, ctor> private method __construct ] {

          - Parameters [0] {
          }
        }

        Method [ <internal:Druid> private method __clone ] {
        }

        Method [ <internal:Druid> private method __sleep ] {
        }

        Method [ <internal:Druid> private method __wakeup ] {
        }

        Method [ <internal:Druid, dtor> public method __destruct ] {
        }

        Method [ <internal:Druid> public method debugWitch ] {

          - Parameters [1] {
            Parameter #0 [ <required> $debug ]
          }
        }

        Method [ <internal:Druid> public method setDruidHosts ] {

          - Parameters [1] {
            Parameter #0 [ <required> $hosts ]
          }
        }

        Method [ <internal:Druid> public method setTplPath ] {

          - Parameters [1] {
            Parameter #0 [ <required> $tpl_path ]
          }
        }

        Method [ <internal:Druid> public method getData ] {

          - Parameters [2] {
            Parameter #0 [ <required> $request_json ]
            Parameter #1 [ <optional> $content_array ]
          }
        }

        Method [ <internal:Druid> public method getDataByTpl ] {

          - Parameters [2] {
            Parameter #0 [ <required> $request_json_tpl ]
            Parameter #1 [ <optional> $content_array ]
          }
        }

        Method [ <internal:Druid> public method getDebugInfo ] {
        }
      }
    }
  }
}

```

    