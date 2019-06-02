//
// Created by danielpi on 11/11/18.
//

#include "Restaurant.h"

#include "Dish.h"
#include "Table.h"
#include "Action.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

extern Restaurant* backup;


Restaurant::Restaurant(): open(false), tables{}, menu{}, actionsLog{}, lastId(-1), newList{} {
}

Restaurant::Restaurant(const std::string &configFilePath): open(false), tables{}, menu{}, actionsLog{} , lastId(-1), newList{} {
    std::ifstream myfile;
    myfile.open(configFilePath);
    std:: string line;
    while(myfile.is_open()){
        bool foundNum = false;
        while( !foundNum && getline(myfile, line)) {
            if (line != "/r" && line.find("#") == std::string::npos) {
                foundNum = true;
            }
        }
        bool foundCap = false;
        while(!foundCap && getline(myfile,line)) {
            if (line != "/r" && line.find("#") == std::string::npos) {
                addTables(line);
                foundCap = true;
            }
        }
        while(getline(myfile, line)) {
            if (line != "/r" && line.find("#") == std::string::npos) {
                addDish(line);
            }
        }
        myfile.close();
    }
}

void Restaurant:: addTables(std:: string tablestr){
    std:: stringstream capLine(tablestr);
    std:: string cap;
    std:: vector<int> capacities;
    while (getline(capLine, cap, ',')){
        capacities.push_back(std:: stoi(cap));
    }
    for(unsigned i = 0; i < capacities.size(); i++){
        Table* newTable = new Table(capacities[i]);
        tables.push_back(newTable);
    }
}


void Restaurant:: addDish (std:: string dish){
    std:: stringstream dishForAdd(dish);
    std:: string character;
    std:: vector<std:: string> characters;
    while (getline(dishForAdd, character, ',')){
        characters.push_back(character);
    }
    DishType t;
    if(characters[1] == "ALC"){
        t = ALC;
    }
    else if(characters[1] == "VEG"){
        t = VEG;
    }
    else if(characters[1] == "BVG"){
        t = BVG;
    }
    else if(characters[1] == "SPC"){
        t = SPC;
    }
    Dish newDish (menu.size(), characters[0], std:: stoi(characters[2]), t);
    menu.push_back(newDish);
}

Restaurant:: Restaurant(const Restaurant &other): open(other.open), tables{}, menu{}, actionsLog{} , lastId(other.lastId), newList{} {
    for(unsigned i = 0; i < other.tables.size(); i++){
        tables.push_back(other.tables[i]->copy());
    }
    for(unsigned i = 0; i < other.menu.size(); i++){
        Dish d (other.menu[i].getId(),other.menu[i].getName(),other.menu[i].getPrice(),other.menu[i].getType());
        menu.push_back(d);
    }
    for(unsigned i = 0; i < other.actionsLog.size(); i++){
        actionsLog.push_back(other.actionsLog[i]->Copy());
    }
}

Restaurant:: Restaurant(Restaurant&& other): open(other.open), tables{}, menu{}, actionsLog{} , lastId(other.lastId), newList{} {
    for(unsigned i = 0 ; i < other.tables.size(); i++){
        tables.push_back(other.tables[i]);
    }
    for(unsigned i = 0 ; i < other.tables.size(); i++){
        other.tables[i] = nullptr;
    }
    other.tables.clear();
    for(unsigned i = 0 ; i < other.actionsLog.size(); i++){
        actionsLog.push_back(other.actionsLog[i]);
    }
    for(unsigned i = 0 ; i < other.actionsLog.size(); i++){
        other.actionsLog[i] = nullptr;
    }
    other.actionsLog.clear();
    for(unsigned i = 0 ; i < other.menu.size(); i++){
        menu.push_back(other.menu[i]);
    }
}

Restaurant& Restaurant:: operator=(Restaurant &other){
    if(this == &other){
        return *this;
    }
    open = other.open;
    lastId = other.lastId;
    menu.clear();
    for(unsigned i = 0; i < other.menu.size(); i++){
        Dish d (other.menu[i].getId(),other.menu[i].getName(),other.menu[i].getPrice(),other.menu[i].getType());
        menu.push_back(d);
    }
    for(unsigned i = 0 ; i < tables.size(); i++){
        tables[i] = nullptr;
    }
    tables.clear();
    for(unsigned i = 0; i < other.tables.size(); i++){
        tables.push_back(other.tables[i]->copy());
    }
    for(unsigned i = 0 ; i < actionsLog.size(); i++){
        actionsLog[i] = nullptr;
    }
    actionsLog.clear();
    for(unsigned i = 0; i < other.actionsLog.size(); i++){
        actionsLog.push_back(other.actionsLog[i]->Copy());
    }
    return *this;
}

