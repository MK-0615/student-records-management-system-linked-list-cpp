#include	<iostream>
#include 	<fstream>
#include	<string>
#include	<cstdlib>
#include	"List.h"
#include	"Student.h"

using namespace std;

//Main functions
bool CreateStuList(const char *, List *);
bool DeleteStudent(List *, char *);
bool PrintList(List, int); 
bool InsertExamResult(const char *, List *);
bool PrintStatistic(List);
bool FilterStudent(List, List *, char *, int, int) ;
bool UpdateIDandPhone(List *);
bool FindPotentialFirstClass(List, List *, char *);
int menu();

//Sub-functions
bool Redundant_Stu(List*, Student);
bool Redundant_Exam(Student, Exam);
bool isOutOfRange(char[], int);
void PrintStuInfo(Student, ostream&, int);
void ClearList(List*);
bool Digit(char[]);
void UpperCase(char[]);
void PrintHeader(const char*);

using namespace std;


int main() {
	system("color 02");

	//Declarations
	int choice;
	List examList;
	List studentList, filteredList, firstClass;

	//Loop while choice != 9
	do{
		//Declarations
		char year[10], totalcredit[100];
		choice = menu();
		system("cls");

		switch (choice) {

		//1. Create student list
		case 1:

			PrintHeader("\tCREATE STUDENT LIST");
			if (CreateStuList("student.txt", &studentList))
				cout << "\n\n\t\t\t<<< Student list has successfully created! >>>\n\n";

			else
				cout << "\n\n\t\t--------------------------- .Fail to open file student.txt. ---------------------------\n\n";
			break;

		//2. Delete Student
		case 2:
			
			//Declaration
			char stu_id[12];

			//Input
			PrintHeader("\tDELETE STUDENT");
			cout << "\n\n\t\t\tPlease enter the student id that you wish to delete: ";
			cin >> stu_id;

			UpperCase(stu_id);
			if (!isOutOfRange(stu_id, 7) || !isOutOfRange(stu_id, 10)) {
				if (DeleteStudent(&studentList, stu_id))
					cout << "\n\n\t\t\t<<< Successfully deleted student ID " << stu_id << "! >>>\n\n" << endl;
				else {
					cout << "\n\n\t\t---------------------- .Cannot find the student id in the list. -----------------------" << endl;
					cout << "\n\n\t\t------------------------------------ .Delete fail. ------------------------------------" << endl;
				}
			}
			else{
				cout << "\n\n\t\t\tStudent id entered is out of range";
				cout << "\n\n\t\t\tStudent id format : ";
				cout << "\n\t\t\t1. 7 letters - 1234567\n\t\t\t2. 10 letters - BCS1234567\n\n";
			}
			break;

		//3. Print student list
		case 3:

			//Declaration
			char source[10];

			//Input
			PrintHeader("\tPRINT STUDENT LIST");
			cout << "\n\n\t\t\tHow do you want to display the information?\n";
			cout << "\t\t\t1. Display to screen" << endl;
			cout << "\t\t\t2. File" << endl;
			cout << "\t\t\tChoice: ";
			cin >> source;

			if (!Digit(source)) {
				cout << "\n\n\t\t---------------------------------- .Invalid Input. ------------------------------------" << endl;
				break;
			}
			system("cls");
			PrintHeader("\tPRINT STUDENT LIST");
			if (!PrintList(studentList, stoi(source)))
				cout << "\n\n\t\t--------------------------------- .The list is empty. ---------------------------------" << endl;
			break;

		//4. Insert exam result
		case 4:

			PrintHeader("\tINSERT EXAM RESULT");
			if (InsertExamResult("exam.txt", &studentList))
				cout << "\n\n\t\t\t<<< All exam results have been inserted successfully! >>>\n\n" << endl;
			else{
				cout << "\n\n\t\t---------------------------- .Fail to open file exam.txt!. ----------------------------\n";
				cout << "\n\n\t\t----------------------------- .Cannot insert exam result. -----------------------------" << endl;
			}
			break;

		//5. Print Exam Statistic
		case 5:

			PrintHeader("\tPRINT EXAM STATISTIC");
			if (!PrintStatistic(studentList))
				cout << "\n\n\t\t-------------------------------- .The list is empty. ----------------------------------" << endl;
			break;

		//6. Filter Student
		case 6:

			//Declaration
			char course[10];

			//Input
			PrintHeader("\tFILTER STUDENT");
			cout << "\n\n\t\t\tCourse: ";
			cin >> course;
			cout << "\n\n\t\t\tYear: ";
			cin >> year;
			cout << "\n\n\t\t\tTotal Credit: ";
			cin >> totalcredit;
			cout << endl;

			UpperCase(course);
			if (isOutOfRange(year, 4) || !Digit(year)) {
				cout << "\n\n\t\t----------------------- .Please only enter 4 numbers for year. ------------------------" << endl;
				break;
			}
			if(!Digit(totalcredit)){
				cout << "\n\n\t\t-------------------------- .Invalid Input for Total Credit. ---------------------------" << endl;
				break;
			}
			if (!FilterStudent(studentList, &filteredList, course, stoi(year), stoi(totalcredit)))
				cout << "\n\n\t\t--------------------- .The list1 is empty or list2 is not empty. ----------------------" << endl;
			else
				PrintList(filteredList, 1);
			break;

		//7. Update Student's ID and Phone
		case 7:

			PrintHeader("UPDATE STUDENT'S ID AND PHONE");
			if (!UpdateIDandPhone(&studentList))
				cout << "\n\n\t\t--------------------- .The list1 is empty or list2 is not empty. ----------------------" << endl;
			else {
				cout << "\n\n\t\t\tStudent ID and phone number has been updated successfully!\n" << endl;
				PrintList(studentList, 1);
			}
			break;

		//8. Find Potential First Class Student
		case 8:

			//Input
			PrintHeader("FIND POTENTIAL FIRST CLASS STUDENT");
			cout << "\n\n\t\t\tCourse: ";
			cin >> course;
			cout << endl;

			UpperCase(course);
			if (!FindPotentialFirstClass(studentList, &firstClass, course))
				cout << "\n\n\t\t--------------------- .The list1 is empty or list2 is not empty. ----------------------" << endl;
			else
				PrintList(firstClass, 1);
			break;

		//9. Exit.
		case 9:

			break;

		//Error
		default:

			cout << "\n\n\t\t************************** Invalid input. Please try again. ***************************" << endl;
		}

		cout << "\t\t\t";
		system("pause");
		cout << endl;
		system("cls");

	} while (choice != 9);
	return 0;
}

