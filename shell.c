/*This program was done by Sergio Garcia Cascon (sergio.gcascon@udc.es) and Pedro Pillado Garcia-Gesto (pedro.pilladog@udc.es)*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <pwd.h>
#include <locale.h>
#include <langinfo.h>
#include <stdint.h>
#include <grp.h>
#include <limits.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/resource.h>
#include "ShellList.h"


#define INPUTSIZE 2048
#define LINESIZE 1024
#define READALL ((ssize_t)-1)

node *listMall;
node *listMmap;
node *listShared;

int mem1;
float mem2;
char mem3;

void CmdAutores (char *tr[]){
   if (tr[0] == NULL || strcmp(tr[0],"-n")==0) /* if the input contains "-n" we print the authors */
      printf ("Los autores son Sergio Garcia Cascon and Pedro Pillado Garcia-Gesto\n");
   if (tr[0] == NULL || strcmp(tr[0],"-l")==0) /* if the input contains "-l" we print the logins */
      printf ("Los logins son sergiogcacon@udc.es and pedro.pilladog@udc.es\n");
}

void CmdSalir(){
   exit(0);
}

void CmdPid (char *tr[]){
  if (tr[0] == NULL || strcmp(tr[0],"")==0)
  /*Read the command written by the user*/
     printf ("The pid of the process is %d\n", getpid());
  else if (tr[0] == NULL || strcmp(tr[0],"-p")==0)
  /*Check if the user wants to access the parent process*/
     printf ("The pid of the parent process is %d\n",getppid());
}
void CmdChdir (char *tr[]){
  char cwd[1024]; /*Variable to store the path of the current dirtory*/
  if ((tr[0]) == NULL) {
    if (getcwd(cwd, sizeof(cwd)) == NULL) /*Check if the command is valid*/
      printf("Dirtory not found.\n");
    else
    getcwd(cwd, sizeof(cwd));
    /*If no arguments are passed the current dirtory is printed*/
      printf("Current working dir: %s\n", cwd);
  }
  else{
    chdir(tr[0]); /*Change to the directory specified in the command*/
  }
}

void CmdFecha (){
time_t t = time(NULL);
struct tm tm = *localtime(&t); /* With this command we obtain the present time */

printf("Date: %d-%2d-%2d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday); /* We only print the year, month and day */
}

void CmdHora (){
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  printf("Hour: %2d:%2d:%2d\n", tm.tm_hour, tm.tm_min, tm.tm_sec); /* Here we print the hour, minutes and seconds */

}

void CmdCreate (char *tr[]){
  int fp = 0;
  if (tr[0] == NULL || !strcmp(tr[0],"-d")==0){ /* If we don't type "-d" we create and empty file */
    fp = open(tr[0],O_CREAT, 0777);
    if (fp < 0){
      perror("Error");
    }
  }
  else if (tr[0] != NULL || strcmp(tr[0],"-d")==0){ /*If we introduce "-d" we create a dirtory*/
    if ((mkdir(tr[1], 00777)) == -1){ /*tr[1] is reading the value after "-d" in order to set the dirtory's name*/
      perror("Error"); /*mkdir returns "-1" if it couldn't create the dirtory*/
    }
  }
}

int dir_empty(const char *path){
	struct dirent *ent;
	int retValue = 1;
	DIR *d = opendir(path);
	if (!d) { /*Check if opendir worked correctly*/
		fprintf(stderr, "%s: ", path);
		perror("Error");
		return -1;
	}
	while ((ent = readdir(d))) { /*Access the files of the directory in a loop*/
		if (!strcmp(ent->d_name, ".") || !(strcmp(ent->d_name, ".."))) /*Prevent acess to parent directory or itself*/
			continue;
		retValue = 0;
		break;
	}
	closedir(d); /* We free the memory used by the directory */
	return retValue;
}

int FileSize (char *nom){
/* We declare this function to avoid having to use the struct "stat" every time */
  struct stat s;
  if (lstat(nom,&s)==-1)
    return (long) -1;
  return (long) s.st_size;
}

char FileType (mode_t m) {
  switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
    case S_IFSOCK: return 's'; /*socket */
    case S_IFLNK: return 'l'; /*symbolic link*/
    case S_IFREG: return '-'; /* normal file */
    case S_IFBLK: return 'b'; /*block device*/
    case S_IFDIR: return 'd'; /*directory */
    case S_IFCHR: return 'c'; /*char device*/
    case S_IFIFO: return 'p'; /*pipe*/
    default: return '?'; /*Unknown*/
  }
}

