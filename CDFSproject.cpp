#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<unistd.h>
#include<iostream>
#include<io.h>

#define MAXINODE 50

#define READ 1
#define WRITE 2

#define MAXFILESIZE 2048

#define START 0
#define REGULAR 1
#define SPECIAL 2
#define CURRENT 1
#define END 2

typedef struct superblock
{
    int TotalInodes;
    int FreeiNodes;
}SUPERBLOCK, *PSUPERBLOCK;

typedef struct inode
{
    char FileName[50];
    int iNodeNumber;
    int FileSize;
    int FileActualSize;
    int FileType;
    char *Buffer;
    int LinkCount;
    int ReferenceCount;
    int permission; //1     23
    struct inode *next;
}INODE, *PINODE, **PPINODE;

typedef struct filetable
{
    int readoffset;
    int writeoffset;
    int count;
    int mode; //1 2 3
    PINODE ptrinode;
}FILETABLE,*PFILETABLE;

typedef struct efdt
{
    PFILETABLE ptrfiletable;
}UFDT;

UFDT UFDTArr[50];
SUPERBLOCK SUPERBLOCKobj;
PINODE head = NULL;

void man(char *name)
{
    if(name == NULL)return;
    if(strcmp(name,"create") == 0)
    {
        printf("Description : Used to create new regular file\n");
        printf("Usage : create file_name Permission\n");
    }
    else if(strcmp(name,"read") == 0)
    {
        printf("Description : used to read data from regular file\n");
        printf("Usage : read File_name No_Of_Bytes_To_Read\n");
    }
    else if (strcmp(name,"write") == 0)
    {
        printf("Description : used to write into regular file\n");
        printf("Usage : write File_Name\nAfter this enter the data that we want to write\n");

    }
    else if (strcmp(name,"is") == 0)
    {
        printf("Description : used to list al info of file\n");
        printf("Usage : is\n");
    }
    else if (strcmp(name,"stat") == 0)
    {
        printf("Description : Used to display info of file\n");
        printf("Usage : stat File_name\n");
    }
    else if(strcmp(name,"fstat") == 0)
    {
        printf("Description : used to display info of file\n");
        printf("Usage : sta File_Description\n");
    }
    else if(strcmp(name,"open") == 0)
    {
        printf("Description :used to open exising file\n");
        printf("Usage : open File_name\n");
    }
    else if(strcmp(name,"close") == 0)
    {
        printf("Description : used to close opened file\n");
        printf("Usage : File_name\n");
    }
    else if(strcmp(name,"closeall") == 0)
    {
        printf("Description : used to close allopened file\n");
        printf("Usage : closeall\n");
    }
    else if(strcmp(name,"iseek") == 0)
    {
        printf("Description : used to change file offset\n");
        printf("Usage : iseek File_name ChangeInOffset StartPoint\n");
    }
    else if(strcmp(name,"rm") == 0)
    {
        printf("Description : used to delete the file\n");
        printf("Usage : rm File_Name\n");
    }
    else
    {
        printf("ERROR : No manual entry available. \n");
    }
}

void DisplayHelp()
{
    printf("Is : To list out all files\n");
    printf("clear : to clear console\n");
    printf("open : to open the file \n");
    printf("close : to close the file\n");
    printf("closeall : to close all the opened files\n");
    printf("read : to read contents from files\n");
    printf("write : to write contents into file\n");
    printf("exit : to terminate file system\n");
    printf("stat : to display info of file using name\n");
    printf("fstat : to display info of file using file descriptor\n");
    printf("rm : to delete the file\n");
}

int GetDFromName(char *name)
{
    int i = 0;
    while(i<50)
    {
        if(UFDTArr[i].ptrfiletable != NULL)
            if(strcmp((UFDTArr[i].ptrfiletable->ptrinode->FileName),name) == 0)
            break;
        i++;
    }
    if(i == 50) return -1;
    else        return i;
}

PINODE Get_Inode(char*name)
{
    PINODE temp = head;
    int i =0;
    if(name == NULL)
        return NULL;
        while(temp != NULL)
        {
            if(strcmp(name,temp->FileName) == 0)
                break;
            temp = temp ->next;
        }
    return temp;
}

