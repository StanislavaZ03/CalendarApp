#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <map>
using namespace std;

time_t rawtime;
struct tm * timeinfo;
bool isMonday;

struct Event {
    string name;
    string startDate;
    string endDate;
};

Event events[50];
int eventIndex = 0;

void CheckFirstDay () {
    fstream firstDayFile("calendarFirstDay.txt");

    string day;
    firstDayFile>>day;

    if (day == "Mon") {
        isMonday = true;
    } else {
        isMonday = false;
    }

    firstDayFile.close();
}

void SetFirstWeekday () {
    system("CLS");
    fstream firstDayFile("calendarFirstDay.txt");

    CheckFirstDay();
    string day;

    if (isMonday) day = "Monday";
    else day = "Sunday";

    cout<<"The first day of the week is currently "<<day<<"."<<endl<<endl;
    cout<<"Enter new (Mon / Sun): ";
    string input;
    cin>>input;

    if (input == "Mon") {
        isMonday = true;
        firstDayFile<<input;
        cout<<"Saved!"<<endl;
    }
    else if (input == "Sun") {
        isMonday = false;
        firstDayFile<<input;
        cout<<"Saved!"<<endl;
    }
    else cout<<"Wrong input!"<<endl;

    firstDayFile.close();

    CheckFirstDay();
}

string MonthName (string month) {
    if (month == "01") return "January";
    else if (month == "02") return "February";
    else if (month == "03") return "March";
    else if (month == "04") return "April";
    else if (month == "05") return "May";
    else if (month == "06") return "June";
    else if (month == "07") return "Jule";
    else if (month == "08") return "August";
    else if (month == "09") return "September";
    else if (month == "10") return "October";
    else if (month == "11") return "November";
    else if (month == "12") return "December";
    else return "- -";
}

string DayOfWeek (string weekday, bool isMonday) {
    if (isMonday) {
        if (weekday == "Monday") return "[Mo]   Tu     We     Th     Fr     Sa     Su";
        else if (weekday == "Tuesday") return "Mo     [Tu]   We     Th     Fr     Sa     Su";
        else if (weekday == "Wednesday") return "Mo     Tu     [We]   Th     Fr     Sa     Su";
        else if (weekday == "Thursday") return "Mo     Tu     We     [Th]   Fr     Sa     Su";
        else if (weekday == "Friday") return "Mo     Tu     We     Th     [Fr]   Sa     Su";
        else if (weekday == "Saturday") return "Mo     Tu     We     Th     Fr     [Sa]   Su";
        else if (weekday == "Sunday") return "Mo     Tu     We     Th     Fr     Sa   [Su]";
    } else {
        if (weekday == "Monday") return "Su    [Mo]   Tu     We     Th     Fr     Sa";
        else if (weekday == "Tuesday") return "Su     Mo     [Tu]   We     Th     Fr     Sa";
        else if (weekday == "Wednesday") return "Su     Mo     Tu     [We]   Th     Fr     Sa";
        else if (weekday == "Thursday") return "Su     Mo     Tu     We     [Th]   Fr     Sa";
        else if (weekday == "Friday") return "Su     Mo     Tu     We     Th     [Fr]   Sa";
        else if (weekday == "Saturday") return "Su     Mo     Tu     We     Th     Fr     [Sa]";
        else if (weekday == "Sunday") return "[Su]    Mo     Tu     We     Th     Fr     Sa";
    }
}

int FirstDayOfMonth(int m, int y)
{
    int d = 1;
	return  (d+=m<3?y--:y-2,23*m/9+d+4+y/4-y/100+y/400)%7 ;
}

int NumberToMonth (string month) {
    if (month == "01") return 1;
    else if (month == "02") return 2;
    else if (month == "03") return 3;
    else if (month == "04") return 4;
    else if (month == "05") return 5;
    else if (month == "06") return 6;
    else if (month == "07") return 7;
    else if (month == "08") return 8;
    else if (month == "09") return 9;
    else if (month == "10") return 10;
    else if (month == "11") return 11;
    else if (month == "12") return 12;
}

