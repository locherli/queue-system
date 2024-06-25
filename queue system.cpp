#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <time.h>
#include <fstream>
using namespace std;

void flushBuffer()
{
	char ch;
	while (ch = getchar() != '\n');
}

struct Person
{
	int reference = 0;
	int miss = 0;
};

class BankQueue
{
public:
	list<Person> queue;
	list<Person> priority;
	vector<Person> expired;
	vector<Person> record;
	Person ongoing;

	int index = 0;

	void showMenu()
	{
		cout << "********WELCOME*********" << endl;
		cout << "**  1.加入队列        **" << endl;
		cout << "**  2.显示飞票        **" << endl;
		cout << "**  3.叫号            **" << endl;
		cout << "**  4.查看业务情况    **" << endl;
		cout << "**  5.查看排队情况    **" << endl;
		cout << "**  6.查看业务记录    **" << endl;
		cout << "**  0.退出            **" << endl;
		cout << "************************" << endl;
	}

	void push()
	{
	re:
		Person newPerson;
		newPerson.reference = ++index;

		char choice;
		cout << "您有优先权（VIP或年长）吗? <y/n>" << endl;
		cin >> choice;
		flushBuffer();

		if (choice == 'y' || choice == 'Y')
		{
			priority.push_back(newPerson);

			//show ticket
			system("cls");
			cout << "您的序号是" << index << endl;
			cout << "您之前还有" << priority.size() - 1 << "人" << endl;

			//get the time
			time_t now;
			time(&now);
			tm* p = localtime(&now);
			cout << "您于" << p->tm_hour << ':' <<
				p->tm_min << ':' << p->tm_sec << "取号" << endl;
		}
		else if (choice == 'n' || choice == 'N')
		{
			queue.push_back(newPerson);

			//show ticket
			system("cls");
			cout << "您的序号是" << index << endl;
			cout << "您之前还有" << queue.size() + priority.size() - 1 << "人" << endl;

			//get the time
			time_t now;
			time(&now);
			tm* p = localtime(&now);
			cout << "您于" << p->tm_hour << ':' <<
				p->tm_min << ':' << p->tm_sec << "取号" << endl;
		}
		else
		{
			cout << "Invalid choice" << endl;
			system("pause");
			system("cls");
			goto re;
		}

		system("pause");

	}

	void isMiss()
	{
		cout << "是否应答？<y/n>" << endl;
		char ch = getchar();
		flushBuffer();

		if (ch == 'y' || ch == 'Y')
		{

			if (!priority.empty())
			{
				record.push_back(ongoing);
				ongoing = priority.front();
				priority.pop_front();
			}
			else
			{
				record.push_back(ongoing);
				ongoing = queue.front();
				queue.pop_front();
			}
		}
		else if (ch == 'n' || ch == 'N')
		{
			if (!priority.empty())
			{
				(priority.front()).miss++;

				if ((priority.front()).miss == 3) //no more than three!
				{
					expired.push_back(priority.front());
					priority.pop_back();
					cout << "飞号！" << endl;
				}
			}
			else
			{
				(queue.front()).miss++;

				if ((queue.front()).miss == 3) //no more than three!
				{
					expired.push_back(queue.front());
					priority.pop_back();
					cout << "飞号！" << endl;
				}
			}

		}
		else
		{
			cout << "invalid choice" << endl;
			system("pause");
			system("cls");
			isMiss();
		}
	}

	void callNext()
	{

		if (!priority.empty())  //call people who have priority first
		{
			cout << "请" << (priority.front()).reference << "号办理业务" << endl;
			this->isMiss();
		}
		else if (!queue.empty())     //normal person next
		{
			cout << "请" << (queue.front()).reference << "号办理业务" << endl;
			this->isMiss();
		}
		else
		{
			cout << "无人待号" << endl;
		}

		system("pause");
	}

	void showQueue() const
	{
		cout << "队列：";
		for (auto i : priority)
			cout << i.reference << ' ';
		for (auto j : queue)
			cout << j.reference << ' ';

		cout << endl;
		system("pause");
	}

	void Save()
	{
		ofstream ofs;
		ofs.open("file.scv", ios::out | ios::app);

		for (auto j : record)
		{
			ofs << j.miss << ',' << j.reference;
			ofs << endl;
		}
		ofs.close();
	}

	void showRecord()
	{
		cout << "记录：";
		for (auto i : record)
			cout << i.reference << ' ';
		cout << endl;
		system("pause");
	}

	void loadRecord()
	{
		ifstream ifs;
		ifs.open("file.scv", ios::in);

		if (!ifs.is_open())      //if don't exist
		{
			cout << "file don't exist." << endl;
			ifs.close();
			return;
		}

		char a;
		ifs >> a;
		if (ifs.eof())           //if empty
		{
			cout << "record is empty." << endl;
			ifs.close();
			return;
		}
		ifs.putback(a);

		string data;           //transfer the context
		int pos = -1;

		vector<Person> temp;

		while (getline(ifs, data))
		{
			Person p;
			pos = data.find(',');
			p.miss = stoi(data.substr(0, pos));
			p.reference = stoi(data.substr(pos+1, data.size()));

			temp.push_back(p);
		}

		record = temp;
	}

	void showProcess()
	{
		cout << ongoing.reference << "正在办理业务" << endl;
		system("pause");
	}

	void showExpired()
	{
		cout << "飞票名单：";
		for (auto i : expired)
			cout << i.reference << ' ';
		cout << endl;
		system("pause");
	}
};


int main()
{
	int choice;
	BankQueue bq;
	bq.loadRecord();

	while (true)
	{
		system("cls");
		bq.showMenu();
		cin >> choice;
		flushBuffer();
		switch (choice)
		{
		case 1:
			bq.push();
			break;
		case 2:
			bq.showExpired();
			break;
		case 3:
			bq.callNext();
			break;
		case 4:
			bq.showProcess();
			break;
		case 5:
			bq.showQueue();
			break;
		case 6:
			bq.showRecord();
			break;
		case 0:
			bq.Save();
			exit(0);
			break;

		default:
			cout << "invalid choice" << endl;
			system("pause");
			system("cls");
			main();
			break;
		}

	}

	return 0;
}