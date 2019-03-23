#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h> 

int test(int *a, int n);
int parsetable(int *a,int n);

int main(int argc, char *argv[]) 
{
    
   int i;
   int max = 0;   
   int thread_count; 
   int* a;

    if (argc != 3){
    	printf("Non correct number of arguments\n");
   	return 1;
    }
	
   int elements_table = strtol(argv[1], NULL, 10);    
   a = malloc(elements_table * sizeof(int));
   thread_count = strtol(argv[2], NULL, 10);
                  
   for(i = 0;i < elements_table;i++)
    {
      int x = rand() % elements_table;
      if(x == 0)
        x = x + (rand() % elements_table) + 1;
      a[i] = x ;      
      if(a[i]>max)
        max=a[i];
      
   }    
    
    int *count = malloc((max + 1) * sizeof(int));
    int *temp = malloc((max+1) * sizeof(int));
    int *output = malloc(elements_table * sizeof(int));
    int value;
    int j;
    double delta;
    double start , end;
     //to delta einai log me bash to 2 tou n
     // gia ta log kai pow prepei otan kanoume gcc na valoume k -lm 
     // gia thn math.h sto telos  
    
    for(i=0;i<max+1;i++){
        count[i]=0;
        temp[i]=0;
    }
    //parallhlopoihshmh perioxh 
     start = omp_get_wtime( );
      #pragma omp parallel for num_threads(thread_count) \
      default(none) reduction(+: count[:(max+1)]) \
      shared(elements_table,a)
     for(int i=0;i<elements_table;i++){
        count[a[i]]++;   
      }
    
   delta = (log(elements_table)/log(2)) ;
   int e= (int)delta;
   int duo =2;
     for(j=0;j<e+1;j++){         
     	value = 2<<j;
        #pragma omp parallel for num_threads(thread_count) \
        default(none) \
         shared(max,value,count,temp) private(i)
        for(int i=0;i<max+1;i++){
			if(i%2==0){
				if(i<value){
					temp[i] = count[i];
				}else{
					temp[i] = count[i] + count[i-value];                               
            }
            }else{
            	if(i<value){
					count[i] = temp[i];
				}else{
					count[i] = temp[i] + temp[i-value];		
        }
     }
     end = omp_get_wtime( ); 
     //telos perioxhs
     for (i = 0; i<elements_table; i++){ 
        output[count[a[i]]-1] = a[i]; 
        count[a[i]]--; 
     }    
     
     

     for (i = 0; i<elements_table; i++) 
        a[i] = output[i]; 
    
    test(a,elements_table);
    printf("time in parallel area:%f",(end-start));
    return 0; 
    
}

int test(int *a, int n) {
  // if 0 == false
  int pass = 1; 
  for (int i = 1; i<n; i++)
  {        
      if (a[i] < a[i-1])
      	pass = 0;    
  }
  if(pass)
       	   printf("The list is sorted\n");
     else
           printf("The list isn't sorted\n");  
  return 0; 
}

int parsetable(int *a,int n){

    printf("\n");
    for (int i = 0; i<n; i++) 
        printf("-%d-",a[i]); 
   printf("\n");
 return 0;
}
