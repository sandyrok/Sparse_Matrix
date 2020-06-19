#include<stdio.h>
#include<time.h>
#include<stdbool.h>
#include<stdlib.h>
typedef struct coo_entry
{
  int row;
  int col;
  int val;
} coo_entry;
typedef struct csr_entry
{
  int col_ind;
  int val;
} csr_entry;
typedef struct csc_entry
{
  int row_ind;
  int val;
} csc_entry;
typedef struct coo_entry_node
{
  coo_entry entry;
  struct coo_entry_node* next;
} node;

bool read_validate_coo(int m, int n, int nnz, coo_entry** spar_mat_coo);
bool read_validate_csr(int m, int n, int nnz, csr_entry** spar_mat_csr, int** row_ptr);
bool read_validate_csc(int m, int n, int nnz, csc_entry** spar_mat_csc, int** col_ptr);
bool sort_coo_pref(coo_entry a, coo_entry b, int pref);
void sort_coo_util(int low, int high, int mid, coo_entry* spar_mat_coo, int pref);
void sort_coo_entries(coo_entry* spar_mat_coo, int low, int high, int pref) ;
int rand_gen(int m, int n, float dnz, float dng, coo_entry** spar_mat_coo);
void coo_to_csr_csc(int nnz, int m, int n, int **row_ptr, csr_entry** spar_mat_csr, int **col_ptr, csc_entry** spar_mat_csc, coo_entry* spar_mat_coo, int choice);
int spar_matrix_mul(int ma,int na,int mb, int nb, int* row_ptr, csr_entry* spar_mat_csr, int* col_ptr, csc_entry* spar_mat_csc, coo_entry** spar_mat_coo);
int row_op(int m, int n, int r1, int k1, int r2, int k2, int *col_ptr, csc_entry** spar_mat_csc);
void print_coo_csr_csc(int m, int n, int nnz, coo_entry* spar_mat_coo, int* row_ptr, csr_entry* spar_mat_csr, int* col_ptr, csc_entry* spar_mat_csc, int choice);
void print_coo_util(coo_entry* spar_mat_coo, int nnz);
void print_csr_util(int m,int* row_ptr, csr_entry* spar_mat_csr);
void print_csc_util(int n,int* col_ptr, csc_entry* spar_mat_csc);