//1. Create student list
bool CreateStuList(const char* filename, List* list) {

	//Declarations
	char n[256];
	bool redundant = false;
	Student stulist;
	ifstream infile(filename);

	//Read text file
	if (!infile)
		return false;

	else {
		while (!infile.eof()) {
			for (int i = 0; i < 3; i++)
				infile >> n;
			infile >> stulist.id; //Read student id

			for (int j = 0; j < 2; j++)
				infile >> n;
			infile.getline(stulist.name, 30); //Read student name

			for (int k = 0; k < 2; k++)
				infile >> n;
			infile >> stulist.course; //Read student course

			for (int l = 0; l < 3; l++)
				infile >> n;
			infile >> stulist.phone_no; //Read student phone number

			if (!Redundant_Stu(list, stulist)) {
				if (infile)
					list->insert(stulist); //Insert the student into the list
			}
			else
				redundant = true;

		}
		if (redundant){
			cout << "\n\n\t\t\tThere is/are duplicate record(s)" << endl;
			cout << "\t\t\tOnly non-duplicate record(s) is inserted into the student list" << endl;
		}
	}
	infile.close();
	return true;
}

//2. Delete Student
bool DeleteStudent(List* list, char* stu_id) {

	//Declaration
	Student stulist;

	//Check if the list is empty
	if (list->empty()) {
		cout << "\n\n\t\t--------------------------------- .The list is empty. ---------------------------------" << endl;
		return false;
	}

	//Find and delete the student using student id
	for (int i = 1; i <= list->size(); i++) {
		list->get(i, stulist);
		if (strcmp(stulist.id, stu_id) == 0){
			list->remove(i);
			return true;
		}
	}
	return false;
}

