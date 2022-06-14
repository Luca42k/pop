#include <stdio.h>  /* for printf() and getchar() */
#include <sys/socket.h>  /* for socket(), send() and recv() */
#include <stdlib.h>  /* for atoi() and exit() */
#include <string.h>  /* for memset() */
#include <unistd.h>  /* for close() */
#include <dirent.h>  /* for opendir(), readdir(), closedir() */
#include <netdb.h> /* for adddr search */

// define structure
struct user_info
{
    char domain[300];
    char username[300];
    char password[300];
} user;
#define BUFFSIZE 5000

// implicit code
void hidePassword(char *password)
{
    getchar(); //get a character from the buffer
    //system("stty -icanon"); //not enter to read immediately
    //system("stty -echo"); //prohibit back display
    int i=0;
    while(i < 20)
    {
        password[i]=getchar();
        if(i == 0 && password[i] == 127) //delete
        {
            i=0;
            password[i]='\0'; //push return
            continue; //finish this circulation
        }
        else if(password[i] == 127) //delete
        {
<<<<<<< HEAD
            printf("\b \b");  //\b��Cursor moves to the left backspace clear code
=======
            printf("\b \b");  //\b��������ƶ� backspace clear code
>>>>>>> a5a142e5113447ab6d859d12187d9e3f07448388
            password[i]='\0';
            i-=1;
            continue;
        }
        else if(password[i] == '\n')
        {
            password[i]='\0'; //finish input
            break;
        }
        else
        {
            printf("*");
        }
        i++;
    }
    system("stty echo");
    system("stty icanon");
}

// login
void loginInit()
{
    memset(&info, 0, sizeof(info));
    info.pop3_port = 110;

    printf("Please input the pop3 domain:\n");
    printf(">>");
    scanf("%s", info.pop3_domain);

    printf("Please input username:\n");
    printf(">>");
    scanf("%s", info.pop3_username);

    printf("Please input password:\n");
    printf(">>Enter password: ");
    getPassword(info.pop3_password);
}


// 登陆邮件服务器
int loginServer()
{
    // 发送用户名
    char username[256 + 10];
    memset(username, 0, sizeof(username));
    sprintf(username, "user %s\r\n", info.pop3_username);
    int usernameSize = strlen(username);
    if(send(info.sock, username, usernameSize, 0) <= 0)
    {
        printf("send username error!\n");
        return -1;
    }

    // 反馈ok
    char buffer[BUFFERSIZE];
    memset(buffer, 0, sizeof(buffer));
    if(recv(info.sock, buffer, BUFFERSIZE, 0) <= 0)
    {
        printf("recv username feedback error!\n");
        return -1;
    }

    if(strstr(buffer, "OK") == NULL)
    {
        printf("recv username feedback ok error!\n");
        return -1;
    }

    // 发送密码
    char password[256 + 10];
    memset(password, 0, sizeof(password));
    sprintf(password, "pass %s\r\n", info.pop3_password);
    int passwordSize = strlen(password);
    if(send(info.sock, password, passwordSize, 0) <= 0)
    {
        printf("send password error!\n");
        return -1;
    }

    // 反馈ok
    memset(buffer, 0, sizeof(buffer));
    if(recv(info.sock, buffer, BUFFERSIZE, 0) <= 0)
    {
        printf("recv password feedback error!\n");
        return -1;
    }

    if(strstr(buffer, "OK") == NULL)
    {
        printf("\nrecv password feedback ok error!\n");
        return -1;
    }

    return 0;
}


