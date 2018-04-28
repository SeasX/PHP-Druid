--TEST--
Check for PHP-Druid presence
--SKIPIF--
<?php if (!extension_loaded("druid")) print "skip";?>
--FILE--
<?php
echo "druid extension is available";
?>
--EXPECT--
druid extension is available

