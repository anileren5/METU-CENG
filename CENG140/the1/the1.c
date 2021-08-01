#include <stdio.h>

#define  max 25

#define  red 0
#define  yellow 1
#define  green 2
#define  magenta 3
#define  white 4
#define  blue 5
#define  black 6
#define  cyan 7

int choose_color(int row,int column,int image[max][max]);
void fill(int init_row,int init_column,int image[max][max],int chosen_color,int path_color);
int is_inside(int row, int column);
int copy_paste(int c_row1,int c_col1,int c_row2,int c_col2,int p_row1,int p_col1,int p_row2,int p_col2,int image[max][max]);
void transpose(int image[max][max],int top_left_row,int top_left_col,int bottom_right_row,int bottom_right_col);
void vertical_flip(int image[max][max],int top_left_row,int top_left_col,int bottom_right_row,int bottom_right_col);
void rotate_counterclockwise_90(int image[max][max],int top_left_row,int top_left_col,int bottom_right_row,int bottom_right_col);
void rotate_counterclockwise_180(int image[max][max],int top_left_row,int top_left_col,int bottom_right_row,int bottom_right_col);
void rotate_counterclockwise_270(int image[max][max],int top_left_row,int top_left_col,int bottom_right_row,int bottom_right_col);
void rotate_clockwise_90(int image[max][max],int top_left_row,int top_left_col,int bottom_right_row,int bottom_right_col);
void rotate_clockwise_180(int image[max][max],int top_left_row,int top_left_col,int bottom_right_row,int bottom_right_col);
void rotate_clockwise_270(int image[max][max],int top_left_row,int top_left_col,int bottom_right_row,int bottom_right_col);
int how_many_change(int image[max][max],int backup[max][max],int top_left_row,int top_left_col,int bottom_right_row,int bottom_right_col);


int main(void)
{
    int image[max][max],backup[max][max];
    char op_code;
    int i,j;
    for (i=0;i<max;++i){
        for (j=0;j<max;++j){
            scanf("%d",&image[i][j]);
        }
    }

    for (i=0;i<max;++i){
        for (j=0;j<max;++j){
            backup[i][j] = image[i][j];
        }
    }
    scanf(" %c",&op_code);
    
    if (op_code == 'F')
    {
        int palette_row, palette_column;
        int init_row, init_column;
        int chosen_color,path_color;
        scanf("%d %d",&palette_row,&palette_column);   
        scanf("%d %d",&init_row,&init_column);

        chosen_color = choose_color(palette_row,palette_column,image);
        path_color = image[init_row][init_column]; 
        fill(init_row,init_column,image,chosen_color,path_color);

        for (i=0;i<max;++i){
            for (j=0;j<max;++j){
                printf("%d ",image[i][j]);
            }
            printf("\n");
        }
    }

    else if (op_code == 'P')
    {
        int c_row1,c_col1,c_row2,c_col2;
        int p_row1,p_col1,p_row2,p_col2;
        int changed_count;
        scanf("%d %d %d %d",&c_row1,&c_col1,&c_row2,&c_col2);
        scanf("%d %d %d %d",&p_row1,&p_col1,&p_row2,&p_col2);

        changed_count = copy_paste(c_row1,c_col1,c_row2,c_col2,p_row1,p_col1,p_row2,p_col2,image);

        for (i=0;i<max;++i){
            for (j=0;j<max;++j){
                printf("%d ",image[i][j]);
            }
            printf("\n");
        }
        printf("%d\n",changed_count);
    }

    else if (op_code == 'R')
    {
        char direction;
        int degree;
        int copy_top_left_row,copy_top_left_col,copy_bottom_right_row,copy_bottom_right_col;
        int paste_top_left_row,paste_top_left_col,paste_bottom_right_row,paste_bottom_right_col;  
        int changed_pixel;  
        int i,j;

        scanf(" %c %d",&direction,&degree);
        scanf("%d %d %d %d",&copy_top_left_row,&copy_top_left_col,&copy_bottom_right_row,&copy_bottom_right_col);
        scanf("%d %d %d %d",&paste_top_left_row,&paste_top_left_col,&paste_bottom_right_row,&paste_bottom_right_col);

        copy_paste(copy_top_left_row,copy_top_left_col,copy_bottom_right_row,copy_bottom_right_col,paste_top_left_row,paste_top_left_col,paste_bottom_right_row,paste_bottom_right_col,image);

        if (direction == 'L' && degree == 0) ;
        else if (direction == 'R' && degree == 0);
        else if (direction == 'L' && degree == 90) rotate_counterclockwise_90(image,paste_top_left_row,paste_top_left_col,paste_bottom_right_row,paste_bottom_right_col);
        else if (direction == 'R' && degree == 90) rotate_clockwise_90(image,paste_top_left_row,paste_top_left_col,paste_bottom_right_row,paste_bottom_right_col);
        else if (direction == 'L' && degree == 180) rotate_counterclockwise_180(image,paste_top_left_row,paste_top_left_col,paste_bottom_right_row,paste_bottom_right_col);
        else if (direction == 'R' && degree == 180) rotate_clockwise_180(image,paste_top_left_row,paste_top_left_col,paste_bottom_right_row,paste_bottom_right_col);
        else if (direction == 'L' && degree == 270) rotate_counterclockwise_270(image,paste_top_left_row,paste_top_left_col,paste_bottom_right_row,paste_bottom_right_col);
        else if (direction == 'R' && degree == 270) rotate_clockwise_270(image,paste_top_left_row,paste_top_left_col,paste_bottom_right_row,paste_bottom_right_col);

        changed_pixel = how_many_change(image,backup,paste_top_left_row,paste_top_left_col,paste_bottom_right_row,paste_bottom_right_col);

        for (i=0;i<max;++i){
            for (j=0;j<max;++j){
                printf("%d ",image[i][j]);
            }
            printf("\n");
        }

        printf("%d\n",changed_pixel);
    }
    return 0;   
}