//3. Print student list
bool PrintList(List list, int source) {

	//Declarations
	Student stulist;
	ofstream outfile;

	//Check if the list is empty
	if (list.empty())
		return false;

	//source = 1 -> Display to screen
	//source = 2 -> Write in file
	switch (source){

	case 1:

		for (int i = 1; i <= list.size(); i++) {
			list.get(i, stulist);
			PrintStuInfo(stulist, cout,i);
			cout << endl;
		}
		break;

	case 2:

		outfile.open("student_result.txt");
		if (!outfile) {
			cout << "\n\n\t\t-------------------------- .Cannot open student_result.txt. ---------------------------" << endl;
			return false;
		}
		else {
			for (int i = 1; i <= list.size(); i++) {
				list.get(i, stulist);
				PrintStuInfo(stulist, outfile, i);
			}
		}
		outfile.close();
		break;

	default:

		cout << "\n\n\t\t---------------------------------- .Invalid Input. ------------------------------------" << endl;
	}
	return true;
}

//4. Insert exam result
bool InsertExamResult(const char* filename, List* list) {

	//Declarations
	Exam stuExam;
	Student stulist1, stulist2;
	int pos = 0;
	bool found, redundant = false;
	ifstream infile(filename);
	
	//Check if the file can be opened
	if (!infile)
		return false;

	//Read file
	else {
		while (!infile.eof()) {
			found = false;
			infile >> stulist2.id;

			for (int i = 1; i <= list->size(); i++) { //Find the student using student id
				list->get(i, stulist1);
				if (stulist1.compareID(stulist2)) {
					pos = i;
					found = true;
					break;
				}
			}
			if (!found)
				continue;

			infile >> stuExam.trimester >> stuExam.year >> stuExam.numOfSubjects; //Read Exam Records
			for (int i = 0; i < stuExam.numOfSubjects; i++) //Read Subjects Records
				infile >> stuExam.sub[i].subject_code >> stuExam.sub[i].subject_name >> stuExam.sub[i].credit_hours >> stuExam.sub[i].marks;
			stuExam.calculateGPA(); //Calculate GPA

			//Check duplicate record
			if (!Redundant_Exam(stulist1, stuExam)) {
				stulist1.exam[stulist1.exam_cnt] = stuExam;
				stulist1.exam_cnt++;
			}
			else
				redundant = true;

			stulist1.calculateCurrentCGPA(); //Calculate Current CGPA
			list->set(pos, stulist1); //Update the student details in the list
		}
		if (redundant) {
			cout << "\n\n\t\t\tThere is/are duplicate record(s)" << endl;
			cout << "\t\t\tOnly non-duplicate record(s) is inserted into the list" << endl;
		}
	}
	infile.close();
	return true;
}

//5. Print Exam Statistic
bool PrintStatistic(List list) {

	//Declarations
	Student students;
	char course_name[10][3] = { "" };
	int course_count[10] = { 0 };
	int count = 0, totalsubjects = 0, totaltrimester = 0, totalcredits = 0;
	double totalcgpa = 0.00;
	
	//Check if the list is empty
	if (list.empty())
		return false;

	else {
		for (int i = 1; i <= list.size(); i++) {
			bool found = false;
			list.get(i, students);

			//Calculations
			for (int j = 0; j < students.exam_cnt; j++)
				totalsubjects += students.exam[j].numOfSubjects; //Calculate Total Subjects for All Trimester

			for (int k = 0; k < 10; k++) { //Count number of students of each course
				if (strcmp(students.course, course_name[k]) == 0) {
					course_count[k]++;
					found = true;
					break;
				}
			}
			if (!found) { //There is no record of this course add another one in array
				strcpy(course_name[count], students.course);
				course_count[count]++;
				count++;
			}
			totaltrimester += students.exam_cnt; //Calculate Total Trimesters
			totalcgpa += students.current_cgpa; //Calculate Total CGPA
			totalcredits += students.totalCreditsEarned; //Calculate Total Credits Earned
		}

		//Output
		cout << "\n\n\t\t\tTotal Students: " << list.size() << endl;
		for (int i = 0; i < count; i++)
			cout << "\n\n\t\t\t\t" << course_name[i] << " Students - " << course_count[i] << endl;
		cout << endl;
		cout << "\n\n\t\t\tAverage CGPA: " << fixed << setprecision(5) << totalcgpa / list.size() << endl;
		cout << "\n\n\t\t\tAverage Subjects Taken Per Semester: " << setprecision(2) << static_cast<double>(totalsubjects) / totaltrimester << endl;
		cout << "\n\n\t\t\tAverage Credits Earned Per Semester: " << static_cast<double>(totalcredits) / totaltrimester << endl;
	}
	return true;
}

