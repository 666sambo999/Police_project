﻿#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<map>
#include<list>
#include<Windows.h>
#include<conio.h>

using namespace std;
using std::cout;
using std::endl;

#define tab "\t"
#define delimeter "\n--------------------------------------\n"

std::map<int, std::string> violation =
{
	{0,"Другие правонарушения"},
	{1,"Превышения скоростного режима"},
	{2,"Езда в нетрезвом состоянии"},
	{3,"Проезд на красный сигнал"},
	{4,"Пересечение спрошной линии"},
	{5,"Проезд в запрещенном месте"},
	{6,"Парковка в неположенном месте"},
	{7,"Ремень безопастности"},
	{8,"Отсутствие документов и страховки"},
	{9,"Опасная езда на авто"},
	{10,"Оскорбление полицейского"},
	{11,"Дрифт на перекрестке"},

};

#define CRIME_TAKE_PARAMETRS int year,int month,int day,int hour, int minute
#define CRIME_GIVE_PARAMETRS  year, month, day, hour, minute

class Crime
{
	//std::string licence_plate;	// номерной знак 
	int id;						// правонарушения 
	tm datetime;				// время правонарушения 
	std::string place;				// место нарушения 
public:
	/*const std::string& get_licence_plate()const
	{
		return licence_plate;
	}*/
	int get_id()const
	{
		return id;
	}
	const char* get_time()const
	{
		char* datetime = asctime(&this->datetime);
		datetime[strlen(datetime) - 1] = 0;
		return datetime;
	}
	time_t get_timestamp()const
	{
		tm datetime = this->datetime;
		return mktime(&datetime);
	}
	const std::string& get_place()const
	{
		return place;
	}
	/*void set_licence_plate(const std::string& licence_plate)
	{
		this->licence_plate = licence_plate;
	}*/
	void set_id(int id)
	{
		/*if (violation.find(id) != violation.end())
			this->id = id;
		else this->id = 0;*/
		this->id = violation.find(id) == violation.end() ? 0 : id;
	}
	void set_timestamp(time_t time)
	{
		datetime = *localtime(&time);
	}
	tm* init_datetime()//  преобразуем в timestamp
	{
		const time_t datetime = mktime(&this->datetime);
		return localtime(&datetime);
	}
	void set_datetime(int year, int month, int day, int hour, int minute)
	{
		this->datetime = tm{};
		datetime.tm_year = year - 1900;
		datetime.tm_mon = month - 1;
		datetime.tm_mday = day;
		datetime.tm_hour = hour;
		datetime.tm_min = minute;
		this->datetime = *init_datetime();
	}
	// Перегрузка datetime
	void set_datetime(const std::string& datetime)// принимаем строку дату и время 
	{
		this->datetime = tm{};
		const int SIZE = 32;
		char buffer[SIZE]{};
		strcpy(buffer, datetime.c_str());
		// метод c_str()const, который возврящает указатель на RAW - строку, 
		// которую обварачивает объект класса std::string
		int part[5] = {};
		const char delimeters[] = "./:";
		int n = 0;
		for (char* pch = strtok(buffer, delimeters); pch; pch = strtok(NULL, delimeters))
			part[n++] = std::stoi(pch);
		this->datetime.tm_year = part[0] - 1900;
		this->datetime.tm_mon = part[1] - 1;
		this->datetime.tm_mday = part[2];
		this->datetime.tm_hour = part[3];
		this->datetime.tm_min = part[4];
		this->datetime = *init_datetime();
		
	}
	void set_place(const std::string& place)
	{
		this->place = place;
	}
	// конструкторы 
	//Crime(const std::string& licence_plate, int id, const std::string& datatime, const std::string& place)
	Crime(int id, const std::string& datetime, const std::string& place)
	{
		//set_licence_plate(licence_plate);
		set_id(id);
		set_datetime(datetime);
		set_place(place);
	}
	~Crime() {}
};
std::ostream& operator <<(std::ostream& os, const Crime& obj)
{
	//return os << obj.get_licence_plate() << " " << obj.get_id() << " " << obj.get_time() << " " << obj.get_place();
	os << obj.get_time() << " | ";
	os.width(25);
	os << std::left;
	os << obj.get_place() << " | ";
	os << "\t" << violation.at(obj.get_id());
	return os;
}
std::ofstream& operator <<(std::ofstream& ofs, const Crime& obj)
{
	ofs << obj.get_timestamp() << " "; 
	ofs << obj.get_id() << " ";
	ofs << obj.get_place(); 
	return ofs;
}

std::istream& operator>>(std::istream& is, Crime& obj)
{
	int id;
	std::string date;
	std::string time;
	std::string place;
	cout << "Введите нарушение: "; is >> id;
	is.ignore();
	cout << R"(
		0 - Ввести время вручную;\n
		1 - Использовать текущее время;
	)";
	bool current_time;
	cin >> current_time;
	if (current_time)
	{
		obj.set_timestamp(std::time(NULL));
		is.ignore();
	}
	else
	{
		std::getline(is, date, ' ');
		std::getline(is, time, ' ');
		obj.set_datetime(date + " " + time);
	}
	cout << "Введите место проишествия: ";
	SetConsoleCP(1251);
	std::getline(is, place);
	SetConsoleCP(866);
	obj.set_id(id);
	obj.set_place(place);
	return is;
}

