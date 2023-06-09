#include <stdio.h>
#include <stdbool.h>  //bool,true,false所在头文件
#include <string.h>  
#include <stdlib.h>  //system,atoi,itoa函数所在头文件
#include <conio.h> //getch函数所在头文件

#define AUTHORIZECODE "ProgrammerIsSoEasy"  //授权码

//学生结构体
typedef struct student {
    char id[5];
    char name[30];
    char gender[4];
    int age;
    char class[20];
    double score;
} Student;

//链表节点结构体
typedef struct node {
    Student student;
    struct node *PNext;
}Node;

//函数声明
void signIn(char username[],char password[]);
void signUp(char username[],char password[]);
void getPassword(char password[]);
Node *createLinkedList();
void show();
void sort(char optionp[]);
void add();
void find();
Student getStudentByInfo(char *info,char *delimiter);
void writerStudent(Student student);
void update();
void writerStudents();
void delete();
Node *findNode(char which);

bool signInstatus = false; // 登录成功标记，默认未登录
Node *students;  //学生链表指针
Node *pTail;  // 学生链表尾指针

int main(){

    //登录
    while(!signInstatus)
        signIn(NULL,NULL);

    //读取本地学生表到链表
    students = createLinkedList();

    //主菜单
    while(true)
    {
      fflush(stdin);
      system("cls");
      puts(" ┏━*********************━┓ ");
      puts(" ┃      学生管理系统      ┃ ");
      puts(" ┣━*********************━┫ ");
      puts(" ┃      1. 查看学生      ┃ ");
      puts(" ┃      2. 查找学生      ┃ ");
      puts(" ┃      3. 增加学生      ┃ ");
      puts(" ┃      4. 修改学生      ┃ ");
      puts(" ┃      5. 删除学生      ┃ ");
      puts(" ┃      6. 退出系统      ┃ ");
      puts(" ┗━*********************━┛");
      printf("请选择：\n");
      switch(getchar())
      {
        case '1':
           show();
           break;
        case '2':
           find();
           break;
        case '3':
           add();
           break;
        case '4':
           update();
           break;
        case '5':
           delete();
           break;
        case '6':
           exit(0);
        default:
           printf("输入错误！");
      }
     system("pause");
    }

}

//登录
void signIn(char *username,char *password)
{
    if(username == NULL && password == NULL)
    {
        username = (char *) malloc(sizeof(char) * 20);
        password = (char *) malloc(sizeof(char) * 20);
        puts("============== 登录 ==================");
        printf("用户名：");
        gets(username);
        printf("密码：");
        getPassword(password);
    }

    //打开用户列表root
    FILE *fp = fopen("./users.csv","r");
    if(fp == NULL)
    {
        puts("用户文件打开失败，请检查！");
        exit(1);
    }else {
        char user[50];
        bool isNotExist = true;
        //打开成功后读取每一行，一行则代表一个用户：username,password
        while(fgets(user,50,fp) != NULL){
            unsigned int tail = strlen(user)-1;
            //将行末的回车符删除
            if(user[tail] == '\n') {
                user[tail] = '\0';
            }
            //截取用户名和密码，并和输入的用户名密码作比较
            if(strcmp(username,strtok(user,",")) == 0) {
                isNotExist = false;
                if(strcmp(password,strtok(NULL,",")) == 0) {
                    //如果用户名和密码正确，登录成功
                    signInstatus = true;
                    printf("登录成功");
                    system("pause");
                } else {
                    puts("密码错误！");
                }
            }
        }
        //遍历完文件后若未发现用户，则提示注册
        if(isNotExist) {
            puts("没有此用户！按回车键重新输入，按其他键注册");
            if(getch() != '\r') {
                signUp(username,password);
            }

        }
    }
    fclose(fp);
}

//注册功能
void signUp(char *username,char *password) {
    //打开用户列表
    FILE *fp = fopen("./user.csv" , " a");
    if(fp == NULL)
    {
        puts("用户文件打开失败，请检查！");
        exit(1);
    } else {
        printf("请确认密码：");
        char repeat[20];
        getPassword(repeat);
        if(strcmp(password,repeat) == 0) {
            printf("请输入授权码：");
            char code[50];
            gets(code);
            //两次密码一致且授权码正确，则写入注册用户信息
            if(strcmp(code,AUTHORIZECODE)==0)
            {
                fputs(username,fp);
                putc(',',fp);
                fputs(password,fp);
                fputc('\n',fp);
                fflush(fp);
                signInstatus = true;
                printf("注册成功！已自动登录");
                system("pause");
            }else {
                puts("两次密码不一致");
            }
        }
        
    }
    fclose(fp);
}

