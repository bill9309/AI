#include <stdio.h>
#include <stdlib.h>
int board[7][7];
int can_be_attackted(int i,int j);
int attempts[4];
void search_possible_solutions(int i,int j,int k,int l);
int columns_searched[4];
int find_next_solution();
int backtrack(int);
int find_spot(int);
void clear_board();
int main(){
  clear_board();
  int i,j,k,l;
  for(i = 0; i <= 3;i ++ ){
    attempts[i] = 0;
  }
  for(i = 0; i <= 3; i++){
    for(j = i+1 ; j <= 4; j++){
      for(k = j+1; k <= 5; k++){
        for(l = k+1; l <= 6; l++){
          search_possible_solutions(i,j,k,l);
        }
      }
    }
  }
}


void clear_board(){
  int i,j;
  for(i = 0; i < 7;i++){
    for(j = 0; j < 7;j++){
      board[i][j] = 0;
    }
  }
}


int can_be_attackted(int i,int j){
  int k;
  int a;
  for(k = 0; k <= 6; k++){
    if(board[i][k] || board[k][j]){
      return 1;
    }
  }
  for(a = 0 ; a<= 6; a++){
    if( i-a >= 0 && j-a >= 0 ){
      if(board[i-a][j-a]== 1){
        return 1;
      }
    }
    if(i+a <= 6 && j+a <= 6){
      if(board[i+a][j+a] == 1){
        return 1;
      }
    }
    if(i-a >= 0 && j+a <= 6){
      if(board[i-a][j+a] == 1){
        return 1;
      }
    }
    if(i+a <= 6 && j-a >= 0){
      if(board[i+a][j-a] == 1){
        return 1;
      }
    }
  }
  return 0;
}
void search_possible_solutions(int i, int j, int k, int l){
  columns_searched[0] = i;
  columns_searched[1] = j;
  columns_searched[2] = k;
  columns_searched[3] = l;
  int iter;
  for(iter = 0; iter <= 3; iter++){
    attempts[iter] = 0;
  }
  int finished;
  int m,n;
  int is_valid;
  while(1){
    if(find_next_solution() == 0){
      clear_board();
      break;
    }
    is_valid = 1;
    for(m = 0; m < 7; m++){
      for(n = 0; n < 7; n++){
        if(!can_be_attackted(m,n)){
          is_valid = 0;
          break;
        }
      }
    }
    if(is_valid){
      for(i = 0; i <= 6;i++){
        for(j = 0; j <= 6; j++){
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

int find_next_solution(){
  clear_board();
  int i;
  int not_an_answer = 0;
  int answer_exists = 1;
  for(i = 0; i <= 3; i++){
    board[attempts[i]][columns_searched[i]] = 1;
  }
  if(attempts[0] == 0 && attempts[1] == 0 && attempts[2] == 0 && attempts[3] == 0){
    not_an_answer = 1;
  }
  if(not_an_answer){
    board[attempts[1]][columns_searched[1]] = 0;
    board[attempts[2]][columns_searched[2]] = 0;
    board[attempts[3]][columns_searched[3]] = 0;
    find_spot(1);
    if(find_spot(2) == 0){
      backtrack(2);
    }
    if(find_spot(3) == 0){
      answer_exists = backtrack(3);
    }
  }
  else{
    board[attempts[3]][columns_searched[3]] = 0;
    if(find_spot(3) == 0){
      answer_exists = backtrack(3);
    }

  }
//  printf("%d%d\t%d%d\t%d%d\t%d%d\n",attempts[0],columns_searched[0],attempts[1],columns_searched[1],attempts[2],columns_searched[2],attempts[3],columns_searched[3]);
  return answer_exists;
}

int find_spot(int position){
  int i = attempts[position];
  if( i == 6){
    return 0;
  }
  i++;
  while(i < 6 && can_be_attackted(i,columns_searched[position])){
    i++;
  }
  if(i == 6){
    if(can_be_attackted(i,columns_searched[position])){
      return 0;
    }
    else{
      board[attempts[position]][columns_searched[position]] = 0;
      attempts[position] = i;
      board[i][columns_searched[position]] = 1;
    }
  }
  else{
    board[attempts[position]][columns_searched[position]] = 0;
    attempts[position] = i;
    board[i][columns_searched[position]] = 1;
  }
  return 1;
}

int backtrack(int position){
  board[attempts[position]][columns_searched[position]] = 0;
  attempts[position] = 0;
  if(position == 1){
    if(attempts[0] == 6){
      return 0;
    }

    else{
      while(attempts[0] < 6){
        board[attempts[0]][columns_searched[0]] = 0;
        attempts[0]++;
        board[attempts[0]][columns_searched[0]] = 1;
        if(find_spot(1) == 1){
          return 1;
        }
      }
      return 0;
    }
  }


if( attempts[position-1] == 6){
  while(attempts[0] <= 6 && backtrack(position -1 )){
  if(find_spot(position) == 1){
    return 1;
  }
  }
}
else{
  while(attempts[ position -1 ] < 6){
    board[attempts[position-1]][columns_searched[position-1]] = 0;
    attempts[position - 1 ]++;
    if(!can_be_attackted(attempts[position-1],columns_searched[position-1])){
    board[attempts[position-1]][columns_searched[position-1]] = 1;
    if(find_spot(position) == 1){
      return 1;
    }
  }
  }

  while(attempts[0] <= 6 && backtrack(position - 1)){
    if(find_spot(position) == 1){
      return 1;
    }
  }
}
return 0;
}
