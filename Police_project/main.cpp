#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<ctime>
#include<map>
#include<list>
#include<Windows.h>

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
	{10,"Оскрбление полицейского"},
	{11,"Дрифт на перекрестке"},

};

#define CRIME_TAKE_PARAMETRS int year,int month,int day,int hour, int minute
#define CRIME_GIVE_PARAMETRS  year, month, day, hour, minute

class Crime
{
	//std::string licence_plate;	// номерной знак 
	int id;						// правонарушения 
	tm datatime;				// время правонарушения 
	string place;				// место нарушения 
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
		char* datatime = asctime(&this->datatime);
		datatime[strlen(datatime) - 1] = 0;
		return datatime;
	}
	const std::string get_place()const
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
	tm* init_datatime()//  преобразуем в time штамп
	{
		const time_t datatime = mktime(&this->datatime);
		return localtime(&datatime);
	}
	void set_datatime(int year, int month, int day, int hour, int minute)
	{
		this->datatime = tm{};
		datatime.tm_year = year - 1900;
		datatime.tm_mon = month - 1;
		datatime.tm_mday = day;
		datatime.tm_hour = hour;
		datatime.tm_min = minute;
		this->datatime = *init_datatime();
	}
	// Перегрузка datatime
	void set_datatime(const std::string& datatime)// принимаем строку дату и время 
	{
		this->datatime = tm{};
		const int SIZE = 32;
		char buffer[SIZE]{};
		strcpy(buffer, datatime.c_str());
		// метод c_str()const, который возврящает указатель на RAW - строку, 
		// которую обварачивает объект класса std::string
		int part[5] = {};
		const char delimeters[] = "./:";
		int n = 0;
		for (char* pch = strtok(buffer, delimeters); pch; pch = strtok(NULL, delimeters))
			part[n++] = std::stoi(pch);
		this->datatime.tm_year = part[0] - 1900;
		this->datatime.tm_mon = part[1] - 1;
		this->datatime.tm_mday = part[2];
		this->datatime.tm_hour = part[3];
		this->datatime.tm_min = part[4];
		this->datatime = *init_datatime();
	}
	void set_place(const std::string& place)
	{
		this->place = place;
	}
	// конструкторы 
	//Crime(const std::string& licence_plate, int id, const std::string& datatime, const std::string& place)
	Crime(int id, const std::string& datatime, const std::string& place)
	{
		//set_licence_plate(licence_plate);
		set_id(id);
		set_datatime(datatime);
		set_place(place);
	}
	~Crime() {}
};
std::ostream& operator <<(std::ostream& os, const Crime& obj)
{
	//return os << obj.get_licence_plate() << " " << obj.get_id() << " " << obj.get_time() << " " << obj.get_place();
	os << " " << obj.get_time() << "|";
	os.width(20);
	os << std::left;
	os << obj.get_place() << "|";
	os << "\t" << violation.at(obj.get_id());
	return os;
}

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
				Crime(8,"2023.06.26 03.30", "ул.Ворошилова"),
			}
		},
	};
	for (std::map<std::string, std::list<Crime>>::iterator it = base.begin(); it != base.end(); ++it)
	{
		cout << it->first << ":\n";
		for (std::list<Crime>::iterator l_it = it->second.begin(); l_it != it->second.end(); ++l_it)
		{
			cout << " \t|" << *l_it << endl;
		}
		cout << delimeter << endl;
	}
}