void CreateDILB()
{

    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    while(i <= MAXINODE)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        newn->LinkCount = 0;
        newn -> ReferenceCount = 0;
        newn -> FileType = 0;
        newn -> FileSize = 0;

            newn -> Buffer = NULL;
            newn -> next = NULL;
            newn -> iNodeNumber = i;
            if(temp == NULL)
            {
                head = newn;
                temp = head;
            }
            else
            {
                temp-> next = newn;
                temp = temp -> next;
            }
            i++;
    }
    printf("DILB created successfully\n");
}

void InitailiseSuperBlock()
{
    int i = 0;
    while(i<MAXINODE)
    {
        UFDTArr[i].ptrfiletable = NULL;
        i++;
    }
    SUPERBLOCKobj.TotalInodes = MAXINODE;
    SUPERBLOCKobj.FreeiNodes = MAXINODE;
}

int CreateFile(char *name,int permission)
{

    int i =0;
    PINODE temp = head;

    if((name == NULL) || (permission == 0) || (permission>3))
    {
        return -1;

    }
    if(SUPERBLOCKobj.FreeiNodes == 0)
    {
        SUPERBLOCKobj.FreeiNodes--;
    }
    if(Get_Inode(name) != NULL)
    {
        return -3;
    }
    while(temp!= NULL)
    {
        if(temp->FileType == 0)
        {
            break;
            temp = temp -> next;
        }

    }
    while(i<50)
    {
        if(UFDTArr[i].ptrfiletable == NULL)
        {
            break;;
            i++;
        }
    }

    UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));

    UFDTArr[i].ptrfiletable->count = 1;
    UFDTArr[i].ptrfiletable->mode = permission;
    UFDTArr[i].ptrfiletable->readoffset = 0;
    UFDTArr[i].ptrfiletable->writeoffset = 0;

    UFDTArr[i].ptrfiletable->ptrinode = temp;

    strcpy(UFDTArr[i].ptrfiletable->ptrinode->FileName,name);
    UFDTArr[i].ptrfiletable->ptrinode->FileType =REGULAR;
    UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount = 1;
    UFDTArr[i].ptrfiletable->ptrinode->LinkCount = 1;
    UFDTArr[i].ptrfiletable->ptrinode->FileSize = MAXFILESIZE;
    UFDTArr[i].ptrfiletable->ptrinode->FileActualSize = 0;
    UFDTArr[i].ptrfiletable->ptrinode->permission = permission;
    UFDTArr[i].ptrfiletable->ptrinode->Buffer = (char*)malloc(MAXFILESIZE);

    return i;

}

int rm_File(char * name)
{
    int fd = 0;
    fd = GetDFromName(name);
    if(fd == -1)
    {
        return -1;
    }
    (UFDTArr[fd].ptrfiletable->ptrinode->LinkCount)--;

    if(UFDTArr[fd].ptrfiletable->ptrinode->LinkCount == 0)
    {
        UFDTArr[fd].ptrfiletable->ptrinode->FileType = 0;
        free(UFDTArr[fd].ptrfiletable);
    }

    UFDTArr[fd].ptrfiletable = NULL;
    (SUPERBLOCKobj.FreeiNodes)++;
}

int ReadFile(int fd, char *arr,int isize)
{
    int read_size = 0;
    if(UFDTArr[fd].ptrfiletable == NULL)
    {
        return -1;
    }
    if(UFDTArr[fd].ptrfiletable->ptrinode->permission != READ && UFDTArr[fd].ptrfiletable->ptrinode->permission != READ+WRITE)
    {
        return -2;
    }
    if(UFDTArr[fd].ptrfiletable->readoffset ==UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)
    {
        return -3;
    }
    if(UFDTArr[fd].ptrfiletable->ptrinode->FileType!= REGULAR)
    {
        return -4;
    }
    read_size = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)-(UFDTArr[fd].ptrfiletable->readoffset);
    if(read_size<isize)
    {
        strncpy(arr,(UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->readoffset),read_size);
        UFDTArr[fd].ptrfiletable->readoffset = UFDTArr[fd].ptrfiletable->readoffset+read_size;

    }
    else
    {
        strncpy(arr,(UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->readoffset),isize);
        (UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->readoffset) +isize;
    }
    return isize;
}

