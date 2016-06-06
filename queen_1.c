#include <stdio.h>
#include <stdlib.h>
int block_x(int);
int block_y(int);
int can_be_attackted(int i,int j);
int is_valid;
int board[6][6];
int a,b,c,d,e,f;
void clear_board();
int main(){
  clear_board(board);
  int i,j,k;
  int first,second,third;
  int number_of_possible_solutions = 7140;
  int possible_solutions[number_of_possible_solutions][3];
  int l = 0;
  for(i = 0;i <= 33; i++){
    for(j = i+1; j <= 34; j++){
      for(k = j+1; k<= 35;k++){
        possible_solutions[l][0] = i;
        possible_solutions[l][1] = j;
        possible_solutions[l][2] = k;
        l++;
      }
    }
  }
  for(l = 0; l <= 7139; l++){
    is_valid = 1;
    board[block_x(possible_solutions[l][0])][block_y(possible_solutions[l][0])] = 1;
    board[block_x(possible_solutions[l][1])][block_y(possible_solutions[l][1])] = 1;
    board[block_x(possible_solutions[l][2])][block_y(possible_solutions[l][2])] = 1;
    for(i = 0; i < 6; i++){
      for(j = 0; j < 6; j++){
        if(can_be_attackted(i,j) == 0){
          is_valid = 0;
          break;
        }
      }
      if(is_valid == 0){
        break;
      }
    }

    if(is_valid){
      //printf("%d %d %d\n",possible_solutions[l][0],possible_solutions[l][1],possible_solutions[l][2]);
      for(i = 0; i <= 5;i++){
        for(j = 0; j <= 5; j++){
          if(board[i][j] == 1){
            printf("O");
          }
          else{
            printf("X");
          }
        }
        printf("\n");
      }
      printf("---------------\n");
    }
    clear_board();
  }
  
}

int block_x(int position){
  return position % 6;
}

int block_y(int position){
  return position / 6;
}
void clear_board(){
  int i,j;
  for(i = 0; i < 6;i++){
    for(j = 0; j < 6;j++){
      board[i][j] = 0;
    }
  }
}

int can_be_attackted(int i,int j){
  int k;
  int a;
  for(k = 0; k <= 5; k++){
    if(board[i][k] || board[k][j]){
      return 1;
    }
  }
  for(a = 0 ; a<= 5; a++){
    if( i-a >= 0 && j-a >= 0 ){
      if(board[i-a][j-a]== 1){
      return 1;
    }
  }
  if(i+a <= 5 && j+a <= 5){
    if(board[i+a][j+a] == 1){
      return 1;
    }
  }
  if(i-a >= 0 && j+a <= 5){
    if(board[i-a][j+a] == 1){
      return 1;
    }
  }
  if(i+a <= 5 && j-a >= 0){
    if(board[i+a][j-a] == 1){
      return 1;
    }
  }
  }
    return 0;
}
