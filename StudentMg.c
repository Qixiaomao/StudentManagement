#include <stdio.h>
#include <stdbool.h>  //bool,true,false����ͷ�ļ�
#include <string.h>  
#include <stdlib.h>  //system,atoi,itoa��������ͷ�ļ�
#include <conio.h> //getch��������ͷ�ļ�

#define AUTHORIZECODE "ProgrammerIsSoEasy"  //��Ȩ��

//ѧ���ṹ��
typedef struct student {
    char id[5];
    char name[30];
    char gender[4];
    int age;
    char class[20];
    double score;
} Student;

//����ڵ�ṹ��
typedef struct node {
    Student student;
    struct node *PNext;
}Node;

//��������
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

bool signInstatus = false; // ��¼�ɹ���ǣ�Ĭ��δ��¼
Node *students;  //ѧ������ָ��
Node *pTail;  // ѧ������βָ��

int main(){

    //��¼
    while(!signInstatus)
        signIn(NULL,NULL);

    //��ȡ����ѧ��������
    students = createLinkedList();

    //���˵�
    while(true)
    {
      fflush(stdin);
      system("cls");
      puts(" ����*********************���� ");
      puts(" ��      ѧ������ϵͳ      �� ");
      puts(" �ǩ�*********************���� ");
      puts(" ��      1. �鿴ѧ��      �� ");
      puts(" ��      2. ����ѧ��      �� ");
      puts(" ��      3. ����ѧ��      �� ");
      puts(" ��      4. �޸�ѧ��      �� ");
      puts(" ��      5. ɾ��ѧ��      �� ");
      puts(" ��      6. �˳�ϵͳ      �� ");
      puts(" ����*********************����");
      printf("��ѡ��\n");
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
           printf("�������");
      }
     system("pause");
    }

}

//��¼
void signIn(char *username,char *password)
{
    if(username == NULL && password == NULL)
    {
        username = (char *) malloc(sizeof(char) * 20);
        password = (char *) malloc(sizeof(char) * 20);
        puts("============== ��¼ ==================");
        printf("�û�����");
        gets(username);
        printf("���룺");
        getPassword(password);
    }

    //���û��б�root
    FILE *fp = fopen("./users.csv","r");
    if(fp == NULL)
    {
        puts("�û��ļ���ʧ�ܣ����飡");
        exit(1);
    }else {
        char user[50];
        bool isNotExist = true;
        //�򿪳ɹ����ȡÿһ�У�һ�������һ���û���username,password
        while(fgets(user,50,fp) != NULL){
            unsigned int tail = strlen(user)-1;
            //����ĩ�Ļس���ɾ��
            if(user[tail] == '\n') {
                user[tail] = '\0';
            }
            //��ȡ�û��������룬����������û����������Ƚ�
            if(strcmp(username,strtok(user,",")) == 0) {
                isNotExist = false;
                if(strcmp(password,strtok(NULL,",")) == 0) {
                    //����û�����������ȷ����¼�ɹ�
                    signInstatus = true;
                    printf("��¼�ɹ�");
                    system("pause");
                } else {
                    puts("�������");
                }
            }
        }
        //�������ļ�����δ�����û�������ʾע��
        if(isNotExist) {
            puts("û�д��û������س����������룬��������ע��");
            if(getch() != '\r') {
                signUp(username,password);
            }

        }
    }
    fclose(fp);
}

//ע�Ṧ��
void signUp(char *username,char *password) {
    //���û��б�
    FILE *fp = fopen("./user.csv" , " a");
    if(fp == NULL)
    {
        puts("�û��ļ���ʧ�ܣ����飡");
        exit(1);
    } else {
        printf("��ȷ�����룺");
        char repeat[20];
        getPassword(repeat);
        if(strcmp(password,repeat) == 0) {
            printf("��������Ȩ�룺");
            char code[50];
            gets(code);
            //��������һ������Ȩ����ȷ����д��ע���û���Ϣ
            if(strcmp(code,AUTHORIZECODE)==0)
            {
                fputs(username,fp);
                putc(',',fp);
                fputs(password,fp);
                fputc('\n',fp);
                fflush(fp);
                signInstatus = true;
                printf("ע��ɹ������Զ���¼");
                system("pause");
            }else {
                puts("�������벻һ��");
            }
        }
        
    }
    fclose(fp);
}

//����ѧ������
Node *createLinkedList() {
 Node *pHead = (Node *)malloc(sizeof(Node));
 pTail = pHead;

 //���û��б�
 FILE *fp = fopen("./students.csv","r");
 if(fp == NULL) {
    puts("ѧ���ļ���ʧ�ܣ����飡");
    exit(1);
 } else {
    char info[256];
    //�ɹ���ȡÿһ�У�һ�������һ��ѧ����id,name,gender,age,class,score
    while(fgets(info,256,fp) != NULL) {
        unsigned int tail = strlen(info) -1;
        //����ĩ�ûس���ɾ��
        if(info[tail] == '\n'){
            info[tail] = '\0';
        }
        //�ҽڵ�
        Node *pNode = (Node *)malloc(sizeof(Node));
        pNode->student = getStudentByInfo(info,",");
        pTail->PNext = pNode;
        pTail = pTail->PNext;
        pTail->PNext = NULL;
    }
 }

   return pHead;
}