int Is_Dir (char *nom){
/* We use this function to avoid having to declare a stat every time we want to know if a file is a directory */
  struct stat s;
  if (lstat(nom,&s)==-1)
    return 0;
  return S_ISDIR (s.st_mode);
}

int DeletRec(const char *tr){ /* Recursive function used to delete non-empty directories */
  int retValue = 0;
  DIR *dir;
  struct dirent *d;
  char *aux = malloc(LINESIZE);
  struct stat mystat;

  if (((dir = opendir(tr)) == NULL)){ /* If we can't access the directory we return -1 */
    retValue = -1;
    return retValue;
  }
  else{ /* We open the directory and read every file contained in it */
    while(!retValue && (d = readdir(dir))){
      /*Prevent accessing the parent directory or itself*/
      if(!strcmp(d->d_name, "..") || !strcmp(d->d_name,".")){
        continue;
      }
      sprintf(aux, "%s/%s", tr, d->d_name); /* We build the path, storing it in "aux" */
      if(!lstat(aux, &mystat)){
        if (Is_Dir(aux)){ /* if the new path is a directory, we use the function recursively */
          retValue = DeletRec(aux);
        }
        else { /* Else, we remove the file */
          retValue = unlink(aux);
        }
      }
    }
    free(aux); /* We free the memory allocated to aux */
    closedir(dir); /* We close the directory */
  }
  if (retValue == 0){
    retValue = rmdir(tr);
  }
  return retValue;
}

void CmdDelete(char *tr[]){
  if (!strcmp(tr[0],"")){
    printf("Invalid argument for delete.");
  }
  else if(!strcmp(tr[0],"-r")){
    if (DeletRec(tr[1]) == -1){
      perror("Error");
    }
  }
  else if (strcmp(tr[0],"-r")){
    if (Is_Dir(tr[0]) && dir_empty(tr[0])){ /* if the directory is empty, it's removed */
      rmdir(tr[0]);
    }
    else if (Is_Dir(tr[0]) && !dir_empty(tr[0])){ /* If we introduce a non-empty directory, it gives an error */
      rmdir(tr[0]);
      perror("Error");
    }
    else {
      unlink(tr[0]); /* We delete the file */
    }
  }
}

char * ConvierteModo(mode_t m){
  static char permisos[12];
  strcpy (permisos,"----------");

  permisos[0]=FileType(m);
  if (m&S_IRUSR) permisos[1]='r';
  if (m&S_IWUSR) permisos[2]='w';
  if (m&S_IXUSR) permisos[3]='x';
  if (m&S_IRGRP) permisos[4]='r';
  if (m&S_IWGRP) permisos[5]='w';
  if (m&S_IXGRP) permisos[6]='x';
  if (m&S_IROTH) permisos[7]='r';
  if (m&S_IWOTH) permisos[8]='w';
  if (m&S_IXOTH) permisos[9]='x';
  if (m&S_ISUID) permisos[3]='s';
  if (m&S_ISGID) permisos[6]='s';
  if (m&S_ISVTX) permisos[9]='t';
  return (permisos);
}

char *MonthConv(int l){
/* We introduce the int given by "timeinfo->tm_mon" (see in CmdQuery) and transform it to a string */
    switch (l) {
      case 0: return "Jan";
      case 1: return "Feb";
      case 2: return "Mar";
      case 3: return "Apr";
      case 4: return "May";
      case 5: return "Jun";
      case 6: return "Jul";
      case 7: return "Aug";
      case 8: return "Sep";
      case 9: return "Oct";
      case 10: return "Nov";
      case 11: return "Dec";
      default: return "Fail";
    }
}