std::ifstream& operator >> (std::ifstream& ifs, Crime& obj)
{
	int id; 
	time_t timestamp;
	std::string place;
	ifs >> id >> timestamp;
	std::getline(ifs, place, ',');
	obj.set_id(id);
	obj.set_timestamp(timestamp);
	return ifs; 
}

void print(const std::map <std::string, std::list<Crime>>& base);
void save(const std::map <std::string, std::list<Crime>>& base, const std::string& filename);
void load(std::map <std::string, std::list<Crime>>& base, const std::string& filename);


void main()
{
	setlocale(LC_ALL, "");

	/*Crime crime("a777аa196", 1, "2023.06.29 20.21", "ул. Ленина");
	cout << crime << endl;*/
	
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleDisplayMode(hConsole, CONSOLE_FULLSCREEN_MODE, NULL);

	setlocale(LC_ALL, "");

	/*Crime crime("a777àa196", 1, "2023.06.29 20.21", "óë. Ëåíèíà");
	cout << crime << endl;*/

	std::map<std::string, std::list<Crime>> base =
	{
		{
			"a666aa196", std::list<Crime>
			{
				Crime(1,"2023.06.29 21.00", "ул. Ленина"),
				Crime(4,"2023.06.29 21.00", "ул. Ленина"),
				Crime(9,"2023.06.29 21.00", "ул. Ленина")}
		},
		{
			"x111ам196", std::list<Crime>
			{
				Crime(5,"2023.06.21 12.00", "ул. Коммуны"),
				Crime(7,"2023.06.12 19.00", "ул. Красноармейская")
			}
		},
		{
			"а222сс96", std::list<Crime>
			{
				Crime(2,"2023.06.22 16.00", "ул. Луговая"),
				Crime(11,"2023.06.26 23.00", "ул. Дачная"),
			}
		},
		{
			"с666су96", std::list<Crime>
			{
				Crime(3,"2023.06.23 18.00", "ул. Декабристов"),
				Crime(5,"2023.06.26 09.10", "ул. Декабристов"),
			}
		},
		{
			"е777хк174", std::list<Crime>
			{
				Crime(10,"2023.06.27 03.25", "ул.Ворошилова"),
				Crime(8,"2023.06.26 03.30", "ул.Московская"),
			}
		},
	};

	char key;
	do
	{
		system("CLS");
		cout << "1. Распечатка базы данных;" << endl; 
		cout << "2. Распечатка базы по заданному номеру;" << endl; 
		cout << "3. Распечатка базы по диапозону номеру;" << endl; 
		cout << "4. Сохранение базы в файл;" << endl; 
		cout << "5. Загрузка базы из файла;" << endl; 
		cout << "6. Добавление записи в базу;" << endl; 
		key = _getch();
		switch (key)
		{
		case '1': print(base);	break;
		case '4': save(base, "base.txt"); break;
		case '6': 
			for (std::pair<int, std::string> i : violation)cout << "\t" << i.first << "\t" << i.second << endl;
			std::string licence_plate;
			Crime crime(0,"2010.01.01 00:00", "Somewere");
			cout << "Введите номер автомобиля: "; cin >> licence_plate;
			cout << "Введие правонарушения: "; cin >> crime;
			base[licence_plate].push_back(crime);
		}
	} while (key!=27&& key !=0);
	
}

void print(const std::map <std::string, std::list<Crime>>& base)
{
	for (std::map<std::string, std::list<Crime>>::const_iterator it = base.begin(); it != base.end(); ++it)
	{
		cout.width(8);
		cout << it->first << ":\n";
		for (std::list<Crime>::const_iterator l_it = it->second.begin(); l_it != it->second.end(); ++l_it)
		{
			cout << " \t|" << *l_it << endl;
		}
		cout << delimeter << endl;
	}
	system("PAUSE");
}

void save(const std::map <std::string, std::list<Crime>>& base, const std::string& filename)
{
	std::ofstream fout(filename);
	for (std::map<std::string, std::list<Crime>>::const_iterator it = base.begin(); it != base.end(); ++it)
	{
		fout << it->first << ":\t";
		for (std::list<Crime>::const_iterator l_it = it->second.begin(); l_it != it->second.end(); ++l_it)
		{
			fout << *l_it << ", ";
		}
		fout << endl; 
	}
	fout.close();
	std::string command = "start notepad " + filename;
	system(command.c_str());
}

void load(std::map <std::string, std::list<Crime>>& base, const std::string& filename)
{
	std::ifstream fin(filename);
	if (fin.is_open())
	{
		// Todo: read to file
		while (!fin.eof())
		{

		}
		fin.close();
	}
	else
	{
		std::cerr << "Error: file not found" << endl; 
	}
}