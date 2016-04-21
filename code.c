#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <string.h>
#include <time.h>
#include <string.h>
#define LOGIN_TIMEOUT 30
#define MAXBUFLEN 255
RETCODE error;
HENV henv = NULL;
HDBC conn = NULL;
HSTMT hstmt = NULL;
void DBTest();
void check_Book();
void borrow_Book();
void return_Book();
void add_Book();
void proof_Manage();
void borrow_query(HSTMT hstmt, char *query);
int card_query(HSTMT hstmt, char *query);
void New_Card_query(HSTMT hstmt, char *query);
void return_query(HSTMT hstmt, char *query);
void Single_add_query(HSTMT hstmt, char *query);
void book_query(HSTMT hstmt, char *query);
void now_Amount_query(HSTMT hstmt, char *query, int *now_Amount);
int New_Card_test_query(HSTMT hstmt, char *query);
void Delete_Card_query(HSTMT hstmt, char *query);
int main()
{
	DBTest();
	system("pause");
	return 0;
}
void DBTest()
{
	int choice;
	error = SQLAllocEnv(&henv);//���价��
	error = SQLAllocConnect(henv, &conn);//��������
	error = SQLConnect(conn, "HBdb", SQL_NTS, "SA", SQL_NTS, "whbY52015", SQL_NTS);
	printf("-----------------------------------\n");
	printf("\tѡ�������\n");
	while (1){
		printf("1.ͼ�� 2.���� 3.���� 4.ͼ����� 5.����֤���� 0.�˳�ϵͳ\n");
		printf("����������ţ�\n");
		scanf("%d", &choice);
		getchar();
		switch (choice)
		{
		case 0:
			SQLDisconnect(conn);
			SQLFreeConnect(conn);
			SQLFreeEnv(henv);
			return;
		case 1:
			check_Book();
			break;
		case 2:
			borrow_Book();
			break;
		case 3:
			return_Book();
			break;
		case 4:
			add_Book();
			break;
		case 5:
			proof_Manage();
			break;
		default:
			printf("�����Ŵ������������룡\n");
		}
		printf("\n");
	}
	MessageBox(NULL, TEXT("ִ�гɹ�"), TEXT("����"), MB_OK);
}
void check_Book()
{
	char query[100], buff[20], *cquery;
	//cquery means const string query
	int choice = 0;
	while (1){
		printf("1.��ѯȫ�� 2.��������ѯ 3.����Ų�ѯ 0.�˳�\n");
		printf("��������Ҫ�ķ����ţ�\n");
		scanf("%d", &choice);
		getchar();
		switch (choice)
		{
		case 0:
			return;
		case 1:
			error = SQLAllocStmt(conn, &hstmt);
			cquery = "select * from book";
			book_query(hstmt, cquery);
			break;
		case 2:
			error = SQLAllocStmt(conn, &hstmt);
			printf("����������\n");
			gets(buff);
			sprintf(query, "select * from book where title = '%s'", buff);
			//query="select * from book where ";
			book_query(hstmt, query);
			break;
		case 3:
			error = SQLAllocStmt(conn, &hstmt);
			printf("������ţ�\n");
			gets(buff);
			sprintf(query, "select * from book where bno = '%s'", buff);
			//query="select * from book where ";
			book_query(hstmt, query);
			break;
		default:
			printf("�����Ŵ���\n");
		}
		printf("\n");
	}
}
void borrow_Book()
{
	char query[200],cno[8],bno[11];
	int choice = 0;
	int now_Amount = 0;//�����ʼ��...
	printf("�������֤�ţ�\n");
	error = SQLAllocStmt(conn, &hstmt);
	gets(cno);
	sprintf(query, "select * from card where cno = %s", cno);
	if (card_query(hstmt, query) == 0)return;
	while (1){
		printf("1.��ѯ�ѽ��鼮 2.������Ž��� 0.�˳�\n");
		printf("��������Ҫ�ķ����ţ�\n");
		scanf("%d", &choice);
		getchar();
		switch (choice)
		{
		case 0:
			return;
		case 1:
			error = SQLAllocStmt(conn, &hstmt);
			sprintf(query, "select book.* from book INNER JOIN borrow on book.bno = borrow.bno where borrow.cno = %s ", cno);
			//ע�ⷵ�ظ�ʽ�Ͳ�ѯ�鼮��ͬ������δ�����ѹ黹ͼ��ļ�¼
			printf("�ѽ��鼮��\n");
			book_query(hstmt, query);
			break;
		case 2:
			error = SQLAllocStmt(conn, &hstmt);
			printf("��������Ž����鼮��\n");
			gets(bno);
			sprintf(query, "select stock from book where bno = %s", bno);
			now_Amount_query(hstmt, query, &now_Amount);
			if (now_Amount){
				sprintf(query, "update book set stock = stock - 1 where bno = %s insert into borrow values('%s','%s',DEFAULT,DEFAULT) ",bno,cno,bno);
				borrow_query(hstmt, query);
			}
			else printf("�����������ͼ�����޿�棡\n");
			break;
		default:
			printf("�����Ŵ���\n");
		}
		printf("\n");
	}
}
void return_Book()
{
	char query[200], cno[8], bno[11];
	int choice = 0;
	int now_Amount = 0;//�����ʼ��...
	printf("�������֤�ţ�\n");
	error = SQLAllocStmt(conn, &hstmt);
	gets(cno);
	sprintf(query, "select * from card where cno = %s", cno);
	if (card_query(hstmt, query) == 0)return;
	while (1){
		printf("1.��ѯ�ѽ��鼮 2.������Ż��� 0.�˳�\n");
		printf("��������Ҫ�ķ����ţ�\n");
		scanf("%d", &choice);
		getchar();
		switch (choice)
		{
		case 0:
			return;
		case 1:
			error = SQLAllocStmt(conn, &hstmt);
			sprintf(query, "select book.* from book INNER JOIN borrow on book.bno = borrow.bno where borrow.cno = %s ", cno);
			//ע�ⷵ�ظ�ʽ�Ͳ�ѯ�鼮��ͬ������δ�����ѹ黹ͼ��ļ�¼
			printf("�ѽ��鼮��\n");
			book_query(hstmt, query);
			break;
		case 2:
			error = SQLAllocStmt(conn, &hstmt);
			printf("��������Ź黹�鼮��\n");
			gets(bno);
			sprintf(query, "update book set stock = stock + 1 where bno = %s \
						    delete from borrow where cno=%s and bno=%s ", bno, cno, bno);
			return_query(hstmt, query);
			break;
		default:
			printf("�����Ŵ���\n");
		}
		printf("\n");
	}
}
void add_Book()
{
	char query[300];//�˴�buff��øĳ�bno
	int choice = 0;
	char book_ID[11], book_category[11], book_name[21], press[21], writer[11],cno[8];
	int year = 0;
	float price = 0.0;
	int num = 0,i=0;
	printf("�������Ա���ţ�\n");
	error = SQLAllocStmt(conn, &hstmt);
	gets(cno);
	sprintf(query, "select * from card where cno = %s and type='M'", cno);
	if (card_query(hstmt, query) == 0)return;
	while (1){
		printf("1.������� 2.������� 0.�˳�\n");
		printf("��������Ҫ�ķ����ţ�\n");
		scanf("%d", &choice);
		getchar();
		switch (choice)
		{
		case 0:
			return;
		case 1:
			error = SQLAllocStmt(conn, &hstmt);
			printf("���룺���\t���\t����\t������\t���\t����\t�۸񣺣����ϸ��ո�ʽ���룩\n");
			scanf("%s", book_ID);
			scanf("%s", book_category);
			scanf("%s", book_name);
			scanf("%s", press);
			scanf("%d",&year);
			scanf("%s", writer);
			scanf("%f",&price);
			sprintf(query, "if((select count(bno) from book where bno=%s)=0)insert into book values('%s','%s','%s','%s',%d,'%s',%f,1,1) else update book set total = total + 1, stock = stock + 1 where bno = %s ", book_ID, book_ID, book_category, book_name, press, year, writer, price, book_ID);
			Single_add_query(hstmt, query);
			break;
		case 2:
			printf("�������ͼ��������\n");
			scanf("%d", &num);
			printf("���룺���\t���\t����\t������\t���\t����\t�۸�\t�������ϸ��ո�ʽ���룩\n");
			scanf("%s", book_ID);
			scanf("%s", book_category);
			scanf("%s", book_name);
			scanf("%s", press);
			scanf("%d", &year);
			scanf("%s", writer);
			scanf("%f", &price);
			error = SQLAllocStmt(conn, &hstmt);
			sprintf(query, "if((select count(bno) from book where bno=%s)=0)insert into book values('%s','%s','%s','%s',%d,'%s',%f,%d,%d) else update book set total = total + %d, stock = stock + %d where bno = %s ", book_ID, book_ID, book_category, book_name, press, year, writer, price,num,num,num,num, book_ID);
			Single_add_query(hstmt, query);
			break;
		default:
			printf("�����Ŵ���\n");
		}
		printf("\n");
	}
}
void proof_Manage()
{
	char query[200];
	//cquery means const string query
	int choice = 0;
	char cno[8], name[11], department[41], type[2],password[16];
	printf("�������Ա���ţ�\n");
	error = SQLAllocStmt(conn, &hstmt);
	gets(cno);
	sprintf(query, "select * from card where cno = %s and type='M'", cno);
	if (card_query(hstmt, query) == 0)return;
	while (1){
		printf("1.�½����鿨 2.ɾ�����鿨 0.�˳�\n");
		printf("��������Ҫ�ķ����ţ�\n");
		scanf("%d", &choice);
		getchar();
		switch (choice)
		{
		case 0:
			return;
		case 1:
			error = SQLAllocStmt(conn, &hstmt);
			printf("����ѧ�Ż򹤺�\t����\tרҵ\t����\t���룺\n");
			//�򻯣����ö�̬���俨��
			scanf("%s", cno);
			scanf("%s", name);
			scanf("%s", department);
			scanf("%s", type);
			scanf("%s", password);
			sprintf(query, "select count(cno) from card where cno=%s",cno);
			if (New_Card_test_query(hstmt, query) != 0){
				printf("���鿨�Ѵ��ڣ�\n");
				break;
			}
			//�����Ƿ����ж�Ӧ�Ľ��鿨
			sprintf(query, "INSERT INTO card values('%s','%s','%s','%s','%s')",cno, name, department, type, password);
			New_Card_query(hstmt, query);
			//û������ɵ�query���룬��Ҫ����д
			break;
		case 2:
			error = SQLAllocStmt(conn, &hstmt);
			printf("���뿨����֤��\n");
			gets(cno);
			sprintf(query, "select * from card where cno = %s", cno);
			if (card_query(hstmt, query) == 0)break;
			sprintf(query, "delete from card where cno = %s", cno);
			Delete_Card_query(hstmt, query);
			break;
		default:
			printf("�����Ŵ���\n");
		}
		printf("\n");
	}
}