//6. Filter Student
bool FilterStudent(List list1, List* list2, char* course, int year, int totalcredit) {

	//Declarations
	bool found = false;
	string year2;
	Student student1;

	ClearList(list2);

	//Calculation
	if (year > 2000)
		year -= 2000;
	else
		year -= 1900;

	//Check if list1 is empty or list2 is not empty
	if (list1.empty() || !list2->empty())
		return false;

	else {
		for (int i = 1; i <= list1.count; i++) {
			list1.get(i, student1);

			//Calculate the year enrolled of the student
			year2 = student1.id;
			year2 = year2.substr(0, 2);

			//Insert the student into the list2 if he/she fulfills the 3 conditions
			if (strcmp(student1.course, course) == 0 && year == stoi(year2) && student1.totalCreditsEarned >= totalcredit) {
				list2->insert(student1); 
				found = true;
			}
		}
		//No student fulfills the 3 conditions
		if (!found)
			cout << "\n\n\t\t--------------------------------- .No record found!. ----------------------------------" << endl;
		return true;
	}
}

//7. Update Student's ID and Phone
bool UpdateIDandPhone(List* list) {

	//Declarations
	Student student, student1;
	char newid[20], newphone[20];

	//Check if list is empty
	if (list->empty())
		return false;

	else {
		for (int i = 1; i <= list->count; i++) {
			list->get(i, student);

			if (student.id[0] == 'B')
				continue;

			//Update student id
			strcpy(newid, "B");
			strcat(newid, student.course);
			strcat(newid, student.id);

			//Update student phone number
			for (int j = 0; j < 7; j++) {
				if (student.phone_no[j] == '-') {
					for (int k = j; k < 8; k++)
						student.phone_no[k] = student.phone_no[k + 1];
				}
			}
			if (student.phone_no[0] == '0' || student.phone_no[0] == '2' || student.phone_no[0] == '4' ||
				student.phone_no[0] == '6' || student.phone_no[0] == '8')
				strcpy(newphone, "02");
			else
				strcpy(newphone, "01");
			strcat(newphone, student.phone_no);

			//Copy updated student id and phone number into student details
			strcpy(student.id, newid);
			strcpy(student.phone_no, newphone);

			list->set(i, student); //Update student details into the list
		}
		return true;
	}
}

//8. Find Potential First Class Student
bool FindPotentialFirstClass(List list1, List* list2, char* course) {

	//Declarations
	Student student;
	bool found = false;

	ClearList(list2);

	//Check if list1 is empty or list2 is not empty
	if (list1.empty() || !list2->empty())
		return false;

	for (int i = 1; i <= list1.size(); i++) {

		//Declarations
		int tricount = 0, totalcredits = 0;
		bool fail = false;

		list1.get(i, student);

		//Find the student(s) in the course entered
		if (strcmp(student.course, course) == 0) { 

			found = true;

			if (student.exam_cnt < 3) //Check if student.exam_cnt < 3
				continue;

			else {
				for (int j = 0; j < student.exam_cnt; j++) { //Loop for each trimester (1 exam_cnt = 1 trimester)
					totalcredits = 0;
					for (int k = 0; k < student.exam[j].numOfSubjects; k++)
						totalcredits += student.exam[j].sub[k].credit_hours; //Calculate Total Credits Hours for Each Trimester
	
					if (!(student.exam[j].gpa < 3.50000)) { //Check if each trimester gpa < 3.50000
						if (student.exam[j].gpa >= 3.75000 && totalcredits >= 12) //Check for each trimester gpa and totalcredits
							tricount++;
					}
					else
						fail = true;
				}
				if (tricount >= 3 && !fail) //Insert the student into the list if he/she fulfills the conditions
					list2->insert(student);
			}
		}
	}
	if (!found){
		cout << "\n\n\t\t\tThere is no student with course " << course << " in the list\n\n";
		return true;
	}
	if (list2->size() == 0)
		cout << "\n\n\t\t\tThere is no student in " << course << " that has potential to get first class." << endl;
	return true;
}