// 与邮件服务器建立tcp连接
int socketTcp()
{
    struct sockaddr_in addr;
    addr.sin_port = htons(info.pop3_port);
    addr.sin_family = AF_INET;

    // 域名获取ip
    struct hostent *host = gethostbyname(info.pop3_domain);
    if(host == NULL)
    {
        printf("gethostbyname error!\n");
        return -1;
    }
    memcpy(&addr.sin_addr, host->h_addr, host->h_length);

    // 创建socket
    info.sock = socket(AF_INET, SOCK_STREAM, 0);
    if(info.sock == -1)
    {
        printf("socket error!\n");
        return -1;
    }

    // 连接
    if(connect(info.sock, (const struct sockaddr*)&addr, sizeof(struct sockaddr_in)) == -1)
    {
        printf("connect error!\n");
        return -1;
    }

    // 服务器反馈ok
    char buffer[BUFFERSIZE];
    memset(buffer, 0, sizeof(buffer));
    if(recv(info.sock, buffer, BUFFERSIZE, 0) <= 0)
    {
        printf("recv error!\n");
        return -1;
    }

    if(strstr(buffer, "OK") == NULL)
    {
        printf("recv ok error!\n");
        return -1;
    }

    return 0;
}


// 登陆
int login()
{
    // tcp连接
    if(socketTcp() == -1)
    {
        printf("TCP connect error!\n");
        return -1;
    }

    // 登陆服务器
    if(loginServer() == -1)
    {
        printf("login server Error!\n");
        return -1;
    }

    return 0;
}


int display()
{
    int opt = 0;
    system("clear");
    printf("*************************************************************\n");
    printf("1.Get a list of messages and sizes\n");
    printf("2.Get mail status\n");
    printf("3.Dispaly mail in detail\n");
    printf("4.Search text in all mails\n");
    printf("5.Display by subjects\n");
    printf("6.Download the mail and delete in the remote service\n");
    printf("7.Quit\n");
    printf("*************************************************************\n");
    printf("Please choose number:\n");
    printf(">>");
    scanf("%d", &opt);
    return opt;
}


int display2()
{
    int opt = 0;
    printf("Press 1 to return to main interface\n");
    printf("Press 2 to return to quit\n");
    printf(">>");
    scanf("%d", &opt);
    return opt;
}


int getList()
{
    // 发送list，接收服务器返回的消息
    char str[] = "list\r\n";
    int strSize = strlen(str);
    if(send(info.sock, str, strSize, 0) <= 0)
    {
        printf("send list error!\n");
        return -1;
    }

    char buffer[BUFFERSIZE];
    memset(buffer, 0, sizeof(buffer));
    if(recv(info.sock, buffer, BUFFERSIZE, 0) <= 0)
    {
        printf("recv list feedback error!\n");
        return -1;
    }

    printf("%s\n", buffer);
    return 0;
}


int getStatus()
{
    // 发送stat，接收服务器返回的消息
    char str[] = "stat\r\n";
    int strSize = strlen(str);
    if(send(info.sock, str, strSize, 0) <= 0)
    {
        printf("send stat error!\n");
        return -1;
    }

    char buffer[BUFFERSIZE];
    memset(buffer, 0, sizeof(buffer));
    if(recv(info.sock, buffer, BUFFERSIZE, 0) <= 0)
    {
        printf("recv stat feedback error!\n");
        return -1;
    }

    printf("%s\n", buffer);
    return 0;
}


// 获取纯文本邮件正文
char* getContentPlain(char* buffer)
{
    char* str = "Content-Transfer-Encoding: base64";
    if(strstr(buffer, str) == NULL)
    {
        return NULL;
    }
    // printf("buffer: %s\n", buffer);

    char* str1 = "X-Coremail-Antispam";
    char* str2 = "\r\n";
    char* bufferX = strstr(buffer, str1);
    if(bufferX == NULL)
    {
        return NULL;
    }
    bufferX = strstr(bufferX, str2);
    if(bufferX == NULL)
    {
        return NULL;
    }

    //printf("bufferX: %s\n", bufferX);

    strtok(bufferX, str2);
    char* content = strtok(NULL, str2);
    //printf("content: %s\n", content);
    return content;
}


// 获取超文本邮件的正文
char* getContentMultipart(char* buffer)
{
    char* str = "Content-Transfer-Encoding: base64";
    char* bufferC = strstr(buffer, str);
    if(bufferC == NULL)
    {
        return NULL;
    }

    char* str1 = "\r\n";
    char* buffer1 = strtok(bufferC, str1);
    if(buffer1 == NULL)
    {
        return NULL;
    }

    char* str2 = "\n";
    buffer1 = strtok(NULL, str2);
    if(buffer1 == NULL)
    {
        return NULL;
    }

    char* content = strtok(NULL, str1);
    return content;
}


