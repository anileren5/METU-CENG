#include "Piece.h"


/****************************************************/
/****               *** PIECE ***                ****/

/* Contsructor : initializes <edges> of the object to the those given in the
   argument. The edges given in the argument array should be cloned first,
   and its clones should be reserved, not the edges themselves.
   Note that the ordering of the given edges will always be as follows:
   edges[0]: Left side edge, 
   edges[1]: Bottom side edge, 
   edges[2]: Right side edge, 
   edges[3]: Top side edge.
*/
Piece::Piece(Edge* edgeList[4]) {
    // TODO
    Edge *left, *bottom, *right, *top; 
    left = edgeList[0]->clone();
    bottom = edgeList[1]->clone();
    right = edgeList[2]->clone();
    top = edgeList[3]->clone();
    edges[0] = left;
    edges[1] = bottom;
    edges[2] = right;
    edges[3] = top;
}

/* Destructor  : It should destruct its edges too.
*/
Piece::~Piece() {
    // TODO
    if (edges[0]) delete edges[0];
    if (edges[1]) delete edges[1];
    if (edges[2]) delete edges[2];
    if (edges[3]) delete edges[3];
}

/* Copy constructor : Apply deep copy.
*/
Piece::Piece(const Piece& piece) {
    // TODO
    Edge *left, *bottom, *right, *top;
    left = piece.edges[0]->clone();
    bottom = piece.edges[1]->clone();
    right = piece.edges[2]->clone();
    top = piece.edges[3]->clone();
    edges[0] = left;
    edges[1] = bottom;
    edges[2] = right;
    edges[3] = top;
}

// This is already implemented for you, do NOT change it!
ostream& operator<< (ostream& os, const Piece& piece) {
    for (int i = 0; i < 4; i++)
		os << piece.edges[i]->getId() << "  \n";
	return os;
}
