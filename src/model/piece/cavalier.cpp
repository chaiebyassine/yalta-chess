#include "Cavalier.h"
#include "../plateau/Plateau.h"
#include "../plateau/Case.h"
#include "../base/moveHelper.h"
#include <algorithm>

Cavalier::Cavalier(const Position& pos, Couleur coul, Joueur* j) : Piece(pos, coul, j) {}

// Cavalier : 8 sauts en L (2 cases ortho + 1 case perpendiculaire)
std::vector<Position> Cavalier::mouvementsPossibles(const Plateau& plateau) const {
    std::vector<Position> moves;
    auto [x, y] = positionTo2D(position);
    if (x < 0) return moves;

    // 2 en x + 1 en y
    MoveHelper::knightHop(plateau, x, y,  1, 0, 0,  1, couleur, moves);
    MoveHelper::knightHop(plateau, x, y,  1, 0, 0, -1, couleur, moves);
    MoveHelper::knightHop(plateau, x, y, -1, 0, 0,  1, couleur, moves);
    MoveHelper::knightHop(plateau, x, y, -1, 0, 0, -1, couleur, moves);
    // 2 en y + 1 en x
    MoveHelper::knightHop(plateau, x, y, 0,  1,  1, 0, couleur, moves);
    MoveHelper::knightHop(plateau, x, y, 0,  1, -1, 0, couleur, moves);
    MoveHelper::knightHop(plateau, x, y, 0, -1,  1, 0, couleur, moves);
    MoveHelper::knightHop(plateau, x, y, 0, -1, -1, 0, couleur, moves);

    // Cas spéciaux : jonctions topologiques du plateau Yalta.
    // La jonction en (4,3) permet un saut via (-1,+1) vers (8,8) ou (3,4),
    // suivi d'un pas orthogonal — non capturé par knightHop standard.
    if (x == 4 && y == 3) {
        MoveHelper::tryAdd(plateau, 9, 8, couleur, moves); // via jonction → (8,8) + (1,0) = (-4,2,2)
        MoveHelper::tryAdd(plateau, 3, 5, couleur, moves); // via jonction → (3,4) + (0,1) = (4,-5,1)
    }
    // La jonction en (8,4) permet un saut via (-1,-1) vers (4,8) ou (3,3),
    // suivi d'un pas orthogonal — même motif symétrique.
    if (x == 8 && y == 4) {
        MoveHelper::tryAdd(plateau, 5, 8, couleur, moves); // via jonction → (4,8) + (1,0) = (2,1,-3)
        MoveHelper::tryAdd(plateau, 3, 2, couleur, moves); // via jonction → (3,3) + (0,-1) = (1,-2,1)
    }
    // La jonction en (3,3) permet un saut via (+1,+1) vers (4,8) ou (8,4),
    // suivi d'un pas orthogonal.
    if (x == 3 && y == 3) {
        MoveHelper::tryAdd(plateau, 4, 9, couleur, moves); // via jonction → (4,8) + (0,1) = (1,1,-2)
        MoveHelper::tryAdd(plateau, 9, 4, couleur, moves); // via jonction → (8,4) + (1,0) = (-3,2,1)
    }
    // La jonction en (3,4) permet un saut via (+1,-1) vers (4,3) ou (8,8),
    // suivi d'un pas orthogonal.
    if (x == 3 && y == 4) {
        MoveHelper::tryAdd(plateau, 5, 3, couleur, moves); // via jonction → (4,3) + (1,0) = (2,2,-4)
        MoveHelper::tryAdd(plateau, 8, 9, couleur, moves); // via jonction → (8,8) + (0,1) = (-5,1,4)
    }

    // Dédoublonnage
    std::sort(moves.begin(), moves.end());
    moves.erase(std::unique(moves.begin(), moves.end()), moves.end());
    return moves;
}