//Menu
int menu() {

	char choice[100];
	cout << R"(
                           __  __   ___         __.....__        _..._              
                          |  |/  `.'   `.   .-''         '.    .'     '.            
                          |   .-.  .-.   ' /     .-''"'-.  `. .   .-.   .           
                          |  |  |  |  |  |/     /________\   \|  '   '  |           
                          |  |  |  |  |  ||                  ||  |   |  |   _    _  
                          |  |  |  |  |  |\    .-------------'|  |   |  |  | '  / | 
                          |  |  |  |  |  | \    '-.____...---.|  |   |  | .' | .' | 
                          |__|  |__|  |__|  `.             .' |  |   |  | /  | /  | 
                                              `''-...... -'   |  |   |  ||   `'.  | 
                                                              |  |   |  |'   .'|  '/
                                                              '--'   '--' `-'  `--' 
)" << endl;
	cout << "\t\t\t\t\t1. Create student list" << endl;
	cout << "\t\t\t\t\t2. Delete Student" << endl;
	cout << "\t\t\t\t\t3. Print student list" << endl;
	cout << "\t\t\t\t\t4. Insert exam result" << endl;
	cout << "\t\t\t\t\t5. Print Exam Statistic" << endl;
	cout << "\t\t\t\t\t6. Filter Student" << endl;
	cout << "\t\t\t\t\t7. Update Student's ID and Phone" << endl;
	cout << "\t\t\t\t\t8. Find Potential First Class Student" << endl;
	cout << "\t\t\t\t\t9. Exit." << endl;
	cout << "\t\t\t\t\tSelection (1-9): ";
	cin >> choice;

	if (!Digit(choice))
		return -1;
	return stoi(choice);
}

//Sub-functions

//Check Duplicate Record(s) of Student
bool Redundant_Stu(List *list, Student stulist) {

	Student item;

	for (int i = 1; i <= list->size(); i++) {
		list->get(i, item);
		if (strcmp(stulist.name, item.name) == 0)
			return true;
	}
	return false;
}

//Check Duplicate Record(s) of Exam
bool Redundant_Exam(Student stulist, Exam examlist) {

	for (int j = 0; j < stulist.exam_cnt; j++) {
		if (stulist.exam[j].trimester == examlist.trimester && stulist.exam[j].year == examlist.year)
			return true;
	}
	return false;
}

//Check if the value is out of range
bool isOutOfRange(char array[], int size) {

	if (strlen(array) == size)
		return false;
	return true;
}

//Print Student Info into Screen or File
void PrintStuInfo(Student stu, ostream &out, int num) {

	out << "******************************************************STUDENT " << num << "******************************************************";
	stu.print(out);
	out << "\n--------------------------------------------------PAST EXAM RESULT:--------------------------------------------------";
	if (stu.exam_cnt > 0) {
		for (int i = 0; i < stu.exam_cnt; i++)
			stu.exam[i].print(out);
	}
	else {
		out << "\n! THIS STUDENT HAVEN'T TAKEN ANY EXAM !\n\n";
	}
	out << "******************************************************STUDENT " << num << "******************************************************\n\n\n";
}

//Clear all the Nodes in the List
void ClearList(List *list) {
	while (!list->empty())
		list->remove(1);
}

//Check if the char array value is digit
bool Digit(char array[]) {
	for (int i = 0; i < strlen(array); i++) {
		if (!isdigit(array[i])) 
			return false;
	}
	return true;
}

//Change the char array into uppercase
void UpperCase(char array[]) {
	for (int i = 0; i < strlen(array); i++)
		array[i] = toupper(array[i]);
}

//Print Header
void PrintHeader(const char* name) {
	cout << "\t\t+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
	cout << "\t                                   " << name;
	cout << "\n\t\t+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
}