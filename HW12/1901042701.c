/*
    Bahadir Etka Kilinc
        1901042701
         HW_12

    Description

        Program reads characters and its frequencies from 
        reference.txt for creating Huffman Tree. Then 
        reads string from message.txt and encode in these characters
        with created Huffman tree. Lastly with this encoded file decodes
        to decoded.txt

    Notes
    
        Created on Ubuntu OS
        Compiled and tested with gcc 9.2.1


*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct node{
    
    char ch;
    int freq;
    struct node *left,*right;

}Node;
Node *tree_root=NULL;
struct list{

    Node *node;
    struct list *link;

}*root=NULL;

typedef struct path{
    int level;
    struct path *link;
}Path;

Path *path_list=NULL;

int char_flag; // flag for the character has found or not

void read_char_freq(char *filename);
void create_Huffman_tree();
void sort_list();
void swap(struct list *node1, struct list *node2);
void add_list_root(Node *n);
void delete_last_node();
void find_character(Node *head, char ch, int level);
void add_path_list(int l);
void delete_path_list();
char* mean_bit_pattern();
int find_max_level();
void read_and_encode(char *input, char *output);
void decode_encrypted(char *en , char *de);
char find_on_tree(char *str);

int main(){

    // reads characters and its frequencies from reference.txt creates list
    read_char_freq("reference.txt");
    //sorts created list
    sort_list();
    //with these created nodes creates huffman tree
    create_Huffman_tree();
    // lastly list's root has huffman tree's root
    struct list *temp = root;
    tree_root = temp -> node;
    //reads string from message.txt and encode it to encoded.txt
    read_and_encode("message.txt","encoded.txt");
    //reads encoded bits and writes to meaningful characters to decoded.txt
    decode_encrypted("encoded.txt","decoded.txt");
  
	return 0;
}
void read_char_freq(char *filename){

    char ch;
    struct list *temp;
    FILE *ptr;
    int flag,i;

    ptr = fopen(filename,"r");
    
    if(ptr != NULL){

        while(fscanf(ptr,"%c",&ch) != EOF){
            
            flag = 0; // initially had that character
            
            temp = root;

            while(temp != NULL){
                if(ch == temp -> node -> ch){
                    flag = 1;
                    break;
                }
                temp = temp -> link;
            }
            if(flag){

                temp -> node -> freq += 1;

            }
            else{

                Node *p = (Node*)malloc(sizeof(Node));

                p->ch = ch;
                p->freq = 1;
                p->left = NULL;
                p->right = NULL;

                add_list_root(p);
        
            }

        }           

    }
    else{
        printf("file is not opened succesfully\n");
    }
    fclose(ptr);
}
void create_Huffman_tree(){

    struct list *prev,*next;
    struct list *temp = root;
    do{
        prev = next = temp;
        next = next -> link;
        
        while(next -> link != NULL){
            prev = prev->link;
            next = next -> link;
        }
        Node *p = (Node*)malloc(sizeof(Node));
       
        p->ch = '$';
        p->freq = prev->node->freq + next->node->freq;
        p->left = prev->node;
        p->right = next->node;
        add_list_root(p);
        sort_list();
        delete_last_node();
        delete_last_node();
            
    }while(temp -> link -> link != NULL);
        
        prev = next = root;
        next = next -> link;

        Node *p = (Node*)malloc(sizeof(Node));

        p->ch = '$';
        p->freq = prev->node->freq + next->node->freq;
        p->left = prev->node;
        p->right = next->node;

        add_list_root(p);
        sort_list();
        delete_last_node();
        delete_last_node();        

}

void add_list_root(Node *n){

    struct list *temp = (struct list*)malloc(sizeof(struct list));

    temp -> node = n;
    temp -> link = NULL;

        if(root == NULL){

            root = temp;
        }
        else{

            struct list *ptr=root;

            while(ptr -> link != NULL) ptr = ptr->link;

            ptr -> link = temp;

        }

}
void sort_list(){

    struct list *temp = root;
    struct list *prev,*next;
    prev = next = temp;

    int flag;

    if(temp -> link != NULL && temp != NULL){

        do{
            flag = 0;
            prev = next = temp;
            next = next -> link;
            
            while(next != NULL){

                if(prev -> node -> freq < next -> node -> freq){
                    swap(prev,next);
                    flag = 1;
                }

                prev = prev -> link;
                next = next -> link;
                
            }

        }while(flag != 0);

    }
}
void swap(struct list *node1, struct list *node2){

    Node *temp;

    temp = node1->node;
    node1->node = node2->node;
    node2->node = temp;

}
void delete_last_node(){

    struct list *temp=root;

    if(temp -> link != NULL){

        while(temp -> link -> link != NULL) temp = temp -> link;

        struct list *ptr = temp -> link;

        temp -> link = NULL;

        free(ptr);
    }

}
void find_character(Node *head, char ch, int level){
    
    if(!char_flag) add_path_list(level);
    if(head -> ch == ch) char_flag = 1;
    
    if(head -> left != NULL){

        find_character(head->left,ch,level+1);

    }
    if(head -> right != NULL){

        find_character(head->right,ch,level+1);
    }    
}

void add_path_list(int l){

    Path *temp = (Path*)malloc(sizeof(Path));
    temp -> level = l;
    temp -> link = NULL;

    if(path_list == NULL){
        path_list = temp;
    }
    else{
        Path* ptr=path_list;

        while(ptr -> link != NULL) ptr = ptr -> link;

        ptr -> link = temp;
    }
}
char * mean_bit_pattern(){

    Path *temp = path_list;
    char *t;
    int max_level = find_max_level();

    t = (char*)calloc(max_level+1,sizeof(char));
    int counter = 0,i=0;
    while(temp -> link != NULL){
        if(temp ->level < temp ->link->level){
             t[counter] = '0';
             counter++;
        }
        if(temp->level == temp -> link ->level){
            t[counter-1] = '1';
        }
        if(temp -> level > temp -> link -> level ){
            for(i=temp->link->level; i<max_level ; i++) t[i] = '\0';
            t[temp -> link -> level -1] = '1';
            counter = temp -> link -> level;
        }

        temp = temp -> link;
    }

    t[max_level] = '\0';
    
    return t;
  
}
int find_max_level(){

    Path *temp = path_list;
    int max;
    if(temp != NULL) max=temp->level;         
        while(temp!= NULL){
            
            if(max  < temp -> level) max = temp -> level;
            temp = temp -> link;
        }

        return max;
}
void delete_path_list(){
   
    Path *temp;

    while(path_list != NULL){

        temp = path_list;
        path_list = path_list -> link;
        free(temp);
        
    }
    path_list = NULL;

}
void read_and_encode(char *input, char *output){

    FILE *in,*out;
    in = fopen(input,"r");
    out = fopen(output,"w");
    char ch;
    char *ptr;
    if(in != NULL && out != NULL){
        while(fscanf(in,"%c",&ch) != EOF){

            char_flag=0;
            delete_path_list();
            find_character(tree_root,ch,0);

            ptr = mean_bit_pattern();
        
            fprintf(out,"%s ",ptr);

        }
    }
    fclose(in);
    fclose(out);

}
void decode_encrypted(char *en , char *de){

    FILE *in,*out;
    in = fopen(en,"r");
    out= fopen(de,"w");
    char *buffer = (char*)malloc(sizeof(char));
    char ch;
    int i=0;
    if(in != NULL && out != NULL){
        while(fscanf(in,"%c",&ch) != EOF){
  
            if(ch != ' '){
                buffer[i] = ch;
                i++;
                buffer=realloc(buffer,sizeof(char)*(i+1));
            }
            if(ch == ' '){
                buffer[i] = '\0';
                fprintf(out,"%c",find_on_tree(buffer));
                free(buffer);
                char* buffer=(char*)malloc(sizeof(char));
                i=0;
            }   
        }
    }

        fclose(in);
        fclose(out);

}
char find_on_tree(char *str){

    Node *temp = tree_root;
    int i=0;
    
        while(str[i] != '\0'){

            if(str[i] == '1') temp = temp -> right;
            if(str[i] == '0') temp = temp -> left;
            i++;

        }

    return temp -> ch;

}