int DaysOfMonth (int month, int year) {
    if (month == 1) return 31;
    else if (month == 2) {
        if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) {
            return 29;
        } else {
            return 28;
        }
    }
    else if (month == 3) return 31;
    else if (month == 4) return 30;
    else if (month == 5) return 31;
    else if (month == 6) return 30;
    else if (month == 7) return 31;
    else if (month == 8) return 31;
    else if (month == 9) return 30;
    else if (month == 10) return 31;
    else if (month == 11) return 30;
    else if (month == 12) return 31;
}

int stringToInt(string s) {
    stringstream s1(s);
    int result = 0;
    s1 >> result;
    return result;
}

bool isEventOnADate(Event event, string date) {
    int d = stringToInt(date.substr(0, 2));
    int m = stringToInt(date.substr(3, 2));
    int y = stringToInt(date.substr(6, 4));
    int eventStartD = stringToInt(event.startDate.substr(0, 2));
    int eventStartM = stringToInt(event.startDate.substr(3, 2));
    int eventStartY = stringToInt(event.startDate.substr(6, 4));
    int eventEndD = stringToInt(event.endDate.substr(0, 2));
    int eventEndM = stringToInt(event.endDate.substr(3, 2));
    int eventEndY = stringToInt(event.endDate.substr(6, 4));
    bool isOkay = false;

    if (eventStartY < y && eventEndY > y) {
        isOkay = true;
    } else if (eventStartY == y) {
        if (eventStartM < m && eventEndM > m) {
            isOkay = true;
        } else if (eventStartM == m) {
            if (eventStartD <= d && eventEndD >= d) {
                isOkay = true;
            }
        }
    } else if (eventEndY == y) {
        if (eventEndM > m && eventStartM < m) {
            isOkay = true;
        } else if (eventEndM == m) {
            if (eventStartD >= d && eventEndD <= d) {
                isOkay = true;
            }
        }
    }

    if (isOkay) {
        return true;
    }

    return false;
}

void ShowCalendar() {
    system("CLS");
    cout<<"Enter month (MM/YYYY): ";
    string input, month, year;
    cin>>input;
    month = input.substr(0, 2);
    year = input.substr(3, 4);

    cout<<"            "<<MonthName(month)<<" "<<year<<endl;
    char buffer [80];
    char buffer2 [80];
    char buffer3 [80];
    char buffer4 [80];
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (buffer,80,"%A",timeinfo);
    strftime (buffer2,80,"%m",timeinfo);
    strftime (buffer3,80,"%Y",timeinfo);
    strftime (buffer4,80,"%d",timeinfo);
    string weekday = buffer;
    string curr_month = buffer2;
    string curr_year = buffer3;
    string curr_day = buffer4;
    bool isNow = false;
    if (month == curr_month && year == curr_year) {
        string week = DayOfWeek(buffer, isMonday);
        cout<<week<<endl;
        isNow = true;
    } else if (isMonday){
        cout<<"Mo     Tu     We     Th     Fr     Sa     Su"<<endl;
    } else {
        cout<<"Su     Mo     Tu     We     Th     Fr     Sa"<<endl;
    }

    int x = stringToInt(year);

    int h = stringToInt(curr_day);

    int firstDay = FirstDayOfMonth(NumberToMonth(month), x);
    if (firstDay == 0 && isMonday) firstDay = 7;

    int c = 1;
    if (!isMonday) {
        c = 0;
    }

    for (int i = 0; i < firstDay - c; i++) {
        cout<<"       ";
    }

    int weekCount = firstDay - c;

    for (int i = 1; i <= DaysOfMonth(NumberToMonth(month), x); i++) {
        stringstream day;
        day << i;
        string dayOut = day.str();
        int cnt = 0;

        if (isNow && i == h) {
                dayOut = "[";
                stringstream d1;
                d1 << i;
                dayOut += d1.str();
                dayOut += "]";
        }
        for (int j = 0 ; j < eventIndex; j++) {
            string date = "";
            if (i <= 9) {
                date += "0";
            }
            stringstream d2;
            d2 << i;
            date += d2.str();
            date += "/";
            date += input;
            if (isEventOnADate(events[j], date)) {
                cnt++;
            }
        }
        if (cnt > 0) {
            dayOut += "(";
            dayOut += char(cnt + '0');
            dayOut += ")";
        }
        cout<<left<<setw(7)<<dayOut;
        weekCount++;
        if (weekCount == 7) {
            weekCount = 0;
            cout<<endl;
        }
    }
    cout<<endl;
}

