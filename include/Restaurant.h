#ifndef RESTAURANT_H_
#define RESTAURANT_H_


#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"



class Restaurant{		
public:
	Restaurant();
    Restaurant(const std::string &configFilePath);
    Restaurant(const Restaurant &other);
    Restaurant(Restaurant&& other);
    Restaurant& operator=(Restaurant &other);
    Restaurant operator=(Restaurant&& other);
    virtual ~Restaurant();
    BaseAction* newAction(std::string s);
	std::vector<Customer*>& addCus (std::vector<std::string> v);  // function for the 'start' from Resturant
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
    void addTables(std:: string tablestr);
    void addDish (std:: string dish);
    std:: vector <Customer*> getNewList();
private:
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
    int lastId;  // to initiallize to 0 in constructor
	std::vector<Customer*> newList;  // a field for the 'start' from resturant
};

#endif