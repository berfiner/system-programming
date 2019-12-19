#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <ctype.h>
struct node {
    char * command_name;
    char * undo_command_name;
    char * input_files;
    struct node * next;

};

struct node * head=NULL;
void undomkdir(struct node * stackhead);
void undomv(struct node * stackhead);
void AddNewDataNode(char* input);
void PrintStack(struct node *);
void SplitBufferToArray(char *buffer, char * delim, char ** Output);
void undochmod(struct node * stackhead);
int main(int argv, char* argc[]) {
    //input : $HISTFILE
    char** lines = NULL;
    struct node *newnode;
    size_t len = 0;
    ssize_t read;

    FILE* hist = fopen(argc[1],"r");
     if(hist == NULL) {
       return 1;
     }
    /*
       while ((read = getline(&line, &len, hist)) != -1) {
           AddNewDataNode(line);
      }
    */

    lines = malloc(929 * sizeof(char*));
    int i;

    for (i=0; i < 929; i++)
    {
      lines[i] = malloc(1000);
      //lines[i][0] = '\0'; // terminate for the case that last line does not contain characters
      fgets(lines[i], 1000, hist); // read up to 999 characters and terminate string
        AddNewDataNode(lines[i]);
    }
   // PrintStack(head);
    //undomkdir(head);
    undomv(head);
    //undochmod(head);
     fclose(hist);


    return 0;
}
char* substring(const char* str)
{
         
    char *strn=strchr(str,' ');
    return strn;
}


  void AddNewDataNode(char* input){

      struct node *newnode,*curnode;
      //creates a new node
      newnode = malloc (sizeof (struct node));
      newnode->undo_command_name=NULL;
      newnode->input_files=NULL;
      newnode->command_name=input;
      newnode->next=NULL;
      char *inputs=NULL;

      if(strstr(newnode->command_name,"mkdir")!=NULL){
          newnode->undo_command_name="rmdir";
          newnode->input_files=substring(newnode->command_name);
          
      }
      if(strstr(newnode->command_name,"mv")!=NULL){
          newnode->undo_command_name="mv";
          newnode->input_files=substring(newnode->command_name);
         
      }
      if(strstr(newnode->command_name,"chmod")!=NULL){
          newnode->undo_command_name="chmod";
          newnode->input_files=substring(newnode->command_name);

      }
      

      if(head==NULL) head=newnode;
      else {
          newnode->next=head;
          head=newnode;
      }
      
  }

char* replace_char(char* str, char find, char replace){
    char *current_pos = strchr(str,find);
    while (current_pos){
        *current_pos = replace;
        current_pos = strchr(current_pos,find);
    }
    return str;
}
void replaceLast(char * str, char oldChar, char newChar)
{
    int i, lastIndex;

    lastIndex = -1;
    i = 0;

    while(str[i] != '\0')
    {
        if(str[i] == oldChar)
        {
            lastIndex = i;
        }

        i++;
    }

    if(lastIndex != -1)
    {
        str[lastIndex] = newChar;
    }
}

void undochmod(struct node * stackhead){
    struct node *cur=stackhead;
    char command[200];

    while(cur!=NULL){
        
        if(strstr(cur->command_name,"chmod")!=NULL){
            if(strchr(cur->input_files,',')==NULL){
                if(strchr(cur->input_files,'+')!=NULL){
                    char *a=strchr(cur->input_files,'+');
                    sprintf(command,"%s%s",cur->undo_command_name,cur->input_files);
                    char *b=replace_char(command,'+','-');
                    printf("%s",command);
                    system(command);
                }
                if(strchr(cur->input_files,'-')!=NULL){
                    char *c=strchr(cur->input_files,'-');
                    sprintf(command,"%s%s",cur->undo_command_name,cur->input_files);
                    char *d=replace_char(command,'-','+');
                    printf("%s",command);
                    system(command);
                }
            }
            else{
                char **a=(char **)malloc(50*sizeof(char*));
                char **c=(char **)malloc(50*sizeof(char*));
                SplitBufferToArray(cur->input_files," ",a);
                char *b=(char *)malloc(50*sizeof(char));
                char *m=(char *)malloc(50*sizeof(char));
                char *n=(char *)malloc(50*sizeof(char));
                strcat(command,cur->undo_command_name);
                strcat(command," ");
                int i=0;
                while(a[i]!=NULL){
                   a[i++];

                }
             strcpy(b,a[2]);
                SplitBufferToArray(a[1],",",c);
                int j=0;
                 while(c[j]!=NULL){

                    if(strchr(c[j],'+')!=NULL){
                        m=replace_char(c[j],'+','-');
                        strcat(command,m);
                        strcat(command,",");

                     }
                     else if(strchr(c[j],'-')!=NULL){
                         n=replace_char(c[j],'-','+');
                         strcat(command,n);
                         strcat(command,",");


                     }
                     a[j++];

                 }
                a[j]=NULL;
                replaceLast(command,',',' ');
                strcat(command,b);
                printf("%s",command);
                system(command);
  
                
        }
        }
        
        cur=cur->next;
        
    }
    
}

