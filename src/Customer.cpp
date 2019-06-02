//
// Created by danielpi on 11/11/18.
//

#include "Customer.h"
#include "Dish.h"

Customer:: Customer(std::string c_name, int c_id) : name(c_name), id(c_id) {}

std::string Customer:: getName() const{
    return name;
}

int Customer:: getId() const{
    return id;
}



VegetarianCustomer:: VegetarianCustomer(std::string name, int id) : Customer(name, id){}


std::vector<int> VegetarianCustomer:: order(const std::vector<Dish> &menu){
    std:: vector<int> orderList;
    int tmpID = -1;
    int tmPrice = -1;
    for(unsigned i = 0; i < menu.size(); i++){
        if(menu[i].getType() == VEG){
            if(tmpID == -1){
                tmpID = menu[i].getId();
            }
            else{
                if(menu[i].getId() < tmpID){
                    tmpID = menu[i].getId();
                }
            }
        }
    }
    if(tmpID != -1){
        orderList.push_back(tmpID);
        tmpID = -1;
    }
    for(unsigned i = 0; i < menu.size(); i++){
        if(menu[i].getType() == BVG){
            if(menu[i].getPrice() > tmPrice){
                tmPrice = menu[i].getPrice();
                tmpID = menu[i].getId();
            }
            else if(menu[i].getPrice() == tmPrice){
                if(menu[i].getId() < tmpID){
                    tmpID = menu[i].getId();
                }
            }
        }
    }
    if(tmpID != -1){
        orderList.push_back(tmpID);
    }
    return orderList;
}

std::string VegetarianCustomer:: toString() const{
    std::string s = getName() + "," + "veg";
    return s;
}

Customer* VegetarianCustomer:: copy (){
    VegetarianCustomer* c = new VegetarianCustomer (this->getName(),this->getId());
    return c;
}



CheapCustomer:: CheapCustomer(std::string name, int id) : Customer(name, id), ordered(false) {}


std::vector<int> CheapCustomer:: order(const std::vector<Dish> &menu){
    std:: vector<int> orderList;
    if(ordered){
        return orderList;
    }
    else{
        int tmPrice = -1;
        int tmpID = -1;
        for(unsigned i = 0; i < menu.size(); i++){
            if(tmPrice == -1){
                tmPrice = menu[i].getPrice();
                tmpID = menu[i].getId();
            }
            else{
                if(menu[i].getPrice() < tmPrice){
                    tmPrice = menu[i].getPrice();
                    tmpID = menu[i].getId();
                }
                else if(menu[i].getPrice() == tmPrice){
                    if(menu[i].getId() < tmpID){
                        tmpID = menu[i].getId();
                    }
                }
            }
        }
        if(tmpID != -1){
            orderList.push_back(tmpID);
        }
        ordered = true;
        return  orderList;
    }
}

std::string CheapCustomer:: toString() const{
    std::string s = getName() + "," + "chp";
    return s;
}

Customer* CheapCustomer:: copy (){
    CheapCustomer* c = new CheapCustomer (this->getName(),this->getId());
    c->ordered = this->ordered;
    return c;
}


SpicyCustomer:: SpicyCustomer(std::string name, int id) : Customer(name,id), numOfOrder(1) {}


std::vector<int> SpicyCustomer:: order(const std::vector<Dish> &menu){
    std:: vector<int> orderList;
    int tmpID = -1;
    int tmPrice = -1;
    if(numOfOrder == 1){
        for(unsigned i = 0; i < menu.size(); i++){
            if(menu[i].getType() == SPC){
                if(menu[i].getPrice() > tmPrice){
                    tmPrice = menu[i].getPrice();
                    tmpID = menu[i].getId();
                }
                else if(menu[i].getPrice() == tmPrice){
                    if(menu[i].getId() < tmpID){
                        tmpID = menu[i].getId();
                    }
                }
            }
        }
    }
    else{
        for(unsigned i = 0; i < menu.size(); i++){
            if(menu[i].getType() == BVG){
                if(tmPrice == -1){
                    tmPrice = menu[i].getPrice();
                    tmpID = menu[i].getId();
                }
                else{
                    if(menu[i].getPrice() < tmPrice){
                        tmPrice = menu[i].getPrice();
                        tmpID = menu[i].getId();
                    }
                    else if(menu[i].getPrice() == tmPrice){
                        if(menu[i].getId() < tmpID){
                            tmpID = menu[i].getId();
                        }
                    }
                }
            }
        }
    }
    if(tmpID != -1){
        orderList.push_back(tmpID);
        numOfOrder = numOfOrder+1;
    }
    return orderList;
}

std::string SpicyCustomer:: toString() const{
    std::string s = getName() + "," + "spc";
    return s;
}

Customer* SpicyCustomer:: copy (){
    SpicyCustomer* c = new SpicyCustomer (this->getName(),this->getId());
    c->numOfOrder = this->numOfOrder;
    return c;
}



AlchoholicCustomer:: AlchoholicCustomer(std::string name, int id): Customer(name,id), numOfOrder(1), ALCdishes{} {}


std::vector<int> AlchoholicCustomer:: order(const std::vector<Dish> &menu){
    std:: vector<int> orderList;
    if(numOfOrder == 1){
        for(unsigned i = 0; i < menu.size(); i++){
            if(menu[i].getType() == ALC){
                ALCdishes.push_back(menu[i]);
            }
        }
    }
    int tmPrice = -1;
    int tmpID = -1;
    int ind;
    for(unsigned i = 0; i < ALCdishes.size(); i++){
        if(tmPrice == -1){
            tmPrice = ALCdishes[i].getPrice();
            tmpID = ALCdishes[i].getId();
            ind = i;
        }
        else{
            if(ALCdishes[i].getPrice() < tmPrice){
                tmPrice = ALCdishes[i].getPrice();
                tmpID = ALCdishes[i].getId();
                ind = i;
            }
            else if(ALCdishes[i].getPrice() == tmPrice){
                if(ALCdishes[i].getId() < tmpID){
                    tmpID = ALCdishes[i].getId();
                    ind = i;
                }
            }
        }
    }
    if(tmpID != -1){
        orderList.push_back(tmpID);
        numOfOrder = numOfOrder+1;
        std:: vector<Dish> tmpALC;
        for(unsigned i = 0; i < ALCdishes.size(); i++){
            if(i != (unsigned)ind){
                tmpALC.push_back(ALCdishes[i]);
            }
        }
        ALCdishes.clear();
        for(unsigned i = 0; i < tmpALC.size(); i++){
            ALCdishes.push_back(tmpALC[i]);
        }
    }
    return orderList;
}

std::string AlchoholicCustomer:: toString() const{
    std::string s = getName() + "," + "alc";
    return s;
}

Customer* AlchoholicCustomer:: copy (){
    AlchoholicCustomer* c = new AlchoholicCustomer (this->getName(),this->getId());
    c->numOfOrder = this->numOfOrder;
    for(unsigned i = 0; i < ALCdishes.size(); i++){
        c->ALCdishes.push_back(this->ALCdishes[i]);
    }
    return c;
}




