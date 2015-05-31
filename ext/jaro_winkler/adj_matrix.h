#ifndef ADJ_MATRIX_H
#define ADJ_MATRIX_H 1
#include "codepoints.h"

/**
 * A sparse matrix data structure to hold the adjacency matrix. Because values
 * are either zero or one, the actual value does not need to be stored only
 * the indices of the nonzero entries.
 *
 * http://en.wikipedia.org/wiki/Sparse_matrix#Compressed_row_Storage_.28CRS_or_CSR.29
 */
struct AdjMatrix_t {
  unsigned int     d_capacity;	/* how many elements can this matrix hold */
  unsigned int     d_length;	/* how many elements does it currently have */
  UnicodeCharCode  d_maxchar;	/* one higher than the maximum Unicode character code */
  int             *d_rowstart;	/* there are d_maxchar + 2 of these */
  UnicodeCharCode *d_colindex;	/* there are d_capacity of these */
};

typedef struct AdjMatrix_t AdjMatrix;

AdjMatrix* adj_matrix_new         (unsigned int capacity);
/**
 * adj_matrix_add is very inefficient. It's more efficient to add
 * all the elements in a single call to add_multiple.
 */
void       adj_matrix_add         (AdjMatrix *matrix, UnicodeCharCode x, UnicodeCharCode y);
void       adj_matrix_add_multiple(AdjMatrix *matrix, 
				   const UnicodeCharCode *x,
				   const UnicodeCharCode *y,
				   unsigned int num);
char       adj_matrix_find        (const AdjMatrix *matrix, UnicodeCharCode x, UnicodeCharCode y);
void       adj_matrix_free        (AdjMatrix *matrix);
AdjMatrix* adj_matrix_default     ();

#endif /* ADJ_MATRIX_H */
