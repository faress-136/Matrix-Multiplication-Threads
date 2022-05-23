#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>

struct Matrix{
    int x_1,y_1,x_2,y_2;
    int array[200][200];
    
};
struct Matrix matrix[5];

//void matrix_multiplication(int matrix_1[row_1][col_1],int matrix_2[row_2][col_2]){
//    int sum =0;
//    for (int i=0; i<row_1; i++) {
//        for (int j=0; j<col_2; j++) {
//            for (int m=0; m<row_2; m++) {
//                sum = sum + matrix_1[i][m] *matrix_2[m][j];
//            }
//            mult_matrix[i][j]=sum;
//            sum=0;
//        }
//    }
//    printf("Product of the matrices:\n");
//
//    for (int i = 0; i < row_1; i++) {
//        for (int j = 0; j < col_2; j++)
//            printf("%d\t", mult_matrix[i][j]);
//
//        printf("\n");
//    }


void matrix_multiplication(){
    int sum =0;
    for (int i=0; i<matrix[0].x_1; i++) {
        for (int j=0; j<matrix[1].y_2; j++) {
            for (int m=0; m<matrix[1].x_2; m++) {
                sum = sum + matrix[0].array[i][m] *matrix[1].array[m][j];
            }
            matrix[2].array[i][j]=sum;
            sum=0;
        }
    }
    printf("The output of normal matrix multiplication\n");
    for (int i = 0; i < matrix[0].x_1; i++) {
        for (int j = 0; j < matrix[1].y_2; j++)
            printf("%d  ", matrix[2].array[i][j]);
        
        printf("\n");
    }
    
}


void *row_threads(void *arg){
    int *value_passed = arg;
    for (int j=0; j<matrix[1].y_2; j++) {
        matrix[3].array[*value_passed][j]=0;
        for (int m=0; m<matrix[1].x_2; m++) {
            matrix[3].array[*value_passed][j] += matrix[0].array[*value_passed][m] *matrix[1].array[m][j];
        }
    }
    /*   for (int i = 0; i < matrix[0].x_1; i++) {
     for (int j = 0; j < matrix[1].y_2; j++){
     printf("%d\t", matrix[3].array[i][j]);
     }
     printf("////////////\n");
     }
     */
    return NULL;
}




void multiplication_row(){
    int count=0;
    pthread_t no_threads[matrix[0].x_1 * matrix[1].y_2];
    int pass_value;
    for (int i=0; i<matrix[0].x_1; i++) {
        pass_value = i;
        pthread_create(&no_threads[count], NULL, row_threads,&i);
        pthread_join(no_threads[count],NULL);
        count++;
    }
    
    
}
struct Passed_index {
    int index_1;
    int index_2;
};
//    struct Passed_index passed_index;
//    struct Passed_index passed_index2;
void *elements_thread(void *arg){
    struct Passed_index pass2 = *(struct Passed_index*) arg;
    int i = pass2.index_1;
    int j = pass2.index_2;
    for (int m=0; m<matrix[1].x_2; m++) {
        matrix[4].array[i][j] += matrix[0].array[i][m] * matrix[1].array[m][j];
    }
    
    
    
    return NULL;
}


void multiplication_elements(){
    
    pthread_t no_threads[matrix[0].x_1 * matrix[1].y_2];
    struct Passed_index passed_index;
    int count = 0;
    for (int i =0; i<matrix[0].x_1; i++) {
        for (int j =0; j<matrix[1].y_2; j++) {
            passed_index.index_1 = i;
            passed_index.index_2 = j;
            pthread_create(&no_threads[count], NULL, elements_thread,&passed_index);
//            pthread_join(no_threads[count], NULL);
            count++;
        }
    }
    count =0;
    for (int i =0; i<matrix[0].x_1; i++) {
        for (int j =0; j<matrix[1].y_2; j++) {
            passed_index.index_1 = i;
            passed_index.index_2 = j;
//            pthread_create(&no_threads[count], NULL, elements_thread,&passed_index);
                        pthread_join(no_threads[count], NULL);
            count++;
        }
    }
    
    
    
    
}



