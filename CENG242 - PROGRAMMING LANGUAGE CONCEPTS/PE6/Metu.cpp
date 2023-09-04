#include "Metu.h"

// YOU CAN COMPLETELY CHANGE THIS FILE AS YOU NEED. //

/****************************************************/
/****                *** METU ***                ****/

/* Constructor : Initialize your own variables if you
   need.
*/
Metu::Metu() {
	// TODO
   n_rows = -1;
   n_columns = -1;
}

/* Destructor  : It is responsible of the destruction of
   the Course and OpenCourse objects and the registered
   students, and the constrcuted seating plan, if any.
*/
Metu::~Metu() {
	// TODO
}

/* This method returns the student whose id is given
   in the argument. You need to find the corresponding
   student among the registered students.
*/
Student& Metu::getStudent(int id) {
	// TODO
   for (int i=0;i<registered_students.size();i++) if (registered_students[i]->getId() == id) return *registered_students[i];
}

/* This method directly adds (without creating a copy)
   the given student to the list of registered students.
*/
void Metu::registerStudent(Student* student) {
	// TODO
   this->registered_students.push_back(student);
}

/* This method directly adds (without creating a copy)
   the given course to the list of base course objects
   (not among the derived ones).
*/
void Metu::registerCourse(const Course& course) {
	// TODO
   this->registered_courses.push_back(&course);
}

/* This method constructs a new OpenCourse object by deriving
   from the Course object given in the first argument and
   using the <term> and <quota> values given in the second
   and third arguments, respectively.
   Also, for each student given in the corresponding lists as
   function arguments, the newly created OpenCourse is tried 
   to be added in order.
   Note that the priority order is given from greater to less
   as follows: Seniors > Juniors > Sophomores > Freshmans.
   After the priority ordering, you should try to add the
   course for each student of the same type in the order inside
   the corresponding list.
   Also, during the course addition, you should take the course
   quota in consider.
   In the end, there may be left the students who could not add
   the course because of the quota or the course prerequisite.
   Finally, you should return the created OpenCourse object.

   By the way; of course it would be nicer to define
   this method in "virtual" in order to use it for different
   types of Students. However, we do not want to test
   you with "virtual" keyword in this PE. We've left it
   to next PE.
*/
OpenCourse& Metu::openCourse(const Course& course, string term, int quota, 
							vector<Freshman*> freshmans, vector<Sophomore*> sophomores, vector<Junior*> juniors, vector<Senior*> seniors) {
	// TODO,
   OpenCourse* new_OpenCourse = new OpenCourse(course,term,open_courses.size(),quota);
   open_courses.push_back(new_OpenCourse);
   int used_quota = 0;
   for (int i=0;i<seniors.size();i++){      
      if (used_quota == quota) break;
      else {
         if (seniors[i]->addCourse(*new_OpenCourse) == true) used_quota ++;
      }
   }
   for (int i=0;i<juniors.size();i++){
      if (used_quota == quota) break;
      else {
         if (juniors[i]->addCourse(*new_OpenCourse) == true) used_quota ++;
      }
   }
   for (int i=0;i<sophomores.size();i++){
      if (used_quota == quota) break;
      else {
         if (sophomores[i]->addCourse(*new_OpenCourse) == true) used_quota ++;
      }
   }
   for (int i=0;i<freshmans.size();i++){
      if (used_quota == quota) break;
      else {
         if (freshmans[i]->addCourse(*new_OpenCourse) == true) used_quota ++;
      }
   }
   return *new_OpenCourse;
}

/* This method upgrades the given Freshman object to
   the Sophomore status by constructing a new Sophomore
   object with the properties of the given Freshman.
   You should delete the given Freshman and place the 
   newly created Sophomore object to its location in the 
   students array of Metu object.
   In the end, you should return the created Sophomore.

   By the way; of course it would be nicer to define 
   this method in "virtual" in order to use it for different 
   types of Students. However, we do not want to test 
   you with "virtual" keyword in this PE. We've left it 
   to next PE.
*/
Sophomore* Metu::upgradeStudent(Freshman& student) {
	// TODO
}

/* This method upgrades the given Sophomore object to
   the Junior status by constructing a new Junior
   object with the properties of the given Sophomore.
   You should delete the given Sophomore and place the
   newly created Junior object to its location in the
   students array of Metu object.
   In the end, you should return the created Junior.

   By the way; of course it would be nicer to define
   this method in "virtual" in order to use it for different
   types of Students. However, we do not want to test
   you with "virtual" keyword in this PE. We've left it
   to next PE.
*/
Junior* Metu::upgradeStudent(Sophomore& student) {
	// TODO
}

