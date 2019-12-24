#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>


//linked list for keeping the words
struct list{
	struct list *next;
	char *word;
	int count;
	};

//hashtable has double-pointer array to keeping linked lists
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
	
	//input files takes from the argument
	int fd = open(argv[1],O_RDONLY);
	if(fd==-1){
		printf("Failed to open and read the file. \n");
		exit(1);
	}	
	
	struct hashTable *new_hash = (struct hashTable*)malloc(sizeof(struct hashTable));
	new_hash=create_hash();
	
	//pointer of char keeps the words which are read from file
	char *buf = malloc(50*sizeof(char));
	
	//it'll stop reading the file when buf equals to zero
	while(1) {
		buf = readcount(fd);
		if(buf==0) break;
		add_hash(buf,new_hash);
	}
	
	print_hash(new_hash);
	find_max(new_hash);
	
	
	close(fd);
	
return 0;}

//reads from the file until sees space or new line
char *readcount(int fd){
	int sz, i=0;
	char c;
	//keeps the word which are read from the file
	char *word = malloc(50*sizeof(char));
	sz=read(fd,&c,1);
	//when size is zero, any char is not read
	if(sz==0) return 0;
	while(!(c==' '||c=='\n')){
		word[i]=c;
		sz=read(fd,&c,1);
		i++;
		}	
	
	char *p;
	//if word from read has any symbols, it splits word from symbols
	p=strtok(word,".,?!1234567890");
		
	return p;
	}

//creates hashtable
struct hashTable *create_hash(){
	struct hashTable *hash = (struct hashTable*)malloc(sizeof(struct hashTable));
	if(hash != NULL){
		//reserves a space from memory for all possible letters
		hash->array=(struct list **)malloc(30*sizeof(struct list*));
		int i=0;
		//creates for each letter
		while(i<30){
			hash->array[i]=NULL;
			i++;
			}
		}
	return hash;
}

//adds new word into the hash
void add_hash(char *c, struct hashTable *hash){
	//new node for inserting new word (if it is not exist)
	struct list *new_node=(struct list*)malloc(sizeof(struct list));
	//the case of lower case letter
	int pos = c[0]-96;
	//the case of upper case letter
	int pos2 = c[0]-64;
	int curpos;
	char *new_c = malloc(sizeof(c));
	//it changes all letter to lower case in the word
	for(int i = 0; i < strlen(c); i++){
		new_c[i]=tolower(c[i]);
		}
	new_node->word=new_c;
	new_node->next=NULL;
	new_node->count=1;
	
	struct list *curnode;
	//checks for lower case
	if(pos>0 && pos<=29){
		curpos = pos;
		}
	//checks for upper case
	else if(pos2>0 && pos2<=29){
		curpos = pos2;
		}
	curnode=hash->array[curpos];
	if(hash->array[curpos]==NULL){
		hash->array[curpos]=new_node;
		}
	else{
		
		//in order to check the same word appears in the list
		int k=0;
		int i=0;
		while(curnode->next!=NULL){			
			if(strcasecmp(curnode->word,c)==0){
				//if it is appeared, increase the count
				curnode->count += 1;
				k++;
				break;
				}
				curnode=curnode->next;
			//this is checks for only the last element
			if(strcasecmp(curnode->word,c)==0 && curnode->next==NULL){
				curnode->count += 1;
				k++;	
			}
		}	
		//these cases for last element in the list (or the first) 
		if(k==0 && strcasecmp(curnode->word,c)!=0){
			curnode->next=new_node;
		}
		else if(k==0 && strcasecmp(curnode->word,c)==0){
			curnode->count += 1;
			}
	}
}

//prints the dictionary
void print_hash(struct hashTable *hash){
	struct list *curlist;
	struct hashTable *curhash;
	curhash=hash;
	
	if(hash == NULL) printf("File is empty. \n");
	else{
		for(int i = 0; i<30; i++){
			if(curhash->array[i]!=NULL){
				char letter;
				//changes x's ascii in to char
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

//finds the most used word
void find_max(struct hashTable *hash){
	struct list *curlist;
	struct hashTable *curhash;
	curhash=hash;
	struct list *max;
	max=curlist;
	
	if(hash == NULL) printf("File is empty. \n");
	else{
		//this loop finds the most used word
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
					//this case checks for multiple most used words
					if(curlist->count == max->count&&curlist->word!=max->word){
						printf("Most used word in the txt file is %s with count of %d \n",curlist->word,curlist->count);
						}
					curlist=curlist->next;
					j++;
					}
					//this case checks for the end of the list
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

















