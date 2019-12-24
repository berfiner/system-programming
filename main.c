#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main (int argc, char *argv[]){
    
    char *cmdargs[] ={ "/bin/bash","./chmod.sh", argv[2], argv[3], NULL };
    char *cmdargs2[] ={ "/bin/bash","./diff.sh", argv[2], argv[3], NULL };
    char *args[]={"./revoke_history.o",argv[2],NULL};
    
    char *args2[]={"./dictionary.o",argv[2],NULL};
    
   if((strcmp(argv[1],"mod_changer")!=0 ) && (strcmp(argv[1],"difference_maker")!=0) && (strcmp(argv[1],"revoke_history")!=0) && (strcmp(argv[1],"most_used_word")!=0)){
       printf("Please enter valid command\n");
   }

    
    if(strcmp(argv[1],"mod_changer")==0){
        if(argv[2]==NULL && argv[3] ==NULL){
            printf("Please enter valid permission types\n");
        }
        else
        execv(cmdargs[0], cmdargs);
    }
    else if(strcmp(argv[1],"difference_maker")==0){
        if(argv[2]==NULL && argv[3] ==NULL){
            printf("Please enter two .txt files\n");
        }
        else
        execv(cmdargs2[0], cmdargs2);
        
        
    }
    else if(strcmp(argv[1],"revoke_history")==0){
        if(argv[2]==NULL){
            printf("Please enter $HISTFILE\n");
        }
        else
        execvp(args[0],args);
        
        
    }
    else if(strcmp(argv[1],"most_used_word")==0){
        if(argv[2]==NULL){
            printf("Please enter valid txt file\n");
        }
        else
        execvp(args2[0],args2);
        
        
    }


    
    return 0;
    
}
