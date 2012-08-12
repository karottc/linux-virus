#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>

#define INF_PATH    "/home/cy/work/linux_virus"
#define BUFSIZE     200

char currentPath[BUFSIZE]="";
static int count = 0;

void do_antivirus()
{
    DIR *dp;
    struct dirent *dirp;
    struct stat buf,cur_dir_buf;

    int len;  //文件名的长度
    char str_buf[BUFSIZE];

    if (!strcmp(currentPath,""))
    {
        //得到当前目录
        if (getcwd(currentPath,BUFSIZE)==NULL)
        {
            printf("getcwd failure !!\n");
            return;
        }

        printf("get current directory: %s !!\n",currentPath);

        //改变工作路径
        chdir(INF_PATH);
    }
    //打开目录流
    if ((dp=opendir("."))==NULL)
    {
        printf("opendir failure!!!\n");
        return;
    }
    //遍历该目录下的每个文件
    while ((dirp=readdir(dp)) != NULL)
    {
        len = strlen(dirp->d_name);
        //如果是.C文件，就进行恢复，
        if (dirp->d_name[len-1]=='c' &&
            dirp->d_name[len-2]=='.')
        {
            printf("C FILE:  %s\n",dirp->d_name);
            if (antivirus(dirp->d_name) == 0)
            {
                count++;
                printf("object file %s has been recovered !!\n",dirp->d_name);
            }
            printf("\n-------------------------------\n");
            continue;
        }

        //得到该文件的状态
        if (stat(dirp->d_name,&buf) < 0)
        {
            printf("get stat of file failure!!\n");
            continue;
        }
        //判断是否使目录
        if (!S_ISDIR(buf.st_mode))
        {
            continue;
        }
        //是./..就略过
        if (!strcmp(dirp->d_name,".") ||
            !strcmp(dirp->d_name,".."))
        {
            continue;
        }

        printf("the directory: %s\n",dirp->d_name);
        //.pulse文件夹中的文件使runtime，碰到会有文件，所以也略过
        if (!strcmp(dirp->d_name,".pulse"))
        {
            continue;
        }
        
        chdir(dirp->d_name);
        do_antivirus();
        chdir("..");
    }
    closedir(dp);
}

int antivirus(char *file_name)
{
    //需要感染的目标文件，已经被病毒文件，和一个临时文件
    FILE *fp_obj,*fp_tmp;
    //对文件一行一行的的扫描，每行的最大容量
    char buf[BUFSIZE];
    //一个标识，对文件感染的判断
    int flag;
    //临时文件名
    char *tmp_buf;
    //得到目标文件的stat
    struct stat statBuf;
    //用来放置文件的访问时间和修改时间
    struct utimbuf timeBuf;

    //打开目标文件，用于读写
    if ((fp_obj=fopen(file_name,"r+"))==NULL)
    {
        printf("open object file %s failure !!!\n",file_name);
        return 1;
    }
    //得到目标文件的stat
    if (stat(file_name,&statBuf) < 0)
    {
        printf("get object file %s stat failure !!\n",file_name);
        return 1;
    }
    //存放目标文件的修改时间和访问时间，这个避免被发现文件已被感染
    timeBuf.actime = statBuf.st_atime;
    timeBuf.modtime = statBuf.st_mtime;
   
    //新建一个临时文件
    if ((tmp_buf=tmpnam(NULL))==NULL)
    {
        printf("create temp file failure !!\n");
        return 1;
    }
    //打开临时文件，每次都是在临时文件中添加内容
    if ((fp_tmp=fopen(tmp_buf,"a+"))==NULL)
    {
        printf("open temp file failure !!\n");
        return 1;
    }
    //使用完后删除临时文件
    unlink(tmp_buf);

    flag = 1;
    //一行一行遍历目标文件
    while (fgets(buf,BUFSIZE,fp_obj) != NULL)
    {
        //目标文件已被感染的标志
        if (!strcmp(buf,"/* linux virus start */\n"))
        {
            // printf("object file %s has been infected !!\n",file_name);
            flag = 0;
            break;
        }

        //删除do_virus();语句
        if (!strcmp(buf,"\tdo_virus();\n"))
        {
            continue;
        }

        fputs(buf,fp_tmp);
    }
    
    //目标文件没有main函数，即不会被感染，直接返回
    if (flag != 0)
    {
        printf("object don't been infected !!\n");
        return 1;
    }

    //重置文件指针
    rewind(fp_tmp);
    if ((fp_obj=fopen(file_name,"w+"))==NULL)
    {
        printf("open object file %s failure !!!\n",file_name);
        return 1;
    }
    //将临时文件的内容覆盖掉目标文件的内容
    while (fgets(buf,BUFSIZE,fp_tmp) != NULL)
    {
        fputs(buf,fp_obj);
    }
    //关闭临时文件和目标文件
    fclose(fp_tmp);
    fclose(fp_obj);

    //设置目标文件的访问时间和修改时间为原来的时间
    if (utime(file_name,&timeBuf) < 0)
    {
        printf("set access and modify time failure !!!\n");
        return 1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    do_antivirus();
    printf("number of recover c file is: %d\n",count);
    return 0;
}
