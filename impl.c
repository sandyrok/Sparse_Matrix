#include "header.h"

bool is_invalid_size(int m, int n)
{
  if ( m > 150 || n > 100 || m < 1 || n < 1 )
  {printf("Allowed row size: 1 - 150, column size: 1 - 100\n");
   return true;
  }
   return false;
}

bool read_validate_coo(int m, int n, int nnz, coo_entry** spar_mat_coo)
{
   
    if(is_invalid_size(m,n))
        return false;
    if(nnz < 0)
      printf("No of non-negative integers should be non-negative\n");
    if(nnz == 0)
      return true;
    
    int i,j,k,prev;
    bool valid = true;
    *spar_mat_coo = (coo_entry *)malloc(nnz*sizeof(coo_entry));
    if(*spar_mat_coo == NULL )
      {
         printf("Memory Not Allocated\n");
	 exit(0);
      }
      
      for(i=0;i<nnz;i++)
        scanf("%d", &(*spar_mat_coo)[i].val);
     for(i=0,prev=0;i<nnz;i++)
        scanf("%d", &(*spar_mat_coo)[i].row); 
     for(i=0;i<nnz;i++)
        scanf("%d", &(*spar_mat_coo)[i].col); 
        
      sort_coo_entries(*spar_mat_coo, 0, nnz -1 , 1);    
     for(i=0,prev=0;i<nnz;i++)
        { 
           j = (*spar_mat_coo)[i].row;
           if( j <= 0 || j < prev || j > m)
             valid = false;
           prev = j;
        }
     for(i=0, prev = 0;i<nnz;i++)
        {
           j = (*spar_mat_coo)[i].col;
           if( j <= 0 || j > n || ( (j == prev)  && ((*spar_mat_coo)[i-1].row == (*spar_mat_coo)[i].row )) )
             { valid = false; 
             }
            prev = j;
        }
    if(!valid)
       { free(*spar_mat_coo);
         *spar_mat_coo = NULL;
       }
    return valid;

}

bool read_validate_csr(int m, int n, int nnz, csr_entry** spar_mat_csr, int** row_ptr)
{
      int i,j,k,prev;
    bool valid = true;
    if(is_invalid_size(m,n))
        return false;
    if(nnz < 0)
      printf("No of non-negative integers should be non-negative\n");
    if(nnz == 0)
      return true;
    
   
    *spar_mat_csr = (csr_entry *)malloc(nnz*sizeof(csr_entry));
    *row_ptr = (int *)malloc((m+1)*sizeof(int));
    if( *spar_mat_csr == NULL && *row_ptr == NULL )
      {
         printf("Memory Not Allocated\n");
	 exit(0);
      }
     for(i=0;i<nnz;i++)
        scanf("%d", &(*spar_mat_csr)[i].val);
     for(i=0;i<nnz;i++)
        { 
           scanf("%d", &j); 
           if( j <= 0 || j > n)
             valid = false;
           (*spar_mat_csr)[i].col_ind = j; 
        }
     for(i=0, prev = 0;i<=m;i++)
        {
           scanf("%d", &j); 
         if( j <= 0 || j < prev || j > nnz + 1)
             valid = false;
             prev = j;
            (*row_ptr)[i] = j -1 ;
         }
     if(  (*row_ptr)[0]!=0 || (*row_ptr)[m] != nnz )
        valid = false;
     if(valid)
      {  int xl,xr; 
         for(i = 0; i < m && valid; i++)
         {
              xl = (*row_ptr)[i];
              xr = (*row_ptr)[i+1];
              if(xl - xr > n)
                valid = false;
              
              for(int prev = 0 ;xl<xr && valid; xl++)
                 { if((*spar_mat_csr)[xl].col_ind <= prev)
                    valid = false;  prev = (*spar_mat_csr)[xl].col_ind; }
         }
      }
    return valid;

}

