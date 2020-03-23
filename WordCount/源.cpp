#include<stdio.h>
#include<string.h>
#include <direct.h>
#include <io.h>

#define ERROR -1    //错误状态
#define OK 1        //可行状态
#define LINE 1      //统计行数
#define CHAR 2      //统计字符数
#define WORD 3      //统计词数
#define SHOW -2      //递归处理文件
#define CFILE 1     //是否为c文件
#define MAXPATH 100 //路径最大值
  

//判断是否为C文件
int cFile(char op[]) {
    char* suffix;
    suffix = strrchr(op, '.');
    if (suffix != NULL) {
        if (strncmp(suffix, ".c", 2) == 0) {
            return CFILE;
        }
        else {
            printf("\n---请输入c文件！---\n");
            return 0;
        }
    }
    else {
        ("\n---请输入c文件！---\n");
        return 0;
    }
}

//检测文件格式
int checkFile(char* op[], FILE *file) {
    if (op[1] == NULL) {
        printf("\n---请输入操作指令！---\n");
        return ERROR;
    }
    
    if (cFile(op[2]) != CFILE) {
        return ERROR;
    }
    if (op[2] != NULL){
        //输入了文件名
        file = fopen(op[2], "r");
        if (file == NULL) {
            printf("\n---文件不存在！---\n");
            return ERROR;
        }
        return OK;
    }
    else {
        //未输入文件名
        printf("\n---请输入文件名！---\n");
        return ERROR;
    }
}


//检测并转化操作参数
int switchOp(char* op[]) {
    if (op[1] != NULL && op[1][2] == NULL) {
        if (op[1][0] != '-') {
            //指令格式错误
            return ERROR;
        }
        if (op[1][1] == 'l') {
            return LINE;
        }
        else if (op[1][1] == 'c') {
            return CHAR;
        }
        else if (op[1][1] == 'w') {
            return WORD;
        }
        else if (op[1][1] == 's') {
            return SHOW;
        }
        else {
            //不存在的指令
            printf("\n---指令错误!【可用指令：-c,-l,-w】---\n");
            return ERROR;
        }
    }
    else {
        //未输入指令
        printf("\n---请输入操作指令!---\n");
        return ERROR;
    }
}

//统计字符数
int countCharacter(FILE* file) {
    int character = 0, chNum = 0;
    while (!feof(file))
    {
        character = fgetc(file);
        //换行符不计
        if (character == '\n') {
            continue;
        }
        chNum++;
    }
    if (chNum == 0) {
        return 0;
    }
    return --chNum;
}

//统计行数
int countLine(FILE* file) {
    int lineNum = 0, word = 0;
    int flag = 0; //标识是否为空文件，空文件0行
    if (file != NULL) {
        while (!feof(file))
        {
            word = fgetc(file);
            //遇换行符
            if (word == '\n') {
                lineNum++;
            }
        }        
        return lineNum;      
    }  
}



//统计词数
int countWord(FILE* file) {
        int character = 0, word = 0, isWord = 0;
        while (!feof(file))
        {
            character = fgetc(file);
            if ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z'))
            {
                if (isWord == 0) {
                    isWord = 1;
                }        
            }
            else 
            {
                if (isWord == 1) {
                    word++;
                    isWord = 0;
                }             
            }
        }
        return word;      
}

