#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_pokenum.h"

#include <poker_defs.h>
#include <enumdefs.h>

ZEND_DECLARE_MODULE_GLOBALS(pokenum)

const zend_function_entry pokenum_functions[] = {
	PHP_FE(pokenum, NULL)
	PHP_FE(pokenum_param, NULL)
	PHP_FE(pokenum_error, NULL)
	PHP_FE(pokenum_errno, NULL)
	{NULL, NULL, NULL}
};

zend_module_entry pokenum_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_POKENUM_EXTNAME,
	pokenum_functions,
	PHP_MINIT(pokenum),
	PHP_MSHUTDOWN(pokenum),
	PHP_RINIT(pokenum),
	NULL,
	NULL,
#if ZEND_MODULE_API_NO >= 20010901
	PHP_POKENUM_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};

PHP_INI_BEGIN()
PHP_INI_ENTRY("pokenum.iterations", "100000", PHP_INI_ALL, NULL)
PHP_INI_END()

#ifdef COMPILE_DL_POKENUM
ZEND_GET_MODULE(pokenum)
#endif

static void php_pokenum_init_globals(zend_pokenum_globals *pokenum_globals) {
}

PHP_RINIT_FUNCTION(pokenum) {
	POKENUM_G(pokenum_err) = NULL;
	POKENUM_G(pokenum_errn) = 0;
	return SUCCESS;
}

