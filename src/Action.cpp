//
// Created by danielpi on 11/11/18.
//

#include <string>
#include <iostream>
#include "Action.h"
#include "Table.h"
#include "Restaurant.h"
#include "Dish.h"
#include "Customer.h"

extern Restaurant* backup;

BaseAction:: BaseAction() : errorMsg(""), status(PENDING) {}

ActionStatus BaseAction:: getStatus() const{
    return status;
}

void BaseAction:: complete(){
    status = COMPLETED;
}

void BaseAction:: error(std::string errorMsg){
    status = ERROR;
    this-> errorMsg = errorMsg;
}

std::string BaseAction:: getErrorMsg() const{
    return errorMsg;
}

std::string BaseAction:: getStringStatus() const{
    std:: string s;
    if(getStatus() == ERROR){
        s = " Error: " + getErrorMsg();
    }
    else if(getStatus() == COMPLETED){
        s = " Completed";
    }
    else{
        s = " Pending";
    }
    return s;
}

void BaseAction:: setStatus(ActionStatus s){
    status = s;
}

void BaseAction:: setErrorMsg(std::string s){
    errorMsg = s;
}



OpenTable:: OpenTable(int id, std::vector<Customer *> &customersList) : BaseAction(), tableId(id), customers{} {
    for(unsigned i = 0; i < customersList.size(); i++){
        customers.push_back(customersList[i]->copy());
    }
}

OpenTable:: OpenTable(const OpenTable &other) : BaseAction(), tableId(other.tableId), customers{} {
    for(unsigned i = 0; i < other.customers.size(); i++){
        customers.push_back(other.customers[i]->copy());
    }
}

OpenTable:: OpenTable(OpenTable&& other) : BaseAction(), tableId(other.tableId), customers{} {
    for(unsigned i = 0; i < other.customers.size(); i++){
        customers.push_back(other.customers[i]);
    }
    for(unsigned i = 0; i < other.customers.size(); i++){
        other.customers[i] = nullptr;
    }
    other.customers.clear();
}


OpenTable:: ~OpenTable(){
    for(unsigned i = 0; i < customers.size(); i++){
        customers[i] = nullptr;
    }
    customers.clear();
}

void OpenTable:: act(Restaurant &restaurant){
    if(restaurant.getNumOfTables()-1 < tableId || restaurant.getTable(tableId)->isOpen()){
        std:: cout << "Error: Table does not exist or already open" << std:: endl;
        error("Table does not exist or already open");
        for(unsigned i = 0 ; i < restaurant.getNewList().size(); i++){
            delete(restaurant.getNewList()[i]);
        }
    }
    else{
        restaurant.getTable(tableId)->openTable();
        for(unsigned i = 0; i < customers.size(); i++){
            restaurant.getTable(tableId)->addCustomer(customers[i]);
        }
        complete();
    }
}

std::string OpenTable:: toString() const{
    std::string s;
    s= "open " + std:: to_string(tableId);
    for(unsigned i = 0; i < customers.size(); i++){
        s = s +  " " + customers[i]->toString();
    }
    s = s + getStringStatus();
    return s;
}

BaseAction* OpenTable:: Copy(){
    BaseAction* a = new OpenTable(tableId,customers);
    a->setStatus(this->getStatus());
    a->setErrorMsg(this->getErrorMsg());
    return a;
}



Order:: Order(int id) : BaseAction(), tableId(id) {}

void Order:: act(Restaurant &restaurant){
    if(restaurant.getNumOfTables()-1 < tableId || !(restaurant.getTable(tableId)->isOpen())){
        std:: cout << "Error: Table does not exist or is not open" << std:: endl;
        error("Table does not exist or is not open");
    }
    else{
        restaurant.getTable(tableId)->order(restaurant.getMenu());
        complete();
        for (unsigned i = 0 ; i < restaurant.getTable(tableId)->getLastOrder().size() ; i++){
            std::cout<<restaurant.getTable(tableId)->getCustomer(restaurant.getTable(tableId)->getLastOrder()[i].first)->getName()<<" ordered "<< restaurant.getTable(tableId)->getLastOrder()[i].second.getName()<<std::endl;
        }
    }
}

std::string Order:: toString() const{
    std::string s;
    s = "order " + std::to_string(tableId) + getStringStatus();
    return s;
}