void CmdQuery(char *tr[]){
  int i = 0;
  struct stat mystat;
  struct tm * timeinfo;
  struct passwd *pwd;
  struct group   *grp;
    while(tr[i] != NULL){ /* We search the different words of the string */
      stat(tr[i], &mystat);
      printf("%8ld ", mystat.st_ino);
      printf("%s ", ConvierteModo(mystat.st_mode));
      printf("%ld ", mystat.st_nlink);
      if ((pwd = getpwuid(mystat.st_uid)) != NULL)
        printf("%s ", pwd->pw_name);
      if ((grp = getgrgid(mystat.st_gid)) != NULL)
        printf("%s", grp->gr_name);
      printf(" %5d ", FileSize(tr[i]));
      timeinfo = gmtime(&(mystat.st_mtime));
      printf("%s ", MonthConv(timeinfo->tm_mon)); /* Month gives a number, so we convert it to the name of the month*/
      printf("%d ", timeinfo->tm_mday);
      printf("%d:", timeinfo->tm_hour);
      printf("%d ", timeinfo->tm_min);
      printf(" %s\n", tr[i]);
      i++;
    }
}

/* statQuery was created in order to be used by CmdList, passing only one word as an argument */
void statQuery(char *tr){
  struct stat mystat;
  struct tm * timeinfo;
  struct passwd *pwd;
  struct group   *grp;
      stat(tr, &mystat);
      printf("%8ld ", mystat.st_ino);
      printf("%s ", ConvierteModo(mystat.st_mode));
      printf("%ld ", mystat.st_nlink);
      if ((pwd = getpwuid(mystat.st_uid)) != NULL)
        printf("%s ", pwd->pw_name);
      if ((grp = getgrgid(mystat.st_gid)) != NULL)
        printf("%s", grp->gr_name);
      printf(" %5d ", FileSize(tr));
      timeinfo = gmtime(&(mystat.st_mtime));
      printf("%s ", MonthConv(timeinfo->tm_mon));
      printf("%d ", timeinfo->tm_mday);
      printf("%d:", timeinfo->tm_hour);
      printf("%d ", timeinfo->tm_min);
      printf(" %s\n", tr);
}

int RecDir (char *dir, int n, int r, int h){
  DIR *p;
  char aux[LINESIZE];
  struct dirent *d;
    if (!(Is_Dir(dir))){ /* If it isn't a directory, we print its attributes*/
      statQuery(dir);
      return 0;
    }
    else{
      if (Is_Dir(dir)) {
        if ((p=opendir(dir))==NULL)
            return -1;
        else{
          if (!r) { /* Case of the instruction not being recursive */
/*In the following ifs the output is changed depending on the flags chosen*/
            if (n && h){
              printf(" %s: %d\n", dir, FileSize(dir)); /* We print just the name and the size of the file */
            }
            else if (!(n) && h){
              statQuery(dir);
            }
            else if (n && !(h)){
              if (dir[0] != '.'){ /* if the file starts with . we don't print it, because it's a hidden file */
                printf(" %s: %d\n", dir, FileSize(dir));
              }
            }
            else if (!(n) && !(h)){
              if (dir[0] != '.'){
                statQuery(dir);
              }
            }
          }
          else{
            while((d=readdir(p))!=NULL){
                sprintf(aux, "%s/%s", dir, d->d_name); /* Update the path for the child files/directories*/
/*In the following ifs the output is changed depending on the flags chosen*/
                  if (n && h){
                    printf(" %s: %d\n", d->d_name, FileSize(aux));
                  }
                  else if (!(n) && h){
                    statQuery(aux);
                  }
                  else if (n && !(h)){
                    if (d->d_name[0] != '.'){
                      printf(" %s: %d\n", d->d_name, FileSize(aux));
                    }
                  }
                  else if (!(n) && !(h)){
                    if (d->d_name[0] != '.'){
                      statQuery(aux);
                    }
                  }
                  /*Prevent accessing the parent directory or itself*/
                  if (strcmp(d->d_name,"..") && strcmp(d->d_name, ".")){
                    if (r && Is_Dir(aux)){
                      statQuery(dir);
                      RecDir(aux,n,r,h);
                    }
              }
            }
          }
        }
       }
    return closedir(p);
    }
}

