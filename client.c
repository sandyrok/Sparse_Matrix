#include "header.h"
       		

int main()
{
  int m,n,m_a,n_a,m_b, n_b, nnz,cnt,nnz_a,nnz_b, nnz_c;
  int print_choice,flag = 1;
  float dnz,dng, dnz_a,dng_a,dnz_b,dng_b;
  coo_entry* spar_mat_coo = NULL, *spar_mat_coo_a = NULL, *spar_mat_coo_b = NULL,*spar_mat_coo_c = NULL;
  csr_entry* spar_mat_csr = NULL, *spar_mat_csr_a = NULL;
  csc_entry* spar_mat_csc = NULL, *spar_mat_csc_b = NULL, *spar_mat_csc_row_op = NULL;
  int *row_ptr = NULL, *val = NULL, *col_ind = NULL, *row_ptr_a = NULL, *col_ptr_b = NULL;
  int *col_ptr = NULL, *val1 = NULL, *row_ind = NULL;
  int* mul_row = NULL, *mul_col = NULL, *mul_val = NULL;
  int i,r1,k1,r2,k2;
  int choice, option;
  bool validity_csr, validity_csc, validity_coo;
 
 while(flag) {
   scanf("%d", &choice);  
  switch(choice) 
  {
     case 1:  scanf("%d%d",&m,&n);
              scanf("%f%f",&dnz,&dng);
              nnz = rand_gen(m,n,dnz,dng,&spar_mat_coo);
              print_coo_util(spar_mat_coo, nnz);
              break;    
     case 2:  scanf("%d", &option);
              if(option == 1)
                { if(spar_mat_coo == NULL)
                   {  scanf("%d%d",&m,&n);
                      scanf("%f%f",&dnz,&dng);
                      nnz = rand_gen(m,n,dnz,dng,&spar_mat_coo);
                   } }
              else if(option == 2){ 
                  scanf("%d%d%d",&m,&n,&nnz);
                  if(!read_validate_coo(m,n,nnz,&spar_mat_coo))
                    { printf("Not a valid COO representation\n"); break;}
                 }
              else
                 { printf("Invalid option\n"); break;}
             coo_to_csr_csc(nnz, m, n, &row_ptr, &spar_mat_csr, &col_ptr, &spar_mat_csc, spar_mat_coo, 3);
             print_csr_util(m, row_ptr, spar_mat_csr);
             print_csc_util(n, col_ptr, spar_mat_csc);
             break;
     case 3: scanf("%d", &option);
             if(option == 1){
                 if(spar_mat_csr == NULL || spar_mat_csc ==NULL)
                   {  scanf("%d%d",&m_a,&n_a);
                      scanf("%f%f",&dnz_a,&dng_a);
                      nnz_a = rand_gen(m_a,n_a,dnz_a,dng_a,&spar_mat_coo_a);
                      coo_to_csr_csc(nnz_a, m_a, n_a, &row_ptr_a, &spar_mat_csr_a, NULL, NULL, spar_mat_coo_a, 1);
                      scanf("%d%d",&m_b,&n_b);
                      scanf("%f%f",&dnz_b,&dng_b);
                      nnz_b = rand_gen(m_b,n_b,dnz_b,dng_b,&spar_mat_coo_b);
                      coo_to_csr_csc(nnz_b, m_b, n_b, NULL, NULL, &col_ptr_b, &spar_mat_csc_b, spar_mat_coo_b, 2);
                   }
                   else {
                    spar_mat_csr_a = spar_mat_csr;
                    spar_mat_csc_b = spar_mat_csc;
                    row_ptr_a = row_ptr;
                    col_ptr_b = col_ptr;
                   m_a = m_b = m;
                   n_a = n_b = n;
                   }
                   
                   } 
              else if(option == 2)
                   {
                      validity_csr = validity_csc = true;
                      scanf("%d%d%d",&m_a,&n_a,&nnz_a);
                       validity_csr = read_validate_csr(m_a,n_a,nnz_a, &spar_mat_csr_a,&row_ptr_a);
                       scanf("%d%d%d",&m_b,&n_b,&nnz_b);
                       validity_csc = read_validate_csc(m_b,n_b,nnz_b, &spar_mat_csc_b,&col_ptr_b);
                       if(!validity_csr)
                          { printf("Not a valid CSR representation\n"); break;}
                       if(!validity_csc)
                           { printf("Not a valid CSC representation\n"); break;}
                   }
              else
                 { printf("Invalid option\n"); break;}
              print_csr_util(m_a, row_ptr_a, spar_mat_csr_a); 
              print_csc_util(n_b, col_ptr_b, spar_mat_csc_b);
              nnz_c = spar_matrix_mul(m_a, n_a, m_b, n_b, row_ptr_a, spar_mat_csr_a, col_ptr_b, spar_mat_csc_b, &spar_mat_coo_c);
              if(nnz != -1)
              print_coo_util(spar_mat_coo_c, nnz_c);   
              break;
             
     case 4: scanf("%d", &option);
             if(option == 1)
                { if(spar_mat_csc == NULL || col_ptr == NULL )
                   {  scanf("%d%d",&m,&n);
                      scanf("%f%f",&dnz,&dng);
                      nnz = rand_gen(m,n,dnz,dng,&spar_mat_coo);
                      coo_to_csr_csc(nnz, m, n, NULL, NULL, &col_ptr, &spar_mat_csc_row_op, spar_mat_coo, 2);
                   } 
                   else
                      spar_mat_csc_row_op = spar_mat_csc;   
                   
                }
              else if(option == 2){
                  validity_csc = true; 
                  scanf("%d%d%d",&m,&n,&nnz);
                  validity_csc = read_validate_csc(m,n,nnz, &spar_mat_csc_row_op,&col_ptr);
                   
                 }
              else
                 { printf("Invalid option\n"); break;}
              
              print_csc_util(n, col_ptr, spar_mat_csc_row_op);
              scanf("%d%d%d%d",&r1,&k1,&r2,&k2);
              if(!validity_csc)
               { printf("Not a valid CSC representation\n"); break;}
	      nnz = row_op( m,n,r1,k1,r2,k2,col_ptr,&spar_mat_csc_row_op);
              if(nnz!= -1)
              print_csc_util(n, col_ptr, spar_mat_csc_row_op);
              spar_mat_csc = spar_mat_csc_row_op;
              print_coo_util(spar_mat_coo_c, nnz_c); 
              break;
     case 5:  scanf("%d", &option);
              if(option == 1)
              {
                if(spar_mat_coo_c == NULL || spar_mat_csc == NULL || spar_mat_csr == NULL)
               { scanf("%d%d",&m,&n);
                 scanf("%f%f",&dnz,&dng);
                 scanf("%d", &print_choice);
                 nnz = rand_gen(m,n,dnz,dng,&spar_mat_coo_c);
                 coo_to_csr_csc(nnz, m, n, &row_ptr, &spar_mat_csr, NULL, NULL, spar_mat_coo, 1);
                 coo_to_csr_csc(nnz, m, n, NULL, NULL, &col_ptr, &spar_mat_csc, spar_mat_coo, 2);
               }
                scanf("%d", &print_choice);
                switch(print_choice)
                {
                   case 1: print_coo_csr_csc(m, n, nnz, spar_mat_coo_c, NULL, NULL, NULL, NULL, print_choice);
                           break;
                   case 2: print_coo_csr_csc(m, n, nnz, NULL, row_ptr, spar_mat_csr, NULL, NULL, print_choice);
                           break;
                   case 3: print_coo_csr_csc(m, n, nnz, NULL, NULL, NULL, col_ptr, spar_mat_csc, print_choice);
                           break;
                   default:printf("Invalid option f\n"); break;
                }
              }
              else if(option == 2){
                 scanf("%d", &print_choice);
                  switch(print_choice)
                {
                   case 1:   scanf("%d%d%d",&m,&n,&nnz);
                            if(!read_validate_coo(m,n,nnz,&spar_mat_coo))
                                { printf("Not a valid COO representation\n"); break;};
                             print_coo_csr_csc(m, n, nnz, spar_mat_coo, NULL, NULL, NULL, NULL, print_choice);  
                             break;
                             
                   case 2:  scanf("%d%d%d",&m,&n,&nnz);
                            if(!read_validate_csr(m,n,nnz, &spar_mat_csr,&row_ptr))
                               { printf("Not a valid CSR representation\n"); break;}
                              print_coo_csr_csc(m, n, nnz, NULL, row_ptr, spar_mat_csr, NULL, NULL, print_choice);
                               break;
                   case 3: scanf("%d%d%d",&m,&n,&nnz);
                           if(!read_validate_csc(m,n,nnz, &spar_mat_csc,&col_ptr))
                               { printf("Not a valid CSC representation\n"); break;}
                              print_coo_csr_csc(m, n, nnz, NULL, NULL, NULL, col_ptr, spar_mat_csc, print_choice);
                           break;
                   default:printf("Invalid option\n"); break;
                }
              }
              else
              {
                printf("Invalid option\n"); break;
              }
              break;
        default: flag = 0; 
   }
    }  
  return 0;  
}
