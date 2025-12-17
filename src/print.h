#pragma once
#include "bc.h"

/**
 * Imprime une base de connaissances sous forme d'ASCII art
 * reliant les propositions d'entrée et les règles (R1, R2, ...).
 * Les propositions en prémisse négatives sont préfixées par '!'.
 * @param bc Base de connaissances à afficher.
 * @return Aucun.
 */
void bc_print_ascii(const BC *bc);