void CmdList(char *tr[]){
  DIR *p;
  struct dirent *d;
  char cwd[1024];
  int h = 0, r = 0, n = 0, i , nFlags2;
  for(i=0 ; tr[i]!=NULL;i++){
    if (!strcmp(tr[i],"-r")) r = 1;
    else if (!strcmp(tr[i],"-h")) h = 1;
    else if (!strcmp(tr[i],"-n")) n = 1;
    else break;
  }
  if (Is_Dir(tr[i]) && !r){
    if ((p=opendir(tr[i]))==NULL)
        perror("Error");
    else{
      while((d = readdir(p)) != NULL){
        statQuery(d->d_name);
      }
  }
  }
  if (tr[i]==NULL){
    if (getcwd(cwd, sizeof(cwd)) == NULL) /*Check if the command is valid*/
      perror("Error");
    else
      if ((p=opendir(cwd))==NULL)
          perror("Error");
      else{
        while((d = readdir(p)) != NULL){
          statQuery(d->d_name);
        }
    }
  }
  nFlags2 = h + r + n;
  /*Check number of flags are activated in order to provide a proper input to RecDir*/
  for ( i = nFlags2;tr[i] != NULL; i++){
      RecDir(tr[i],n,r,h);
  }
}

/*******************************************************************************/
void CmdMalloc(char *tr[]){
  long size;
  char *timeinfo;
  void *MemAddr;
  time_t currTime;
  time(&currTime);
  timeinfo = ctime(&currTime);
  struct data *m=malloc(sizeof(struct data));
  if (tr[1] != NULL){
    size = strtoul(tr[1],NULL,10);
    MemAddr = malloc(size);
    m->size = size;
    m->MemAddr = MemAddr;
    m->timeinfo = timeinfo;
    printf("allocated %s at %p.\n",tr[1], MemAddr);
    listMall = insertList(listMall,m);
  }
  else {
    printListMall(listMall);
  }
}

void *MmapFichero (char * fichero, int protection, char *timeinfo){
  int df, map= MAP_PRIVATE, modo = O_RDONLY;
  struct stat s;
  void *p;
  if (protection&PROT_WRITE)
    modo = O_RDWR;

  if (stat(fichero,&s)==-1 || (df=open(fichero,modo))==-1)
    return NULL;

  if ((p=mmap (NULL,s.st_size, protection, map, df, 0))==MAP_FAILED)
    return NULL;

  return p;
}

void CmdMmap (char *tr[]){
  char *perm, *timeinfo, *name;
  int fd;
  void *p;
  int protection = 0;
  time_t currTime;
  time(&currTime);
  timeinfo = ctime(&currTime);
  struct data *m=malloc(sizeof(struct data));
  if (tr[1]!=NULL){
    name = strdup(tr[1]);
    if ((perm=tr[2]) != NULL && strlen(perm)<4){
      if (strchr(perm,'r') != NULL) protection|=PROT_READ;
      if (strchr(perm,'w') != NULL) protection|=PROT_WRITE;
      if (strchr(perm,'x') != NULL) protection|=PROT_EXEC;
    }
    if ((p=MmapFichero(tr[1],protection,timeinfo))==NULL)
      perror("Impossible to map file");
    else {
      fd = open(tr[1], O_RDONLY);
      printf("File %s mapped in %p\n", tr[1],p);
      m->MemAddr = p;
      m->size = FileSize(tr[1]);
      m->timeinfo = timeinfo;
      m->fd = fd;
      m->name = name;
      listMmap = insertList(listMmap,m);
    }
  }
  else {
      printListMmap(listMmap);
  }
  close(fd);
}