int choose_color(int row,int column,int image[max][max])
{
    if ((row<0)&&(column<0)) return red;
    else if ((row<0)&&(0<=column)&&(column<max)) return yellow;
    else if ((row<0)&&(column>=max)) return green;
    else if ((0<=row)&&(row<max)&&(column>=max)) return magenta;
    else if ((row>=max)&&(column>=max)) return white;
    else if ((row>=max)&&(0<=column)&&(column<max)) return blue;
    else if ((row>=max)&&(column<0)) return black;
    else if ((column<0)&&(0<=row)&&(row<max)) return cyan;
    else if ((0<=row)&&(row<max)&&(0<=column)&&(column<max)) return image[row][column];
    else return -1;
}

int is_inside(int row,int column)
{
    if ((row>=0)&&(row<max)&&(column>=0)&&(column<max)) return 1;
    else return 0;
}

void fill(int init_row,int init_column,int image[max][max],int chosen_color,int path_color)
{
    if (chosen_color != image[init_row][init_column])
    {
    image[init_row][init_column] = chosen_color;

    if (is_inside(init_row+1,init_column) && image[init_row+1][init_column] == path_color) fill(init_row+1,init_column,image,chosen_color,path_color);
    if (is_inside(init_row+1,init_column+1) && image[init_row+1][init_column+1] == path_color) fill(init_row+1,init_column+1,image,chosen_color,path_color);
    if (is_inside(init_row,init_column+1) && image[init_row][init_column+1] == path_color) fill(init_row,init_column+1,image,chosen_color,path_color);
    if (is_inside(init_row-1,init_column+1) && image[init_row-1][init_column+1] == path_color) fill(init_row-1,init_column+1,image,chosen_color,path_color);
    if (is_inside(init_row-1,init_column) && image[init_row-1][init_column] == path_color) fill(init_row-1,init_column,image,chosen_color,path_color);
    if (is_inside(init_row-1,init_column-1) && image[init_row-1][init_column-1] == path_color) fill(init_row-1,init_column-1,image,chosen_color,path_color);
    if (is_inside(init_row,init_column-1) && image[init_row][init_column-1] == path_color) fill(init_row,init_column-1,image,chosen_color,path_color);
    if (is_inside(init_row+1,init_column-1) && image[init_row+1][init_column-1] == path_color) fill(init_row+1,init_column-1,image,chosen_color,path_color);
    }

    else ;
}

