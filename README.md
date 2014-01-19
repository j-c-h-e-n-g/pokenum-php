This is pokenum from poker-eval built as a php extension.
It currently lacks a lot of error handling.

Syntax: pokenum(int GameType, array Hands [, array BoardCards [, array DeadCards]])
Syntax: pokenum_error() (returns false if no errors, a string otherwise)
Syntax: pokenum_errno() (returns errno, 0 if no error have occured)
Syntax: pokenum_param([int GameType]) (returns information about all/specific game type)

error/errno is reset on every use of pokenum()

Valid GameTypes (constants):
PN_TEXAS   PN_OMAHA   PN_7STUD    PN_5DRAW    PN_LOWBALL
PN_TEXAS8  PN_OMAHA8  PN_7STUD8   PN_5DRAW8   PN_LOWBALL27
                      PN_7STUDNSQ PN_5DRAWNSQ PN_RAZZ

Types ending with 8 are hi/lo games.

We currently have one ini setting, pokenum.iterations.
Specify it like: ini_set('pokenum.iterations', 100000); (defualt: 100000)

Example usage (AhAc vs KdKc):

    print_r(pokenum(PN_TEXAS, array('Ah Ac', 'Kd Kc')));"
    Array (
	[players] => 2
	[iterations] => 100000
	[hands] => Array (
		[0] => Array (
			[hand] => Array (
				[0] => Ac
				[1] => Ah
			)
			[win] => 81742
			[lose] => 17778
			[tie] => 480
			[ev] => 0.81982
		)
		[1] => Array (
			[hand] => Array (
				[0] => Kc
				[1] => Kd
			)
			[win] => 17778
			[lose] => 81742
			[tie] => 480
			[ev] => 0.18018
		)
	)
    )