void *ObtainMemoryShmget(key_t key, off_t tam, node *l){
  void *p;
  int aux, id, flags = 0777;
  struct shmid_ds s;
  struct data *item = malloc(sizeof(struct data));
  time_t timeNow;
  char *timeinfo;

  if (tam)
    flags=flags | IPC_CREAT | IPC_EXCL;

  if (key == IPC_PRIVATE){
    errno = EINVAL;
    return NULL;
  }

  if ((id=shmget(key,tam,flags))==-1)
    return NULL;

  if ((p=shmat(id,NULL,0))==(void*)-1){
    aux=errno;
    if (tam)
      shmctl(id,IPC_RMID,NULL);
    errno=aux;
    return (NULL);
  }

  shmctl (id,IPC_STAT,&s);
  printf("Shmget memory with key %d assigned to %p\n",key, p);

  timeNow = time(&timeNow);
  timeinfo = ctime(&timeNow);

  item->key = key;
  item->timeinfo = timeinfo;
  item->MemAddr = p;
  item->size = s.shm_segsz;

  listShared = insertList(listShared, item);
  return(p);
}

void CmdCreateShared (char *tr[]){
  key_t k;
  off_t tam=0;
  void *p;

  if (tr[1]==NULL||tr[2]==NULL){
    printListShared(listShared);
    return;
  }
  k = (key_t) atoi(tr[1]);
  if (tr[2]!=NULL)
    tam = (off_t) atoll(tr[2]);
  if ((p = ObtainMemoryShmget(k,tam,listShared))==NULL)
    perror ("Impossible to obtain shmget memory");
}

void CmdShared (char *tr[]){ /* FUNSIONA MAL MIRAR PDF */
  key_t k;
  void *p;

  if (tr[1]==NULL){
    printListShared(listShared);
  }
  else{
    k = (key_t) atoi(tr[1]);
    if ((p = ObtainMemoryShmget(k,0,listShared))==NULL)
      perror ("Impossible to obtain shmget memory");
  }
}

void CmdAllocate(char *tr[]){
  if (tr[0] == NULL){
    printListMall(listMall);
    printListMmap(listMmap);
    printListShared(listShared);
    return;
  }
  else{
    if (!strcmp(tr[0],"-malloc")){
      CmdMalloc(tr);
    }
    if (!strcmp(tr[0],"-mmap")){
      CmdMmap(tr);
    }
    if (!strcmp(tr[0],"-createshared")){
      CmdCreateShared(tr);
    }
    if (!strcmp(tr[0],"-shared")){
      CmdShared(tr);
    }
  }
}

void CmdMallocD(char *tr[]){
  node *item;
  struct data *info;

    if (tr[1] == NULL){
      printListMall(listMall);
    }
    else{
        item = searchListMall(listMall,tr[1]);
        if (item == NULL){
          printListMall(listMall);
        }
        else{
            info = item->data;
            free(info->MemAddr);
            removeNode(&listMall,item);
        }
    }
}

void CmdMmapD(char *tr[]){
  node *item;
  struct data *info;
    if (tr[1] == NULL){
      printListMmap(listMmap);
    }
    else{
        item = searchListMmap(listMmap,tr[1]);
        if (item == NULL){
          printListMmap(listMmap);
        }
        else{
            info = item->data;
            munmap(info->MemAddr,info->size);
            removeNode(&listMmap,item);
        }
    }
}

void CmdSharedD(char *tr[]){ /*Problema borrando memory address (malloc?)*/
  node *item;
  struct data *item2;
  key_t k;
    if (tr[1] == NULL){
      printListShared(listShared);
      return;
    }
    else{
      k = (key_t) atol(tr[1]); /*Podemos volver a utilizar una key?*/
      item = searchNodeShared(listShared,k);
      if (item == NULL){
        printListShared(listShared);
      }
      else{
        item2 = item->data;
        shmdt(item2->MemAddr);
        removeNode(&listShared,item);
      }
    }
}

void MallAux(void *MemAddr){
  node *item;

    if (MemAddr == NULL){
      printListMall(listMall);
    }
    else{
        item = searchListAddr(listMall,MemAddr);
        if (item == NULL){
          printListMall(listMall);
        }
        else{
            removeNode(&listMall,item);
        }
    }
}

