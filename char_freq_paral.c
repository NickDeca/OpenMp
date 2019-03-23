#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <omp.h> 
#define N 128
#define base 0

int main (int argc, char *argv[]) {
	
	FILE *pFile;
	long file_size;
	char * buffer;
	char * filename;
	size_t result;
	int i, j, freq[N];
        int thread_count;
        double start , end;

        if (argc != 3) {
		printf ("Usage : %s <file_name>\n", argv[0]);
		return 1;
        }
	filename = argv[1];
        thread_count = strtol(argv[2], NULL, 10);
	pFile = fopen ( filename , "rb" );
	if (pFile==NULL) {printf ("File error\n"); return 2;}

	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	file_size = ftell (pFile);
	rewind (pFile);
	printf("file size is %ld\n", file_size);
	
	// allocate memory to contain the file:
	buffer = (char*) malloc (sizeof(char)*file_size);
	if (buffer == NULL) {printf ("Memory error\n"); return 3;}

	// copy the file into the buffer:
	result = fread (buffer,1,file_size,pFile);
	if (result != file_size) {printf ("Reading error\n"); return 4;} 
	//gemizei 128 8eseis me 0
	for (j=0; j<N; j++){
		freq[j]=0;
	}
        //auto p 8eloume parallhlo , gemizei to freq 
        //parallhlopoihshmh perioxh 
         start = omp_get_wtime( );  
        #pragma omp parallel for num_threads(thread_count) \
        reduction(+: freq) private(i)
	for (int i=0; i < file_size; i++){
		freq[buffer[i] - base]++;
	}		
        end = omp_get_wtime( ); 
       //telos perioxhs
	for (j=31; j<N; j++){
		printf("%d = %d\n", j+base, freq[j]);
	}	

        printf("time in parallel area:%f",(end-start));
	fclose (pFile);
	free (buffer);

	return 0;
}
