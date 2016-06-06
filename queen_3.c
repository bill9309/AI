#include <stdio.h>
#include <stdlib.h>
int board[8][8];
void clear_board();
void print_board();
void generate_queen_layout(int,int,int,int);
int can_be_attacked_by_queen(int,int);
int find_knight(int,int);
int num_knights;
int num_not_attacked;
int blocks_not_attacked[9][2];
void clear_blocks_not_attacked();
void clear_knight_positions();
int can_be_attacked_by_knight(int,int);
int knight_positions[9][2];
int main(){
  int i,j,k,p;
  int m,n;
  int iter;
  int q;
  clear_board();
  for(i = 0; i <= 15; i++){
    for(j = i + 1; j <= 61; j++){
      for(k = j + 1; k <= 62; k++){
        for(p = k + 1; p <= 63; p++){
          clear_knight_positions();
          clear_board();
          clear_blocks_not_attacked();
          num_not_attacked = 0;
          iter = 0;
          generate_queen_layout(i,j,k,p);
          for(m = 0; m < 8; m++){
            for(n = 0; n < 8; n++){
              if( !can_be_attacked_by_queen(m,n)  ){
                blocks_not_attacked[num_not_attacked][0] = m;
                blocks_not_attacked[num_not_attacked][1] = n;
                num_not_attacked++;
                if(num_not_attacked >= 9){
                  clear_board();
                  break;
                }

              }
            }
            if(num_not_attacked >= 9){
              clear_board();
              break;
            }

          }
         if(num_not_attacked < 9){
           if(num_not_attacked == 1){
             board[blocks_not_attacked[0][0]][blocks_not_attacked[0][1]] = 1;
             print_board();
           }
           else{
             num_knights = find_knight(blocks_not_attacked[0][0],blocks_not_attacked[0][1]);
             int knight_can_do;
             for(iter = 0; iter < num_knights; iter++){
               if(board[knight_positions[iter][0]][knight_positions[iter][1]] == 0){
                 board[knight_positions[iter][0]][knight_positions[iter][1]] = 2;
                 knight_can_do = 1;
                 for(q = 0; q < num_not_attacked; q++){
                   if(!can_be_attacked_by_knight(blocks_not_attacked[q][0],blocks_not_attacked[q][1])){
                     board[knight_positions[iter][0]][knight_positions[iter][1]] = 0;
                     knight_can_do = 0;
                     break;
                   }
                 }
               }
               if(knight_can_do){
                 print_board();
                 board[knight_positions[iter][0]][knight_positions[iter][1]] = 0;
               }
             }

           }
         }
        }
      }
    }
  }
}

void generate_queen_layout(int i, int j, int k, int p){
  int positions[4];
  positions[0] = i;
  positions[1] = j;
  positions[2] = k;
  positions[3] = p;
  int iter;
  int which_quarter,which_position_in_quarter;
  int column,row;
  for(iter = 0;iter <= 3; iter++){
    which_quarter = positions[iter] / 16;
    which_position_in_quarter = positions[iter] % 16;
    column = which_position_in_quarter % 4;
    row = which_position_in_quarter /4;
    if(which_quarter == 0 || which_quarter == 1){
      board[row][column + which_quarter * 4] = 1;
    }
    else{
      board[row + 4][column + (which_quarter - 2) * 4] = 1;
    }
  }
}








