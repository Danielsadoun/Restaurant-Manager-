//
// Created by danielpi on 11/11/18.
//

#include "Table.h"
#include "Customer.h"

Table:: Table(int t_capacity) : capacity(t_capacity), open(false), customersList{}, orderList{}, lastOrder{} {}

Table:: Table (const Table &other) : capacity(other.capacity), open(other.open), customersList{}, orderList{}, lastOrder{} {
    for(unsigned i = 0; i < other.customersList.size(); i++){
        customersList.push_back(other.customersList[i]->copy());
    }
    for(unsigned i = 0; i < other.orderList.size(); i++){
        orderList.push_back(other.orderList[i]);
    }
}

Table:: Table(Table&& other) : capacity(other.capacity), open(other.isOpen()), customersList{}, orderList{}, lastOrder{} {
    for(unsigned i = 0; i < other.customersList.size(); i++){
        customersList.push_back(other.customersList[i]);
    }
    for(unsigned i = 0; i < other.customersList.size(); i++){
        other.customersList[i] = nullptr;
    }
    other.customersList.clear();
    for(unsigned i = 0; i < other.orderList.size(); i++){
        orderList.push_back(other.orderList[i]);
    }
}

Table& Table:: operator=(Table &other){
    if(this == &other){ return *this; }
    capacity = other.capacity;
    open = other.isOpen();
    for(unsigned i = 0; i < customersList.size(); i++){
        customersList[i]= nullptr;
    }
    customersList.clear();
    for(unsigned i = 0; i < other.customersList.size(); i++){
        customersList.push_back(other.customersList[i]->copy());
    }
    orderList.clear();
    for(unsigned i = 0; i < other.orderList.size(); i++){
        orderList.push_back(other.orderList[i]);
    }
    return *this;
}

Table Table:: operator=(Table&& other){
    if(this == &other){ return *this; }
    capacity = other.capacity;
    open = other.isOpen();
    for(unsigned i = 0; i < customersList.size(); i++){
        customersList[i]= nullptr;
    }
    customersList.clear();
    for(unsigned i = 0; i < other.customersList.size(); i++){
        customersList.push_back(other.customersList[i]);
    }
    for(unsigned i = 0; i < other.customersList.size(); i++){
        other.customersList[i] = nullptr;
    }
    other.customersList.clear();
    orderList.clear();
    for(unsigned i = 0; i < other.orderList.size(); i++){
        orderList.push_back(other.orderList[i]);
    }
    return *this;
}

Table:: ~Table(){
    for(unsigned i = 0; i < customersList.size(); i++){
        this->removeCustomer(customersList[i]->getId());
    }
    for(unsigned i = 0; i < customersList.size(); i++){
        customersList[i] = nullptr;
    }
    customersList.clear();
    orderList.clear();
    lastOrder.clear();
}

int Table:: getCapacity() const{
    return capacity;
}

void Table:: addCustomer(Customer* customer){//add customer to the end of the vector
    customersList.push_back(customer);
}

void Table:: removeCustomer(int id){
    for(unsigned i =0 ; i < customersList.size(); i++){
        if(customersList[i]->getId() == id){
            delete(customersList[i]);
        }
    }
}

Customer* Table:: getCustomer(int id){
    if(customersList.size() == 0){
        return nullptr;
    }
    else{
        bool enter = false;
        Customer* c;
        for(unsigned i = 0; i < customersList.size(); i++) {
            if (customersList[i]->getId() == id) {
                enter = true;
                c = customersList[i]->copy();
            }
        }
        if(enter == true){
            return c;
        } else{
            return nullptr;
        }
    }
}

std::vector<Customer*>& Table:: getCustomers(){
    return customersList;
}

std::vector<OrderPair>& Table:: getOrders(){
    return orderList;
}

std::string Table::getStatus() {
    if (isOpen())
        return "open";
    else
        return "close";
}

void Table:: order(const std::vector<Dish> &menu){
    lastOrder.clear();
    for(unsigned i = 0; i < customersList.size(); i++){
        std:: vector<int> cusOL = customersList[i]->order(menu);
        for(unsigned j = 0; j < cusOL.size(); j++){
            for(unsigned k = 0; k < menu.size(); k++){
                if(menu[k].getId()==cusOL[j]){
                    OrderPair pair (customersList[i]->getId(),menu[k]);
                    orderList.push_back(pair);
                    lastOrder.push_back(pair);
                }
            }
        }
    }
}

bool Table:: CusInTheTable (int id)  {   // get customer id and check if he is in the table
    bool isExist = false;
    for (unsigned i = 0 ; customersList.size() && !(isExist) ; i++){
        if (customersList[i]->getId() == id)
            isExist = true;
    }
    return isExist;
}

void Table:: openTable(){
    open = true;
}

void Table:: closeTable() {
    open = false;
}

int Table:: getBill(){
    int bill = 0;
    for (unsigned i = 0 ; i<orderList.size() ; i++){
        bill = bill + orderList[i].second.getPrice();
    }
    return bill;


}

bool Table:: isOpen(){
    return open;
}

std::vector<OrderPair> Table:: getLastOrder(){
    return lastOrder;
}

Table* Table:: copy(){
    Table* t = new Table(this->getCapacity());
    t->open = this->isOpen();
    for(unsigned i = 0; i < this->getCustomers().size(); i++ ){
        t->getCustomers().push_back(this->getCustomers()[i]->copy());
    }
    for(unsigned i = 0; i < this->getOrders().size(); i++){
        t->getOrders().push_back(this->getOrders()[i]);
    }
    return t;
}