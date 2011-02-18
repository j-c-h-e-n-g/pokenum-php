--TEST--
Check that AcAd vs AcKd doesn't work
--CREDITS--
Steeve Lennmark
--SKIPIF--
<?php if (!extension_loaded("pokenum")) echo "skip"; ?>
--FILE--
<?php
var_dump(pokenum(PN_TEXAS, array(array('Ac', 'Ad'), array('Ac', 'Kd'))) === false);
--EXPECT--
bool(true)