bool read_validate_csc(int m, int n, int nnz, csc_entry** spar_mat_csc, int** col_ptr)
{
   
    if(is_invalid_size(m,n))
        return false;
    if(nnz < 0)
      printf("No of non-negative integers should be non-negative\n");
    if(nnz == 0)
      return true;
    int i,j,k,prev;
    bool valid = true;
    *spar_mat_csc = (csc_entry *)malloc(nnz*sizeof(csc_entry));
    *col_ptr = (int *)malloc((n+1)*sizeof(int));
    if( *spar_mat_csc == NULL && *col_ptr == NULL )
      {
         printf("Memory Not Allocated\n");
	 exit(0);
      }
     
     for(i=0;i<nnz;i++)
        scanf("%d", &(*spar_mat_csc)[i].val);
     for(i=0,prev=0;i<nnz;i++)
        { 
           scanf("%d", &j); 
           if( j <= 0 || j > m)
             valid = false;
           (*spar_mat_csc)[i].row_ind = j; 
        }
   
     for(i=0, prev = 0;i<=n;i++)
        {
           
           scanf("%d", &j); 
           if( j <= 0 || j < prev || j > nnz + 1)
             valid = false;
             prev = j;
           (*col_ptr)[i] = j -1 ; 
        }
     if(  (*col_ptr)[0]!= 0 || (*col_ptr)[n] != nnz )
        valid = false;
     if(valid)
      {  int xl,xr; 
         for(i = 0; i < n && valid; i++)
         {
              xl = (*col_ptr)[i];
              xr = (*col_ptr)[i+1];
              if(xl - xr > m)
                valid = false;
              
              for(int prev = 0 ;xl<xr && valid; xl++)
                 { if((*spar_mat_csc)[xl].row_ind <= prev)
                    valid = false; prev  = (*spar_mat_csc)[xl].row_ind; }
         }
      }
    return valid;
}


bool sort_coo_pref(coo_entry a, coo_entry b, int pref)
{
    if(pref == 1)
       return a.row < b.row || ( a.row == b.row && a.col < b.col) ;
    
    return a.col < b.col || ( a.col == b.col && a.row < b.row ) ;
}


void sort_coo_util(int low, int high, int mid, coo_entry* spar_mat_coo, int pref) 
{ 
  int nl, nr;
  int i,j,k; 
  nl = mid - low + 1;
  nr = high - mid;
   coo_entry left[nl], right[nr];
   
    for(j=0;j<nl;j++)
       left[j] = spar_mat_coo[ low + j];
    for(j=0;j<nr;j++)
       right[j] = spar_mat_coo[ mid + 1 + j];
   for(i=0, j = 0, k=low; i < nl && j < nr; )
       if(sort_coo_pref( left[i], right[j], pref)) 
            spar_mat_coo[k++] = left[i++];
       else
            spar_mat_coo[k++] = right[j++];  
    while (i<nl) 
       spar_mat_coo[k++] = left[i++];
    while (j<nr) 
    spar_mat_coo[k++] = right[j++];  
}
 
void print_coo_util(coo_entry* spar_mat_coo, int nnz)
{
  if(spar_mat_coo == NULL)
    return;
  int i,j,k;
  for(i=0;i<nnz;i++)
       printf("%4d ", spar_mat_coo[i].val);
  printf("\n");
  for(i=0;i<nnz;i++)
       printf("%4d ", spar_mat_coo[i].row);
       printf("\n");
  for(i=0;i<nnz;i++)
       printf("%4d ", spar_mat_coo[i].col); 
  printf("\n");
} 

void print_csr_util(int m,int* row_ptr, csr_entry* spar_mat_csr)
{
  if(spar_mat_csr == NULL)
    return;
  int i,nnz = row_ptr[m];
  for(i=0;i<nnz;i++)
     printf("%4d ", spar_mat_csr[i].val);
  printf("\n");
  for(i=0;i<nnz;i++)
       printf("%4d ", spar_mat_csr[i].col_ind); 
  printf("\n");
  for(i=0;i<=m;i++)
         printf("%4d ",row_ptr[i]+1);  
   printf("\n");      
  
}

