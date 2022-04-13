/*      Bahadir Etka Kilinc
            1901042701.c
                HW11

    Description

        The program includes two parts.First part reads all integers 
        from list.txt and save it to dynamic array and linked list separately.
        Second part takes integers from user and creates linked list and dynamic 
        array with inputtedintegers. Then finds different elements at the same
        row.

    Notes

        Created on Ubuntu OS
        Compiled and tested with gcc 9.2.1


*/


#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

enum indexes{
    min_index = 0,
    max_index = 1,
    mean_index = 2,
    std_index = 3
};
typedef struct nodes{
    int data;
    struct nodes *link;
}node;
typedef struct difference{
    int n1;
    int n2;
}dif;
void part1();
void part2();
int *read_array(char *filename,long int *size);
node *read_linkedlist(char *filename,long int *size);
float *stats_array(int *ptr);
float *stats_linkedlist(node *ptr);
dif* find_different_elements(node* node_ptr,int *ptr);
node* create_linkedlist();
int* create_dynamic_array(); 
double my_pow(double num, int pow);

int main(){

    int option;

    do{

        printf("\n_____Please select from menu______\n");
        printf("1)Execute Part1\n2)Execute Part2\n0)Exit\n");
        scanf("%d",&option);
        
        switch(option){
            case 1:
                part1();
            break;
            case 2:
                part2();
            break;
            case 0:
                printf("Good Bye!\n");
            break;
            default:
                printf("Please select valid option!\n");
            break;

        }

    }while(option != 0);

    return 0;

}
void part1(){
    
    clock_t t;
    double time_read_array,time_read_linkedlist;
    float *stats_read_array,*stats_read_linkedlist;
    long int size_read_array=0,size_read_linkedlist=0;
    
    char filename[]="list.txt";
    int *dynamic_array;
    node *linked_list;
    /* read integers from list and measure time and size of array or linkedlist */
    
    t = clock();
    dynamic_array = read_array(filename,&size_read_array);
    t = clock() - t;
    time_read_array = ((double)t) / CLOCKS_PER_SEC;
    
    t = clock();
    linked_list = read_linkedlist(filename,&size_read_linkedlist);
    t = clock() - t;
    time_read_linkedlist = ((double)t) / CLOCKS_PER_SEC;
    /* do the calculations and save that adress */
    stats_read_array = stats_array(dynamic_array);
        
    stats_read_linkedlist=stats_linkedlist(linked_list);

    printf("\t\t\tread_array()\t\tread_linkedlist()\n");
    printf("--------------------------------------------------------------------\n");
    printf("Time to used      \t%lf\t\t%lf\n",time_read_array,time_read_linkedlist);
    printf("--------------------------------------------------------------------\n");
    printf("Size of array     \t%li\t\t\t%li\n",size_read_array,size_read_linkedlist);
    printf("--------------------------------------------------------------------\n");
    printf("Minumum Element   \t%f\t\t%f\n",stats_read_array[min_index],stats_read_linkedlist[min_index]);
    printf("--------------------------------------------------------------------\n");
    printf("Maximum Element   \t%f\t\t%f\n",stats_read_array[max_index],stats_read_linkedlist[max_index]);
    printf("--------------------------------------------------------------------\n");
    printf("Mean of Elements  \t%f\t\t%f\n",stats_read_array[mean_index],stats_read_linkedlist[mean_index]);
    printf("--------------------------------------------------------------------\n");
    printf("Standart Deviation\t%f\t\t%f\n",stats_read_array[std_index],stats_read_linkedlist[std_index]);
    printf("--------------------------------------------------------------------\n");


        
}
int *read_array(char *filename,long int *size){

    FILE *fileptr = fopen(filename,"r");
    
    int *p = (int*)malloc(sizeof(int));

    *size += sizeof(p);
    
    int counter = 0;    
    if(fileptr != NULL){ // file is opened succesfully.

        while(fscanf(fileptr,"%d,",&p[counter]) != EOF){

            counter++;
            p = realloc(p,sizeof(int) * counter+1);  
            *size += sizeof(int);
        }

        p[counter] = -1;

        return p;

    }
    else{
        printf("File is not opened succesfully!\n");
    }

    fclose(fileptr);


}
node *read_linkedlist(char *filename,long int *size){
    
    node *root;
    node *temp = (node*)malloc(sizeof(node));
    node *prev;
    root = temp;
    FILE *fileptr = fopen(filename,"r");

    *size += sizeof(root); // add to size value
    
    if(fileptr != NULL){
       
       while(fscanf(fileptr,"%d,",&(temp -> data)) != EOF){
           
           temp -> link = (node*)malloc(sizeof(node));
           prev = temp;
           temp = temp->link;
           prev -> link = temp;
           
           *size += sizeof(node); // each add node add size too
        
        }
        temp -> link = NULL;
           
    }
    else{
        printf("File is not opened succesfully\n");
    }
    fclose(fileptr);

    return root;

}
float * stats_array(int *ptr){

    float *arr = (float*)calloc(sizeof(float),4);
    int min,max;
    float mean=0.0,std=0.0;
    int i;
  
    min = max = ptr[0];
        /* find min,max at the same time add all elements to mean value */
        for(i=0; ptr[i] != -1 ; i++){

            if(ptr[i] > max) max = ptr[i];
            if(ptr[i] < min) min = ptr[i];
            mean += ptr[i];

        }
    
        mean /= i; // divide mean to elements number
  
        for(i=0; ptr[i] != -1 ; i++){

            std += my_pow(ptr[i]-mean,2);   
        
        }
        std /= i-1;
        std = sqrt(std);
     
    arr[min_index]=min;
    arr[max_index]=max;
    arr[mean_index]=mean;
    arr[std_index]=std;

    return arr;

}
float * stats_linkedlist(node *ptr){
    int num_of_data = 0;
    float *arr = (float*)calloc(sizeof(float),4);
    int min,max;
    float mean=0.0,std=0.0;
    node* temp= ptr;
    min = max = ptr->data;

        while(temp -> link != NULL){

            if(temp -> data > max) max = temp -> data;
            if(temp -> data < min) min = temp -> data;
            mean += temp -> data;
            num_of_data++;
            temp = temp->link;

        }
        mean /= num_of_data;
    
        temp = ptr;
        while(temp -> link != NULL){

            std += my_pow(temp->data-mean,2);
            temp = temp -> link;

        }
        std /= (num_of_data-1);
        std = sqrt(std);
       
    arr[min_index]=min;
    arr[max_index]=max;
    arr[mean_index]=mean;
    arr[std_index]=std;

    return arr;
    
}
void part2(){

    int *ptr;
    node *node_ptr;
    dif *p;
    int i;
    
    node_ptr = create_linkedlist();
    ptr = create_dynamic_array();

    p = find_different_elements(node_ptr,ptr);
    printf("_____Different elements are_____\n");
    for(i=0; p[i].n1 != -1 && p[i].n2 !=-1 ;i++) printf("%d and %d\n",p[i].n1,p[i].n2);


}
node* create_linkedlist(){
    
    node *root;
    node *temp = (node*)malloc(sizeof(node));
    node *prev;
    root = temp;
    int num = 0;
    
    printf("For creating linked list please enter the elements for stop enter -1\n");
    scanf("%d",&num);
        while(num != -1){
            temp -> data = num;
            temp -> link = (node*)malloc(sizeof(node));
            prev = temp;
            temp = temp->link;
            prev -> link = temp;
            scanf("%d",&num);
                               
        }
        
        temp -> link = NULL;  
    
    return root;
}
int* create_dynamic_array(){

    int *p = (int*)malloc(sizeof(int));
    int counter=0,num;
    printf("For creating dynamic array please enter the elements for stop enter -1\n");
    scanf("%d",&num);
        while(num != -1){

            p[counter] = num;
            counter++;
            p = realloc(p,sizeof(int) * (counter+1));
            scanf("%d",&num);  
            
        }

        p[counter] = -1;

    return p;

}

dif* find_different_elements(node* node_ptr,int *ptr){

    int i=0;
    int counter=0;
    dif *p;

    p=(dif*)malloc(sizeof(dif));
    /*scanning different row elements if there is then save it dynamically*/
        while(ptr[i] != -1 && node_ptr -> link != NULL){

            p[counter].n1 = ptr[i];
            p[counter].n2 = node_ptr->data;
                if(p[counter].n1 != p[counter].n2){
              
                    counter++;
                    p = realloc(p,sizeof(dif)*(counter+1));

                }
            i++;
            node_ptr = node_ptr ->link;

        }
        p[counter].n1=-1;
        p[counter].n2=-1;

    return p;

}
double my_pow(double num, int pow){
    int i;
    double result=1;
        for(i=0; i<pow; i++){
            result *= num;
        }
    return result;
}
