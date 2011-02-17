--TEST--
Compare AhAc to KdKc
--CREDITS--
Steeve Lennmark <steevel@handeldsbanken.se>
--SKIPIF--
<?php if (!extension_loaded("pokenum")) echo "skip"; ?>
--FILE--
<?php
$result = pokenum(PN_TEXAS, array(array('Ah', 'Ac'), array('Kd', 'Kc')));
if (! $result) {
	echo "Not a valid result\n";
} else {
	echo "VALID RESULT\n";
	var_dump($result['players'] === 2);
	var_dump($result['iterations'] === 100000);
	var_dump($result['iterations'] === 100000);
	var_dump($result['hands'][0]['win'] === 81742);
	var_dump($result['hands'][1]['win'] === 17778);
}
?>
--EXPECT--
VALID RESULT
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
