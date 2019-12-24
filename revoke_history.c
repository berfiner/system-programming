/*takes ./bash_history as input
prints reversible commands
reverses the chosen command
 */


#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <ctype.h>

struct node {
    int  id;
    char * command_name;
    char * undo_command_name;
    char * input_files;
    struct node * next;

};
int gid=0;
struct node * head=NULL;
void undomkdir(struct node * stackhead,int id);
void undormdir(struct node * stackhead,int id);
void undomv(struct node * stackhead,int id);
void AddNewDataNode(char* input);
void PrintStack(struct node *);
void SplitBufferToArray(char *buffer, char * delim, char ** Output);
void undochmod(struct node * stackhead,int id);
void undotouch(struct node * stackhead,int id);
int findfileSize(char f_n[]);
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

    lines = malloc(1400 * sizeof(char*));
    int i;

    for (i=0; i < 1500; i++)
    {
      lines[i] = malloc(1500);
      //lines[i][0] = '\0'; // terminate for the case that last line does not contain characters
      fgets(lines[i], 1500, hist); // read up to 999 characters and terminate string
        AddNewDataNode(lines[i]);
    }
     int id;
    PrintStack(head);
    printf("Please enter the id of command you want to revoke: ");
    scanf("%d",&id);
    
   
    undotouch(head,id);
    undomkdir(head,id);
    undormdir(head,id);
   undomv(head,id);
    undochmod(head,id);
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
      newnode->id=0;
      newnode->command_name=input;
      newnode->next=NULL;
      
    
      if(strstr(newnode->command_name,"mkdir")!=NULL){
          newnode->undo_command_name="rmdir";
          newnode->input_files=substring(newnode->command_name);
          gid=gid+1;
          newnode->id=gid;
           
      }
      if(strstr(newnode->command_name,"rmdir")!=NULL){
          newnode->undo_command_name="mkdir";
          newnode->input_files=substring(newnode->command_name);
          gid=gid+1;
          newnode->id=gid;
           
      }

      if(strstr(newnode->command_name,"touch")!=NULL){
          newnode->undo_command_name="rm";
          newnode->input_files=substring(newnode->command_name);
          gid=gid+1;
          newnode->id=gid;
           
      }
      if(strstr(newnode->command_name,"mv")!=NULL){
          newnode->undo_command_name="mv";
          newnode->input_files=substring(newnode->command_name);
           gid=gid+1;
           newnode->id=gid;
      }
      if(strstr(newnode->command_name,"chmod")!=NULL){
          newnode->undo_command_name="chmod";
          newnode->input_files=substring(newnode->command_name);
           gid=gid+1;
           newnode->id=gid;
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

void undochmod(struct node * stackhead,int id){
    struct node *cur=stackhead;
    char command[200];
    while(cur!=NULL){
        
        if(strstr(cur->command_name,"chmod")!=NULL && cur->id==id){
            if(strchr(cur->input_files,',')==NULL){
                if(strchr(cur->input_files,'+')!=NULL){
                    char *a=strchr(cur->input_files,'+');
                    sprintf(command,"%s%s",cur->undo_command_name,cur->input_files);
                    char *b=replace_char(command,'+','-');
                    //printf("%s",command);
                    system(command);
                }
                if(strchr(cur->input_files,'-')!=NULL){
                    char *c=strchr(cur->input_files,'-');
                    sprintf(command,"%s%s",cur->undo_command_name,cur->input_files);
                    char *d=replace_char(command,'-','+');
                    //printf("%s",command);
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

void undomkdir(struct node * stackhead,int id){
    struct node *cur=stackhead;
    char command[500];

    while(cur!=NULL){
        if (cur->undo_command_name!=NULL  && cur->id==id) {
            sprintf(command,"%s%s",cur->undo_command_name,cur->input_files);
            system(command);
            
        }
        cur=cur->next;
    }
    
    
    
}

void undormdir(struct node * stackhead,int id){
    struct node *cur=stackhead;
    char command[500];

    while(cur!=NULL){
        if (cur->undo_command_name!=NULL  && cur->id==id) {
            sprintf(command,"%s%s",cur->undo_command_name,cur->input_files);
            system(command);
        }
        cur=cur->next;
    }
    
}

void undotouch(struct node * stackhead,int id){
    struct node *cur=stackhead;
    char command[500];

    while(cur!=NULL){
        if (cur->undo_command_name!=NULL  && cur->id==id) {
            sprintf(command,"%s%s",cur->undo_command_name,cur->input_files);
            //printf("%s",command);
            system(command);
        }
        
        cur=cur->next;
    }
    
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


void undomv(struct node * stackhead,int id){
    struct node *cur=stackhead;
    char command[500];

    char p[10][10];
    while(cur!=NULL ){
        if(strstr(cur->command_name,"mv")!=NULL && cur->id==id){
            
            if (cur->undo_command_name!=NULL) {
                char **strn=(char **) malloc(150*sizeof(char *));
                SplitBufferToArray(cur->input_files," ",strn);
                char *dest=(char *) malloc(500);
                 char **source=(char **) malloc(150*sizeof(char *));
                char temp[500][500];
                char **b=(char **) malloc(150*sizeof(char *));
               char newdest[550][550];
                int i=0;
                while(strn[i]!=NULL){
                        strn[i++];
                }
                

                 strcpy(dest,strn[i-2]);
              
                strn[i-2]=NULL;
                int j=0;
                while(strn[j]!=NULL){

                        b[j]=strrchr(strn[j++],'/');

                               }

                
                int k=0;
                       while(b[j]!=NULL){
                           
                           source[k]=b[j--];
                           strcpy(temp[k],source[k]);
                           k=k+1;
                                          }
               

                         int a=1;

                              int kn=k-1;
                              
                                 
                              
                                 while(strn[a]!=NULL && source[kn]!=NULL){

                                      strn[a][strlen(strn[a])-strlen(source[kn])]='\0';
                                      strcpy(newdest[kn],strn[a]);

                                      a=a+1;
                                       kn--;
                                  }

          
                               dest[strlen(dest)]='\0';
                 
                
                for(int r=0;r<k;r++){
                    sprintf(command,"%s %s%s %s\n",cur->undo_command_name,dest,temp[r],newdest[r]);
                    printf("command %s",command);
                    system(command);
                 
                }
                
                            
           

        }
        }
        cur=cur->next;
    }

}

void PrintStack(struct node * stackhead){
    struct node *cur=stackhead;
    while(cur!=NULL ){
        if(cur->id !=0)
            printf("%d %s ",cur->id, cur->command_name);
        cur=cur->next;
    }
}