// 解析邮件的正文
char* getContent(char* buffer)
{
    char* str = "Content-Type: multipart/alternative";
    if(strstr(buffer, str) == NULL)
    {
        return getContentPlain(buffer);
    }
    else
    {
        return getContentMultipart(buffer);
    }
}


int num_strchr(const char *str, char c)
{
    const char *pindex = strchr(str, c);

    if (NULL == pindex){
        return -1;
    }
    return pindex - str;
}


// base64解码
char* base64_decode(char* base64)
{
    const char * base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    char * dedata;
    dedata = (char *) malloc(BUFFERSIZE);
    int i = 0, j=0;
    int trans[4] = {0,0,0,0};

    for (;base64[i]!='\0';i+=4) { // 每四个一组，译码成三个字符

        trans[0] = num_strchr(base64char, base64[i]);
        trans[1] = num_strchr(base64char, base64[i+1]);

        dedata[j++] = ((trans[0] << 2) & 0xfc) | ((trans[1]>>4) & 0x03);

        if (base64[i+2] == '='){
            continue;
        }else{
            trans[2] = num_strchr(base64char, base64[i + 2]);
        }

        dedata[j++] = ((trans[1] << 4) & 0xf0) | ((trans[2] >> 2) & 0x0f);

        if (base64[i + 3] == '='){
            continue;
        }else{
            trans[3] = num_strchr(base64char, base64[i + 3]);
        }

        dedata[j++] = ((trans[2] << 6) & 0xc0) | (trans[3] & 0x3f);
    }

    dedata[j] = '\0';

    return dedata;
}


// 更新邮件正文
char* changeBuffer(char* str, char* oldstr, char* newstr)
{
    char bstr[BUFFERSIZE]; //缓冲字符串数组
    memset(bstr,0,sizeof(bstr));

    int i;
    for(i = 0;i < strlen(str);i++){
        if(!strncmp(str+i,oldstr,strlen(oldstr))){
            //查找到目标字符串
            strcat(bstr,newstr);
            //把新字符串贴到缓冲字符串里
            i += strlen(oldstr) - 1;
        }else{ //如果没有找到
            strncat(bstr,str + i,1);//将该字符写入缓冲字符串数组
        }
    }

    strcpy(str,bstr);
    return str;
}


// 显示邮件内容，接收邮件后对正文进行base64解码，并替换原有的密文
int displayDetail()
{
    int opt = 0;
    printf("Input email number to view in detail\n");
    printf(">>");
    scanf("%d", &opt);

    char str[BUFFERSIZE];
    memset(str, 0, sizeof(str));
    sprintf(str, "retr %d\r\n", opt);
    int strSize = strlen(str);

    if(send(info.sock, str, strSize, 0) <= 0)
    {
        printf("send retr error!\n");
        return -1;
    }

    while(1)
    {
        char buffer[BUFFERSIZE];
        memset(buffer, 0, sizeof(buffer));
        int size = recv(info.sock, buffer, BUFFERSIZE, 0);
        if(size <= 0)
        {
            printf("recv retr feedback error!\n");
            return -1;
        }
        char buffer1[BUFFERSIZE];
        memset(buffer1, 0, sizeof(buffer1));
        memcpy(buffer1, buffer, size);

        char* content = getContent(buffer1);
        if(content != NULL)
        {
            // printf("content: %s\n", content);
            char* text = base64_decode(content);
            // printf("text: %s\n", text);
            char* buffer2 = changeBuffer(buffer, content, text);
            printf("%s", buffer2);
        }
        else
        {
            printf("%s", buffer);
        }

        // printf("%s", buffer);
        if(strstr(buffer, ".\r\n") != NULL)
        {
            break;
        }
    }
    return 0;
}


