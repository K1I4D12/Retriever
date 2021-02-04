#include<stdio.h>
#include<string.h>
#include<dirent.h>
#include<time.h>
#include<windows.h>
#include <pthread.h>
#define MAX_SIZE 30
#define NUM_THREADS 3
void Folder(DIR *folder,char *qq,char nndir[]);
void File(char *file,char*qq);
void Find(void *args);
int flag=0;
struct args {FILE *f1;char *qq;};
int main(int argc,char *argv[]){
    clock_t start,finish;
    char qq[MAX_SIZE];
    char nndir[100];
    DIR *dir;
    if(argc<3){
        puts("You must input tow args.(./find.exe file line)");
        return 0;
    }
    start = clock();
    puts("####################################START####################################");
    strncpy(nndir,argv[1],MAX_SIZE);
    strncpy(qq,argv[2],MAX_SIZE);
    if((dir=opendir(nndir))==NULL){
        File(nndir,qq);
    }
    else
    {
        Folder(dir,qq,nndir);
    }
    puts("####################################DOWN####################################");
    finish = clock();
    printf("Using time:%fs",(double)(finish-start)/CLOCKS_PER_SEC);
    system("pause");
    return 0;
}


void File(char *file,char *qq){
    FILE *f1;
    f1 = fopen(file,"r");
    int i;
    pthread_t threads[NUM_THREADS];
    void *status;
    struct args arg={f1,qq};
    for(i=0;i<NUM_THREADS;i++){
        int thread = pthread_create(&threads[i],NULL,Find,(void*)(void*)&arg);
        if(thread){
            puts("Thread start faild!");
            exit(0);
        }
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], &status);
    }
}
void Find(void *arg){
    struct args *narg = (struct args *)arg;
    FILE *f1=narg->f1;
    char *qq=narg->qq;
    char line[MAX_SIZE];
    while(fgets(line,MAX_SIZE,f1)){
        if(strstr(line,qq)){
            puts("Find it:");
            fputs(line,stdout);
            fputs("Continue or Break?(c|b):",stdout);
            if(getchar()=='b'){
                flag=1;
                break;
            }
        
    }
}
}

void Folder(DIR *dir,char *qq,char nndir[100]){
    DIR *ndir;
    struct dirent *ptr;
    char nnndir[100];
    while((ptr=readdir(dir))!=NULL){
        if(strcmp(ptr->d_name,".")==0||strcmp(ptr->d_name,"..")==0){
            continue;
        }
        if(flag){break;}
        strncpy(nnndir,nndir,MAX_SIZE);
        strcat(nnndir,"\\\\");
        strncat(nnndir,ptr->d_name,MAX_SIZE);
        if((ndir=opendir(nnndir))!=NULL){
            Folder(ndir,qq,nnndir);
        }
        else{
            File(nnndir,qq);
        }

    }
}