//递归处理目录下文件
/*
_finddata_t结构体说明
struct _finddata_t{
	unsigned attrib;
	time_t time_create;
	time_t time_access;
	time_t time_write;
	_fsize_t size;
	char name[_MAX_FNAME];
 };
 unsigned atrrib：文件属性的存储位置。文件属性是用位表示的，主要有以下一些：
 _A_ARCH（存档）、 _A_HIDDEN（隐藏）、_A_NORMAL（正常）、_A_RDONLY（只读）、
 _A_SUBDIR（文件夹）、_A_SYSTEM（系统）

 char name[_MAX_FNAME]：文件的文件名。这里的_MAX_FNAME是一个常量宏，它在头
 文件中被定义，表示的是文件名的最大长度。



 针对_finddata_t结构体的主要函数：

long _findfirst( char *filespec, struct _finddata_t *fileinfo )；
     返回值：如果查找成功的话，将返回一个long型的唯一的查找用的句柄。这个句柄将会在_findnext函数中被使用。失败返回-1.
     参数：
        filespec：标明文件的字符串，可支持通配符。
        fileinfo ：这里就是用来存放文件信息的结构体的指针。

int _findnext( long handle, struct _finddata_t *fileinfo );
     返回值：若成功返回0，否则返回-1。
     参数：
        handle：即由_findfirst函数返回回来的句柄。
        fileinfo：文件信息结构体的指针。找到文件后，函数将该文件信息放入此结构体中。

int _findclose( long handle );
     返回值：成功返回0，失败返回-1。
     参数：
     handle ：_findfirst函数返回回来的句柄。
*/
void showAll(char* path) {  
    _finddata_t fileDir;
    long handle;
    char dir[MAXPATH];
    strcpy(dir, path);
    //路径添加'/'
    strcat(dir, "/*.*");

    //临时保存作用
    char pathTemp[MAXPATH];
    char nameTemp[MAXPATH];
    FILE* file;
    char* abPath;

    if ((handle = _findfirst(dir, &fileDir)) != -1l) {
        while (_findnext(handle, &fileDir) == 0) {          
                //是文件夹:fileDir.attrib == _A_SUBDIR
                if (fileDir.attrib == _A_SUBDIR && strcmp(fileDir.name, ".") != 0 && strcmp(fileDir.name, "..") != 0) {
                    strcpy(dir, path);
                    //strcat(dir, "/");
                    strcat(dir, fileDir.name);
                    //递归文件夹
                    showAll(dir);
                }
                else {
                    //是文件
                    if (strcmp(fileDir.name, ".") != 0 && strcmp(fileDir.name, "..") != 0) {
                        //是c文件
                        if (cFile(fileDir.name) == CFILE) {
                            strcat(path, "/");
                            strcpy(pathTemp, path);
                            strcpy(nameTemp, fileDir.name);
                            abPath = strcat(pathTemp, nameTemp);
                            printf("\n*****************************************\n");
                            printf("\t%s%s\n", path, fileDir.name);
                            file = fopen(abPath, "r");                                          
                            printf("\n\t行数：%d 行\n", countLine(file));
                            file = fopen(abPath, "r");
                            printf("\n\t字符数：%d 个\n", countCharacter(file));
                            file = fopen(abPath, "r");
                            printf("\n\t词数: %d 个\n", countWord(file));
                            printf("\n*****************************************\n");                           
                        }
                    }           
                }
            }      
    }          
    _findclose(handle);
}

int main(int argc,char* op[])
{   
    //获取当前路径
    char buffer[MAXPATH];
    char* path = getcwd(buffer, MAXPATH);
    //将斜杠'\'转换成'/'
    for (int i = 0; path[i] != NULL; i++) {
        if (path[i] == '\\') {
            path[i] = '/';
        }
    }

    int operation = switchOp(op);

    FILE *file = NULL;
    if (operation > 0) {
        if (checkFile(op, file) == OK) {
            file = fopen(op[2], "r");
            printf("%s",op[2]);
        }
        else {
            return 0;
        }
    }
      

	switch (operation)
	{
	case ERROR:      
        return 0;
	case LINE:
        printf("\n---行数：%d 行---\n", countLine(file));        
        fclose(file);
		break;
	case CHAR:
        printf("\n---字符数：%d 个---\n", countCharacter(file));
        fclose(file);
		break;
	case WORD:
        printf("\n---词数: %d 个---\n", countWord(file));      
        fclose(file);
		break;
    case SHOW:
        showAll(path);
        break;
	default:
		printf("\n---指令错误!---\n");
		break;
	}
    
}