Restaurant Restaurant:: operator=(Restaurant&& other){
    if(this == &other){ return *this; }
    open = other.open;
    lastId = other.lastId;
    for(unsigned i = 0 ; i < tables.size(); i++){
        tables[i] = nullptr;
    }
    tables.clear();
    for(unsigned i = 0 ; i < other.tables.size(); i++){
        tables.push_back(other.tables[i]);
    }
    for(unsigned i = 0 ; i < other.tables.size(); i++){
        other.tables[i] = nullptr;
    }
    other.tables.clear();
    for(unsigned i = 0 ; i < actionsLog.size(); i++){
        actionsLog[i] = nullptr;
    }
    actionsLog.clear();
    for(unsigned i = 0 ; i < other.actionsLog.size(); i++){
        actionsLog.push_back(other.actionsLog[i]);
    }
    for(unsigned i = 0 ; i < other.actionsLog.size(); i++){
        other.actionsLog[i] = nullptr;
    }
    other.actionsLog.clear();
    menu.clear();
    for(unsigned i = 0 ; i < other.menu.size(); i++){
        menu.push_back(other.menu[i]);
    }
    return *this;
}

Restaurant:: ~Restaurant(){
    this->lastId = -1;
    this->open = false;
    for (unsigned i = 0; i < tables.size(); i++) {
        delete (tables[i]);
    }
    for (unsigned i = 0; i < tables.size(); i++) {
        tables[i] = nullptr;
    }
    tables.clear();
    for (unsigned i = 0 ; i < actionsLog.size() ; i++) {
        delete(actionsLog[i]);
    }
    for(unsigned i = 0; i < actionsLog.size(); i++){
        actionsLog[i] = nullptr;
    }
    actionsLog.clear();
    menu.clear();
}

void Restaurant:: start(){
    open = true;
    std:: cout << "Restaurant is now open!" <<std::endl;
    bool isClose = false;
    while (!isClose && open){
        std::string s;
        getline(std::cin,s);
        BaseAction* action = newAction(s);
        action->act(*this);
        actionsLog.push_back(action);
        for(unsigned i = 0; i < newList.size(); i++){
            newList[i] = nullptr;
        }
        newList.clear();
        if(s == "closeall"){
            isClose = true;
        }
    }
}

BaseAction* Restaurant:: newAction(std::string s) {
    std::string::size_type st;
    std::istringstream tokenStream(s);
    std::string space;
    std::vector<std::string> actions;
    BaseAction* action;
    while (getline(tokenStream, space, ' ')) {
            actions.push_back(space);
    }
    if ((actions[0] == "open")) {
        int i = stoi(actions[1], &st);
        actions.erase(actions.begin(), actions.begin() + 2);
        std::vector<Customer *> &customers = addCus(actions);
        action = new OpenTable(i, customers);
        return action;
    }
    if (actions[0] == "order") {
        int i = stoi(actions[1], &st);
        action = new Order(i);
        return action;
    }
    if (actions[0] == "move"){
        int src = stoi(actions[1],&st);
        int dst = stoi(actions[2],&st);
        int id = stoi(actions[3],&st);
        action = new MoveCustomer(src,dst,id);
        return action;
    }
    if (actions[0] == "status"){
        int id = stoi(actions[1],&st);
        action = new PrintTableStatus (id);
        return action;
    }
    if(actions[0] == "close") {
        int id = stoi(actions[1], &st);
        action = new Close(id);
        return action;
    }
    if(actions[0] == "closeall"){
        action = new CloseAll();
        return action;
    }
    if(actions[0] == "menu"){
        int id = stoi(actions[1],&st);
        action = new PrintTableStatus(id);
        return action;
    }
    if(actions[0] == "log"){
        action = new PrintActionsLog();
        return action;
    }
    if(actions[0] == "backup"){
        action = new BackupRestaurant();
        return action;
    }
    if(actions[0] == "restore"){
        action = new RestoreResturant();
        return action;
    }
    return nullptr;
}

std::vector<Customer*>& Restaurant:: addCus (std::vector<std::string> v){
    for (unsigned i = 0 ; i < v.size() ; i++){
        std::string s1 = ",";
        size_t split = v[i].find(s1);
        if (split != (unsigned)-1){
            std::string n = v[i].substr(0,split);
            std::string t = v[i].substr(split+1);
            if (t == "veg"){
                lastId = lastId+1;
                VegetarianCustomer* customer = new VegetarianCustomer(n,lastId);
                newList.push_back(customer);
            }
            else if (t == "chp"){
                lastId = lastId+1;
                CheapCustomer* customer = new CheapCustomer(n,lastId);
                newList.push_back(customer);
            }
            else if (t == "spc"){
                lastId = lastId+1;
                SpicyCustomer* customer = new SpicyCustomer(n,lastId);
                newList.push_back(customer);
            }
            else if (t == "alc"){
                lastId = lastId+1;
                AlchoholicCustomer* customer = new AlchoholicCustomer(n,lastId);
                newList.push_back(customer);
            }
        }
    }
    return newList;
}


int Restaurant:: getNumOfTables() const{
    return tables.size();
}

Table* Restaurant:: getTable(int ind){
    return tables[ind];
}


const std::vector<BaseAction*>& Restaurant:: getActionsLog() const{
    return actionsLog;
}

std::vector<Dish>& Restaurant:: getMenu(){
    return menu;
}

std:: vector <Customer*> Restaurant:: getNewList(){
    return newList;
};

