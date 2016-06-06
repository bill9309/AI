//This is a program for building a 8 input 3 output encoder with Back Propogation Neural Network
//There are three layers. Input Layer has 8 units, Output has 3 units, Hidden Layer has a number that the user can determine

/*Ideal input and output

0 0 0 0 0 0 0 1 | 0 0 0
0 0 0 0 0 0 1 0 | 0 0 1
0 0 0 0 0 1 0 0 | 0 1 0
0 0 0 0 1 0 0 0 | 0 1 1
0 0 0 1 0 0 0 0 | 1 0 0
0 0 1 0 0 0 0 0 | 1 0 1
0 1 0 0 0 0 0 0 | 1 1 0
1 0 0 0 0 0 0 0 | 1 1 1

*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
int number_units_hidden;
int learning_coefficient;
int trial_times;
//int * input_layer;
//int * hidden_layer;
//int * output_layer;
double ** input_hidden_connections;
double ** hidden_output_connections;
int ** training_data_input;
int ** training_data_output;
double * input_input;
double * output_input;
double * input_hidden;
double * output_hidden;
double * input_output;
double * output_output;
double sigmoid(double x){
  return 1/(1 + exp( -1.0 * x));
}

void initialize_connections(){
  int i,j;
  input_hidden_connections = malloc(sizeof(double *) * 8);
  for(i = 0; i < 8; i++){
    input_hidden_connections[i] = malloc(sizeof(double) * number_units_hidden);
    for(j = 0; j < number_units_hidden; j++){
      input_hidden_connections[i][j] = 1.0 * rand()/RAND_MAX;
    }
  }
  hidden_output_connections = malloc(sizeof(double * ) * number_units_hidden);
  for(i = 0; i < number_units_hidden; i++){
    hidden_output_connections[i] = malloc(sizeof(double) * 3);
    for(j = 0; j < 3; j++){
      hidden_output_connections[i][j] = 1.0 * rand()/RAND_MAX;
    }
  }
  }



void set_training_data(){
  int i;
  training_data_input = malloc(sizeof(int *) * 8);
  for(i = 0; i < 8; i ++){
    training_data_input[i] = calloc(8, sizeof(int));
  }
  for(i = 0; i < 8; i++){
    training_data_input[i][7 - i] = 1;
  }
  training_data_output = malloc(sizeof(int * ) * 8);
  for(i = 0; i < 8; i++){
    training_data_output[i] = calloc(3, sizeof(int));
  }
  training_data_output[1][2] = 1;
  training_data_output[2][1] = 1;
  training_data_output[3][1] = 1;
  training_data_output[3][2] = 1;
  training_data_output[4][0] = 1;
  training_data_output[5][0] = 1;
  training_data_output[5][2] = 1;
  training_data_output[6][0] = 1;
  training_data_output[6][1] = 1;
  training_data_output[7][0] = 1;
  training_data_output[7][1] = 1;
  training_data_output[7][2] = 1;
}

void train_network(){
  int i,j,k,l;
  input_input = calloc(8, sizeof(double));
  output_input = malloc(sizeof(double) * 8);
  input_hidden = calloc(number_units_hidden, sizeof(double));
  output_hidden = malloc(sizeof(double) * number_units_hidden);
  input_output = calloc(3, sizeof(double));
  output_output = malloc(sizeof(double) * 3);
  double error_derivative;
  for(i = 0; i < trial_times; i++){
    int random = rand() % 8;

    //START calculating output of input layer
    for(j = 0; j < 8; j++){
       output_input[j] = training_data_input[random][j];
       //output_input[j] = sigmoid(input_input[j]);
    }
    //FIN calculating output of input layer

    //START calculating output of hidden layer
    for(j = 0; j < number_units_hidden; j++){
      input_hidden[j] = 0;
       for(k = 0; k < 8; k++){
         input_hidden[j] += input_hidden_connections[k][j] * output_input[k];
       }
    }
   for(j = 0; j < number_units_hidden; j++){
     output_hidden[j] = sigmoid(input_hidden[j]);
   }
   //FIN calculating output of hidden layer

   //START calculating output of output layer
   for(j = 0; j < 3; j++){
     input_output[j] = 0;
     for(k = 0; k < number_units_hidden; k++){
       input_output[j] += hidden_output_connections[k][j] * output_hidden[k];
     }
   }
   for(j = 0; j < 3; j++){
     output_output[j] = sigmoid(input_output[j]);
   }
   //FIN calculating output of output layer

   //START updating connection strength between input and hidden layer
   for(j = 0; j < 8; j++){
     for(k = 0; k < number_units_hidden; k++){
        double error_derivative_1 = fabs(output_output[0] - training_data_output[random][0]) * output_output[0] * (1 - output_output[0]) * hidden_output_connections[k][0];
        double error_derivative_2 = fabs(output_output[1] - training_data_output[random][1]) * output_output[1] * (1 - output_output[1]) * hidden_output_connections[k][1];
        double error_derivative_3 = fabs(output_output[2] - training_data_output[random][2]) * output_output[2] * (1 - output_output[2]) * hidden_output_connections[k][2];
        error_derivative = error_derivative_1 + error_derivative_2 + error_derivative_3;
        input_hidden_connections[j][k] -= learning_coefficient * error_derivative * output_hidden[k] * (1- output_hidden[k]) * output_input[j];
     }
   }
   //FIN updating connection strength between input and hidden layer

   //START updating connection strength between hidden and output layer
   for(j = 0; j < number_units_hidden; j++){
     for(k = 0; k < 3; k++){
       error_derivative = fabs(output_output[k] - training_data_output[random][k]);
       hidden_output_connections[j][k] -= learning_coefficient * error_derivative * output_output[k] * (1 - output_output[k]) * output_hidden[j];
    }
  }
   //FIN updating connection strength between hidden and output layer


  }
}

void test_network(){
  int i,j,k;
  for(i = 0; i < 8; i++){
    for(j = 0; j < 8; j++){
      output_input[j] = training_data_input[i][j];
      //output_input[j] = sigmoid(input_input[j]);
    }
    for(j = 0; j < number_units_hidden; j++){
      input_hidden[j] = 0;
       for(k = 0; k < 8; k++){
          input_hidden[j] += input_hidden_connections[k][j] * output_input[k];
       }
       output_hidden[j] = sigmoid(input_hidden[j]);
    }
    for(j = 0; j < 3; j++){
      input_output[j] = 0;
      for(k = 0; k < number_units_hidden; k++){
        input_output[j] += hidden_output_connections[k][j] * output_hidden[k];
      }
      output_output[j] = sigmoid(input_output[j]);
    }
    printf("Input: %d %d %d %d %d %d %d %d Output: %f %f %f\n",training_data_input[i][0],training_data_input[i][1],
  training_data_input[i][2],training_data_input[i][3],training_data_input[i][4],training_data_input[i][5],
training_data_input[i][6],training_data_input[i][7],output_output[0],output_output[1],output_output[2]);
  }
}

int main(int argc, char ** argv){
  if(argc < 4){
    printf("Usage: %s Number_Units_Hidden Learning_Coefficient Trial_Times\n" ,argv[0]);
    exit(1);
  }
  number_units_hidden = atoi(argv[1]);
  learning_coefficient = atof(argv[2]);
  trial_times = atoi(argv[3]);
  srand((unsigned)time(NULL));
  initialize_connections();
  set_training_data();
  train_network();
  test_network();
  return 0;
}