int can_be_attacked_by_queen(int i,int j){
  int k;
  int a;
  for(k = 0; k <= 7; k++){
    if(board[i][k] == 1 || board[k][j] == 1){
      return 1;
    }
  }
  for(a = 0 ; a<= 7; a++){
    if( i-a >= 0 && j-a >= 0 ){
      if(board[i-a][j-a]== 1){
        return 1;
      }
    }
    if(i+a <= 7 && j+a <= 7){
      if(board[i+a][j+a] == 1){
        return 1;
      }
    }
    if(i-a >= 0 && j+a <= 7){
      if(board[i-a][j+a] == 1){
        return 1;
      }
    }
    if(i+a <= 7 && j-a >= 0){
      if(board[i+a][j-a] == 1){
        return 1;
      }
    }
  }

  return 0;
}
int can_be_attacked_by_knight(int i,int j){
  if( i-1 >= 0 && j-2 >= 0){
    if(board[i - 1][j - 2] == 2){
      return 1;
    }
  }
  if(i-2 >= 0 && j-1 >= 0){
    if(board[i - 2][j - 1] == 2){
      return 1;
    }
  }
  if(i -2 >= 0 && j+1 <= 7){
  if(board[i - 2][j + 1] == 2){
    return 1;
  }
  }
  if(i -1 >= 0 && j+2 <= 7){
    if(board[i - 1][j + 2] == 2){
      return 1;
    }
  }
  if(i + 1 <= 7 && j - 2 >= 0){
    if(board[i + 1][j - 2] == 2){
      return 1;
    }
  }
  if(i + 2 <= 7 && j -1 >= 0){
    if(board[i + 2][j - 1] == 2){
      return 1;
    }
  }
  if(i+2 <= 7 && j + 1 <= 7){
    if(board[i + 2][j + 1] == 2){
      return 1;
    }
  }
  if(i+1 <= 7 && j+2 <= 7){
    if(board[i + 1][j + 2] == 2){
      return 1;
    }
  }
  return 0;
}
//Given a square,this function looks for places to place the knight,such that the square can be attacked.
int find_knight(int i, int j){
  int m;
  int count = 0;
  knight_positions[count][0] = i;
  knight_positions[count][1] = j;
  count++;
  if( i-1 >= 0 && j-2 >= 0){

    knight_positions[count][0] = i - 1;
    knight_positions[count][1] = j - 2;
    count++;
  }
  if(i-2 >= 0 && j-1 >= 0){
    knight_positions[count][0] = i - 2;
    knight_positions[count][1] = j - 1;
    count++;
  }
  if(i -2 >= 0 && j+1 <= 7){
    knight_positions[count][0] = i - 2;
    knight_positions[count][1] = j + 1;
    count++;
  }
  if(i -1 >= 0 && j+2 <= 7){
    knight_positions[count][0] = i - 1;
    knight_positions[count][1] = j + 2;
    count++;
  }
  if(i + 1 <= 7 && j - 2 >= 0){
    knight_positions[count][0] = i + 1;
    knight_positions[count][1] = j - 2;
    count++;
  }
  if(i + 2 <= 7 && j -1 >= 0){
    knight_positions[count][0] = i + 2;
    knight_positions[count][1] = j - 1;
    count++;
  }
  if(i+2 <= 7 && j + 1 <= 7){
    knight_positions[count][0] = i + 2;
    knight_positions[count][1] = j + 1;
    count++;
  }
  if(i+1 <= 7 && j+2 <= 7){
    knight_positions[count][0] = i + 1;
    knight_positions[count][1] = j + 2;
    count++;
  }
  return count;

}



void clear_board(){
  int i,j;
  for(i = 0; i < 8;i++){
    for(j = 0; j < 8;j++){
      board[i][j] = 0;
    }
  }
}
void clear_blocks_not_attacked(){
  int i;
  for(i = 0; i < 9; i++){
    blocks_not_attacked[i][0] = -1;
    blocks_not_attacked[i][0] = -1;
  }
}
void print_board(){
  int i,j;
  for(i = 0; i < 8;i++){
    for(j = 0; j < 8; j++){
      if(board[i][j] == 1){
        printf("Q");
      }
      else if(board[i][j] == 0){
        printf("*");
      }
      else{
        printf("K");
      }
    }
    printf("\n");
  }
  printf("---------------\n");
}
void clear_knight_positions(){
  int i;
  for(i = 0; i < 9; i++){
    knight_positions[i][0] = 0;
    knight_positions[i][1] = 0;
  }
}