// 遍历本地目录，统计邮件数量
int searchMails()
{
    char path[BUFFERSIZE];
    memset(path, 0, sizeof(path));
    getcwd(path, BUFFERSIZE);

    struct dirent **name_list;
    int n = scandir(path, &name_list, 0, alphasort);
    int count = 0, index = 0;
    while(index < n)
    {
        char* suffix = strrchr(name_list[index]->d_name, '.');
        if(suffix != NULL)
        {
            char* str = ".eml";
            if(!strcmp(suffix, str))
            {
                count++;
            }
        }
        free(name_list[index++]);
    }
    free(name_list);
    return count;
}


// 遍历本地目录下的邮件，匹配邮件解码后的内容和text
int searchMailText(char* text)
{
    char path[BUFFERSIZE];
    memset(path, 0, sizeof(path));
    getcwd(path, BUFFERSIZE);

    struct dirent **name_list;
    int n = scandir(path, &name_list, 0, alphasort);

    int count = 0, index = 0;
    while(index < n)
    {
        char* filename = name_list[index]->d_name;
        char* suffix = strrchr(filename, '.');
        if(suffix != NULL)
        {
            char str1[] = ".eml";
            if(!strcmp(suffix, str1))
            {
                FILE* fp = fopen(filename, "r");
                if(fp != NULL)
                {
                    char buffer[BUFFERSIZE];
                    while(1)
                    {
                        memset(buffer, 0, sizeof(buffer));
                        if(fread(buffer, 1, BUFFERSIZE, fp) <= 0)
                        {
                            break;
                        }

                        char buffer1[BUFFERSIZE];
                        memset(buffer1, 0, sizeof(buffer1));
                        memcpy(buffer1, buffer, BUFFERSIZE);

                        char* content = getContent(buffer1);

                        if(content != NULL)
                        {
                            char* decode = base64_decode(content);
                            if(strstr(decode, text) != NULL)
                            {
                                printf("The mail of %s has the text.\n", filename);
                                count++;
                                break;
                            }
                        }
                    }
                    fclose(fp);
                }
            }
        }
        free(name_list[index++]);
    }
    // printf("count %d\n", count);
    free(name_list);
    return count;
}


// 搜索本地邮件正文是否包含text
int searchText()
{
    printf("Please input the text you want to search:\n");
    printf(">>");
    char buffer[BUFFERSIZE];
    memset(buffer, 0, sizeof(buffer));
    scanf("%s", buffer);

    int mailNum = searchMails();
    printf("you have %d emails\n", mailNum);

    int includeNum = searchMailText(buffer);
    printf("This is (are) %d mail(s) including the text.\n", includeNum);

    return 0;
}


// 搜索本地邮件主题，遍历目录下的邮件，输出subject
int searchSubject()
{
    char path[BUFFERSIZE];
    memset(path, 0, sizeof(path));
    getcwd(path, BUFFERSIZE);

    struct dirent **name_list;
    int n = scandir(path, &name_list, 0, alphasort);
    int index = 0;
    while(index < n)
    {
        char* filename = name_list[index]->d_name;
        char* suffix = strrchr(filename, '.');
        if(suffix != NULL)
        {
            char* str = ".eml";
            if(!strcmp(suffix, str))
            {
                FILE* fp = fopen(filename, "r");
                if(fp != NULL)
                {
                    char buffer[BUFFERSIZE];
                    while(1)
                    {
                        memset(buffer, 0, sizeof(buffer));
                        if(fread(buffer, 1, BUFFERSIZE, fp) <= 0)
                        {
                            break;
                        }

                        char* str1 = "Subject: ";
                        char *bufferS = strstr(buffer, str1);
                        if(bufferS != NULL)
                        {
                            int str1Size = strlen(str1);
                            bufferS += str1Size;

                            char* str2 = "\r\n";
                            char *subject = strtok(bufferS, str2);
                            if(subject != NULL)
                            {
                                printf("the %s mail's Subject is %s\n", filename, subject);
                                break;
                            }
                        }
                    }
                    fclose(fp);
                }
            }
        }
        free(name_list[index++]);
    }
    free(name_list);
    return 0;
}


