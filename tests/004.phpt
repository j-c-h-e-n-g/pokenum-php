--TEST--
Check passing of strings as arguments
--CREDITS--
Steeve Lennmark <steevel@handeldsbanken.se>
--SKIPIF--
<?php if (!extension_loaded("pokenum")) echo "skip"; ?>
--FILE--
<?php
var_dump(pokenum(PN_TEXAS, array('Ac Ad', 'Kd Kc')) !== false);
var_dump(pokenum(PN_TEXAS, array('Ac Ad', 'Kd Kc'), '7d 7h 7s') !== false);
var_dump(pokenum(PN_TEXAS, array('Ac Ad', 'Kd Kc'), '7d 7h 7s', '7c') !== false);
--EXPECT--
bool(true)
bool(true)
bool(true)