int WriteFile(int fd, char *arr, int isize)
{
    if(((UFDTArr[fd].ptrfiletable->mode) != WRITE) && ((UFDTArr[fd].ptrfiletable->mode) != READ+WRITE))
    {
        return -1;
    }
    if(((UFDTArr[fd].ptrfiletable->ptrinode->permission) && ((UFDTArr[fd].ptrfiletable->mode) != READ+WRITE)))
    {
        return -1;
    }
    if((UFDTArr[fd].ptrfiletable->writeoffset) == MAXFILESIZE)
    {
        return -2;
    }
    if((UFDTArr[fd].ptrfiletable->ptrinode->FileType) != REGULAR)
    {
        return -3;
    }
    strncpy((UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->writeoffset),arr,isize);
    (UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->writeoffset) + isize;

    (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + isize;
    return isize;
}

int OpenFile(char * name, int mode)
{
    int i= 0;
    PINODE temp = NULL;
    if(name == NULL || mode <= 0)
    {
        return -1;

    }
    temp = Get_Inode(name);
    if(name == NULL)
    {
        return -2;

    }
    if(temp->permission<mode)
    {
        return -3;
    }
    while(i<50)
    {
        if(UFDTArr[i].ptrfiletable == NULL)
        {
            break;
            i++;
        }

    }

    UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));
    if(UFDTArr[i].ptrfiletable == NULL)
    {
        return -1;
    }
    UFDTArr[i].ptrfiletable->count = 1;
    UFDTArr[i].ptrfiletable->mode = mode;
    if(mode == READ+WRITE)
    {
        UFDTArr[i].ptrfiletable->readoffset = 0;
        UFDTArr[i].ptrfiletable->writeoffset = 0;
    }
    else if(mode == READ)
    {
        UFDTArr[i].ptrfiletable->readoffset =0;
    }
    else if(mode == WRITE)
    {
        UFDTArr[i].ptrfiletable->writeoffset = 0;
    }
    UFDTArr[i].ptrfiletable->ptrinode = temp;
    (UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)++;

    return i;
}

void CloseFileByName(int fd)
{
    UFDTArr[fd].ptrfiletable->readoffset = 0;
    UFDTArr[fd].ptrfiletable->writeoffset = 0;
    (UFDTArr[fd].ptrfiletable->ptrinode->ReferenceCount)--;
}