PHP_MINIT_FUNCTION(pokenum) {
	ZEND_INIT_MODULE_GLOBALS(pokenum, php_pokenum_init_globals, NULL);

	REGISTER_INI_ENTRIES();

	REGISTER_LONG_CONSTANT("PN_TEXAS",                 game_holdem,              CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PN_TEXAS8",                game_holdem8,             CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PN_OMAHA",                 game_omaha,               CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PN_OMAHA8",                game_omaha8,              CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PN_7STUD",                 game_7stud,               CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PN_7STUD8",                game_7stud8,              CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PN_7STUDNSQ",              game_7studnsq,            CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PN_RAZZ",                  game_razz,                CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PN_5DRAW",                 game_5draw,               CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PN_5DRAW8",                game_5draw8,              CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PN_5DRAWNSQ",              game_5drawnsq,            CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PN_LOWBALL",               game_lowball,             CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PN_LOWBALL27",             game_lowball27,           CONST_CS | CONST_PERSISTENT);

	REGISTER_LONG_CONSTANT("PN_ERR_TYPE",              PN_ERR_TYPE,              CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PN_ERR_CARD_TYPE",         PN_ERR_CARD_TYPE,         CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PN_ERR_CARD_UNKNOWN",      PN_ERR_CARD_UNKNOWN,      CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PN_ERR_CARD_DUPLICATE",    PN_ERR_CARD_DUPLICATE,    CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PN_ERR_BOARD_TOO_MANY",    PN_ERR_BOARD_TOO_MANY,    CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PN_ERR_BOARD_TOO_FEW",     PN_ERR_BOARD_TOO_FEW,     CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PN_ERR_PLAYERS_TOO_MANY",  PN_ERR_PLAYERS_TOO_MANY,  CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PN_ERR_HAND_ARRAY",        PN_ERR_HAND_ARRAY,        CONST_CS | CONST_PERSISTENT);

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(pokenum) {
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}

PHP_MINFO_FUNCTION(pokenum) {
	php_info_print_table_start();
	php_info_print_table_header(2, "pokenum support", "enabled");
	php_info_print_table_end();
}

zval *pokenumCardsToArray(StdDeck_CardMask *cards) {
	zval *zdelim, *zstr, *zretval;

	MAKE_STD_ZVAL(zdelim);
	MAKE_STD_ZVAL(zstr);
	MAKE_STD_ZVAL(zretval);
	array_init(zretval);

	ZVAL_STRING(zdelim, " ", 1);
	ZVAL_STRING(zstr, DmaskString(StdDeck, *cards), 1);
	php_explode(zdelim, zstr, zretval, LONG_MAX);

	return zretval;
}

zval *pokenumCardStringToArray(zval *current) {
	char card[3], *board = estrdup(Z_STRVAL_P(current));
	zval *copy = NULL;
	int i = 0, x, letter = 0;
	size_t len = Z_STRLEN_P(current);

	memset(card, 0, sizeof(card));

	ALLOC_INIT_ZVAL(copy);
	Z_TYPE_P(copy) = IS_NULL;

	/* Each card is exactly two chars, we let the mask check if they are correct. */
	for (x=0; x<len; x++) {
		char t = board[x];

		/* T, J, Q, K, A, X */
		if ((t >= '2' && t <= '9') || (t >= 'a' && t <= 'x') || (t >= 'A' && t <= 'X')) {
			card[letter++] = t;
		} else {
			continue;
		}

		if (letter == 2) {
			if (i == 0) array_init(copy);
			add_index_string(copy, i++, card, 1);
			memset(card, 0, sizeof(card));
			letter = 0;
		}
	}

	efree(board);

	return copy;
}

PHP_FUNCTION(pokenum) {
	long game;
	zval *hands = NULL, *board = NULL, *dead = NULL, **value = NULL, **val;
	HashPosition pos;
	HashTable *hash;

	int niter = INI_INT("pokenum.iterations"), npockets = 0, nboard = 0, i = 0, card;
	StdDeck_CardMask pockets[ENUM_MAXPLAYERS], card_board, card_dead, card_dead_real;
	enum_result_t result;
	enum_gameparams_t *gameParams;

	POKENUM_G(pokenum_errn) = 0;

	// GameType, Hands, Board, Dead
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz|zz",
			&game, &hands, &board, &dead) == FAILURE) {
		spprintf(&POKENUM_G(pokenum_err), 0, "Incorrect parameters. Pass a constant as first argument.");
		POKENUM_G(pokenum_errn) = PN_ERR_ARGS;
		RETURN_FALSE;
	}
	if (NULL == (gameParams = enumGameParams(game))) {
		spprintf(&POKENUM_G(pokenum_err), 0, "You didn't pass a valid game type.");
		POKENUM_G(pokenum_errn) = PN_ERR_TYPE;
		RETURN_FALSE;
	}

	// Reset cards
	for (i=0; i<ENUM_MAXPLAYERS; i++) {
		StdDeck_CardMask_RESET(pockets[i]);
	}
	StdDeck_CardMask_RESET(card_dead);
	StdDeck_CardMask_RESET(card_dead_real);
	StdDeck_CardMask_RESET(card_board);

	array_init(return_value);

	if (dead && Z_TYPE_P(dead) != IS_NULL) { // We have a list of dead cards
		if (Z_TYPE_P(dead) == IS_STRING) {
			dead = pokenumCardStringToArray(dead);
		} else if (Z_TYPE_P(dead) != IS_ARRAY) {
			spprintf(&POKENUM_G(pokenum_err), 0, "You must pass Array or String as dead card(s)");
			POKENUM_G(pokenum_errn) = PN_ERR_CARD_TYPE;
			RETURN_FALSE;
		}

		if (Z_TYPE_P(dead) != IS_NULL) {
			hash = Z_ARRVAL_P(dead);
			zend_hash_internal_pointer_reset_ex(hash, &pos);
			while (zend_hash_get_current_data_ex(hash, (void **) &val, &pos) == SUCCESS) {
				if (DstringToCard(StdDeck, Z_STRVAL_PP(val), &card) == 0) {
					spprintf(&POKENUM_G(pokenum_err), 0, "Unknown card: %s", Z_STRVAL_PP(val));
					POKENUM_G(pokenum_errn) = PN_ERR_CARD_UNKNOWN;
					RETURN_FALSE;
				}
				if (StdDeck_CardMask_CARD_IS_SET(card_dead, card)) {
					spprintf(&POKENUM_G(pokenum_err), 0, "Duplicate card. %s already used",
							Z_STRVAL_PP(val));
					POKENUM_G(pokenum_errn) = PN_ERR_CARD_DUPLICATE;
					RETURN_FALSE;
				}

				StdDeck_CardMask_SET(card_dead, card);
				StdDeck_CardMask_SET(card_dead_real, card);
				zend_hash_move_forward_ex(hash, &pos);
			}
		}
	}

	if (board && Z_TYPE_P(board) != IS_NULL) { // We have a list of board cards
		if (Z_TYPE_P(board) == IS_STRING) {
			board = pokenumCardStringToArray(board);
		} else if (Z_TYPE_P(board) != IS_ARRAY) {
			spprintf(&POKENUM_G(pokenum_err), 0, "You must pass Array or String as board card(s)");
			POKENUM_G(pokenum_errn) = PN_ERR_CARD_TYPE;
			RETURN_FALSE;
		}

		if (Z_TYPE_P(board) != IS_NULL) {
			hash = Z_ARRVAL_P(board);
			zend_hash_internal_pointer_reset_ex(hash, &pos);
			i = 0;
			while (zend_hash_get_current_data_ex(hash, (void **) &val, &pos) == SUCCESS) {
				if (nboard >= gameParams->maxboard) {
					spprintf(&POKENUM_G(pokenum_err), 0, "Board contains too many cards");
					POKENUM_G(pokenum_errn) = PN_ERR_BOARD_TOO_MANY;
					RETURN_FALSE;
				}
				if (DstringToCard(StdDeck, Z_STRVAL_PP(val), &card) == 0) {
					spprintf(&POKENUM_G(pokenum_err), 0, "Unknown card: %s", Z_STRVAL_PP(val));
					POKENUM_G(pokenum_errn) = PN_ERR_CARD_UNKNOWN;
					RETURN_FALSE;
				}
				if (StdDeck_CardMask_CARD_IS_SET(card_dead, card)) {
					spprintf(&POKENUM_G(pokenum_err), 0, "Duplicate card. %s already used.",
							Z_STRVAL_PP(val));
					POKENUM_G(pokenum_errn) = PN_ERR_CARD_DUPLICATE;
					RETURN_FALSE;
				}

				StdDeck_CardMask_SET(card_board, card);
				StdDeck_CardMask_SET(card_dead, card);
				nboard++;

				zend_hash_move_forward_ex(hash, &pos);
			}

			if (nboard > 0 && nboard < 3) {
				spprintf(&POKENUM_G(pokenum_err), 0, "You need atleast 3 board cards.",
						Z_STRVAL_PP(val));
				POKENUM_G(pokenum_errn) = PN_ERR_BOARD_TOO_FEW;
				RETURN_FALSE;
			}
		}
	}

	if (Z_TYPE_P(hands) != IS_ARRAY) {
		spprintf(&POKENUM_G(pokenum_err), 0, "You have to pass in hands as an array");
		POKENUM_G(pokenum_errn) = PN_ERR_HAND_ARRAY;
	} else {
		hash = Z_ARRVAL_P(hands);
		zend_hash_internal_pointer_reset_ex(hash, &pos);
		while (zend_hash_get_current_data_ex(hash, (void **) &val, &pos) == SUCCESS) {
			HashPosition handpos;
			HashTable *hand_hash;
			zval **hand;

			if (Z_TYPE_PP(val) == IS_STRING) {
				*val = pokenumCardStringToArray(*val);
			} else if (Z_TYPE_PP(val) != IS_ARRAY) {
				spprintf(&POKENUM_G(pokenum_err), 0, "You must pass Array or String as hand card(s)");
				POKENUM_G(pokenum_errn) = PN_ERR_CARD_TYPE;
				RETURN_FALSE;
			}

			if (Z_TYPE_P(*val) != IS_NULL) {
				hand_hash = HASH_OF(*val);

				if (npockets >= ENUM_MAXPLAYERS) {
					spprintf(&POKENUM_G(pokenum_err), 0, "Too many players in pot");
					POKENUM_G(pokenum_errn) = PN_ERR_PLAYERS_TOO_MANY;
					RETURN_FALSE;
				}

				zend_hash_internal_pointer_reset_ex(hand_hash, &handpos);
				while (zend_hash_get_current_data_ex(hand_hash, (void **) &hand, &handpos) == SUCCESS) {
					if (DstringToCard(StdDeck, Z_STRVAL_PP(hand), &card) == 0) {
						spprintf(&POKENUM_G(pokenum_err), 0, "Unknown card: %s", Z_STRVAL_PP(hand));
					POKENUM_G(pokenum_errn) = PN_ERR_CARD_UNKNOWN;
						RETURN_FALSE;
					}
					if (StdDeck_CardMask_CARD_IS_SET(card_dead, card)) {
						spprintf(&POKENUM_G(pokenum_err), 0, "Duplicate card. %s already used.",
								Z_STRVAL_PP(hand));
						POKENUM_G(pokenum_errn) = PN_ERR_CARD_DUPLICATE;
						RETURN_FALSE;
					}
			
					StdDeck_CardMask_SET(pockets[npockets], card);
					StdDeck_CardMask_SET(card_dead, card);

					zend_hash_move_forward_ex(hand_hash, &handpos);
				}

				npockets++;

				zend_hash_move_forward_ex(hash, &pos);
			}
		}
	}

	enumResultClear(&result);
	if (! enumSample(game, pockets, card_board, card_dead, npockets, nboard, niter, 0, &result)) {
		zval *t_hands = NULL;
		MAKE_STD_ZVAL(t_hands);
		array_init(t_hands);

		add_assoc_string(return_value, "name", gameParams->name, 1);
		add_assoc_long(return_value, "players", result.nplayers);
		add_assoc_long(return_value, "iterations", result.nsamples);

		if (board && Z_TYPE_P(board) != IS_NULL) {
			add_assoc_zval(return_value, "board", pokenumCardsToArray(&card_board));
		}

		if (dead && Z_TYPE_P(dead) != IS_NULL) {
			add_assoc_zval(return_value, "dead", pokenumCardsToArray(&card_dead_real));
		}

		for (i=0; i<result.nplayers; i++) {
			zval *hash_hand;
			MAKE_STD_ZVAL(hash_hand);
			array_init(hash_hand);

			add_assoc_zval(hash_hand, "hand", pokenumCardsToArray(&pockets[i]));
			add_assoc_long(hash_hand, "win", result.nwinhi[i]);
			add_assoc_long(hash_hand, "lose", result.nlosehi[i]);
			add_assoc_long(hash_hand, "tie", result.ntiehi[i]);
			add_assoc_double(hash_hand, "ev", result.ev[i] / result.nsamples);

			add_next_index_zval(t_hands, hash_hand);
		}

		add_assoc_zval(return_value, "hands", t_hands);
	}

	enumResultFree(&result);
}

