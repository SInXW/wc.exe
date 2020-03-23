#include<stdio.h>
#include<string.h>
#include <direct.h>
#include <io.h>

#define ERROR -1    //����״̬
#define OK 1        //����״̬
#define LINE 1      //ͳ������
#define CHAR 2      //ͳ���ַ���
#define WORD 3      //ͳ�ƴ���
#define SHOW -2      //�ݹ鴦���ļ�
#define CFILE 1     //�Ƿ�Ϊc�ļ�
#define MAXPATH 100 //·�����ֵ
  

//�ж��Ƿ�ΪC�ļ�
int cFile(char op[]) {
    char* suffix;
    suffix = strrchr(op, '.');
    if (suffix != NULL) {
        if (strncmp(suffix, ".c", 2) == 0) {
            return CFILE;
        }
        else {
            printf("\n---������c�ļ���---\n");
            return 0;
        }
    }
    else {
        ("\n---������c�ļ���---\n");
        return 0;
    }
}

//����ļ���ʽ
int checkFile(char* op[], FILE *file) {
    if (op[1] == NULL) {
        printf("\n---���������ָ�---\n");
        return ERROR;
    }
    
    if (cFile(op[2]) != CFILE) {
        return ERROR;
    }
    if (op[2] != NULL){
        //�������ļ���
        file = fopen(op[2], "r");
        if (file == NULL) {
            printf("\n---�ļ������ڣ�---\n");
            return ERROR;
        }
        return OK;
    }
    else {
        //δ�����ļ���
        printf("\n---�������ļ�����---\n");
        return ERROR;
    }
}


//��Ⲣת����������
int switchOp(char* op[]) {
    if (op[1] != NULL && op[1][2] == NULL) {
        if (op[1][0] != '-') {
            //ָ���ʽ����
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
            //�����ڵ�ָ��
            printf("\n---ָ�����!������ָ�-c,-l,-w��---\n");
            return ERROR;
        }
    }
    else {
        //δ����ָ��
        printf("\n---���������ָ��!---\n");
        return ERROR;
    }
}

//ͳ���ַ���
int countCharacter(FILE* file) {
    int character = 0, chNum = 0;
    while (!feof(file))
    {
        character = fgetc(file);
        //���з�����
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

//ͳ������
int countLine(FILE* file) {
    int lineNum = 0, word = 0;
    int flag = 0; //��ʶ�Ƿ�Ϊ���ļ������ļ�0��
    if (file != NULL) {
        while (!feof(file))
        {
            word = fgetc(file);
            //�����з�
            if (word == '\n') {
                lineNum++;
            }
        }        
        return lineNum;      
    }  
}



//ͳ�ƴ���
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

//�ݹ鴦��Ŀ¼���ļ�
/*
_finddata_t�ṹ��˵��
struct _finddata_t{
	unsigned attrib;
	time_t time_create;
	time_t time_access;
	time_t time_write;
	_fsize_t size;
	char name[_MAX_FNAME];
 };
 unsigned atrrib���ļ����ԵĴ洢λ�á��ļ���������λ��ʾ�ģ���Ҫ������һЩ��
 _A_ARCH���浵���� _A_HIDDEN�����أ���_A_NORMAL����������_A_RDONLY��ֻ������
 _A_SUBDIR���ļ��У���_A_SYSTEM��ϵͳ��

 char name[_MAX_FNAME]���ļ����ļ����������_MAX_FNAME��һ�������꣬����ͷ
 �ļ��б����壬��ʾ�����ļ�������󳤶ȡ�



 ���_finddata_t�ṹ�����Ҫ������

long _findfirst( char *filespec, struct _finddata_t *fileinfo )��
     ����ֵ��������ҳɹ��Ļ���������һ��long�͵�Ψһ�Ĳ����õľ����������������_findnext�����б�ʹ�á�ʧ�ܷ���-1.
     ������
        filespec�������ļ����ַ�������֧��ͨ�����
        fileinfo �����������������ļ���Ϣ�Ľṹ���ָ�롣

int _findnext( long handle, struct _finddata_t *fileinfo );
     ����ֵ�����ɹ�����0�����򷵻�-1��
     ������
        handle������_findfirst�������ػ����ľ����
        fileinfo���ļ���Ϣ�ṹ���ָ�롣�ҵ��ļ��󣬺��������ļ���Ϣ����˽ṹ���С�

int _findclose( long handle );
     ����ֵ���ɹ�����0��ʧ�ܷ���-1��
     ������
     handle ��_findfirst�������ػ����ľ����
*/
void showAll(char* path) {  
    _finddata_t fileDir;
    long handle;
    char dir[MAXPATH];
    strcpy(dir, path);
    //·�����'/'
    strcat(dir, "/*.*");

    //��ʱ��������
    char pathTemp[MAXPATH];
    char nameTemp[MAXPATH];
    FILE* file;
    char* abPath;

    if ((handle = _findfirst(dir, &fileDir)) != -1l) {
        while (_findnext(handle, &fileDir) == 0) {          
                //���ļ���:fileDir.attrib == _A_SUBDIR
                if (fileDir.attrib == _A_SUBDIR && strcmp(fileDir.name, ".") != 0 && strcmp(fileDir.name, "..") != 0) {
                    strcpy(dir, path);
                    //strcat(dir, "/");
                    strcat(dir, fileDir.name);
                    //�ݹ��ļ���
                    showAll(dir);
                }
                else {
                    //���ļ�
                    if (strcmp(fileDir.name, ".") != 0 && strcmp(fileDir.name, "..") != 0) {
                        //��c�ļ�
                        if (cFile(fileDir.name) == CFILE) {
                            strcat(path, "/");
                            strcpy(pathTemp, path);
                            strcpy(nameTemp, fileDir.name);
                            abPath = strcat(pathTemp, nameTemp);
                            printf("\n*****************************************\n");
                            printf("\t%s%s\n", path, fileDir.name);
                            file = fopen(abPath, "r");                                          
                            printf("\n\t������%d ��\n", countLine(file));
                            file = fopen(abPath, "r");
                            printf("\n\t�ַ�����%d ��\n", countCharacter(file));
                            file = fopen(abPath, "r");
                            printf("\n\t����: %d ��\n", countWord(file));
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
    //��ȡ��ǰ·��
    char buffer[MAXPATH];
    char* path = getcwd(buffer, MAXPATH);
    //��б��'\'ת����'/'
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
        printf("\n---������%d ��---\n", countLine(file));        
        fclose(file);
		break;
	case CHAR:
        printf("\n---�ַ�����%d ��---\n", countCharacter(file));
        fclose(file);
		break;
	case WORD:
        printf("\n---����: %d ��---\n", countWord(file));      
        fclose(file);
		break;
    case SHOW:
        showAll(path);
        break;
	default:
		printf("\n---ָ�����!---\n");
		break;
	}
    
}