string makeTodayTomorrow(string date) {
    int d = stringToInt(date.substr(0, 2));
    int m = stringToInt(date.substr(3, 2));
    int y = stringToInt(date.substr(6, 4));

    if (d < DaysOfMonth(m, y)) {
        d++;
    } else if (m < 12) {
        m++;
        d = 1;
    } else {
        y++;
        d = 1;
        m = 1;
    }

    string result;

    stringstream d2;
    d2 << d;
    string d3 = d2.str();

    if (d <= 9) {
        result += "0";
    }

    result += d3;
    result += "/";

    stringstream m2;
    m2 << m;
    string m3 = m2.str();

    if (m <= 9) {
        result += "0";
    }

    result += m3;
    result += "/";

    stringstream y2;
    y2 << y;
    string y3 = y2.str();

    result += y3;

    return result;
}

void MenuMessage() {

    char buffer [80], buffer3[80];
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (buffer,80,"%A, %d %B %Y.",timeinfo);
    strftime (buffer3,80,"%d/%m/%Y",timeinfo);

	cout<<"Welcome! Today is "<<buffer<<endl<<endl;

	bool flag = true, tomorrow = false;
	string buffer2 = buffer3;
    int eventsCount = 0;

	for (int i = 0; i < 2; i++) {

        for (int i = 0; i < eventIndex; i++) {
                if (isEventOnADate(events[i], buffer2)) {
                    eventsCount++;
                }
        }

        if (eventsCount > 0) {
            break;
        } else {
            tomorrow = true;
            buffer2 = makeTodayTomorrow(buffer2);
        }
	}

	if (eventsCount == 1) {
        cout<<"You have 1 event ";
        if (tomorrow) {
            cout<<"tomorrow.";
        } else {
            cout<<"today.";
        }
        cout<<endl<<endl;
	} else if (eventsCount > 1) {
	    cout<<"You have "<<eventsCount<<" events ";
        if (tomorrow) {
            cout<<"tomorrow.";
        } else {
            cout<<"today.";
        }
        cout<<endl<<endl;
	}

	cout<<"Choose an option:"<<endl;
	cout<<"    1. Show calendar"<<endl;
	cout<<"    2. Show schedule"<<endl;
	cout<<"    3. List events"<<endl;
	cout<<"    4. Add event"<<endl;
	cout<<"    5. Remove event"<<endl;
	cout<<"    6. Set first weekday"<<endl<<endl;
}

void CheckEvents() {
    fstream eventsFile("calendarEvents.txt");

    string name, sDate, eDate;

    while (getline(eventsFile, name)) {
        getline(eventsFile, sDate);
        getline(eventsFile, eDate);
        Event event;
        event.name = name;
        event.startDate = sDate;
        event.endDate = eDate;
        events[eventIndex] = event;
        eventIndex++;
    }
}

int SearchByName(string name) {
    for (int i = 0; i < eventIndex; i++) {
        if (events[i].name == name) {
            return i;
        }
    }
    return -1;
}

