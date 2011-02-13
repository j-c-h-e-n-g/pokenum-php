--TEST--
Check for pokenum presence
--SKIPIF--
<?php if (!extension_loaded("pokenum") && !dl("pokenum.so")) echo "skip"; ?>
--FILE--
<?php echo "pokenum extension is available"; ?>
--EXPECT--
pokenum extension is available
