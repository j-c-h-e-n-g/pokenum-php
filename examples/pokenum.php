<?php
/*
$ pokenum -mc 100000 -h Ah Kh - 9c 9d -- Jh Th 2c
Holdem Hi: 100000 sampled boards containing 2c Jh Th
cards     win   %win      lose  %lose       tie   %tie        EV
Ah Kh   63325  63.33     36675  36.67         0   0.00     0.633
9c 9d   36675  36.67     63325  63.33         0   0.00     0.367
*/
$result = pokenum(PN_TEXAS, array('Ah Kh', '9c 9d'), 'Jh Th 2c');
if (false === $result) {
	printf("POKENUM_ERROR: (%d): %s\n", pokenum_errno(), pokenum_error());
	exit;
}

printf("%s: %d sampled boards containing %s\n",
		$result['name'],
		$result['iterations'],
		implode(' ', $result['board']));
echo "cards     win   %win      lose  %lose       tie   %tie        EV\n";
foreach ($result['hands'] as $hand) {
	printf("%s %7d %6.2f   %7d %6.2f   %7d %6.2f     %5.3f\n",
			implode(' ', $hand['hand']),
			$hand['win'],
			$hand['win'] / $result['iterations'] * 100,
			$hand['lose'],
			$hand['lose'] / $result['iterations'] * 100,
			$hand['tie'],
			$hand['tie'] / $result['iterations'] * 100,
			$hand['ev']);
}
