#include "Ship.h"
#include "Model.h"
#include <iostream>
#include <string>
using namespace std;

// initialize then output constructor message
Island::Island(const string &name_, Point position_, double fuel_ = 0., double production_rate_ = 0.) :
        Sim_object(name_), position(position_), fuel(fuel_), production_rate(production_rate_)
{
    cout << "Island " << get_name() << " constructed\n";
}

// output destructor message
~Island::Island()
{
    cout << "Island " << get_name() << " destructed\n";
}

// Return whichever is less, the request or the amount left,
// update the amount on hand accordingly, and output the amount supplied.
double Island::provide_fuel(double request)
{
    int min = request < fuel ? request : fuel;
    fuel -= min;
    cout << "Island " << get_name() << " supplied " << min << " tons of fuel\n";
    return min;
}

// Add the amount to the amount on hand, and output the total as the amount the Island now has.
void Island::accept_fuel(double amount)
{
    fuel += amount;
    cout << "Island " << get_name() << " now has " << fuel << " tons\n";
}

// if production_rate > 0, compute production_rate * unit time, and add to amount, and print an update message
void Island::update() override
{
    fuel += production_rate;
    cout << "Island " << get_name() << " now has " << fuel << " tons\n";
}

// output information about the current state
void Island::describe() const override
{
    cout << "\nIsland " << get_name() << " at position " << position << "\nFuel available: " << fuel << " tons";
}

// ask model to notify views of current state
void Island::broadcast_current_state() override
{
    g_Model_ptr->notify_location(get_name(), position);
}