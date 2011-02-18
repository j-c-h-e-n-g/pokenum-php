<?php
/*
$ pokenum -mc 100000 -h Ah Kh - 9c 9d -- Jh Th 2c
Holdem Hi: 100000 sampled boards containing 2c Jh Th
cards     win   %win      lose  %lose       tie   %tie        EV
Ah Kh   63325  63.33     36675  36.67         0   0.00     0.633
9c 9d   36675  36.67     63325  63.33         0   0.00     0.367

[-mc niter] [-t] [-O]                                                                                      
[-h|-h8|-o|-o8|-7s|-7s8|-7snsq|-r|-5d|-5d8|-5dnsq|-l|-l27]
<pocket1> - <pocket2> - ... [ -- <board> ] [ / <dead> ]
*/

$types = array(
	'h' => PN_TEXAS,
	'h8' => PN_TEXAS8,
	'o' => PN_TEXAS,
	'o8' => PN_OMAHA8,
	'7s' => PN_7STUD,
	'7s8' => PN_7STUD8,
	'r' => PN_RAZZ,
	'5d' => PN_5DRAW,
	'5d8' => PN_5DRAW8,
	'l27' => PN_5DRAW_27
);
$type = '';

$args = $_SERVER['argv'];
array_shift($args);
if ($args[0][0] == '-') {
	$type = substr(array_shift($args), 1);
}

if (! isset($types[$type])) {
	printf("USAGE\n");
	exit;
}

list($hands, $board) = explode('--', implode(' ', $args));
$hands = explode('-', $hands);
list($board, $dead) = explode('/', $board);

$result = pokenum($types[$type], $hands, $board, $dead);
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
