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
	MYSQL mysql;    //一个数据库结构体
	MYSQL_RES* res; //一个结果集结构体
	MYSQL_ROW row;  //char** 二维数组，存放一条条记录
	//初始化数据库
	mysql_init(&mysql);
	//设置编码方式
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	//连接数据库
	//判断如果连接失败就输出连接失败。
	//注意你连接的账户名密码
	if (mysql_real_connect(&mysql, "localhost", "root", "123456", "plate", 3306, NULL, 0) == NULL)
	{
		cout << "连接失败！" << endl;
	}
	//注意你连接的账户名密码

//插入数据
	

	string s2 = "川A77777";


    char que[200];
	//sprintf()函数构造sql语句
    sprintf(que, "insert into  plateinfor(ReadTime,Name) values(now(),'%s');",s2);
    int ret=mysql_query(&mysql, que);
    if (ret)
    {
        cout << "插入数据失败" << endl;
    }
    else
    {
	   cout << "插入数据成功,共插入：" << mysql_affected_rows(&mysql) << "行" << endl;
    }
    


//查询数据
	mysql_query(&mysql, "select * from plateinfor");
	//获取结果集
	res = mysql_store_result(&mysql);
	//显示数据

	cout << "ReadTime||Name" << endl;
	//给ROW赋值，判断ROW是否为空，不为空就打印数据。
	while (row = mysql_fetch_row(res))
	{
		cout<<row[0]<<"  ";//打印车牌读取时间
		cout<< row[1];//打印车排名
		cout << endl;
	}
	//释放结果集
	mysql_free_result(res);
	//关闭数据库
	mysql_close(&mysql);
	//停留等待
	system("pause");
	return 0;
}

#endif