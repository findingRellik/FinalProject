#include "ClassTree.h"
#include <string>
//#include <stdlib.h>  /*atoi*/
//#include <stdio.h>
//#include <iostream>
ClassTree::ClassTree()
{
    root = NULL;
    //ctor
}

ClassTree::~ClassTree()
{
    //dtor
}

void ClassTree::addClassNode(string name, string department, int courseNumber, string teacher, string time, string day, int enrollment, int capacity)
{
    ClassNode *temp = root; //set temp to begin it's search at root
    ClassNode *parent = NULL;

    ClassNode *newnode = new ClassNode(name, department, courseNumber, teacher, time, day, enrollment, capacity); //node that temp is being compared against
    newnode->left = NULL;
    newnode->right = NULL;
    newnode->parent = NULL;

    while(temp != NULL)
    {
        parent = temp;

        if(newnode->name.compare(temp->name) < 0){
            temp = temp->left;
        }
        else{
            temp = temp->right;
        }
    }
    if (parent == NULL){
        root = newnode;
    }
    else if(newnode->name.compare(parent->name) < 0){
        parent->left = newnode;
        newnode->parent = parent;
    }
    else{
        parent->right = newnode;
        newnode->parent = parent;
    }
}

void ClassTree::printClassInventory(){
    printClassInventory(root);
}

void ClassTree::printClassInventory(ClassNode *node){
    if(node->left != NULL){
        printClassInventory(node->left);
    }
    cout<<"Class: "<<node->name<<" ("<<node->department<<" "<<node->courseNumber<<")"<<endl;
    cout<<"Teacher: "<<node->teacher<<endl;
    cout<<"Time: "<< node->time<<", "<<node->day<<endl;
    if(node->enrollment == node->capacity){
        cout<<"This class is full."<<endl;
    }
    else if(node->enrollment == node->capacity-1){
        cout<<"There is 1 spot left in this class."<<endl;
    }
    else{
        int space = node->capacity-node->enrollment;
        //cout<<"Enrollment/Capacity: "<<node->enrollment<<"/"<<node->capacity<<endl;
        cout<<"There are "<<space<<" spots left in this class."<<endl;
    }
    cout<<"-----------"<<endl;
    if(node->right != NULL){
        printClassInventory(node->right);
    }
}


ClassNode* ClassTree::search(string name){
    ClassNode *temp = root;

    while(temp != NULL){

        if(name.compare(temp->name) < 0){
            temp = temp->left;
        }
        else if (name.compare(temp->name) > 0){
            temp = temp->right;
        }
        else{
            return temp;
        }
    }
    return NULL;
}

bool ClassTree::classConflict(string title, string schedule)
{
    int t1, t2, c1, c2;
    classTimes(t1,t2,title);//Class time of enrolling class
    classTimes(c1,c2, schedule);//class time of class on schedule

    if(c1 >= t1 && c1 <= t2)
        return true;
    else if(c2 >= t1 && c2 <=t2)
        return true;
    else
        return false;
}

/**Checks if the searched/enrolled class conflicts with schedule*/
void ClassTree::classTimes(int& time1, int& time2, string class1)
{
    int num1, num2;
    int counter = 0;
    for(int i = 0; i < class1.length(); i++)
    {
        if(i == 0 || class1[i-1] == '-')//(this number):xx
        {
            num1 = class1[i] - 48;
            if(class1[i+1] != ':') //For two digit cases
            {
                num1 *= 10;
                num1 += ((int)class1[i+1]-48);//e.g. 11 or 12 o'clock
            }
            else //if(num1 < 7 && num1 > 0) //Converting to military time 1pm = 13 --1pm-6pm
                num1 += 12;
        }
        else if(class1[i-1] == ':')//  xx:(This number)
        {
            num2 = 10*((int)class1[i]-48);
            num2 += ((int)class1[i+1]-48);
            if(counter == 0)
            {
                time1 = 100*num1 + num2;
                counter++;
            }
            else
            {
                time2 = 100*num1 + num2;
                //cout<<time1<<" && "<<time2<<endl;
                return;
            }
        }
    }

}

void ClassTree::enrollClass(string title){
    ClassNode *foundClass = search(title);

    if(foundClass != NULL){
        if(foundClass->enrollment == foundClass->capacity){
            cout << foundClass-> name <<" is full. You cannot enroll in this class." <<endl;
        }
        else{

            bool found = false;

            for(int i = 0; i < schedule.size(); i++){ //check if they already signed up for it
                cout<<schedule[i].day<<"   "<<foundClass->day<<endl;
                if(schedule[i].name == title){
                    found = true;
                    cout<<"You are already signed up for "<<schedule[i].name<<"."<<endl;
                }
                //Checks if class conflict with each other
                else if(schedule[i].day == foundClass->day)
                {
                    if(classConflict(foundClass->time, schedule[i].time) == true)
                    {
                        found = true;
                        cout<<"You can't add this class because it conflicts with "<<schedule[i].name<<"."<<endl;
                    }
                }
            }
            if(!found){
                addClassToSchedule(foundClass->name, foundClass->department, foundClass->courseNumber, foundClass->teacher, foundClass->time, foundClass->day);
                foundClass->enrollment++;
                cout << foundClass->name << " has been successfully added to your schedule." << endl;
            }
        }
    }
    else{
        cout << "Class not found." << endl;
    }
}

void ClassTree::addClassToSchedule(string name, string department, int courseNumber, string teacher, string time, string day){

    ClassNode newclass;
    newclass.name = name;
    newclass.department = department;
    newclass.courseNumber = courseNumber;
    newclass.teacher = teacher;
    newclass.time = time;
    newclass.day = day;
    schedule.push_back(newclass);

}

void ClassTree::dropClass(string title){

    bool found = false;

    for(int i = 0; i < schedule.size(); i++){
        if(schedule[i].name == title){
            found = true;
            schedule.erase(schedule.begin() + i);
            cout<<title<< " has been successfully removed from your schedule."<<endl;
        }
    }

    if(!found){
        cout<<title<< " was not found in your schedule."<<endl;
    }

    //increase class enrollment
    if(found){
        ClassNode *foundClass = search(title);
        foundClass->enrollment--;
    }

}

void ClassTree::printClassSchedule(){
    if(schedule.size() == 0){
        cout<<"Your schedule is currently empty. Select option 2 to start adding classes to your schedule."<<endl;
    }
    else{
        for(int i = 0; i < schedule.size(); i++){
            cout << "Class: "<< schedule[i].name<< " (" << schedule[i].department << " " << schedule[i].courseNumber << ")" << endl;
            cout << "Teacher: " << schedule[i].teacher << endl;
            cout << "Time: " << schedule[i].time << ", " << schedule[i].day <<endl;
            cout<<"-----------"<<endl;
        }
    }
}