PHP_FUNCTION(pokenum_param) {
	zval *game = NULL;
	long start = 0, end = game_NUMGAMES - 1, multi = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &game) == FAILURE) {
		RETURN_NULL();
	}

	if (game && Z_TYPE_P(game) != IS_NULL) {
		start = end = Z_DVAL_P(game);
	} else {
		multi = 1;
		array_init(return_value);
	}

	while (start <= end) {
		zval *ttt = NULL;
		enum_gameparams_t *gameParams = enumGameParams(start);

		if (gameParams == NULL) {
			RETURN_NULL();
		}

		MAKE_STD_ZVAL(ttt);
		array_init(ttt);

		add_assoc_long(ttt, "game", start);
		add_assoc_string(ttt, "name", gameParams->name, 1);
		add_assoc_long(ttt, "minpocket", gameParams->minpocket);
		add_assoc_long(ttt, "maxpocket", gameParams->maxpocket);
		add_assoc_long(ttt, "maxboard",  gameParams->maxboard);
		add_assoc_long(ttt, "haslopot",  gameParams->haslopot);
		add_assoc_long(ttt, "hashipot",  gameParams->hashipot);

		if (multi) {
			add_index_zval(return_value, start, ttt);
		} else {
			RETURN_ZVAL(ttt, 1, 1);
		}

		++start;
	}
}

PHP_FUNCTION(pokenum_error) {
	RETVAL_FALSE;

	if (POKENUM_G(pokenum_err) && strlen(POKENUM_G(pokenum_err))) {
		RETVAL_STRING(POKENUM_G(pokenum_err), 1);
	}
}

PHP_FUNCTION(pokenum_errno) {
	RETVAL_LONG(POKENUM_G(pokenum_errn));
}
