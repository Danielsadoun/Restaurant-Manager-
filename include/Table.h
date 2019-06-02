#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    Table(int t_capacity);
    Table(const Table &other);
    Table(Table&& other);
    Table& operator=(Table &other);
    Table operator=(Table&& other);
    virtual ~Table();
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    bool CusInTheTable (int id);   // get customer id and check if he is in the table
    void order(const std::vector<Dish> &menu);
    void openTable();
    std::string getStatus();
    void closeTable();
    int getBill();
    bool isOpen();
    std::vector<OrderPair> getLastOrder();
    Table* copy();
private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
    std::vector<OrderPair> lastOrder;
};


#endif