BaseAction* Order:: Copy(){
    BaseAction* a = new Order(tableId);
    a->setStatus(this->getStatus());
    a->setErrorMsg(this->getErrorMsg());
    return a;
}


MoveCustomer:: MoveCustomer(int src, int dst, int customerId) : BaseAction(), srcTable(src), dstTable(dst), id(customerId) {}

void MoveCustomer:: act(Restaurant &restaurant){
    if (restaurant.getNumOfTables()-1 < srcTable  || restaurant.getNumOfTables()-1 < dstTable || !(restaurant.getTable(srcTable)->isOpen()) || !(restaurant.getTable(dstTable)->isOpen() || !(restaurant.getTable(srcTable)->CusInTheTable(id))  || (unsigned) restaurant.getTable(dstTable)->getCapacity() == restaurant.getTable(dstTable)->getCustomers().size())) {
        std::cout << "Error: Cannot move customer" << std:: endl;
        error("Cannot move customer");
    }
    else {
        for (unsigned i = 0 ; i < restaurant.getTable(srcTable)->getCustomers().size() ; i++){     // move the customer
            if (restaurant.getTable(srcTable)->getCustomers()[i]->getId() == id){
                restaurant.getTable(dstTable)->getCustomers().push_back(restaurant.getTable(srcTable)->getCustomers()[i]);
                restaurant.getTable(srcTable)->getCustomers().erase(restaurant.getTable(srcTable)->getCustomers().begin()+i);
            }
        }
        std:: vector<OrderPair> toSave;
        for (unsigned i = 0 ; i < restaurant.getTable(srcTable)->getOrders().size() ; i++){         // move the customer's order
            if (restaurant.getTable(srcTable)->getOrders()[i].first == id){
                restaurant.getTable(dstTable)->getOrders().push_back(restaurant.getTable(srcTable)->getOrders()[i]);
            }
            else{
                toSave.push_back(restaurant.getTable(srcTable)->getOrders()[i]);
            }
        }
        restaurant.getTable(srcTable)->getOrders().clear();
        for(unsigned i = 0; i < toSave.size(); i++){
            restaurant.getTable(srcTable)->getOrders().push_back(toSave[i]);
        }
        if(restaurant.getTable(srcTable)->getCustomers().size() == 0){
            restaurant.getTable(srcTable)->closeTable();
        }
        complete();
    }
}

std::string MoveCustomer:: toString() const{
    std:: string s = "move " + std::to_string(srcTable)+" "+ std::to_string(dstTable)+" " + std::to_string(id) + " " + getStringStatus();
    return s;
}

BaseAction* MoveCustomer:: Copy(){
    BaseAction* a = new MoveCustomer(srcTable,dstTable,id);
    a->setStatus(this->getStatus());
    a->setErrorMsg(this->getErrorMsg());
    return a;
}


Close:: Close(int id): BaseAction(), tableId(id) {}

void Close:: act(Restaurant &restaurant){
    if(restaurant.getNumOfTables()-1 < tableId || !(restaurant.getTable(tableId)->isOpen())){
        std:: cout << "Error: Table does not exist or isnt open" << std:: endl;
        error("Table does not exist or isnt open");
    }
    else{
        std::cout<<"Table "<< tableId << " was closed. "<< "Bill "<<restaurant.getTable(tableId)->getBill()<<"NIS"<<std::endl;
        restaurant.getTable(tableId)->closeTable();
        complete();
    }
}

std::string Close:: toString() const{
    std::string s;
    s = "close table " + std::to_string(tableId) + " " + getStringStatus();
    return s;
}

BaseAction* Close:: Copy(){
    BaseAction* a = new Close(tableId);
    a->setStatus(this->getStatus());
    a->setErrorMsg(this->getErrorMsg());
    return a;
}


CloseAll:: CloseAll() : BaseAction() {}

void CloseAll:: act(Restaurant &restaurant){
    for (int i = 0 ; i < restaurant.getNumOfTables() ; i++){
        if (restaurant.getTable(i)->isOpen()) {
            BaseAction* c = new Close(i);
            c->act(restaurant);
            delete(c);
        }
    }
    complete();
}

std::string CloseAll:: toString() const{
    std::string s = "close all " + getStringStatus();
    return s;
}

BaseAction* CloseAll:: Copy(){
    BaseAction* a = new CloseAll();
    a->setStatus(this->getStatus());
    a->setErrorMsg(this->getErrorMsg());
    return a;
}