int copy_paste(int c_row1,int c_col1,int c_row2,int c_col2,int p_row1,int p_col1,int p_row2,int p_col2,int image[max][max])
{
    int c_upper_left_row,c_upper_left_column,c_bottom_right_row,c_bottom_right_column;
    int p_upper_left_row,p_upper_left_column;
    int changed_count;
    int len_ver,len_hor;
    int i,j;
    c_upper_left_row = (c_row1<=c_row2) ? c_row1 : c_row2;
    c_upper_left_column = (c_col1<=c_col2) ? c_col1 : c_col2;

    c_bottom_right_row = (c_row1>=c_row2) ? c_row1 : c_row2;
    c_bottom_right_column = (c_col1>=c_col2) ? c_col1 :c_col2;

    p_upper_left_row = (p_row1<=p_row2) ? p_row1 : p_row2;
    p_upper_left_column = (p_col1<=p_col2) ? p_col1 : p_col2;

    len_ver = c_bottom_right_row - c_upper_left_row;
    len_hor = c_bottom_right_column - c_upper_left_column;

    changed_count = 0;

    for (i=0;i<=len_ver;++i){
        int c_row,p_row;
        c_row = c_upper_left_row + i;
        p_row = p_upper_left_row + i; 
        for (j=0;j<=len_hor;++j){
            int c_col,p_col;
            c_col = c_upper_left_column + j;
            p_col = p_upper_left_column + j;

            if (image[c_row][c_col]!=image[p_row][p_col])
            {
                image[p_row][p_col] = image[c_row][c_col];
                changed_count +=1;
            }
        }
    }

    return changed_count;
}

void transpose(int image[max][max],int top_left_row,int top_left_col,int bottom_right_row,int bottom_right_col)
{
    int i,j,k,x,b,size;
    size = bottom_right_row - top_left_row;
    for (i=top_left_row,k=0,b=size;i<=bottom_right_row && k<=size && b>=0;++i,k++,--b){
        for (j=top_left_col+k,x=0;j<=bottom_right_col && x<=b;j++,x++){
            int temp;
            temp = image[i][j];
            image[i][j] = image[i+x][j-x];
            image[i+x][j-x] = temp;
        }
    }
}

void vertical_flip(int image[max][max],int top_left_row,int top_left_col,int bottom_right_row,int bottom_right_col)
{
    int i,j,k;
    for (i=top_left_col;i<=bottom_right_col;++i){
        for (j=top_left_row,k=bottom_right_row;j<k;++j,--k){
            int temp = image[j][i];
            image[j][i] = image[k][i];
            image[k][i] = temp;
        }
    }
}

void rotate_counterclockwise_90(int image[max][max],int top_left_row,int top_left_col,int bottom_right_row,int bottom_right_col)
{
    transpose(image,top_left_row,top_left_col,bottom_right_row,bottom_right_col);
    vertical_flip(image,top_left_row,top_left_col,bottom_right_row,bottom_right_col);
}

void rotate_counterclockwise_180(int image[max][max],int top_left_row,int top_left_col,int bottom_right_row,int bottom_right_col)
{
    rotate_counterclockwise_90(image,top_left_row,top_left_col,bottom_right_row,bottom_right_col);
    rotate_counterclockwise_90(image,top_left_row,top_left_col,bottom_right_row,bottom_right_col);
}

void rotate_counterclockwise_270(int image[max][max],int top_left_row,int top_left_col,int bottom_right_row,int bottom_right_col)
{
    rotate_counterclockwise_90(image,top_left_row,top_left_col,bottom_right_row,bottom_right_col);
    rotate_counterclockwise_90(image,top_left_row,top_left_col,bottom_right_row,bottom_right_col);
    rotate_counterclockwise_90(image,top_left_row,top_left_col,bottom_right_row,bottom_right_col);
}

void rotate_clockwise_90(int image[max][max],int top_left_row,int top_left_col,int bottom_right_row,int bottom_right_col)
{
    rotate_counterclockwise_270(image,top_left_row,top_left_col,bottom_right_row,bottom_right_col);
}

void rotate_clockwise_180(int image[max][max],int top_left_row,int top_left_col,int bottom_right_row,int bottom_right_col)
{
    rotate_counterclockwise_180(image,top_left_row,top_left_col,bottom_right_row,bottom_right_col);
}

void rotate_clockwise_270(int image[max][max],int top_left_row,int top_left_col,int bottom_right_row,int bottom_right_col)
{
    rotate_counterclockwise_90(image,top_left_row,top_left_col,bottom_right_row,bottom_right_col);
}

int how_many_change(int image[max][max],int backup[max][max],int top_left_row,int top_left_col,int bottom_right_row,int bottom_right_col)
{
    int counter = 0;
    int i,j;
    for (i=top_left_row;i<=bottom_right_row;++i){
        for (j=top_left_col;j<=bottom_right_col;++j){
            if (image[i][j] != backup[i][j]) counter +=1 ;
        }
    }
    return counter;
}