int main() {
    
    FILE *fp,*fpp;
    fp = fopen("Matrix.txt", "r");
    fpp = fopen("results","w");
    fscanf(fp,"%d %d ",&matrix[0].x_1,&matrix[0].y_1);                  //take row and coloums size
    printf("row= %d columns= %d \n",matrix[0].x_1,matrix[0].y_1);
    //print row and coloumn values
    // intialize matrix with size given
    for (int i=0; i<matrix[0].x_1; i++) {                   // insert elements into matrix_1
        for (int j=0; j<matrix[0].y_1; j++) {
            fscanf(fp,"%d",&matrix[0].array[i][j]);
            printf("%d ",matrix[0].array[i][j]);
            
        }
        printf("\n");
    }
    fscanf(fp, "%d %d ",&matrix[1].x_2,&matrix[1].y_2);
    printf("row= %d colomns= %d \n",matrix[1].x_2,matrix[1].y_2);
    for (int i=0; i<matrix[1].x_2; i++) {                   // insert elements into matrix_2
        for (int j=0; j<matrix[1].y_2; j++) {
            fscanf(fp,"%d",&matrix[1].array[i][j]);
            printf("%d ",matrix[1].array[i][j]);
        }
        printf("\n");
    }
    
    if (matrix[0].y_1 != matrix[1].x_2) {           //check matrix multiplication can be done or not
        printf("........Matrix Multiplication can't be done........\n");
        fprintf(fpp,"........Matrix Multiplication can't be done........\n");
        return 0;
    }
    //    int mult_matrix[x_1][y_2];
    
    
    clock_t start_pro = clock();
    matrix_multiplication();       //Calling matrix multiplication function
    clock_t end_pro = clock();
    double time_pro = ((double) (end_pro - start_pro))/ CLOCKS_PER_SEC;;
    fprintf(fpp,"Time of normal multiplication = %f s\n",time_pro);
    fprintf(fpp,"The output of normal matrix multiplication is diplayed in terminal screen\n");
    /*  for (int i = 0; i < matrix[0].x_1; i++) {
     for (int j = 0; j < matrix[1].y_2; j++)
     fprintf(fpp,"%d  ", matrix[2].array[i][j]);
     
     fprintf(fpp,"\n");
     }*/
    
    fprintf(fpp,"..................................\n");
    
    
    
    clock_t start_row = clock();
    multiplication_row();
    clock_t end_row = clock();
    double time_row = ((double) (end_row - start_row))/ CLOCKS_PER_SEC;;
    fprintf(fpp,"Time of row multiplication = %f s\n",time_row);
    fprintf(fpp,"The output of row multiplication\n");
    for (int i = 0; i < matrix[0].x_1; i++) {
        for (int j = 0; j < matrix[1].y_2; j++){
            // printf("%d\t", matrix[3].array[i][j]);
            fprintf(fpp,"%d  ",matrix[3].array[i][j]);
        }
        // printf("\n");
        fprintf(fpp,"\n");
    }
    
    fprintf(fpp,"..................................\n");
    
    clock_t start_ele = clock();
    multiplication_elements();
    clock_t end_ele = clock();
    double time_ele = ((double) (end_ele - start_ele))/ CLOCKS_PER_SEC;;
    fprintf(fpp,"Time of element multiplication = %f s\n",time_ele);
    fprintf(fpp,"The output of elements multiplication\n");
    for (int i = 0; i < matrix[0].x_1; i++) {
        for (int j = 0; j < matrix[1].y_2; j++){
            // printf("%d\t", matrix[4].array[i][j]);
            fprintf(fpp,"%d  ",matrix[4].array[i][j]);
        }
        //printf("\n");
        fprintf(fpp,"\n");
    }
    
    
    
    // return 0;
//}

fprintf(fpp,"Time of row multiplication = %f s\n",time_row);
fprintf(fpp,"The output of row multiplication\n");
for (int i = 0; i < matrix[0].x_1; i++) {
    for (int j = 0; j < matrix[1].y_2; j++){
        // printf("%d\t", matrix[3].array[i][j]);
        fprintf(fpp,"%d  ",matrix[3].array[i][j]);
    }
    // printf("\n");
    fprintf(fpp,"\n");
}

fprintf(fpp,"..................................\n");

clock_t start_ele2 = clock();
multiplication_elements();
clock_t end_ele2 = clock();
double time_ele2 = ((double) (end_ele2 - start_ele2))/ CLOCKS_PER_SEC;;
fprintf(fpp,"Time of element multiplication = %f s\n",time_ele2);
fprintf(fpp,"The output of elements multiplication\n");
for (int i = 0; i < matrix[0].x_1; i++) {
    for (int j = 0; j < matrix[1].y_2; j++){
        // printf("%d\t", matrix[4].array[i][j]);
        fprintf(fpp,"%d  ",matrix[4].array[i][j]);
    }
    //printf("\n");
    fprintf(fpp,"\n");
}



return 0;
}


