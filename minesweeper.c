#include "minesweeper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void select_recursion(struct cell cel);


void init_game(struct cell *game, int dim, int *dim_sizes, int num_mines, int **mined_cells) {

    int cell_count = 1;
    for(int i = 0; i < dim; i++){ //Calculates total number of cells
      cell_count *= dim_sizes[i];
    }

    for(int i = 0; i < cell_count; i++){ //Initalises the selected field of struct cell
      game[i].selected = 0;
      game[i].hint = 0;
    }

    int cartpro[2000][MAX_DIM]; //sets coordinates in its specific location by self made algorithm
    int z = 0;
    int o = 1;
    int x = 1;
    for(int i = 0; i < dim; i++){
      if(i != 0){
        o *= dim_sizes[i-1];
      }
      if(dim != 1 && i != (dim - 1)){
        x = dim_sizes[i+1];
      }
      if(i == (dim - 1)){
        x = dim_sizes[i];
      }
      if(i % 2 == 0){
        if(i == 0){
          for(int k = 0; k < x; k++){
            for(int j = k; j < cell_count; j += x){
              if(z == (dim_sizes[i] - 1)){
                cartpro[j][i] = z % dim_sizes[i];
                z = 0;
              }else{
                cartpro[j][i] = z % dim_sizes[i];
                z++;
              }
            }
          }
        }else{
          for(int l = 0; l < o; l += x){
            for(int k = 0; k < x; k++){
              for(int j = l + k; j < cell_count; j += o){
                if(z == (dim_sizes[i] - 1)){
                  cartpro[j][i] = z % dim_sizes[i];
                  z = 0;
                }else{
                  cartpro[j][i] = z % dim_sizes[i];
                  z++;
                }
              }
            }
          }
        }
      }else{
        if(i == 1){
          for(int j = 0; j < cell_count; j++){
            if(z == (dim_sizes[i] - 1)){
              cartpro[j][i] = z % dim_sizes[i];
              z = 0;
            }else{
              cartpro[j][i] = z % dim_sizes[i];
              z++;
            }
          }
        }else{
          for(int l = 0; l < o; l += x){
            for(int k = 0; k < x; k++){
              for(int j = l + k; j < cell_count; j += o){
                if(z == (dim_sizes[i] - 1)){
                  cartpro[j][i] = z % dim_sizes[i];
                  z = 0;
                }else{
                  cartpro[j][i] = z % dim_sizes[i];
                  z++;
                }
              }
            }
          }
        }
      }
    }


    for(int i = 0; i < cell_count; i++){ //Setting up the coordinates for each cell
      memcpy(game[i].coords, cartpro[i], sizeof(game[i].coords));

      for(int j = 0; j < num_mines; j++){ //Checking if the coordinates has a mine
        int count = 0;
        for(int k = 0; k < dim; k++){
          if(mined_cells[j][k] == cartpro[i][k]){
            count++;
          }
        }
        if(count == dim){
          game[i].mined = 1;
          break;
        }
      }
    }


    for(int i = 0; i < cell_count; i++){ //Checking Adjacent cells
      int adcount = 0;
      for(int k = i + 1; k < cell_count; k++){
        int count = 0;
        for(int j = 0; j < dim; j++){
          if(game[i].coords[j] - game[k].coords[j] == 0 || game[i].coords[j] - game[k].coords[j] == 1 || game[i].coords[j] - game[k].coords[j] == -1){
            count++;
          }
        }
        if(count == dim){
          game[i].adjacent[adcount] = &game[k];
          adcount++;
          if(game[k].mined == 1){
            game[i].hint += 1;
          }
        }
      }
      game[i].num_adjacent = adcount;//Setting the number of adjacent cells of this (i) cell
    }

    for(int i = (cell_count - 1); i >= 0; i--){ //Checking Adjacent cells again
      for(int k = i - 1; k >= 0; k--){
        int count = 0;
        for(int j = 0; j < dim; j++){
          if(game[i].coords[j] - game[k].coords[j] == 0 || game[i].coords[j] - game[k].coords[j] == 1 || game[i].coords[j] - game[k].coords[j] == -1){
            count++;
          }
        }
        if(count == dim){
          game[i].adjacent[game[i].num_adjacent] = &game[k];
          game[i].num_adjacent += 1;
          if(game[k].mined == 1){
            game[i].hint += 1;
          }
        }
      }
    }


    return;
}

int select_cell(struct cell *game, int dim, int *dim_sizes, int *coords) {

    for(int i = 0; i < dim; i++){
      if(coords[i] < 0){
        return 0;
      }
      if(coords[i] >= dim_sizes[i]){
        return 0;
      }
    }

    int cell_count = 1;
    for(int i = 0; i < dim; i++){ //Calculates total number of cells
      cell_count *= dim_sizes[i];
    }

    for(int i = 0; i < cell_count; i++){
      int count = 0;
      for(int j = 0; j < dim; j++){
        if(game[i].coords[j] == coords[j]){
          count++;
        }
      }
      if(count == dim){ //If this cell is selected
        if(game[i].selected == 1){ //If already previously selected
          return 0;
        }else if(game[i].mined == 1){ //If this cell has a mine
          game[i].selected = 1;
          return 1;
        }else{ //This will execute if it is a new cell and call the recursive function to select the adjacent cells
          game[i].selected = 1;
          select_recursion(game[i]);
          int check = 0;
          int m = 0;
          for(int c = 0; c < cell_count; c++){ //Checks wining condition
            if(game[c].selected == 1 && game[c].mined == 0){
              check++;
            }
            if(game[c].mined == 1){
              m++;
            }
          }

          if(check == (cell_count - m)){ //Returns if won or game continues
            return 2;
          }else{
            return 0;
          }
        }
      }
    }

    return 0;
}

void select_recursion(struct cell cel){ //Recursive function that selects every adjacent cell until it has reached a non zero number of adjacent mined cells
    if(cel.hint == 0){
      for(int i = 0; i < cel.num_adjacent; i++){
        if(cel.adjacent[i]->selected == 0){
          cel.adjacent[i]->selected = 1;
          select_recursion(*(cel.adjacent[i]));
        }
      }
    }
    return;
}