void print_csc_util(int n,int* col_ptr, csc_entry* spar_mat_csc)
{
  if(spar_mat_csc == NULL)
    return;
  int i,nnz = col_ptr[n];
  for(i=0;i<nnz;i++)
     printf("%4d ", spar_mat_csc[i].val);
  printf("\n");
  for(i=0;i<nnz;i++)
       printf("%4d ", spar_mat_csc[i].row_ind); 
  printf("\n");
  for(i=0;i<=n;i++)
         printf("%4d ",col_ptr[i]+1);  
   printf("\n");      
}


 
void sort_coo_entries(coo_entry* spar_mat_coo, int low, int high, int pref) 
{ 
   int mid;
   if(low <high)
   {
       mid = (low + high) / 2;
       sort_coo_entries(spar_mat_coo, low, mid, pref);
       sort_coo_entries(spar_mat_coo, mid + 1, high, pref);
       sort_coo_util(low, high, mid,  spar_mat_coo, pref);
   }
} 


int rand_gen(int m, int n, float dnz, float dng, coo_entry** spar_mat_coo)
{
	
	 if(is_invalid_size(m,n))
	     return 0;
         if( dng > dnz ){
                printf(" No of negative numbers can't be greater than no of non-zero numbers\n"); return 0;}
         if(dng < 0 || dng > 100 || dnz <0 || dnz > 100){
                printf("Density as a perecentage should lie between 0 and 100\n"); return 0;}
	
	int nnz,nng,r,c,tmp;
	int i,j,k,mx;
    int *flag;
    flag = (int *)malloc(m*n*sizeof(int));
	if(flag == NULL)
	{
		printf("Memory Not Allocated\n");
	    exit(0);
	}
	for(i=0;i<m*n;i++)
      flag[i] = i;
	nnz = (dnz/100)*m*n;
	nng = (dng/100)*m*n;
      if(nnz == 0)
         { *spar_mat_coo = NULL; return 0;}
   

	*spar_mat_coo = (coo_entry *)malloc(nnz*sizeof(coo_entry));
    if(*spar_mat_coo == NULL )
   	{
           printf("Memory Not Allocated\n");
	   exit(0);
    }
	
    
	srand(time(NULL));
	for(i=0,k=0;i<nnz;i++)
	{
             mx = m*n-i;
             j = rand()%mx;
             tmp = flag[j];
             flag[j]=flag[mx-1];
              j = tmp;
		r =  j/n+1;
        c = j%n+1;
        (*spar_mat_coo)[i].row = r;
	    (*spar_mat_coo)[i].col = c;
		if(k<nng)
		{
			k++;
		    (*spar_mat_coo)[i].val= -(rand()%100 + 1);
		}
		else 
			(*spar_mat_coo)[i].val = rand()%100 + 1;

    }
        sort_coo_entries(*spar_mat_coo, 0, nnz -1 , 1); 
	return nnz;
}
void coo_to_csr_csc(int nnz, int m, int n, int **row_ptr, csr_entry** spar_mat_csr, int **col_ptr, csc_entry** spar_mat_csc, coo_entry* spar_mat_coo, int choice)
{
    int i,j,k,t_row,t_col,t_vall,tmp;
    if(choice%2) {
    sort_coo_entries(spar_mat_coo, 0, nnz -1 , 1); 
    *row_ptr = (int *)malloc((m+1)*sizeof(int));
	*spar_mat_csr = (csr_entry *)malloc(nnz*sizeof(csr_entry));
    if(*row_ptr == NULL || *spar_mat_csr == NULL )
   	{
       printf("Memory Not Allocated\n");
	   exit(0);
    }
    for(i=0,k=0,tmp = 0; i<nnz ;i++)
    {
       (*spar_mat_csr)[i].val = spar_mat_coo[i].val;
       (*spar_mat_csr)[i].col_ind = spar_mat_coo[i].col;
        if(spar_mat_coo[i].row != tmp){
           while( tmp < spar_mat_coo[i].row )
           {
               (*row_ptr)[k++] = i;
               tmp++;
           }  
        }
    }
    while(k <=m)
    (*row_ptr)[k++] = nnz;
    }
    
    if(choice/2) {
      sort_coo_entries(spar_mat_coo, 0, nnz -1 , 2); 
   
    *col_ptr = (int *)malloc((n+1)*sizeof(int));
    *spar_mat_csc = (csc_entry *)malloc(nnz*sizeof(csc_entry));
	
	if(*col_ptr == NULL || *spar_mat_csc == NULL)
   	{
       printf("Memory Not Allocated\n");
	   exit(0);
    }
    for(i=0,k=0,tmp = 0; i<nnz ;i++)
    {
       
       (*spar_mat_csc)[i].val = spar_mat_coo[i].val;
       (*spar_mat_csc)[i].row_ind = spar_mat_coo[i].row;
        if(spar_mat_coo[i].col != tmp){
           while( tmp < spar_mat_coo[i].col )
           {
               (*col_ptr)[k++] = i;
               tmp++;
           }  
        }
    }
    while(k<=n)
    (*col_ptr)[k++] = nnz;
   }
}

