<?php
/**
 * A couple of pokenum examples
 */
var_dump(pokenum(POKENUM_TEXAS, array(array('Ah', 'Ad'), array('Kd', 'Kc'), array('Qc', 'Qd'))));
var_dump(pokenum(POKENUM_OMAHA, array(array('Ad', 'Ac', 'Ts', 'Js'), array('8d', '7d', '6c', '5c'))));
var_dump(pokenum(POKENUM_TEXAS, array(array('Ah', 'Ad'), array('Kd', 'Kc')), array('5d', '5c', '5h')));

var_dump(pokenum(POKENUM_TEXAS,
	array(
		array('Ac', 'Ad'),
		array('Kh', 'Kd')
	),
	array('5d', 'Td', '2d')
));
var_dump(pokenum(POKENUM_TEXAS,
	array(
		array('Ac', 'Ad'),
		array('Kh', 'Kd')
	),
	array('5d', 'Td', '2d'),
	array('4d', 'Kc')
));

echo "\n--------------------------------------\n";
echo "ERROR HANDLING";
echo "\n--------------------------------------\n";

var_dump(pokenum(POKENUM_TEXAS, array(array('Ac', 'Ac'), array('Kh', 'Kd'))));
var_dump(pokenum_error());