void AddEvent() {
    system("CLS");
    Event event;
    string name, sDate, eDate;

    cout<<"Enter name: ";
    cin.ignore();
    getline(cin, name);
    event.name = name;
    cout<<endl<<endl<<"Enter start date (DD/MM/YYYY): ";
    cin>>sDate;
    event.startDate = sDate;
    cout<<endl<<endl<<"Enter end date (DD/MM/YYYY): ";
    cin>>eDate;
    event.endDate = eDate;
    bool areDatesOkay = false;

    if (stringToInt(event.startDate.substr(6, 4)) < stringToInt(event.endDate.substr(6, 4))) {
            areDatesOkay = true;
    } else if (stringToInt(event.startDate.substr(6, 4)) == stringToInt(event.endDate.substr(6, 4))) {
        if (stringToInt(event.startDate.substr(3, 2)) < stringToInt(event.endDate.substr(3, 2))) {
            areDatesOkay = true;
        } else if (stringToInt(event.startDate.substr(3, 2)) == stringToInt(event.endDate.substr(3, 2))) {
            if (stringToInt(event.startDate.substr(0, 2)) <= stringToInt(event.endDate.substr(0, 2))) {
                areDatesOkay = true;
            }
        }
    }

    if (areDatesOkay) {
        if (SearchByName(event.name) == -1) {

            fstream eventsFile("calendarEvents.txt", ios::app);

            eventsFile<<event.name<<endl<<event.startDate<<endl<<event.endDate<<endl;

            eventsFile.close();

            events[eventIndex] = event;
            eventIndex++;
        } else {
            cout<<"There is another event with the same name!"<<endl;
        }
    } else {
        cout<<"Incorrect dates!"<<endl;
    }
}

bool isBigger (string date1, string date2) {
    int d1 = stringToInt(date1.substr(0, 2));
    int m1 = stringToInt(date1.substr(3, 2));
    int y1 = stringToInt(date1.substr(6, 4));
    int d2 = stringToInt(date2.substr(0, 2));
    int m2 = stringToInt(date2.substr(3, 2));
    int y2 = stringToInt(date2.substr(6, 4));
    bool result = false;

    if (y1 > y2) {
        result = true;
    } else if (y1 == y2) {
        if (m1 > m2) {
            result = true;
        } else if (m1 == m2) {
            if (d1 > d2) {
                result = true;
            }
        }
    }

    return result;
}

void ListEvents() {
    system("CLS");

    Event buffer;

    for (int i = 0; i < eventIndex; i++) {
        for (int j = i + 1; j < eventIndex; j++) {
            if (isBigger(events[i].startDate, events[j].startDate)) {
                    swap(events[i], events[j]);
            } else if (events[i].startDate == events[j].startDate && events[i].name.compare(events[j].name) > 0) {
                swap(events[i], events[j]);
            }
        }
    }

    cout<<"You have the following events:"<<endl;

    for (int i = 0; i < eventIndex; i++) {
        if (events[i].startDate == events[i].endDate) {
            cout<<i + 1<<". "<<events[i].name<<" ("<<events[i].startDate<<")"<<endl;
        } else {
            cout<<i + 1<<". "<<events[i].name<<" ("<<events[i].startDate<<" - "<<events[i].endDate<<")"<<endl;
        }
    }
}

void RemoveEvent() {
    system("CLS");

    cout<<"Enter name: ";
    string name;
    cin.ignore();
    getline(cin, name);

    int i = SearchByName(name);

    if (i != -1) {
        events[i].startDate = "Invalid";

        fstream eventsFile("calendarEvents.txt", ios::out);

        for (int i = 0; i < eventIndex; i++) {
            if (events[i].startDate != "Invalid") {
                eventsFile<<events[i].name<<endl<<events[i].startDate<<endl<<events[i].endDate<<endl;
            }
        }

        eventsFile.close();

        cout<<"Event deleted successfully!"<<endl;

        eventIndex = 0;

        CheckEvents();
    } else {
        cout<<"Event with that name does not exist!"<<endl;
    }
}

