#define mapWidth 5
#define mapHeight 5
#define prevCount 20
#define nextCount 10

void to_lower(char * sentence);

void print_sentence(char * sentence);

void substrings(char * sentence, char * substr, int * arr);

void removeX(char * sentence, int * arr, int count);

void addSome(char * sentence, int * arr, char * substr);

void battle_ship(char map[mapWidth][mapHeight], int prev_moves[prevCount][2], int next_moves[nextCount][2]);