void MmapAux(void *MemAddr){
  node *item;

    if (MemAddr == NULL){
      printListMmap(listMmap);
    }
    else{
        item = searchListAddr(listMmap,MemAddr);
        if (item == NULL){
          printListMmap(listMmap);
        }
        else{
            removeNode(&listMmap,item);
        }
    }
}

void SharedAux(void *MemAddr){ /*Problema borrando memory address (malloc?)*/
  node *item;
  struct data *item2;
    if (MemAddr == NULL){
      printListShared(listShared);
      return;
    }
    else{
      item = searchListAddr(listShared,MemAddr);
      if (item == NULL){
        printListShared(listShared);
      }
      else{
        item2 = item->data;
        shmdt(item2->MemAddr);
        removeNode(&listShared,item);
      }
    }
}

void CmdAddr(char *tr[]){
  node *l;
  void *MemAddr;
  struct data *item;
  if (tr[0] == NULL){
    printListShared(listShared);
    printListMmap(listMmap);
    printListShared(listShared);
  }
  else{
    MemAddr = (void *) strtoul(tr[0],NULL,0);
    l = searchListAddr(listMall,MemAddr);
    if (l != NULL){
      item = l->data;
      printf("block at address %p deallocated (malloc)\n",item->MemAddr);
      MallAux(MemAddr);//Hacer otro MallocD con addr
    }
    else {
      l = searchListAddr(listMall,MemAddr);
      if (l != NULL){
        item = l->data;
        printf("block at address %p deallocated (mmap)\n",item->MemAddr);
        MmapAux(MemAddr);
      }
      else {
        l = searchListAddr(listShared, MemAddr);
        if (l != NULL){
          item = l->data;
          printf("block at address %p deallocated (shared)\n",item->MemAddr);
          SharedAux(MemAddr);
        }
      }
    }
  }
}

void CmdDeallocate(char *tr[]){
  if (tr[0] == NULL){
    printListMall(listMall);
    printListMmap(listMmap);
    printListShared(listShared);
    return;
  }
  else{
    if (!strcmp(tr[0],"-malloc")){
      CmdMallocD(tr);
      return;
    }
    if (!strcmp(tr[0],"-mmap")){
      CmdMmapD(tr);
      return;
    }
    if (!strcmp(tr[0],"-shared")){
      CmdSharedD(tr);
      return;
    }
    CmdAddr(tr);
  }
}