// 下载邮件
int download(int opt, char* filename)
{
    char str[BUFFERSIZE];
    memset(str, 0, sizeof(str));
    sprintf(str, "retr %d\r\n", opt);
    int strSize = strlen(str);

    // 发送retr
    if(send(info.sock, str, strSize, 0) <= 0)
    {
        printf("send retr error!\n");
        return -1;
    }

    // 创建文件，将接收到的邮件内容写到文件中
    char* str1 = ".eml";
    strcat(filename, str1);
    FILE* fp = fopen(filename, "a+");
    if(fp == NULL)
    {
        printf("fopen error!\n");
        return -1;
    }

    while(1)
    {
        char buffer[BUFFERSIZE];
        memset(buffer, 0, sizeof(buffer));
        int size = recv(info.sock, buffer, BUFFERSIZE, 0);
        if(size <= 0)
        {
            printf("recv retr feedback error!\n");
            fclose(fp);
            return -1;
        }

        fwrite(buffer, size, 1, fp);

        if(strstr(buffer, ".\r\n") != NULL)
        {
            break;
        }
    }

    fclose(fp);
    return 0;
}


// 删除邮件，发送dele命令
int deleteMail(int opt)
{
    char str[BUFFERSIZE];
    memset(str, 0, sizeof(str));
    sprintf(str, "dele %d\r\n", opt);
    int strSize = strlen(str);
    if(send(info.sock, str, strSize, 0) <= 0)
    {
        printf("send dele error!\n");
        return -1;
    }

    char buffer[BUFFERSIZE];
    memset(buffer, 0, sizeof(buffer));
    if(recv(info.sock, buffer, BUFFERSIZE, 0) <= 0)
    {
        printf("recv dele feedback error!\n");
        return -1;
    }

    printf("%s\n", buffer);
    return 0;
}


// 下载完删除邮件
int downloadDelete()
{
    printf("Please input the ID number:\n");
    printf(">>");
    int opt;
    scanf("%d", &opt);

    printf("Please input the filename you want to save:\n");
    printf(">>");
    char filename[BUFFERSIZE];
    memset(filename, 0, sizeof(filename));
    scanf("%s", filename);

    if(download(opt, filename) == -1)
    {
        printf("download error!\n");
        return -1;
    }

    if(deleteMail(opt) == -1)
    {
        printf("delete mail error!\n");
        return -1;
    }

    printf("%s save successfully and delete the email from the remote server\n", filename);
    return 0;
}


// 操作界面
void cmd()
{
    while(1)
    {
        // 展示选项
        int opt = display();
        int opt2 = 0;
        switch (opt)
        {
            case 1:
                // 获取邮件列表信息
                if(getList() == -1)
                {
                    printf("get list error!\n");
                    return;
                }
                opt2 = display2();
                if(opt2 == 2)
                {
                    return;
                }
                break;

            case 2:
                // 获取邮件状态
                if(getStatus() == -1)
                {
                    printf("get status error!\n");
                    return;
                }
                opt2 = display2();
                if(opt2 == 2)
                {
                    return;
                }
                break;

            case 3:
                // 显示base64解码后的邮件
                if(displayDetail() == -1)
                {
                    printf("display detail error!\n");
                    return;
                }
                opt2 = display2();
                if(opt2 == 2)
                {
                    return;
                }
                break;

            case 4:
                // 搜索本地邮件
                if(searchText() == -1)
                {
                    printf("search text error!\n");
                    return;
                }
                opt2 = display2();
                if(opt2 == 2)
                {
                    return;
                }
                break;

            case 5:
                // 显示邮件主题
                if(searchSubject() == -1)
                {
                    printf("search subject error!\n");
                    return;
                }
                opt2 = display2();
                if(opt2 == 2)
                {
                    return;
                }
                break;

            case 6:
                // 下载邮件并远程删除
                if(downloadDelete() == -1)
                {
                    printf("download and delete error!\n");
                    return;
                }
                opt2 = display2();
                if(opt2 == 2)
                {
                    return;
                }
                break;

            case 7:
                return;

            default:
                break;
        }
    }

}


int main(int argc, char* argv[])
{
    // 输入登陆信息
    loginInit();

    // 登陆邮件服务器
    if(login() == -1)
    {
        printf("login error!\n");
        return -1;
    }

    // 执行
    cmd();
    close(info.sock);
    return 0;
}

//这是最后一行