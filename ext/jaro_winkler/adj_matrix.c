#include <stdlib.h>
#include <string.h>
#include "adj_matrix.h"
#include "codepoints.h"

const char *DEFAULT_ADJ_TABLE[] = {
  "A","E", "A","I", "A","O", "A","U", "B","V", "E","I", "E","O", "E","U", "I","O", "I","U", "O","U",
  "I","Y", "E","Y", "C","G", "E","F", "W","U", "W","V", "X","K", "S","Z", "X","S", "Q","C", "U","V",
  "M","N", "L","I", "Q","O", "P","R", "I","J", "2","Z", "5","S", "8","B", "1","I", "1","L", "0","O",
  "0","Q", "C","K", "G","J", "E"," ", "Y"," ", "S"," "
};


AdjMatrix* adj_matrix_new(unsigned int capacity){
  AdjMatrix *matrix = malloc(sizeof(AdjMatrix));
  if (matrix) {
    matrix->d_capacity = capacity;
    matrix->d_length = 0;
    matrix->d_maxchar = 0;
    matrix->d_rowstart = NULL;
    matrix->d_colindex = NULL;
  }
  return matrix;
}

void adj_matrix_add(AdjMatrix *matrix, UnicodeCharCode x, UnicodeCharCode y){
}

static inline UnicodeCharCode
adj_matrix_min(const UnicodeCharCode x,
	       const UnicodeCharCode y)
{
  return (x < y) ? x : y;
}

static inline UnicodeCharCode
adj_matrix_max(const UnicodeCharCode x,
	       const UnicodeCharCode y)
{
  return (x > y) ? x : y;
}

static UnicodeCharCode
adj_matrix_maxrow(const UnicodeCharCode * const x,
		  const UnicodeCharCode * const y,
		  const unsigned int num)
{
  UnicodeCharCode maxrow = 0u;
  for(unsigned int i = 0; i < num; ++i) {
    maxrow = adj_matrix_max(maxrow, adj_matrix_min(x[i], y[i]));
  }
  return maxrow;
}

void adj_matrix_add_multiple(AdjMatrix *matrix,
			     const UnicodeCharCode *x,
			     const UnicodeCharCode *y,
			     unsigned int num)
{
  if (matrix) {
    if (matrix->d_length) {
    }
    else {
      const unsigned int actualnum = ((num < matrix->d_capacity) ? num : matrix->d_capacity);
      unsigned int i;
      int count = 0;
      UnicodeCharCode row, column;
      const UnicodeCharCode maxrow = adj_matrix_maxrow(x, y, actualnum);
      const unsigned int maxrowind = maxrow + 2u;
      matrix->d_colindex = malloc(sizeof(UnicodeCharCode)*matrix->d_capacity);
      memset(matrix->d_colindex, 0, sizeof(UnicodeCharCode)*matrix->d_capacity);
      matrix->d_rowstart = malloc(sizeof(int)*maxrowind);
      memset(matrix->d_rowstart, 0, sizeof(int)*maxrowind);
      matrix->d_maxchar = maxrow + 1;
      for(i = 0; i < actualnum; ++i) {
	row = adj_matrix_min(x[i], y[i]);
	++(matrix->d_rowstart[row]);
      }
      for(i = 1; i <= maxrowind; ++i) {
	int tmp = matrix->d_rowstart[i-1];
	matrix->d_rowstart[i-1] = count;
	count += tmp;
      }
      for(i = 0; i < actualnum; ++i) {
	row = adj_matrix_min(x[i], y[i]);
	column = adj_matrix_max(x[i], y[i]);
	for(count = matrix->d_rowstart[row]; count < matrix->d_rowstart[row+1]; ++count) {
	  if (matrix->d_colindex[count] == 0) {
	    matrix->d_colindex[count] = column;
	    break;
	  }
	}
      }
      matrix->d_length += (int)actualnum;
    }
  }
}

char adj_matrix_find(const AdjMatrix *matrix, UnicodeCharCode x, UnicodeCharCode y){
  if (matrix && (matrix->d_length > 0)) {
    if (y < x) {			/* x should always be < y */
      UnicodeCharCode tmp = y;
      y = x;
      x = tmp;
    }
    if (x < matrix->d_maxchar) {
      const int last = matrix->d_rowstart[x+1];
      /* assumes d_colindex is unordered */
      for(int j = matrix->d_rowstart[x]; j < last; ++j) {
	if (matrix->d_colindex[j] == y) return 1;
      }
    }
  }
  return 0;
}

void adj_matrix_free(AdjMatrix *matrix){
  if (matrix){
    if (matrix->d_rowstart) free(matrix->d_rowstart);
    if (matrix->d_colindex) free(matrix->d_colindex);
    memset(matrix, 0, sizeof(AdjMatrix));
    free(matrix);
  }
}

AdjMatrix* adj_matrix_default(){
  static char first_time = 1;
  static AdjMatrix *ret_matrix;
  if(first_time){
    const unsigned int length = sizeof(DEFAULT_ADJ_TABLE)/sizeof(char*)/2;
    UnicodeCharCode 
      *x = malloc(sizeof(UnicodeCharCode)*length), 
      *y = malloc(sizeof(UnicodeCharCode)*length);
    ret_matrix = adj_matrix_new(length);
    for(unsigned int i = 0; i < length; ++i){
      UnicodeHash h = unicode_hash_new(DEFAULT_ADJ_TABLE[i << 1]);
      x[i] = h.code;
      h = unicode_hash_new(DEFAULT_ADJ_TABLE[(i << 1) + 1]);
      y[i] = h.code;
    }
    adj_matrix_add_multiple(ret_matrix, x, y, length);
    free(x);
    free(y);
    first_time = 0;
  }
  return ret_matrix;
}