void book_query(HSTMT hstmt, char *query)
{
	char book_ID[11], book_category[11], book_name[21], press[21], writer[11];
	int year = 0;
	float price = 0.0;
	int total_Amount = 0, now_Amount = 0;
	SQLINTEGER lot[9];
	error = SQLExecDirect(hstmt, query, SQL_NTS);
	if (error == SQL_SUCCESS){
		SQLBindCol(hstmt, 1, SQL_C_CHAR, book_ID, 10, &lot[0]);
		SQLBindCol(hstmt, 2, SQL_C_CHAR, book_category, 10, &lot[1]);
		SQLBindCol(hstmt, 3, SQL_C_CHAR, book_name, 20, &lot[2]);
		SQLBindCol(hstmt, 4, SQL_C_CHAR, press, 20, &lot[3]);
		SQLBindCol(hstmt, 5, SQL_C_SHORT, &year, 0, &lot[4]);
		SQLBindCol(hstmt, 6, SQL_C_CHAR, writer, 10, &lot[5]);
		SQLBindCol(hstmt, 7, SQL_C_FLOAT, &price, 0, &lot[6]);
		SQLBindCol(hstmt, 8, SQL_C_SHORT, &total_Amount, 0, &lot[7]);
		SQLBindCol(hstmt, 9, SQL_C_SHORT, &now_Amount, 0, &lot[8]);
		printf("��ѯ�����\n");
		printf("----------------------------\n");
		printf("���\t���\t����\t������\t���\t����\t�۸�\t�ܲ�����\t���\n");
		printf("----------------------------\n");
		while (SQLFetch(hstmt) != SQL_NO_DATA_FOUND)
		{
			printf("%s\t%s\t%s\t%s\t%d\t%s\t%.2f\t%d\t%d\n", \
				book_ID, book_category, book_name, press, year, writer, price, \
				total_Amount, now_Amount);
		}
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
}

int card_query(HSTMT hstmt, char *query)
{
	int suc = 0;
	//�����ж�card�Ƿ����
	char Card_ID[8], name[11], department[41], type[2],password[16];
	char input_password[16];
	SQLINTEGER lot[5];
	error = SQLExecDirect(hstmt, query, SQL_NTS);
	if (error == SQL_SUCCESS){
		SQLBindCol(hstmt, 1, SQL_C_CHAR, Card_ID, 8, &lot[0]);
		SQLBindCol(hstmt, 2, SQL_C_CHAR, name, 11, &lot[1]);
		SQLBindCol(hstmt, 3, SQL_C_CHAR, department, 41, &lot[2]);
		SQLBindCol(hstmt, 4, SQL_C_CHAR, type, 2, &lot[3]);
		SQLBindCol(hstmt, 5, SQL_C_CHAR, password, 16, &lot[4]);
		if (SQLFetch(hstmt) == SQL_NO_DATA_FOUND){
			printf("���������������ʹ���\n");
			return 0;
		}
		printf("�������룺\n");
		gets(input_password);
		if (strcmp(input_password, password)){
			SQLFreeStmt(hstmt, SQL_CLOSE);
			printf("�������\n\n");
			return 0;
		}
		printf("��֤�ɹ���\n");
		printf("���鿨��Ϣ��\n");
		printf("----------------------------\n");
		printf("����\t����\tרҵ\t����\n");
		printf("----------------------------\n");
		printf("%s\t%s\t%s\t%s\t\n", Card_ID, name, department, type);
		suc = 1;
	}
	else printf("ϵͳ����\n");
	SQLFreeStmt(hstmt, SQL_CLOSE);
	printf("\n");
	return suc;
}

void now_Amount_query(HSTMT hstmt, char *query, int *now_Amount)
{
	SQLINTEGER length;
	error = SQLExecDirect(hstmt, query, SQL_NTS);
	if (error == SQL_SUCCESS){
		SQLBindCol(hstmt, 1, SQL_C_SHORT, now_Amount, 0, &length);
		SQLFetch(hstmt);
	}
	else printf("�鼮�����ڣ�\n");
	SQLFreeStmt(hstmt, SQL_CLOSE);
}

void borrow_query(HSTMT hstmt, char *query)
{
	error = SQLExecDirect(hstmt, query, SQL_NTS);
	if (error == SQL_SUCCESS){
		printf("����ɹ���\n");
	}
	else printf("ϵͳ���ϣ�����ϵ����Ա��\n");
	SQLFreeStmt(hstmt, SQL_CLOSE);
}

void return_query(HSTMT hstmt, char *query)
{
	error = SQLExecDirect(hstmt, query, SQL_NTS);
	if (error == SQL_SUCCESS){
		printf("�黹�ɹ���\n");
	}
	else printf("δ�����鼮����ȷ���鼮Ϊ�ѽ����鼮��\n");
	SQLFreeStmt(hstmt, SQL_CLOSE);
}

void Single_add_query(HSTMT hstmt, char *query)
{
	error = SQLExecDirect(hstmt, query, SQL_NTS);
	if (error == SQL_SUCCESS){
		printf("ͼ�����ɹ���\n");
	}
	else printf("�����ʽ����\n");
	SQLFreeStmt(hstmt, SQL_CLOSE);
}

void New_Card_query(HSTMT hstmt, char *query)
{
	error = SQLExecDirect(hstmt, query, SQL_NTS);
	if (error == SQL_SUCCESS){
		printf("���鿨�½��ɹ���\n");
	}
	else printf("ϵͳ���ϣ�\n");
	SQLFreeStmt(hstmt, SQL_CLOSE);
}

int New_Card_test_query(HSTMT hstmt, char *query)
{
	SQLINTEGER length;
	int test = 0;
	error = SQLExecDirect(hstmt, query, SQL_NTS);
	if (error == SQL_SUCCESS){
		SQLBindCol(hstmt, 1, SQL_C_SHORT, &test, 0, &length);
	}
	else printf("�����ʽ����\n");
	SQLFreeStmt(hstmt, SQL_CLOSE);
	return test;
}

void Delete_Card_query(HSTMT hstmt, char *query)
{
	error = SQLExecDirect(hstmt, query, SQL_NTS);
	if (error == SQL_SUCCESS){
		printf("���鿨ɾ���ɹ���\n");
	}
	else printf("ϵͳ���ϣ�\n");
	SQLFreeStmt(hstmt, SQL_CLOSE);
}