//创建学生链表
Node *createLinkedList() {
 Node *pHead = (Node *)malloc(sizeof(Node));
 pTail = pHead;

 //打开用户列表
 FILE *fp = fopen("./students.csv","r");
 if(fp == NULL) {
    puts("学生文件打开失败，请检查！");
    exit(1);
 } else {
    char info[256];
    //成功读取每一行，一行则代表一个学生：id,name,gender,age,class,score
    while(fgets(info,256,fp) != NULL) {
        unsigned int tail = strlen(info) -1;
        //将行末得回车符删除
        if(info[tail] == '\n'){
            info[tail] = '\0';
        }
        //挂节点
        Node *pNode = (Node *)malloc(sizeof(Node));
        pNode->student = getStudentByInfo(info,",");
        pTail->PNext = pNode;
        pTail = pTail->PNext;
        pTail->PNext = NULL;
    }
 }

   return pHead;
}



//展示
void show(){
  puts("1.1 学号升序\t 1.2 学号降序");
  puts("2.1 年龄升序\t 2.2 年龄降序");
  puts("3.1 分数升序\t 3.2 分数降序");
  printf("默认以学号升序排列，请选择：");
  char option[4];
  fflush(stdin);
  sort(gets(option));
  puts("\n========================学生表====================\n");
 puts("- - - - - - - - - - - - - - - - - - - - - - - - - - -");
 puts("|学号\t| 姓名\t| 性别\t| 年龄\t| 班级\t\t| 分数\t| ");
 puts("- - - - - - - - - - - - - - - - - - - - - - - - - - -");

 // 遍历输出
    for (Node *pTemp = students->PNext; pTemp != NULL; pTemp = pTemp->PNext) {
        printf("|%s\t| %s\t| %s\t| %d\t| %s\t| %.1lf\t|\n", pTemp->student.id, pTemp->student.name,
               pTemp->student.gender, pTemp->student.age, pTemp->student.class, pTemp->student.score);
        puts("- - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
    }
}


//查找
void find(){
    printf("请输入关键字（学号、姓名、班级）：");
    char keyword[20];
    fflush(stdin);
    gets(keyword);
    puts("\n============ 学生表 ================\n");
    puts("- - - - - - - - - - - - - - - - - - - -");
    puts("|学号\t| 姓名\t| 性别\t| 年龄\t| 班级\t\t| 分数\t| ");
    puts("- - - - - - - - - - - - - - - - - - - -");
   for (Node *pTemp = students->PNext; pTemp != NULL; pTemp = pTemp->PNext) {
        if (strcmp(keyword, pTemp->student.id) == 0 || strcmp(keyword, pTemp->student.name) == 0 || strstr(pTemp->student.class, keyword) != NULL) {
            printf("|%s\t| %s\t| %s\t| %d\t| %s\t| %.1lf\t|\n", pTemp->student.id, pTemp->student.name,
                   pTemp->student.gender, pTemp->student.age, pTemp->student.class, pTemp->student.score);
            puts("- - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
        }
    }

}



//添加功能
void add(){
  puts("- - - - - - - - - - - - - - - - - - - - - - - - - -");
    puts("学号\t姓名\t性别\t年龄\t班级\t\t分数");
    puts("- - - - - - - - - - - - - - - - - - - - - - - - - -");
    char info[256];
    while (true) {
        fflush(stdin);
        gets(info);     // 获取学生信息
        if (strcmp(info, "ok") == 0) {      // 若输入ok则完成添加
            break;
        } else {
            Node *pNode = (Node *) malloc(sizeof(Node));
            Student student = getStudentByInfo(info, "\t");
            bool isNotExist = true;
            // 遍历链表，查询是否学生已经存在
            for (Node *pTemp = students->PNext; pTemp != NULL; pTemp = pTemp->PNext) {
                if (strcmp(student.id, pTemp->student.id) == 0) {
                    isNotExist = false;
                    printf("已存在%s号学生，已跳过该生添加\n", student.id);
                }
            }
            if (isNotExist) {
                pNode->student = student;
                writerStudent(student);
                pTail->PNext = pNode;
                pTail = pTail->PNext;
                pTail->PNext = NULL;
            }
        }
    }
}

//删除
void delete() {
    Node *pNode = findNode('p');
    puts("按回车确认删除，其它键取消：");
    if (getch() == '\r') {
        Node *pTemp = pNode->PNext;
        pNode->PNext = pTemp->PNext;
        free(pTemp);
        writerStudents();
        printf("删除成功!");
    } else {
        printf("已取消，");
    }
}

//修改

void update() {
  Node *pNode = findNode('c');
  puts("请输入新的学生信息：");
  fflush(stdin);
  char info[256];
  gets(info);
  pNode->student = getStudentByInfo(info,"\t");
  writerStudents();
  printf("修改完成!");

}





//传参数'c'返回匹配得当前节点，传其他参数返回前驱节点
Node *findNode(char which){
printf("请输入学生的学号或姓名：");
char keyword[20];
fflush(stdin);
gets(keyword);
puts("- - - - - - - - - - - - - - - - ");
puts("学号\t姓名\t性别\t年龄\t班级\t\t分数\t");
puts("- - - - - - - - - - - - - - - - ");
Node *pTemp;
for(pTemp = students->PNext;pTemp != NULL;pTemp = pTemp->PNext) {
    if (strcmp(keyword,pTemp->PNext->student.id) == 0 || strcmp(keyword,pTemp->PNext->student.name)==0) {
        printf("%s\t%s\t%s\t%d\t%s\t%.1lf\t\n", pTemp->PNext->student.id, pTemp->PNext->student.name,
                   pTemp->PNext->student.gender, pTemp->PNext->student.age, pTemp->PNext->student.class,
                   pTemp->PNext->student.score);
        puts("- - - - - - - - - - - - - - - - - - - - - - - - - - -");
        break;

    }
}
if (which == 'c') {
    return pTemp->PNext;
}
   return pTemp;
}

//将整个链表写回本地
void writerStudents() {
fopen("./students.csv", "w");
    for (Node *pTemp = students->PNext; pTemp != NULL; pTemp = pTemp->PNext) {
        writerStudent(pTemp->student);
    }
}

//追加写入一个学生到本地
void writerStudent(Student student){
FILE *fp = fopen("./students.csv", "a");
    if (fp == NULL) {
        puts("学生表文件打开失败，请检查！");
        exit(1);
    } else {
        fputs(student.id, fp);
        fputc(',', fp);
        fputs(student.name, fp);
        fputc(',', fp);
        fputs(student.gender, fp);
        fputc(',', fp);
        char buffer[10];
        fputs(itoa(student.age, buffer, 10), fp);
        fputc(',', fp);
        fputs(student.class, fp);
        fputc(',', fp);
        fputs(gcvt(student.score, 5, buffer), fp);
        fputc('\n', fp);
        fflush(fp);
    }
    fclose(fp);
}

//对学生链表进行排序
void sort(char option[]) {
int opt = atoi(strtok(option, "."));
    for (Node *pStart = students->PNext; pStart != NULL; pStart = pStart->PNext) {
        Node *pMin = pStart;
        for (Node *pTemp = pStart->PNext; pTemp != NULL; pTemp = pTemp->PNext) {
            switch (opt) {
                default:
                    if (strcmp(pTemp->student.id, pMin->student.id) < 0) {
                        pMin = pTemp;
                    }
                    break;
                case 2:
                    if (pTemp->student.age < pMin->student.age) {
                        pMin = pTemp;
                    }
                    break;
                case 3:
                    if (pTemp->student.score < pMin->student.score) {
                        pMin = pTemp;
                    }
                    break;
            }
        }
        Student student = pMin->student;
        pMin->student = pStart->student;
        pStart->student = student;
    }

    // 如果需要降序，则反转链表
    if (atoi(strtok(NULL, ".")) == 2) {
        Node *pTemp = students->PNext;
        Node *pNew = NULL;
        students->PNext = NULL;

        while (pTemp != NULL) {
            //读取新一个节点后，之前的新节点后移
            pNew = pTemp;
            pTemp = pTemp->PNext;
            pNew->PNext = students->PNext;    //先把头节点后面的第一个节点挂到新节点后面
            students->PNext = pNew;           //再把新节点挂到头节点后面
        }
    }


}

//隐式输入密码
void getPassword(char password[]) {
  int i;
  char c;
while(1){
    c = getch();
    if(c == '\r') {
        break;
    }
    else if (c == '\b') { //遇到退格，需要删除一个星号
        printf("\b \b"); //退格，打一个空格，实质上用空格覆盖掉星号
        --i;
    }else {
        password[i++] = c;
        printf("*");
    }
}
printf("\n");
}

//根据学生信息和信息分隔符返回学生变量
Student getStudentByInfo(char *Info,char *delimiter) {
Student student;
    strcpy(student.id, strtok(Info, delimiter));
    strcpy(student.name, strtok(NULL, delimiter));
    strcpy(student.gender, strtok(NULL, delimiter));
    student.age = atoi(strtok(NULL, delimiter));
    strcpy(student.class, strtok(NULL, delimiter));
    student.score = atof(strtok(NULL, delimiter));
    return student;
}