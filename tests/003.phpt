--TEST--
Check that AcAd vs AcKd doesn't work
--CREDITS--
Steeve Lennmark <steevel@handeldsbanken.se>
--SKIPIF--
<?php if (!extension_loaded("pokenum")) echo "skip"; ?>
--FILE--
<?php
var_dump(pokenum(POKENUM_TEXAS, array(array('Ac', 'Ad'), array('Ac', 'Kd'))) === false);
--EXPECT--
bool(true)