void CmdRmKey(char *tr[]){
  key_t k;
  int id;
  char *key = tr[0];

  if (key == NULL || (k=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
    printf("   rmkey valid_key\n");
    return;
  }
  if ((id=shmget(k,0,0666))==-1){
    perror("shmget: imposible to get shared memory");
    return;
  }
  if(shmctl(id,IPC_RMID,NULL)==-1){
    perror("shmctl: imposible to free shared memory");
  }
}

void CmdMem(char *tr[]){
  printf("Address of CmdQuery: %p\n",CmdQuery);
  printf("Address of cmdList: %p\n",CmdList);
  printf("Address of cmdAllocate: %p\n",CmdAllocate);
  printf("Address of global variable mem1: %p\n",&mem1);
  printf("Address of global variable mem2: %p\n",&mem2);
  printf("Address of global variable mem3: %p\n",&mem3);
}

void CmdMemdump(char *tr[]){
  int i,k,j;
  char *p = (char *) strtoul(tr[0],NULL, 16);
  if (tr[1] == NULL){
    k = 25;
  }
  else {
    k = atol(tr[1]);
  }
  for (i = 1; i <= k; i++){
    printf(" %c ",p[i - 1]);
    if (i % 25 == 0 || i == k){
      printf("\n");
      for (j = (i - 25); j < i; j++){
        printf("%02X ", p[j]);
      }
      printf("\n");
    }
  }
  printf("\n");
}

void SubRecursive(int n){
  /*function that can be more easily called recursively as
   CmdRecursive needs a *char as parameter*/

  char automatic[INPUTSIZE];
  static char statik[INPUTSIZE];
  printf("parameter n:%d in %p\n",n,&n);
  printf("static array in: %p \n",statik);
  printf("Automatic array in %p\n",automatic);
  n--;
  if(n > 0){
    SubRecursive(n);
  }
}

void CmdRecursive(char *tr[]){
  key_t k;
  char *aux;
  aux = strdup(tr[0]); /* PREGUNTAR */
  k = (key_t) atoi(aux);
  SubRecursive(k);
}


ssize_t ReadFile (char *fich, void *p, ssize_t n, int cont){
  ssize_t nleidos,tam = n;
  int df, aux;
  struct stat s;

  if (stat (fich,&s) == -1 || (df = open(fich,O_RDONLY)) == -1)
    return ((ssize_t) - 1);

  if (cont==1){
    tam = (ssize_t) s.st_size;
  }
  if ((nleidos = read (df, p, tam)) == -1){
    aux = errno;
    close(df);
    errno = aux;
    return ((ssize_t)-1);
  }

  close (df);
  return(nleidos);
}

void CmdReadFile(char *tr[]){
  char * file;
  void *address;
  long size;
  int cont;

  file = strdup(tr[0]);
  address = (void *) strtoul(tr[1],NULL,0);
  if (tr[2]!=NULL){
      size = strtoul(tr[2],NULL,10);
      cont = 0;
  }
  else
    cont = 1;
  ReadFile(file, address, size, cont);

}

void WriteFile(char* file, void *address, long size, int o){
  int fd = 0;
  if ((fd = open(file,O_RDONLY)) < 0 && errno==ENOENT ){
    fd = open(file, O_CREAT | O_WRONLY , 0666);
    write(fd,address,size);
    close(fd);
  }
  else{
    if (o == 0){
      printf("Error: file already exists.\n");
    }
    else{
      fd = open(file,O_WRONLY | O_TRUNC);
      write(fd,address,size);
      close(fd);
    }
  }
}

void CmdWriteFile(char *tr[]){
  char * file;
  void *address;
  long size;
  int o = 0;

  file = strdup(tr[0]);
  address = (void *) strtoul(tr[1],NULL,0);
  size = strtoul(tr[2],NULL,10);
  if (tr[3] != NULL)
    o = 1;
  WriteFile(file, address, size, o);
}

int TrocearCadena(char *cadena, char *trozos[]){
  int i=1;
  if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
      return 0;
  while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
      i++;
  return i;
}

/******************************************************************************/


void ProcessInput (char *input){
  struct CMD{
    char *name;
    void (*pfunc)(char **);
    void (*cmdtipos)(char tr[]);
  };
   char *pz[INPUTSIZE/2];
   int n,i;
   static struct CMD cmds[]={
   /*Check if the input matches one of the existing command*/
      {"autores", CmdAutores},
      {"pid", CmdPid},
      {"fin",CmdSalir},
      {"chdir", CmdChdir},
      {"salir",CmdSalir},
      {"exit",CmdSalir},
      {"fecha",CmdFecha},
      {"hora",CmdHora},
      {"create", CmdCreate},
      {"delete", CmdDelete},
      {"query", CmdQuery},
      {"list", CmdList},
      {"allocate", CmdAllocate},
      {"deallocate", CmdDeallocate},
      {"rmkey", CmdRmKey},
      {"mem",CmdMem},
      {"memdump",CmdMemdump},
      {"recursivefunciton",CmdRecursive},
      {"read",CmdReadFile},
      {"write",CmdWriteFile},
    };

    if ((n=TrocearCadena(input,pz))==0)
          return;
    for (i=0;cmds[i].name != NULL; i++)
        if (!strcmp(cmds[i].name,pz[0])){
          (*cmds[i].pfunc)(pz+1);
          return;
        }
}

int main(){

  char input[INPUTSIZE];
  createList(listMall);
  createList(listMmap);
  createList(listShared);
  while (1){
        printf ("-->");
        fgets(input,INPUTSIZE,stdin);
        ProcessInput (input);
  }
}
