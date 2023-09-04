#include "Puzzle.h"


/****************************************************/
/****              *** PUZZLE ***                ****/

/* Contsructor : It takes three integers in the following order:
    The first argument is size of the puzzle which is 2^N for some N >= 0.
    The second argument is the initial row id of the puzzle.
    The third argument is the initial column id of the puzzle.
    
    After initialization with the above arguments, the necessary 
    pointers should be initialized to NULL.
    Note that the final row id in this puzzle is equal to: 
       initial_row_id + size - 1.
    Note that the final column id in this puzzle is equal to: 
       initial_column_id + size - 1.
       
    ATTENTION: If a sub-puzzle member does not include any Piece, namely, 
    if its piece is NULL, then do NOT construct it. In other words, do 
    NOT construct subpuzzles unless they are required. 
*/
Puzzle::Puzzle(int dimension, int rowId, int columndId) {
    // TODO
    this->size = dimension;
    this->first_row = rowId;
    this->first_column = columndId;
    this->top_left = NULL;
    this->top_right = NULL;
    this->bottom_left = NULL;
    this->bottom_right = NULL;
    this->piece = NULL;
}

/* Destructor  : It should destruct its sub-puzzles, yet it is NOT responsible
   of the destruction of its Piece member.
*/
Puzzle::~Puzzle() {
    // TODO
    if (top_left != NULL) delete top_left;
    if (top_right != NULL) delete top_right;
    if (bottom_left != NULL) delete bottom_left;
    if (bottom_right != NULL) delete bottom_right;  
}

/* Copy constructor : It totally applies deep copy except the Piece
   member part. Do not deep copy Piece objects of the puzzle, instead
   do shallow copy for Pieces.
*/
Puzzle::Puzzle(const Puzzle& p) {
    // TODO
    // *this = p;
    copyConstructorHelper(this,p);
}

void Puzzle::copyConstructorHelper(Puzzle* t,const Puzzle& p){
    t->size = p.size;
    t->first_row = p.first_row;
    t->first_column = p.first_column;
    t->piece = p.piece;
    t->top_left = NULL;
    t->top_right = NULL;
    t->bottom_left = NULL;
    t->bottom_right = NULL;
    if (p.top_left != NULL){
        t->top_left = new Puzzle(1,0,0);
        copyConstructorHelper((t->top_left),*(p.top_left));
    }
    if (p.top_right != NULL){
        t->top_right = new Puzzle(1,0,0);
        copyConstructorHelper((t->top_right),*(p.top_right));
    }
    if (p.bottom_left != NULL){
        t->bottom_left = new Puzzle(1,0,0);
        copyConstructorHelper((t->bottom_left),*(p.bottom_left));
    }
    if (p.bottom_right != NULL){
        t->bottom_right = new Puzzle(1,0,0);
        copyConstructorHelper((t->bottom_right),*(p.bottom_right));
    }
}

/* Assignment operator : It totally applies shallow copy.
*/
Puzzle& Puzzle::operator=(const Puzzle& p) {
    // TODO
    this->size = p.size;
    this->first_row = p.first_row;
    this->first_column = p.first_column;
    this->top_left = p.top_left;
    this->top_right = p.top_right;
    this->bottom_left = p.bottom_left;
    this->bottom_right = p.bottom_right;
    this->piece = p.piece;
    return *(this);
}

/* This method directly assigns (without copying) the given Piece object to
   the corresponding sub-puzzle (cell) whose indices given in the second 
   argument in array form as follows <row_id, column_id>.
   
    ATTENTION: If a sub-puzzle member does not include any Piece, namely, 
    if its piece is NULL, then do NOT construct it. In other words, do 
    NOT construct subpuzzles unless they are required.
*/
void Puzzle::placePiece(const Piece& p, int coordinates[2]) {
    // TODO
    int r = coordinates[0];
    int c = coordinates[1];
    if (this->size == 1){
        (this->piece) = &p;
    }
    else {
        bool inLeft, inTop;
        inLeft = c < (first_column + (size/2));
        inTop = r < (first_row + (size/2));
        if ((inLeft == true) && (inTop == true)){ // top-left
            if (this->top_left == NULL){
                this->top_left = new Puzzle((this->size)/2,first_row,first_column);
                this->top_left->placePiece(p,coordinates);            
            }
            else this->top_left->placePiece(p,coordinates);    

        }
        else if ((inLeft == true) && (inTop == false)){ // bottom-left
            if (this->bottom_left == NULL){
                int rp,cp;
                rp = this->first_row + ((this->size)/2);
                cp = this->first_column;
                this->bottom_left = new Puzzle((this->size)/2,rp,cp);
                this->bottom_left->placePiece(p,coordinates);
            }
            else this->bottom_left->placePiece(p,coordinates);
        }
        else if ((inLeft == false) && (inTop == true)){ // top-right
            if (this->top_right == NULL){
                int rp,cp;
                rp = this->first_row;
                cp = this->first_column + ((this->size)/2);
                this->top_right = new Puzzle((this->size)/2,rp,cp);
                this->top_right->placePiece(p,coordinates);
            }
            else this->top_right->placePiece(p,coordinates);
        }
        else { // bottom-right
            if (this->bottom_right == NULL){
                int rp,cp;
                rp = this->first_row + ((this->size)/2);
                cp = this->first_column + ((this->size)/2);
                this->bottom_right = new Puzzle((this->size)/2,rp,cp);
                this->bottom_right->placePiece(p,coordinates);  
            }
            else this->bottom_right->placePiece(p,coordinates);        
        }  
    }
}

