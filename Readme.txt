Functions' Description:
1.rand_gen():
 ->Generates random non-zero entries from m*n possible choices by sampling without replacement.
 ->Prints error messages upon detection of invalid density percentage values, too large or non-positive values of matrix size.
 ->Sorts the resultant pairs using merge sort and produces the COO representation.
2.coo_to_csr_csc():
 ->Takes COO representation and sorts the entries in row major order using merge sort before creating the CSR representation.
 ->Sorts the COO entries in column major order using merge sort and creates the CSC format.
 ->Detects faulty COO representation and prints error message accordingly.
3.spar_matrix_mul():
 ->Takes A in CSR format, B in CSC format. 
 ->Finds the non-zero entries in the result and appends them to the linked list of these values and extracts COO representation from the linkedlist.
 -> Detects faulty CSC and CSR representations and prints error message accordingly.
4.row_op():
 ->For each column in the target row, it checks if the row operation would result in a non-zero element and stores the non-zero element if and when it occurs.
 ->Creates a CSC representation by adding new non-zero entries if any and deleting old non-zero entries which become zero after the row operation.
 ->Detects faulty CSC representations and prints error message accordingly.
5.print_coo_csr_csc()
  ->Detects faulty COO,CSC and CSR representations and prints error message accordingly.
  -> Rejects matrices of size greater than 20*20.
  ->Prints the matrix in m*n form with all the zero entries for each input format.


