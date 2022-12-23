#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node{
    char chr;
    int freq;
    char* code;
    struct node* left;
    struct node* right;
};

struct node* allocate_node(){
    struct node* n = (struct node*)malloc(sizeof(struct node));
    n->chr='\0';
    n->freq=0;
    n->code=NULL;
    n->left=NULL;
    n->right=NULL;
}

struct Heap{
    struct node**arr;
    int arr_len;
    int size;
};

struct Heap* allocate_Heap(int len){
    struct Heap* H = (struct Heap*)malloc(sizeof(struct Heap));
    H->arr_len = len;
    struct node* arr[len];
    H->arr = arr;
    H->size=0;
}

void minHeapify_at(struct Heap* H, int i){
    int n=H->size;
    int left=2*i+1;
    int right=2*i+2;
    int smallest=i;
    if(left<n && H->arr[left]->freq < H->arr[smallest]->freq) smallest=left;
    if(right<n && H->arr[right]->freq < H->arr[smallest]->freq) smallest=right;
    if(smallest!=i){
        struct node* temp=H->arr[smallest];
        H->arr[smallest]=H->arr[i];
        H->arr[i]=temp;
        minHeapify_at(H, smallest);
    }
}

void build_minHeap(struct Heap* H){
    for(int i=H->size/2-1; i>=0; i--) minHeapify_at(H, i);
}

struct node* extract_min(struct Heap* H){
    if(H->size==0) return NULL;
    struct node* mini = H->arr[0];
    H->arr[0]=H->arr[--H->size];
    minHeapify_at(H, 0);
    return mini;
}

void insert(struct Heap* H, struct node* n){
    int idx = H->size++;
    H->arr[idx] = n;
    int pI = (idx-1)/2;
    while(idx>0 && H->arr[pI]->freq > H->arr[idx]->freq){
        struct node* temp = H->arr[pI];
        H->arr[pI] = H->arr[idx];
        H->arr[idx] = temp;
        idx=pI;
        pI = (idx-1)/2;
    }
}


void encoding(struct node* root, char code[], int len, char codes[128][256], FILE* output){
    if(root==NULL) return;
    if(root->left==NULL && root->right==NULL){
        root->code = code;
        strcpy(codes[root->chr], root->code);
        char str[258];
        str[0] = root->chr;
        str[1]='\0';
        strcat(str, "->");
        strcat(str, root->code);
        strcat(str, "\n");
        fputs(str, output);
        printf("%s", str);
    }

    char temp1[256];
    temp1[0]='\0';
    strcat(temp1, code);
    strcat(temp1, "1");
    encoding(root->left, temp1, len+1, codes, output);

    char temp2[256];
    temp2[0]='\0';
    strcat(temp2, code);
    strcat(temp2, "0");
    encoding(root->right, temp2, len+1, codes, output);
}
void compression(char input_name[], char output_name[]){
    FILE* input = fopen(input_name, "r");

    char line[1000005];
    int num=0;
    int arr[128];
    for(int i=0; i<128; i++) arr[i]=0;
    while(fgets(line, 1000005, input)){
        for(int i=0; i<1000005; i++){
            if(line[i]=='\0') break;
            if(line[i]=='\n') continue;
            if(arr[line[i]]++==0) num++;
        }
    }

    struct Heap* H = allocate_Heap(256);
    for(int i=0; i<128; i++){
        if(arr[i]!=0){
            struct node* n = allocate_node();
            n->chr=(char)(i);
            n->freq=arr[i];
            insert(H, n);
        }
    }

    for(int i=1; i<=num-1; i++){
        struct node* dummy = allocate_node();
        dummy->right = extract_min(H);
        dummy->left = extract_min(H);
        dummy->freq = dummy->right->freq + dummy->left->freq;
        insert(H, dummy);
    }

    char codes[128][256];
    struct node* root = H->arr[0];
    char code[256];
    code[0]='\0';
    FILE* output = fopen(output_name, "w");
    encoding(root, code, 0, codes, output);
    fclose(input);

    input = fopen(input_name, "r");
    while(fgets(line, 1000005, input)){
        for(int i=0; i<1000005; i++){
            if(line[i]=='\0') break;
            if(line[i]=='\n') continue;
            fputs(codes[line[i]], output);
        }
    }
    fclose(input);
    fclose(output);
}

void decoding(char line[], int len, char codes[128][256], FILE* output){
    int i=0;
    while(i<len){
        if(line[i]=='\0') break;
        if(line[i]=='\n'){
            fputc('\n', output);
            break;
        }
        char curr[256];
        int m=0;
        for(int j=i; j<len; j++){
            if(line[j]=='\0'){
                i=j;
                break;
            }
            int found=0;
            curr[m++]=line[j];
            curr[m]='\0';
            for(int k=0; k<128; k++){
                if(strcmp(codes[k], curr)==0){
                    fputc((char)(k), output);
                    i=j+1;
                    found=1;
                    break;
                }
            }
            if(found==1) break;
        }
    }
}
void decompression(char input_name[], char output_name[]){
    char codes[128][256];
    char line[1000005];
    FILE* input=fopen(input_name, "r");
    FILE* output=fopen(output_name, "w");
    while(fgets(line, 1000005, input)){
        if(line[0]=='0' || line[0]=='1') decoding(line, 1000005, codes, output);
        else if(line[0]=='\n') continue;
        else if(line[0]=='\0') break;
        else{
            int m=0;
            for(int i=3; i<256; i++){
                if(line[i]=='\0' || line[i]=='\n') break;
                codes[line[0]][m++] = line[i];
            }
            codes[line[0]][m]='\0';
        }
    }
    fclose(input);
    fclose(output);
}

int main(){
    char input_name[1000];
    printf("Enter the name of the input file\n");
    scanf("%s",input_name);

    char output_name[1000];
    printf("Enter the name of the output file\n");
    scanf("%s",output_name);

    int mode;
    printf("Enter mode (0 for compression, 1 for decompression)\n");
    scanf("%d",&mode);

    if(mode==0){
        compression(input_name, output_name);
    }else
        decompression(input_name, output_name);
    char ps[1000] = "Generating ";
    strcat(ps, output_name);
    strcat(ps, "\n");
    printf("%s", ps);
}
