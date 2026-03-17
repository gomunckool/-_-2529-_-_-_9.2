#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

enum Speciality { COMPUTER_SCIENCE, INFORMATICS, MATH_ECON, PHYS_INF, LABOR_TRAINING };
string specialityStr[] = { "Комп’ютерні науки", "Інформатика", "Мат. та економ.", "Фіз. та інформ.", "Трудове навч." };

union ThirdGrade {
    int programming;
    int numericalMethods;
    int pedagogy;
};

struct Student {
    string surname;
    int course;
    Speciality speciality;
    int physicsGrade;
    int mathGrade;
    ThirdGrade thirdGrade;
};

void createStudents(Student* p, const int n);
double getAverageGrade(const Student s);
void printHeader();
void printStudentRow(const Student s, int index);
void printStudents(Student* p, const int n);
bool compare(const Student a, const Student b, bool reverseSurname);
void sortStudents(Student* p, const int n);
int* createIndexAr(const int n);
void indexSort(Student* p, int* I, const int n);
void printIndexed(Student* p, int* I, const int n);
int binarySearch(Student* p, const int n, string surname, int course, double avg);

int main() {
    setlocale(LC_ALL, "Ukrainian");

    int n;
    cout << "Введiть кiлькiсть студентiв: ";
    cin >> n;

    Student* students = new Student[n];
    int* indexAr = nullptr;
    
    int menuItem;
    do {
        cout << "\n--- МЕНЮ ---\n";
        cout << "1. Введення даних (контроль: 4 курси, оцiнки 1-5)\n";
        cout << "2. Вивести таблицю (Рiвень B)\n";
        cout << "3. Фiзичне сортування (Курс -> Сер. бал -> Прiзвище спад.)\n";
        cout << "4. Iндексне сортування (Курс -> Сер. бал -> Прiзвище зрост.)\n";
        cout << "5. Бiнарний пошук студента\n";
        cout << "0. Вихiд\n";
        cout << "Вибiр: ";
        cin >> menuItem;

        switch (menuItem) {
        case 1:
            createStudents(students, n);
            break;
        case 2:
            printStudents(students, n);
            break;
        case 3:
            sortStudents(students, n);
            cout << "Масив вiдсортовано фiзично.\n";
            printStudents(students, n);
            break;
        case 4:
            if (indexAr != nullptr) delete[] indexAr;
            indexAr = createIndexAr(n);
            indexSort(students, indexAr, n);
            cout << "Iндексний масив побудовано.\n";
            printIndexed(students, indexAr, n);
            break;
        case 5: {
            string sName;
            int course;
            double avg;
            cout << "Введiть данi для пошуку:\n";
            cout << " Прiзвище: "; cin >> sName;
            cout << " Курс: "; cin >> course;
            cout << " Середнiй бал (напр. 4.33): "; cin >> avg;
            
            sortStudents(students, n); 
            int found = binarySearch(students, n, sName, course, avg);
            if (found != -1) 
                cout << "Студента знайдено! Позиція у відсортованому списку: " << found + 1 << endl;
            else 
                cout << "Студента не знайдено.\n";
            break;
        }
        }
    } while (menuItem != 0);

    delete[] students;
    if (indexAr != nullptr) delete[] indexAr;
    return 0;
}

void createStudents(Student* p, const int n) {
    for (int i = 0; i < n; i++) {
        cout << "\nСтудент № " << i + 1 << ":\n";
        cout << " Прiзвище: "; cin >> p[i].surname;

        do {
            cout << " Курс (1-4): "; cin >> p[i].course;
            if (p[i].course < 1 || p[i].course > 4)
                cout << " Помилка! Введiть курс вiд 1 до 4.\n";
        } while (p[i].course < 1 || p[i].course > 4);

        int spec;
        do {
            cout << " Спецiальнiсть (0-КН, 1-IНФ, 2-МЕ, 3-ФI, 4-ТН): "; cin >> spec;
        } while (spec < 0 || spec > 4);
        p[i].speciality = (Speciality)spec;

        auto inputGrade = [](string subject) {
            int grade;
            do {
                cout << " Оцiнка з " << subject << " (1-5): "; cin >> grade;
            } while (grade < 1 || grade > 5);
            return grade;
        };

        p[i].physicsGrade = inputGrade("фiзики");
        p[i].mathGrade = inputGrade("математики");

        if (p[i].speciality == COMPUTER_SCIENCE)
            p[i].thirdGrade.programming = inputGrade("програмування");
        else if (p[i].speciality == INFORMATICS)
            p[i].thirdGrade.numericalMethods = inputGrade("чисельних методiв");
        else
            p[i].thirdGrade.pedagogy = inputGrade("педагогiки");
    }
}