/* It returns directly the piece object located in the sub-puzzle (cell)
   whose row id and column id are given in the first and second elements of 
   2D array as argument, respectively. If there is no piece in the requested 
   indices, then this method throws an EmptyCellRequest exception defined in 
   Exception.h file.
*/
const Piece& Puzzle::getPiece(int coordinates[2]) const {
    // TODO
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // Handle the Empty case EXCEPTION
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    int r = coordinates[0];
    int c = coordinates[1];
    if (this->size == 1){
        if (this->piece != NULL) return *(this->piece);
        else throw EmptyCellRequest();
    }
    else {
        bool inLeft, inTop;
        inLeft = c < (first_column + (size/2));
        inTop = r < (first_row + (size/2));
        if ((inLeft == true) && (inTop == true) && (top_left == NULL)) throw EmptyCellRequest();
        if ((inLeft == true) && (inTop == false) && (bottom_left == NULL)) throw EmptyCellRequest();
        if ((inLeft == false) && (inTop == true) && (top_right == NULL)) throw EmptyCellRequest();
        if ((inLeft == false) && (inTop == false) && (bottom_right == NULL)) throw EmptyCellRequest();
        if ((inLeft == true) && (inTop == true)) return (this->top_left->getPiece(coordinates));
        else if ((inLeft == true) && (inTop == false)) return (this->bottom_left->getPiece(coordinates));
        else if ((inLeft == false) && (inTop == true)) return (this->top_right->getPiece(coordinates));
        else return (this->bottom_right->getPiece(coordinates));
    }
}

/* This method returns the sub-puzzle whose initial and final row-column ids 
   are given in the first and second arguments, respectively. The indices
   given in the arguments will be given as an exact correspondence to one of
   the sub-puzzles of the current puzzle object.
   
   ATTENTION: The sub-puzzle to be cropped may correspond to a NULL part of the
   Puzzle object. In that case, it is enough to you to create a new Puzzle 
   object that satisfies the asked properties and return it. OF course, do NOT 
   forget to do this operation by escaping from memory leaks!!!
*/
Puzzle Puzzle::crop(int from[2], int to[2]) const {
    // TODO
    int n = to[0] - from[0] + 1;
    int r = from[0];
    int c = from[1];
    bool inLeft, inTop;
    inLeft = c < (first_column + (size/2));
    inTop = r < (first_row + (size/2));
    if (this->size == n) return *(this);
    if ((inLeft == true) && (inTop == true)){
        if (((this->size)/2) == n){
            if (this->top_left != NULL) return *(this->top_left);
            else return Puzzle(size/2,from[0],from[1]);
        }
        else {
            if (this->top_left != NULL) return this->top_left->crop(from,to);
            else return Puzzle(n,from[0],from[1]);
        }
    }
    else if ((inLeft == true) && (inTop == false)){
        if (((this->size)/2) == n){
            if (this->bottom_left != NULL) return *(this->bottom_left);
            else return Puzzle(size/2,from[0],from[1]);
        }
        else {
            if (this->bottom_left != NULL) return this->bottom_left->crop(from,to);
            else return Puzzle(n,from[0],from[1]);
        }
    }
    else if ((inLeft == false) && (inTop == true)){
        if (((this->size)/2) == n){
            if (this->top_right != NULL) return *(this->top_right);
            else return Puzzle(size/2,from[0],from[1]);
        }
        else {
            if (this->top_right != NULL) return this->top_right->crop(from,to);
            else return Puzzle(n,from[0],from[1]);
        }    
    }
    else {
        if (((this->size)/2) == n){
            if (this->bottom_right != NULL) return *(this->bottom_right);
            else return Puzzle(size/2,from[0],from[1]);
        }
        else {
            if (this->bottom_right != NULL) return this->bottom_right->crop(from,to);
            else return Puzzle(n,from[0],from[1]);
        }       
    }


    
    
    /*if ((this->size) == n){
        
    }
    else {
        int r = from[0];
        int c = from[1];
        bool inLeft, inTop;
        inLeft = c < (first_column + (size/2));
        inTop = r < (first_row + (size/2));

        
        if ((inLeft == true) && (inTop == true)){
            return this->top_left->crop(from,to);
        }
        else if ((inLeft == true) && (inTop == false)){
            return this->bottom_left->crop(from,to);
        }
        else if ((inLeft == false) && (inTop == true)){
            return this->top_right->crop(from,to);
        }
        else{
            return (this->bottom_right)->crop(from,to);
        }
    }*/
}

/****************************************************/
/*        THE BELOW PART WILL NOT BE GRADED.        */
/*      YOU CAN IMPLEMENT IF YOU WOULD LIKE TO.     */
/****************************************************/
/*
    This method puts (patches) the given puzzle onto the corresponding part
    of the current puzzle object. To which part it will be patched on could
    be determined by just checking the initial row-column indices and the size
    of the puzzle given in the argument. 
    Unlike crop() method, the puzzle given in the argument may not be an exact 
    correspondence of some sub-puzzle. It is ensured that the given argument will
    be located on some sub-space of the current puzzle, yet it may *partially* 
    overlap with some or more sub-puzzles (See the figure in the pdf).
    The patch operation requires deep-copy of the corresponding sub-puzzles
    whereas it requires shallow copy of the corresponding Piece objects.
    
    HINT: You may get benefit from placePiece() method and you may start your
    operation by clearing the corresponding parts of the current puzzle.
*/
void Puzzle::patch(const Puzzle&) {
    // TODO
}

