#define _CRT_SECURE_NO_WARNINGS 1

#if 0
#include <iostream>
#include <mysql.h>
#include <ctime>
#include <string>
#include <vector>
using namespace std;


int main()
{
	MYSQL mysql;    //һ�����ݿ�ṹ��
	MYSQL_RES* res; //һ��������ṹ��
	MYSQL_ROW row;  //char** ��ά���飬���һ������¼
	//��ʼ�����ݿ�
	mysql_init(&mysql);
	//���ñ��뷽ʽ
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	//�������ݿ�
	//�ж��������ʧ�ܾ��������ʧ�ܡ�
	//ע�������ӵ��˻�������
	if (mysql_real_connect(&mysql, "localhost", "root", "123456", "plate", 3306, NULL, 0) == NULL)
	{
		cout << "����ʧ�ܣ�" << endl;
	}
	//ע�������ӵ��˻�������

//��������
	

	string s2 = "��A77777";


    char que[200];
	//sprintf()��������sql���
    sprintf(que, "insert into  plateinfor(ReadTime,Name) values(now(),'%s');",s2);
    int ret=mysql_query(&mysql, que);
    if (ret)
    {
        cout << "��������ʧ��" << endl;
    }
    else
    {
	   cout << "�������ݳɹ�,�����룺" << mysql_affected_rows(&mysql) << "��" << endl;
    }
    


//��ѯ����
	mysql_query(&mysql, "select * from plateinfor");
	//��ȡ�����
	res = mysql_store_result(&mysql);
	//��ʾ����

	cout << "ReadTime||Name" << endl;
	//��ROW��ֵ���ж�ROW�Ƿ�Ϊ�գ���Ϊ�վʹ�ӡ���ݡ�
	while (row = mysql_fetch_row(res))
	{
		cout<<row[0]<<"  ";//��ӡ���ƶ�ȡʱ��
		cout<< row[1];//��ӡ������
		cout << endl;
	}
	//�ͷŽ����
	mysql_free_result(res);
	//�ر����ݿ�
	mysql_close(&mysql);
	//ͣ���ȴ�
	system("pause");
	return 0;
}

#endif