double getAverageGrade(const Student s) {
    double third;
    if (s.speciality == COMPUTER_SCIENCE) third = s.thirdGrade.programming;
    else if (s.speciality == INFORMATICS) third = s.thirdGrade.numericalMethods;
    else third = s.thirdGrade.pedagogy;
    return (s.physicsGrade + s.mathGrade + third) / 3.0;
}

void printHeader() {
    cout << "\n======================================================================================================================\n";
    cout << "| " << setw(2) << "№" << " | " << setw(12) << "Прiзвище" << " | " << setw(4) << "Курс" << " | " << setw(17) << "Спецiальнiсть" 
         << " | " << setw(3) << "Фiз" << " | " << setw(3) << "Мат" << " | " << setw(5) << "Прог" << " | " << setw(6) << "Чис.м" << " | " << setw(4) << "Пед" << " | " << setw(5) << "С.Б." << " |\n";
    cout << "----------------------------------------------------------------------------------------------------------------------\n";
}

void printStudentRow(const Student s, int index) {
    cout << "| " << setw(2) << index + 1 << " | " << setw(12) << s.surname << " | " << setw(4) << s.course 
         << " | " << setw(17) << specialityStr[s.speciality] << " | " << setw(3) << s.physicsGrade << " | " << setw(3) << s.mathGrade << " | ";
    
    if (s.speciality == COMPUTER_SCIENCE) 
        cout << setw(5) << s.thirdGrade.programming << " | " << setw(6) << " " << " | " << setw(4) << " " << " | ";
    else if (s.speciality == INFORMATICS) 
        cout << setw(5) << " " << " | " << setw(6) << s.thirdGrade.numericalMethods << " | " << setw(4) << " " << " | ";
    else 
        cout << setw(5) << " " << " | " << setw(6) << " " << " | " << setw(4) << s.thirdGrade.pedagogy << " | ";

    cout << fixed << setprecision(2) << setw(5) << getAverageGrade(s) << " |\n";
}

void printStudents(Student* p, const int n) {
    printHeader();
    for (int i = 0; i < n; i++) printStudentRow(p[i], i);
    cout << "======================================================================================================================\n";
}

bool compare(const Student a, const Student b, bool reverseSurname) {
    if (a.course != b.course) return a.course < b.course;
    
    double avgA = getAverageGrade(a);
    double avgB = getAverageGrade(b);
    if (abs(avgA - avgB) > 1e-6) return avgA < avgB;
    
    if (reverseSurname) return a.surname > b.surname; 
    return a.surname < b.surname; 
}

void sortStudents(Student* p, const int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (compare(p[j+1], p[j], true)) swap(p[j], p[j+1]);
}

int* createIndexAr(const int n) {
    int* I = new int[n];
    for (int i = 0; i < n; i++) I[i] = i;
    return I;
}

void indexSort(Student* p, int* I, const int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (compare(p[I[j+1]], p[I[j]], false)) swap(I[j], I[j+1]);
}

void printIndexed(Student* p, int* I, const int n) {
    printHeader();
    for (int i = 0; i < n; i++) printStudentRow(p[I[i]], i);
    cout << "======================================================================================================================\n";
}

int binarySearch(Student* p, const int n, string surname, int course, double avg) {
    int L = 0, R = n - 1;
    while (L <= R) {
        int m = L + (R - L) / 2;
        double mAvg = getAverageGrade(p[m]);

        if (p[m].surname == surname && p[m].course == course && abs(mAvg - avg) < 0.01)
            return m;

        if (p[m].course < course || 
           (p[m].course == course && mAvg < avg) || 
           (p[m].course == course && abs(mAvg - avg) < 0.01 && p[m].surname > surname))
            L = m + 1;
        else
            R = m - 1;
    }
    return -1;
}
