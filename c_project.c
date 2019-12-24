#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>

struct list{
	struct list *next;
	struct list *prev;
	char *word;
	int count;
	};

struct hashTable{
	struct list **array; 
	}; 

struct hashTable *head_hash=NULL;

char *readcount(int fd);
void add_hash(char *c, struct hashTable *hash);
struct hashTable *create_hash();
void print_hash(struct hashTable *hash);
void find_max(struct hashTable *hash);

int main(int argc, char *argv[]) {
	
	int fd = open(argv[1],O_RDONLY);
	if(fd==-1){
		printf("Failed to open and read the file. \n");
		exit(1);
	}	
	
	struct hashTable *new_hash = (struct hashTable*)malloc(sizeof(struct hashTable));
	new_hash=create_hash();
	
	char *buf = malloc(50*sizeof(char));
	
	while(1) {
		buf = readcount(fd);
		if(buf==0) break;
		add_hash(buf,new_hash);
	}
	
	print_hash(new_hash);
	find_max(new_hash);
	
	
	close(fd);
	
return 0;}


char *readcount(int fd){
	int sz, i=0;
	char c;
	char *word = malloc(50*sizeof(char));
	sz=read(fd,&c,1);
	if(sz==0) return 0;
	while(!(c==' '||c=='\n')){
		word[i]=c;
		sz=read(fd,&c,1);
		i++;
		}	
	
	char *p;
	p=strtok(word,".,?!");
		
	return p;
	}

struct hashTable *create_hash(){
	struct hashTable *hash = (struct hashTable*)malloc(sizeof(struct hashTable));
	if(hash != NULL){
		hash->array=(struct list **)malloc(30*sizeof(struct list*));
		int i=0;
		while(i<30){
			hash->array[i]=NULL;
			i++;
			}
		}
	return hash;
}

void add_hash(char *c, struct hashTable *hash){
	struct list *new_node=(struct list*)malloc(sizeof(struct list));
	int pos = c[0]-96;
	int pos2 = c[0]-64;
	int curpos;
	char *new_c = malloc(sizeof(c));
	for(int i = 0; i < strlen(c); i++){
		new_c[i]=tolower(c[i]);
		}
	new_node->word=new_c;
	new_node->next=NULL;
	new_node->prev=NULL;
	new_node->count=1;
	
	struct list *curnode;
	if(pos>0 && pos<=29){
		curpos = pos;
		}
	else if(pos2>0 && pos2<=29){
		curpos = pos2;
		}
	curnode=hash->array[curpos];
	if(hash->array[curpos]==NULL){
		hash->array[curpos]=new_node;
		}
	else{
		
		int k=0;
		int i=0;
		while(curnode->next!=NULL){			
			if(strcasecmp(curnode->word,c)==0){
				curnode->count += 1;
				k++;
				break;
				}
				curnode=curnode->next;
			if(strcasecmp(curnode->word,c)==0 && curnode->next==NULL){
				curnode->count += 1;
				k++;	
			}
		}	
		if(k==0 && strcasecmp(curnode->word,c)!=0){
			curnode->next=new_node;
			new_node->prev=curnode;
		}
		else if(k==0 && strcasecmp(curnode->word,c)==0){
			curnode->count += 1;
			}
	}
}


void print_hash(struct hashTable *hash){
	struct list *curlist;
	struct hashTable *curhash;
	curhash=hash;
	
	if(hash == NULL) printf("File is empty. \n");
	else{
		for(int i = 0; i<30; i++){
			if(curhash->array[i]!=NULL){
				char letter;
				int x = i+64;
				letter = x;
				printf("Letter is %c: \n",letter);				
				curlist=curhash->array[i];
				if(curlist->next==NULL){
					printf("- %s x %d \n",curlist->word,curlist->count);
					}
				int j=1;
				while(curlist->next!=NULL){
					printf("- %s x %d \n",curlist->word,curlist->count);
					curlist=curlist->next;
					j++;
					if(curlist->next==NULL){
						printf("- %s x %d \n",curlist->word,curlist->count);
						}
					}
				}
			}
		}
}

void find_max(struct hashTable *hash){
	struct list *curlist;
	struct hashTable *curhash;
	curhash=hash;
	struct list *max;
	max=curlist;
	
	if(hash == NULL) printf("File is empty. \n");
	else{
		for(int i = 0; i<30; i++){
			if(curhash->array[i]!=NULL){
				curlist=curhash->array[i];
				int j=1;
				while(curlist->next!=NULL){
					if(curlist->count > max->count){
						max = curlist;
						}
					curlist=curlist->next;
					j++;
					}
					if(curlist->next==NULL){
						if(curlist->count > max->count){
							max = curlist;
							}
						}
				}
		}
		for(int i = 0; i<30; i++){
			if(curhash->array[i]!=NULL){
				curlist=curhash->array[i];
				int j=1;
				while(curlist->next!=NULL){
					if(curlist->count == max->count&&curlist->word!=max->word){
						printf("Most used word in the txt file is %s with count of %d \n",curlist->word,curlist->count);
						}
					curlist=curlist->next;
					j++;
					}
					if(curlist->next==NULL){
						if(curlist->count == max->count && curlist->word!=max->word){
							printf("Most used word in the txt file is %s with count of %d \n",curlist->word,curlist->count);
							}
						}
				}
		}
		printf("Most used word in the txt file is %s with count of %d \n",max->word,max->count);
	}
}

