int spar_matrix_mul(int ma,int na,int mb, int nb, int* row_ptr,csr_entry* spar_mat_csr, int* col_ptr, csc_entry* spar_mat_csc, coo_entry** spar_mat_coo)
{
 
 if(na != mb)
   { printf("Matrices not conformable for multiplication\n");
     return -1;
   }
  node* head = NULL ,*tail = NULL;
  int i,j,k,x,y,tmp;
  int xl,xr,yl,yr,xi,yi,cnt,f;
  for(i = cnt =  0; i < ma; i++)
  {
     xl = row_ptr[i];
     xr = row_ptr[i+1];
     for( j = 0; j< nb; j++)
     {
        yl = col_ptr[j];
        yr = col_ptr[j+1];
        tmp = 0;
        for(xi = xl, yi =yl; xi<xr && yi <yr ;)
        {
           if(spar_mat_csr[xi].col_ind == spar_mat_csc[yi].row_ind)
              tmp  += spar_mat_csr[xi++].val*spar_mat_csc[yi++].val;
           else if(spar_mat_csr[xi].col_ind < spar_mat_csc[yi].row_ind)
              xi++;
           else
              yi++;
        }
         if(tmp)
         {
            cnt++;
            coo_entry ent = { i+1 ,j+1, tmp};
            if(head == NULL)
            {
                head = tail = (node*)malloc(sizeof(node));
                head->entry = ent;
                head-> next = NULL;
            }
            else
            {
                tail->next =  (node*)malloc(sizeof(node));
                tail = tail->next;
                tail->entry = ent;
                tail -> next = NULL;
            }
         }
          
      }
     
  }
  
 *spar_mat_coo = (coo_entry* )malloc(cnt*sizeof(coo_entry));
   for(i=0;head != NULL; head = head->next)
      (*spar_mat_coo)[i++] = head->entry;
   return cnt;
}