int CloseFileByName(char * name)
{
    int i =0;
    i = GetDFromName(name);
    if(i == -1)
    {

        return -1;
    }
    UFDTArr[i].ptrfiletable->readoffset =0;
    UFDTArr[i].ptrfiletable->writeoffset=0;
    (UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;
    return 0;
}

void CloseAllFIle()
{
    int i =0;
    while(i<50)
    {
        if(UFDTArr[i].ptrfiletable != NULL)
        {
            UFDTArr[i].ptrfiletable->readoffset = 0;
            UFDTArr[i].ptrfiletable->writeoffset = 0;
            (UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;
        }
        i++;
    }
}

int LseekFile(int fd, int size, int from)
{
    if((fd<0) || (from >2))
    {
        return -1;
    }
    if(UFDTArr[fd].ptrfiletable == NULL)
    {
        return -1;
    }
    if((UFDTArr[fd].ptrfiletable->mode == READ)|| (UFDTArr[fd].ptrfiletable->mode == READ+WRITE))
    {
        if(from == CURRENT)
            {
                if(((UFDTArr[fd].ptrfiletable->readoffset) + size) > UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)
                {
                    return -1;
                }
            }    
        else if(from == START)
            {
                if(size >(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
                {
                    return -1;
                }
                if(size<0)
                {
                    return -1;
                }
                (UFDTArr[fd].ptrfiletable->readoffset) = size;

            }
        else if(from == END)
            {
                if((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size >MAXFILESIZE)
                {
                    return -1;
                }
                if(((UFDTArr[fd].ptrfiletable->readoffset)+ size) < 0)
                {
                    return -1;
                }
                (UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size;
            }
        
    }

}

void ls_file()
{
    int i = 0;
    PINODE temp = head;

    if(SUPERBLOCKobj.FreeiNodes == MAXINODE)
    {
        printf("There are no files\n");
        return;
    }   

    printf("\nnFile Name\tInode number\tFile size\tlink count\n");
    printf("--------------------------------------------------------");
    while(temp != NULL)
    {
        if(temp->FileType)
        {
            printf("%s\t\t%d\t\t%d\t\t%d\n",temp->FileName,temp->iNodeNumber,temp->FileActualSize,temp->LinkCount);

        }
        temp = temp->next;
    }
    printf("--------------------------------------------------------");
}

int fstat_file(int fd)
{
    PINODE temp = head;
    int i = 0;
    if(fd<0)
    {
        return -1;
    }
    if(UFDTArr[fd].ptrfiletable == NULL)
    {
        return -2;
    }
    temp = UFDTArr[fd].ptrfiletable->ptrinode;

    printf("\n---------Statistical information about file--------\n");
    printf("File name : %s\n",temp->FileName);
    printf("Inode number %d\n",temp->iNodeNumber);
    printf("File size: %d\n",temp->FileSize);
    printf("Actual file size :%d\n",temp->FileActualSize);
    printf("Link count : %d\n",temp->LinkCount);
    printf("Reference count : %d\n",temp->ReferenceCount);

    if(temp->permission == 1)
    {
        printf("File permission : Read only\n");
    }
    else if(temp->permission == 2)
    {
        printf("File permission : Write\n");
    }
    else if(temp->permission == 3)
    {
        printf("File permisson : Read & Write\n");
    }
    printf("--------------------------------------------------------");

    return 0;
}

int stat_file(char*name)
{
    PINODE temp = head;
    int i = 0;
    if(name == NULL)
    {
        return -1;
    }

    while(temp != NULL)
    {
        if(strcmp(name,temp->FileName) == 0)
        {
            break;
            
        }
        temp = temp -> next;
    }
    if(temp == NULL)
    {
        return -2;
    }

    printf("\n---------Statistical information about file--------\n");
    printf("File name : %s\n",temp->FileName);
    printf("Inode number %d\n",temp->iNodeNumber);
    printf("File size: %d\n",temp->FileSize);
    printf("Actual file size :%d\n",temp->FileActualSize);
    printf("Link count : %d\n",temp->LinkCount);
    printf("Reference count : %d\n",temp->ReferenceCount);
    
    if(temp->permission == 1)
    {
        printf("File permission : Read only\n");
    }
    else if(temp->permission == 2)
    {
        printf("File permission : Write\n");
    }
    else if(temp->permission == 3)
    {
        printf("File permisson : Read & Write\n");
    }
    printf("--------------------------------------------------------");

    return 0;
}

int truncate_File(char *name)
{
    int fd = GetDFromName(name);
    if(fd == -1)
    {
        return -1;
    }
    memset(UFDTArr[fd].ptrfiletable->ptrinode->Buffer,0,1024);
    UFDTArr[fd].ptrfiletable->readoffset = 0;
    UFDTArr[fd].ptrfiletable->writeoffset = 0;
    UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize = 0;

}

int main()
{
    char *ptr = NULL;
    int iret = 0, fd = 0, count = 0;
    char command[4][80], str[80], arr[1024];

    InitailiseSuperBlock();
    CreateDILB();

    while(1)
    {
        fflush(stdin);
        strcpy(str,"");
        printf("\nMarvellous VFS : >");

        fgets(str,80,stdin); 
        count = sscanf(str,"%s %s %s %s",command[0],command[1],command[2],command[3]);
        if(count == 1)
        {
            if(strcmp(command[0],"ls") == 0)
            {
                ls_file();
            }
            else if(strcmp(command[0],"closeall") == 0)
            {
                CloseAllFIle();
                printf("All files closed successfully\n");
                continue;
            }
            else if(strcmp(command[0],"clear") == 0)
            {
                system("cls");
                continue;
            }
            else if(strcmp(command[0],"help") == 0)
            {
                DisplayHelp();
                continue;
            }
            else if(strcmp(command[0],"exit")== 0)
            {
                printf("Terminating the Marvellous Virtual File System\n");
                break;
            }
            else
            {
                printf("\nError : Command not found!!!\n");
                continue;
            }            
        }
        else if(count == 2)
        {
            if(strcmp(command[0],"stat ") == 0)
            {
                iret = stat_file(command[1]);
            }
            else if(strcmp(command[0],"fstat") == 0)
            {
                iret = (atoi(command[1]));
                if(iret == -1)
                {
                    printf("Error : Incorrect parameters\n");
                }
                if(iret == -2)
                {
                    printf("Eoor : there is no such file\n");
                }
                continue;
            }
            else if(strcmp(command[0],"close") == 0)
            {
                iret = CloseFileByName(command[1]);
                if(iret == -1)
                {
                    printf("Error : there is no such file\n");
                }
                continue;

            }
            else if(strcmp(command[0],"rm") == 0)
            {
                iret = rm_File(command[1]);
                if(iret == -1)
                {
                    printf("Eoor : there is no such file\n");
                }
                continue;
            }
            else if(strcmp(command[0],"man") == 0)
            {
                man(command[1]);
            }
            else if(strcmp(command[0],"write") == 0)
            {
                fd = GetDFromName(command[1]);
                if(fd == -1)
                {
                    printf("Error : incorrect parameter\n");
                    continue;
                }
                printf("Entert he data : \n");
                scanf("%[^\n]",arr);

                iret = strlen(arr);

                if(iret == 0)
                {
                    printf("Error : incorrect parameters\n");

                    continue;
                }
                iret = WriteFile(fd,arr,iret);

                if(iret == -1)
                {
                    printf("Error : permission denied\n");
                }
                if(iret == -2)
                {
                    printf("Error : there is no sufficient memory\n");
                }
                if(iret == -3)
                {
                    printf("Error : it is not regular file\n");
                }
            
            else if(strcmp(command[0],"truncate") == 0)
            {
                iret = truncate_File(command[1]);
                if(iret == -1)
                {
                    printf("Error : incorrect parameter\n");
                }
                else
                {
                    printf("\nError: command not found!!!\n");
                    continue;
                }
            }   
        }
        else if(count == 3)
        {
            if(strcmp(command[0],"create") == 0)
            {
                iret = CreateFile(command[1],atoi(command[2]));
                if(iret >=0)
                {
                    printf("File successfully created with descriptor : %d\n",iret);

                }
                if(iret == -1)
                {
                    printf("Error : incorrect parameters\n");
                }
                if(iret == -2)
                {
                    printf("Error there is no inodes\n");
                }
                if(iret == -3)
                {
                    printf("Error file already exists\n ");
                }
                if(iret == -4)
                {
                    printf("Error memory allocation failure\n");
                }
                continue;
            }
            else if(strcmp(command[0],"open") == 0)
            {
                iret = OpenFile(command[1],atoi(command[2]));
                if(iret >=0)
                {
                    printf("File successfully created with descriptor : %d\n",iret);

                }
                if(iret == -1)
                {
                    printf("Error : incorrect parameters\n");
                }
                if(iret == -2)
                {
                    printf("Error there is no inodes\n");
                }
                if(iret == -3)
                {
                    printf("Error file already exists\n ");
                }
                if(iret == -4)
                {
                    printf("Error memory allocation failure\n");
                }
                continue;
            }
            else if(strcmp(command[0],"read") == 0)
            {
                fd = GetDFromName(command[1]);
                if(fd == -1)
                {
                    printf("Error incorrect parameters\n");
                    continue;
                }
                ptr = (char*)malloc(sizeof(atoi(command[2]))+1);
                if(ptr == NULL)
                {
                    printf("Error memory allocation failure\n");
                    continue;
                }
                iret = ReadFile(fd,ptr,atoi(command[2]));

                if(iret == -1)
                {
                    printf("Error file not existing\n");
                }
                if(iret == -2)
                {
                    printf("Error permission denied\n");
                }
                if(iret == -3)
                {
                    printf("Error reached at the end of file\n");

                }
                if(iret == -4)
                {
                    printf("Error : it is not regular file\n");
                }
                if(iret > 0)
                {
                    write(2,ptr,iret);
                }
                continue;
            }
            else
            {
                printf("\nERROR : command not found!!!\n");
                continue;
            }
        }
        else if(count == 4)
        {
            if(strcmp(command[0],"lseek") == 0)
            {
                fd = GetDFromName(command[1]);
                if(fd == -1)
                {
                    printf("Error : incorrect parameter\n");
                }
                iret = LseekFile(fd,atoi(command[2]),atoi(command[3]));
                if(iret == -1)
                {
                    printf("\nError unable to perform lseek!!!\n");
                    continue;
                }
            }
        }
        else
        {
            printf("Command not found\n");
            continue;
        }
    }
}
    return 0;
}