PrintMenu:: PrintMenu():BaseAction() {}

void PrintMenu:: act(Restaurant &restaurant){
    for (unsigned i = 0 ; i < restaurant.getMenu().size() ; i++){
        std:: cout<< restaurant.getMenu()[i].getName()<<" "<< restaurant.getMenu()[i].getType()<<" "<<restaurant.getMenu()[i].getPrice()<< "NIS"<< std::endl;
    }
    complete();
}

std::string PrintMenu:: toString() const{
    std::string s = "print menu "+ getStringStatus();
    return s;
}

BaseAction* PrintMenu:: Copy(){
    BaseAction* a = new PrintMenu;
    a->setStatus(this->getStatus());
    a->setErrorMsg(this->getErrorMsg());
    return a;
}


PrintTableStatus:: PrintTableStatus(int id): BaseAction(), tableId(id) {}

void PrintTableStatus:: act(Restaurant &restaurant){
    std::cout<<"Table "<<tableId<< " status: "<<restaurant.getTable(tableId)->getStatus()<<std::endl;
    if(restaurant.getTable(tableId)->isOpen()){
        std::cout<<"Customers:"<< std::endl;
        for (unsigned i = 0 ; i < restaurant.getTable(tableId)->getCustomers().size() ; i++){
            std::cout<< restaurant.getTable(tableId)->getCustomers()[i]->getId()<< " "<< restaurant.getTable(tableId)->getCustomers()[i]->getName()<<std::endl;
        }
        std::cout<<"Orders:" << std::endl;
        for (unsigned i = 0 ; i<restaurant.getTable(tableId)->getOrders().size(); i++){
            std::cout<< restaurant.getTable(tableId)->getOrders()[i].second.getName()<<" "<< restaurant.getTable(tableId)->getOrders()[i].second.getPrice()<< "NIS "<< restaurant.getTable(tableId)->getOrders()[i].first<<std::endl;
        }
        std:: cout<< "Current Bill: " << restaurant.getTable(tableId)->getBill() << "NIS" << std::endl;
    }
    complete();
}

std::string PrintTableStatus:: toString() const{
    std::string s = "status " + std:: to_string(tableId) + getStringStatus();
    return s;
}

BaseAction* PrintTableStatus:: Copy(){
    BaseAction* a = new PrintTableStatus(tableId);
    a->setStatus(this->getStatus());
    a->setErrorMsg(this->getErrorMsg());
    return a;
}



PrintActionsLog:: PrintActionsLog(): BaseAction() {}

void PrintActionsLog:: act(Restaurant &restaurant){
    for (unsigned i = 0 ; i < restaurant.getActionsLog().size(); i++){
        std::cout<<restaurant.getActionsLog()[i]->toString()<< std::endl;
    }
    complete();
}

std::string PrintActionsLog:: toString() const{
    std:: string s = "PrintActionsLog " + getStringStatus();
    return s;
}

BaseAction* PrintActionsLog:: Copy(){
    BaseAction* a = new PrintActionsLog();
    a->setStatus(this->getStatus());
    a->setErrorMsg(this->getErrorMsg());
    return a;
}


BackupRestaurant:: BackupRestaurant() : BaseAction() {}

void BackupRestaurant:: act(Restaurant &restaurant){
    delete(backup);
    backup = new Restaurant(restaurant);
    complete();
}

std::string BackupRestaurant:: toString() const{
    std:: string s = "backup" +  getStringStatus();
    return s;
}

BaseAction* BackupRestaurant:: Copy(){
    BaseAction* a = new BackupRestaurant();
    a->setStatus(this->getStatus());
    a->setErrorMsg(this->getErrorMsg());
    return a;
}


RestoreResturant:: RestoreResturant() : BaseAction() {}

void RestoreResturant:: act(Restaurant &restaurant){
    if (backup == nullptr){
        std::cout<<"Error: <No backup available>";
        error("No backup available");
    }
    else {
        restaurant = *backup;
        complete();
    }

}

std::string RestoreResturant:: toString() const{
    std:: string s = "restore " + getStringStatus();
    return s;
}

BaseAction* RestoreResturant:: Copy(){
    BaseAction* a = new RestoreResturant();
    a->setStatus(this->getStatus());
    a->setErrorMsg(this->getErrorMsg());
    return a;
}