int row_op(int m, int n, int r1, int k1, int r2, int k2, int *col_ptr, csc_entry** spar_mat_csc)
{
   int i,j,k,l,r,r2_ind, nnz = col_ptr[n];
   int result[n],tmp,c_tmp[n+1];
   csc_entry* csc_tmp;   
   for(i=0;i<n;i++)
      result[i] =0;
  
  if ( r1 > m || r1 <= 0 || r2 > m || r2 <=0 )
      { printf("Inavalid row Index Specified for the row operation\n");
        return -1;
      }
      
   for(i=0 ; i<n;i++)
   {
      for( l = col_ptr[i],r = col_ptr[i+1],tmp =0,r2_ind = -1;l<r;l++)
      {
            if((*spar_mat_csc)[l].row_ind == r2)
               { r2_ind = l;
                 tmp += k2 * (*spar_mat_csc)[l].val;
               }
           else  if((*spar_mat_csc)[l].row_ind == r1)
                 tmp += k1 * (*spar_mat_csc)[l].val;
      }
      result[i] = tmp;
      if(tmp && r2_ind ==-1)
        nnz++;
      else if (!tmp && r2_ind != -1)
        nnz--;
   }
   
  
    csc_tmp = *spar_mat_csc;
    *spar_mat_csc = (csc_entry *)malloc(nnz*sizeof(csc_entry));
	if( *spar_mat_csc == NULL)
    {
           printf("Memory Not Allocated\n");
	   exit(0);
    }
	for(i=0;i<=n;i++)
     c_tmp[i] = col_ptr[i];
        

     for(i=0,j=0;i<n;i++)
   {
      col_ptr[i] = j;
      for( l = c_tmp[i],r = c_tmp[i+1];l<r;l++)
      {
           if(csc_tmp[l].row_ind< r2)
           {
              (*spar_mat_csc)[j].row_ind = csc_tmp[l].row_ind;
              (*spar_mat_csc)[j++].val = csc_tmp[l].val;
           }       
           else if(csc_tmp[l].row_ind == r2)
           {
             if(result[i])
               {
                  (*spar_mat_csc)[j].row_ind = csc_tmp[l].row_ind;
                  (*spar_mat_csc)[j++].val  = result[i];
                  result[i] = 0;
               } 
           }
           else
           {
              if(result[i])  
              {  
                 (*spar_mat_csc)[j].row_ind  = r2;
                 (*spar_mat_csc)[j++].val  = result[i];
                 result[i] = 0;
               }
                
                 (*spar_mat_csc)[j].row_ind = csc_tmp[l].row_ind;
                 (*spar_mat_csc)[j++].val = csc_tmp[l].val;
           }
      }
      if(csc_tmp[l-1].row_ind< r2  && result[i] )
      {
        (*spar_mat_csc)[j].row_ind = r2;
        (*spar_mat_csc)[j++].val = result[i];
         result[i] = 0;  
      }
    }
    col_ptr[i] = j; 
 }
 
void print_coo_csr_csc(int m, int n, int nnz, coo_entry* spar_mat_coo, int* row_ptr, csr_entry* spar_mat_csr, int* col_ptr, csc_entry* spar_mat_csc, int choice)
{
  if( m>20 || n> 20)
  {
    printf("matrix size is too big\n");
     return ;
  }  

   int i,j,k,tmp;
    if(choice ==1)
    {
      sort_coo_entries(spar_mat_coo, 0, nnz-1, 1);
      for(i=1,k = 0; i<=m;i++)
      {
         for(j= 1; j<=n;j++)
         {
             if(spar_mat_coo[k].row == i && spar_mat_coo[k].col == j)
                tmp = spar_mat_coo[k++].val;
             else
                tmp = 0;
             printf("%4d ", tmp);
         }
          printf("\n");
       }
    }
    else if(choice == 2)
    {
      int xl, xr;
      for(i=1,k = 0; i<=m;i++)
      {
         xl = row_ptr[i-1];
         xr = row_ptr[i];
         for(j= 1; j<=n;j++)
         {
             if(xl < xr && spar_mat_csr[xl].col_ind == j)
                tmp = spar_mat_csr[xl++].val;
             else
                tmp = 0;
             printf("%4d ", tmp);
         }
          printf("\n");
       }
    
    }
    else if(choice == 3)
    {
        int xl, xr;
      int col_ptr_tmp[n+1]; 
      for(i=0;i<=n;i++)
         col_ptr_tmp[i] = col_ptr[i];
      for(i=1,k = 0; i<=m;i++)
      {
         for(j= 1; j<=n;j++)
         {
             tmp = 0;
             xl = col_ptr_tmp[j-1];
             xr = col_ptr[j];
             for( ;xl<xr && spar_mat_csc[xl].row_ind <= i  && !tmp; xl++)
                if(spar_mat_csc[xl].row_ind == i)
                    tmp = spar_mat_csc[xl].val;
             col_ptr_tmp[j-1] = xl;
             printf("%4d ", tmp); 
         }
          printf("\n");
      }
    }

} 