//չʾ
void show(){
  puts("1.1 ѧ������\t 1.2 ѧ�Ž���");
  puts("2.1 ��������\t 2.2 ���併��");
  puts("3.1 ��������\t 3.2 ��������");
  printf("Ĭ����ѧ���������У���ѡ��");
  char option[4];
  fflush(stdin);
  sort(gets(option));
  puts("\n========================ѧ����====================\n");
 puts("- - - - - - - - - - - - - - - - - - - - - - - - - - -");
 puts("|ѧ��\t| ����\t| �Ա�\t| ����\t| �༶\t\t| ����\t| ");
 puts("- - - - - - - - - - - - - - - - - - - - - - - - - - -");

 // �������
    for (Node *pTemp = students->PNext; pTemp != NULL; pTemp = pTemp->PNext) {
        printf("|%s\t| %s\t| %s\t| %d\t| %s\t| %.1lf\t|\n", pTemp->student.id, pTemp->student.name,
               pTemp->student.gender, pTemp->student.age, pTemp->student.class, pTemp->student.score);
        puts("- - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
    }
}


//����
void find(){
    printf("������ؼ��֣�ѧ�š��������༶����");
    char keyword[20];
    fflush(stdin);
    gets(keyword);
    puts("\n============ ѧ���� ================\n");
    puts("- - - - - - - - - - - - - - - - - - - -");
    puts("|ѧ��\t| ����\t| �Ա�\t| ����\t| �༶\t\t| ����\t| ");
    puts("- - - - - - - - - - - - - - - - - - - -");
   for (Node *pTemp = students->PNext; pTemp != NULL; pTemp = pTemp->PNext) {
        if (strcmp(keyword, pTemp->student.id) == 0 || strcmp(keyword, pTemp->student.name) == 0 || strstr(pTemp->student.class, keyword) != NULL) {
            printf("|%s\t| %s\t| %s\t| %d\t| %s\t| %.1lf\t|\n", pTemp->student.id, pTemp->student.name,
                   pTemp->student.gender, pTemp->student.age, pTemp->student.class, pTemp->student.score);
            puts("- - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
        }
    }

}



//��ӹ���
void add(){
  puts("- - - - - - - - - - - - - - - - - - - - - - - - - -");
    puts("ѧ��\t����\t�Ա�\t����\t�༶\t\t����");
    puts("- - - - - - - - - - - - - - - - - - - - - - - - - -");
    char info[256];
    while (true) {
        fflush(stdin);
        gets(info);     // ��ȡѧ����Ϣ
        if (strcmp(info, "ok") == 0) {      // ������ok��������
            break;
        } else {
            Node *pNode = (Node *) malloc(sizeof(Node));
            Student student = getStudentByInfo(info, "\t");
            bool isNotExist = true;
            // ����������ѯ�Ƿ�ѧ���Ѿ�����
            for (Node *pTemp = students->PNext; pTemp != NULL; pTemp = pTemp->PNext) {
                if (strcmp(student.id, pTemp->student.id) == 0) {
                    isNotExist = false;
                    printf("�Ѵ���%s��ѧ�����������������\n", student.id);
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

//ɾ��
void delete() {
    Node *pNode = findNode('p');
    puts("���س�ȷ��ɾ����������ȡ����");
    if (getch() == '\r') {
        Node *pTemp = pNode->PNext;
        pNode->PNext = pTemp->PNext;
        free(pTemp);
        writerStudents();
        printf("ɾ���ɹ�!");
    } else {
        printf("��ȡ����");
    }
}

//�޸�

void update() {
  Node *pNode = findNode('c');
  puts("�������µ�ѧ����Ϣ��");
  fflush(stdin);
  char info[256];
  gets(info);
  pNode->student = getStudentByInfo(info,"\t");
  writerStudents();
  printf("�޸����!");

}





//������'c'����ƥ��õ�ǰ�ڵ㣬��������������ǰ���ڵ�
Node *findNode(char which){
printf("������ѧ����ѧ�Ż�������");
char keyword[20];
fflush(stdin);
gets(keyword);
puts("- - - - - - - - - - - - - - - - ");
puts("ѧ��\t����\t�Ա�\t����\t�༶\t\t����\t");
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

//����������д�ر���
void writerStudents() {
fopen("./students.csv", "w");
    for (Node *pTemp = students->PNext; pTemp != NULL; pTemp = pTemp->PNext) {
        writerStudent(pTemp->student);
    }
}

//׷��д��һ��ѧ��������
void writerStudent(Student student){
FILE *fp = fopen("./students.csv", "a");
    if (fp == NULL) {
        puts("ѧ�����ļ���ʧ�ܣ����飡");
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

//��ѧ�������������
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

    // �����Ҫ������ת����
    if (atoi(strtok(NULL, ".")) == 2) {
        Node *pTemp = students->PNext;
        Node *pNew = NULL;
        students->PNext = NULL;

        while (pTemp != NULL) {
            //��ȡ��һ���ڵ��֮ǰ���½ڵ����
            pNew = pTemp;
            pTemp = pTemp->PNext;
            pNew->PNext = students->PNext;    //�Ȱ�ͷ�ڵ����ĵ�һ���ڵ�ҵ��½ڵ����
            students->PNext = pNew;           //�ٰ��½ڵ�ҵ�ͷ�ڵ����
        }
    }


}

//��ʽ��������
void getPassword(char password[]) {
  int i;
  char c;
while(1){
    c = getch();
    if(c == '\r') {
        break;
    }
    else if (c == '\b') { //�����˸���Ҫɾ��һ���Ǻ�
        printf("\b \b"); //�˸񣬴�һ���ո�ʵ�����ÿո񸲸ǵ��Ǻ�
        --i;
    }else {
        password[i++] = c;
        printf("*");
    }
}
printf("\n");
}

//����ѧ����Ϣ����Ϣ�ָ�������ѧ������
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