void undomkdir(struct node * stackhead){
    struct node *cur=stackhead;
    char command[50];

    while(cur!=NULL){
        if (cur->undo_command_name!=NULL) {
            sprintf(command,"%s%s",cur->undo_command_name,cur->input_files);
            printf("%s",command);
            system(command);
        }
        cur=cur->next;
    }
    
}
void split(char * str,char ** a){
    char * pch;
    pch = strtok (str," ");
    int i=0;
    while (pch != NULL)
    {
        a[i]=pch;
        pch = strtok (NULL, " ");
        i=i+1;
    }
    
}

void copy_string(char *target, char *source)
{
   while(*source)
   {
      *target = *source;
      source++;
      target++;
   }
   *target = '\0';
}

void SplitBufferToArray(char *buffer, char * delim, char ** Output) {

    int partcount = 0;
    Output[partcount++] = buffer;

    char* ptr = buffer;
    while (ptr != 0) { //check if the string is over
        ptr = strstr(ptr, delim);
        if (ptr != NULL) {
            *ptr = 0;
            
            Output[partcount++] = ptr + strlen(delim);
            ptr = ptr + strlen(delim);
        }

    }
    Output[partcount++] = NULL;
 
}

void undomv(struct node * stackhead){
    struct node *cur=stackhead;
    char command[200];

    char p[10][10];
    while(cur!=NULL){
        if(strstr(cur->command_name,"mv")!=NULL){
            
            if (cur->undo_command_name!=NULL) {
                char **strn=(char **) malloc(50*sizeof(char *));
                SplitBufferToArray(cur->input_files," ",strn);
                strn[2][strlen(strn[2])-1]='\0';
                char *d=(char *) malloc(100);
                strcpy(d,strn[2]);
                
                char * b=strrchr(strn[1],'/');

                char *c=(char *) malloc(100);
                strncpy(c,strn[1],b-strn[1]);

                sprintf(command," %s %s%s %s",cur->undo_command_name,d,b,c);
                printf("%s %s %s \n",d,b,c);

               
       /*
                split(cur->input_files,strn);
                
                strn[1][strlen(strn[1])-1]='\0';
                
                char a[100];
                char d[100];
                copy_string(a,strn[0]);
                 
                      copy_string(d,strn[1]);
                      char * b=strrchr(a,'/');
                      printf("%s",b);
                      char *c=(char *) malloc(100);
                      strncpy(c,a,b-a);
                 
                      sprintf(command,"%s %s%s %s ",cur->undo_command_name,d,b,c);
                free(c);
                free(strn);
    
              
                        
            */

  
/*
                  else{
                sprintf(command,"%s %s/%s . ",cur->undo_command_name,strn[1],strn[0]);
                  }
*/
                              
              // printf("%s",command);
                system(command);

                
        }
        }
        cur=cur->next;
    }
    
}

void PrintStack(struct node * stackhead){
    struct node *cur=stackhead;
    int i=0;
    while(cur!=NULL){
        printf("%s",cur->command_name);
        cur=cur->next;
        i=i+1;
    }
}