int eventDaysDiff(Event event) {
    int d1 = stringToInt(event.startDate.substr(0, 2));
    int m1 = stringToInt(event.startDate.substr(3, 2));
    int y1 = stringToInt(event.startDate.substr(6, 4));
    int d2 = stringToInt(event.endDate.substr(0, 2));
    int m2 = stringToInt(event.endDate.substr(3, 2));
    int y2 = stringToInt(event.endDate.substr(6, 4));
    int days = 0;
    bool flag = true;

    for (int y = y1; y <= y2; y++) {
            if (!flag) break;
        for (int m = m1; m <= 12; m++) {
                if (!flag) break;
            for (int d = d1; d <= DaysOfMonth(m1, y); d++) {
                days++;
                if (y == y2 && m == m2 && d == d2) {
                    flag = false;
                    break;
                }
            }
        }
    }
    return days;
}

void ShowSchedule() {
    system("CLS");

    cout<<"Enter month(MM/YYYY): ";
    string input, month, year;
    cin>>input;
    month = input.substr(0, 2);
    year = input.substr(3, 4);

    cout<<endl<<MonthName(month)<<" "<<year<<endl<<"---------------"<<endl;

    int y = stringToInt(year), eventsCount = 0;
    string date;

    for (int i = 0; i < eventIndex; i++) {
        for (int j = i + 1; j < eventIndex; j++) {
            if (isBigger(events[i].startDate, events[j].startDate)) {
                    swap(events[i], events[j]);
            } else if (events[i].startDate == events[j].startDate && events[i].name.compare(events[j].name) > 0) {
                swap(events[i], events[j]);
            }
        }
    }

    map<string, int> continuouslyEvents;

    for (int i = 1; i <= DaysOfMonth(NumberToMonth(month), y); i++) {
            date = "";

            if (i <= 9) {
                date += "0";
            }

            stringstream day;
            day << i;

            date += day.str();
            date += "/";
            date += input;

            for (int j = 0; j < eventIndex; j++) {
                if (isEventOnADate(events[j], date)) {
                    int weekday = (FirstDayOfMonth(stringToInt(month), stringToInt(year)) + i - 1) % 7;
                    string weekd;

                    if (weekday == 1) weekd = "Mo";
                    else if (weekday == 2) weekd = "Tu";
                    else if (weekday == 3) weekd = "We";
                    else if (weekday == 4) weekd = "Th";
                    else if (weekday == 5) weekd = "Fr";
                    else if (weekday == 6) weekd = "Sa";
                    else if (weekday == 0) weekd = "Su";

                    cout<<weekd<<", "<<date.substr(0, 2)<<"    ";
                    break;
                }
            }

            int linesCount = 0;

            for (int j = 0; j < eventIndex; j++) {
                if (isEventOnADate(events[j], date)) {
                        if (linesCount > 0) {
                            cout<<"          ";
                        }

                        cout<<events[j].name;
                        linesCount++;
                        if (events[j].startDate != events[j].endDate && (continuouslyEvents.find(events[j].name) == continuouslyEvents.end())) {
                            continuouslyEvents.insert(pair<string, int>(events[j].name, 1));
                            cout<<" (Day 1/"<<eventDaysDiff(events[j])<<")"<<endl;
                        } else if (events[j].startDate != events[j].endDate) {
                            int ddd = 0;
                            for (auto &e: continuouslyEvents) {
                                if (e.first == events[j].name) {
                                    e.second++;
                                    ddd= e.second;
                                }
                            }
                            cout<<" (Day "<<ddd<<"/"<<eventDaysDiff(events[j])<<")"<<endl;
                        } else {
                            cout<<endl;
                        }
                }
            }
    }
}

void Menu() {
    while (true) {
        system("CLS");

        MenuMessage();

        cout<<"Enter your choice: ";
        string input;
        cin>>input;

        if (input == "1") {
            ShowCalendar();
            system("pause");
        } else if (input == "2") {
            ShowSchedule();
            system("pause");
        } else if (input == "3") {
            ListEvents();
            system("pause");
        } else if (input == "4") {
            AddEvent();
            system("pause");
        } else if (input == "5") {
            RemoveEvent();
            system("pause");
        } else if (input == "6") {
            SetFirstWeekday();
            system("pause");
        } else {
            cout<<"That is not on the menu!";
        }
    }
}
int main() {
    CheckEvents();
    CheckFirstDay();
    Menu();

    return 0;
}
