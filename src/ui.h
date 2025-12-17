#pragma once
#include "bc.h"

/**
 * Lance l'interface ncurses pour visualiser la base de connaissances,
 * mettre en évidence les faits vrais, et permettre de basculer les
 * faits de base avec les flèches et la touche 'x'. 'q' pour quitter.
 * @param bc Base de connaissances (non modifiée).
 * @return Aucun.
 */
void run_ui(const BC *bc);