/* This method upgrades the given Junior object to
   the Senior status by constructing a new Senior
   object with the properties of the given Junior.
   You should delete the given Junior and place the
   newly created Senior object to its location in the
   students array of Metu object.
   In the end, you should return the created Senior.

   By the way; of course it would be nicer to define
   this method in "virtual" in order to use it for different
   types of Students. However, we do not want to test
   you with "virtual" keyword in this PE. We've left it
   to next PE.
*/
Senior* Metu::upgradeStudent(Junior& student) {
	// TODO
}

/* This method sets the number of rows and number of columns in a
   standard Metu classroom with the given arguments, respectively.
*/
void Metu::setClassroomSize(int row_size, int column_size) {
	// TODO
   n_rows = row_size;
   n_columns = column_size;
   for (int i=0;i<n_rows;i++){
      vector<int> row;
      seating_plan.push_back(row);
      for (int j=0;j<n_columns;j++){
         seating_plan[i].push_back(-1);
      }
   }
}

/* This method tries to construct the 2D seating plan by using
   the given information as function argument one-by-one.
   The information consists of 2 integers and 1 string argument.
   The string parameter can be either "|" or "-".
   If it is "|", then it means location of the student whose id 
   given in the first argument is <row_id, column_id> whereas
   location of the student whose id given in the second argument
   is <row_id + 1, column_id>.
   If it is "-", then it means location of the student whose id 
   given in the first argument is <row_id, column_id> whereas
   location of the student whose id given in the second argument
   is <row_id, column_id + 1>.
*/
void Metu::addCheatInfo(int from_student_id, int to_student_id, string dir) {
	// TODO
   if (dir == "|"){
      vector<int> k;
      k.push_back(from_student_id);
      k.push_back(to_student_id);
      k.push_back(0);
      info.push_back(k);
   }
   else {
      vector<int> k;
      k.push_back(from_student_id);
      k.push_back(to_student_id);
      k.push_back(1);
      info.push_back(k);
   }
   bool flag1 = true;
   for (int i=0;i<copy_ids.size();i++) if (copy_ids[i] == from_student_id) flag1 = false;
   if (flag1) copy_ids.push_back(from_student_id);
   bool flag2 = true;
   for (int i=0;i<copy_ids.size();i++) if (copy_ids[i] == to_student_id) flag2 = false;
   if (flag2) copy_ids.push_back(to_student_id);
}

/* This method prints the given seating plan, in
   other words the 2D array. Between each seat,
   print an empty space, i.e. " " character.
   Also, put an empty space to the end of each row.
   After each row (including the last one), print a
   new line, i.e. "\n" character.
   For the empty seats, print a "X" character (uppercase).
   For non-empty seats, print the id of the student
   sitting there.
*/
void Metu::printSeatingPlan() {
	// TODO

   for (int i=0;i<copy_ids.size();i++){
      vector<int> p;
      p.push_back(copy_ids[i]);
      p.push_back(0);
      p.push_back(0);
      positions.push_back(p);
   }

   for (int i=0;i<info.size();i++){
      if (info[i][2] == 0){ // |
         int ip1,ip2;
         for (int j=0;j<positions.size();j++){
            if (positions[j][0] == info[i][0]) ip1 = j;
            if (positions[j][0] == info[i][1]) ip2 = j;
         }
         positions[ip2][2] = positions[ip1][2] + 1; 
         if (positions[ip1][1] > positions[ip2][1]) positions[ip2][1] = positions[ip1][1];
         else {
            positions[ip1][1] = positions[ip2][1];
         }
      }
      else{
         int ip1,ip2;
         for (int j=0;j<positions.size();j++){
            if (positions[j][0] == info[i][0]) ip1 = j;
            if (positions[j][0] == info[i][1]) ip2 = j;
         }
         //for (int i=0;i<positions.size();i++) if (positions[i][1] == positions[ip2][1] + 1) positions[i][1] = positions[ip1][1] + 2;
         positions[ip2][1] = positions[ip1][1] + 1;
         if (positions[ip1][2] > positions[ip2][2]) positions[ip2][2] = positions[ip1][2];
         else {
            positions[ip1][2] = positions[ip2][2];
         }
      }
   }

   // for (int i=0;i<positions.size();i++) cout << positions[i][0] << " " << positions[i][1] << " " << positions[i][2] << endl;
   
   
   for (int i=0;i<positions.size();i++) seating_plan[positions[i][2]][positions[i][1]] = positions[i][0];
   for (int i=0;i<n_rows;i++){
      for (int j=0;j<n_columns;j++){
         if (seating_plan[i][j] == -1) cout << "X" << " ";
         else cout << seating_plan[i][j] << " ";
      }
      cout << endl;
   }
}

