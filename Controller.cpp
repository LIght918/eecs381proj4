#include "Controller.h"

#include "Model.h"
#include "View.h"
#include "Ship.h"
#include "Island.h"
#include "Geometry.h"
#include "Ship_factory.h"
#include "Utility.h"
#include <string>
#include <iostream>
#include <map>

using namespace std;

const string UNRECOGNIZED_ERROR_MSG = "Unrecognized command!";

const map<string, command_func> COMMAND_FUNC_MAP {
        {"quit", &Controller::quit},

        {"default", &Controller::view_default},
        {"size", &Controller::view_size},
        {"zoom", &Controller::view_zoom},
        {"pan", &Controller::view_pan},
        {"show", &Controller::view_show},

        {"status", &Controller::model_status},
        {"go", &Controller::model_go},
        {"create", &Controller::model_create}
};

const map<string, ship_func> SHIP_FUNC_MAP {
        {"course", &Controller::ship_course},
        {"position", &Controller::ship_position},
        {"destination", &Controller::ship_destination},
        {"load_at", &Controller::ship_load_at},
        {"unload_at", &Controller::ship_unload_at},
        {"dock_at", &Controller::ship_dock_at},
        {"attack", &Controller::ship_attack},
        {"refuel", &Controller::ship_refuel},
        {"stop", &Controller::ship_stop},
        {"stop_attack", &Controller::ship_stop_attack}
};

// output constructor message
Controller::Controller()
{
    cout << "Controller constructed" << endl;
}
// output destructor message
Controller::~Controller()
{
    cout << "Controller destructed" << endl;
}

// create View object, run the program by accepting user commands, then destroy View object
void Controller::run()
{
    View *view = new View;
    g_Model_ptr->attach(view);
    while (true)
    {
        try
        {
            cout << "\nTime " << g_Model_ptr->get_time() << ": Enter command: ";
            string command;
            cin >> command;
            // first, check the command func map if this a command word
            if (COMMAND_FUNC_MAP.find(command) != COMMAND_FUNC_MAP.end())
            {
                // if so, run function and return if it returns true
                if (COMMAND_FUNC_MAP[command](view)) return;
            }
            else
            {
                // if not, find the ship with this name
                try { Ship *ship = g_Model_ptr->get_ship_ptr(command); }
                catch (Error& e) { throw Error(UNRECOGNIZED_ERROR_MSG); }
                // if there is no ship with that name, an unrecognized comman error will be thrown
                // read in the ship command
                string ship_command;
                cin >> ship_command;
                // check the ship func map if this is a command word
                if (SHIP_FUNC_MAP.find(ship_command) != SHIP_FUNC_MAP.end())
                {
                    // if so, run the function
                    SHIP_FUNC_MAP[ship_command](ship);
                }
                else
                {
                    // if not, throw an unrecognized command error
                    throw Error(UNRECOGNIZED_ERROR_MSG);
                }
            }
        }
        catch (Error& e)
        {
            cout << e.what() << endl;
        }
        catch (...)
        {
            cout << "An unknown error occurred!" << endl;
            return;
        }
    }
}

// helper functions
int read_int()
{
    int new_int;
    if (!(cin >> new_int)) throw Error("Expected an integer!");
    return new_int;
}
double read_double()
{
    int new_double;
    if (!(cin >> new_double)) throw Error("Expected a double!");
    return new_double;
}
double read_speed()
{
    double speed = read_double();
    if (speed < 0) throw Error("Negative speed entered!");
    return speed;
}
Ship* read_ship()
{
    string name;
    cin >> name;
    return g_Model_ptr->get_ship_ptr(name);
}
Island* read_island()
{
    string name;
    cin >> name;
    return g_Model_ptr->get_island_ptr(name);
}

// command functions
bool Controller::quit(View *view)
{
    g_Model_ptr->detach(view);
    delete view;
    cout << "Done" << endl;
    return true;
}

// view functions
bool Controller::view_default(View *view)
{
    view->set_defaults();
    return false;
}
bool Controller::view_size(View *view)
{
    view->set_size(read_int());
    return false;
}
bool Controller::view_zoom(View *view)
{
    view->set_scale(read_double());
    return false;
}
bool Controller::view_pan(View *view)
{
    view->set_origin(Point(read_double(), read_double()));
    return false;
}
bool Controller::view_show(View *view)
{
    view->draw();
    return false;
}

// model functions
bool Controller::model_status(View *view)
{
    g_Model_ptr->describe();
    return false;
}
bool Controller::model_go(View *view)
{
    g_Model_ptr->update();
    return false;
}
bool Controller::model_create(View *view)
{
    string new_name;
    cin >> new_name;
    if (new_name.size() < SHORTEN_NAME_LENGTH) throw Error("Name is too short!");
    if (g_Model_ptr->is_name_in_use(new_name)) throw Error("Name is already in use!");
    string new_type;
    cin >> new_type;
    g_Model_ptr->add_ship(create_ship(new_name, new_type, Point(read_double(), read_double())));
    return false;
}

// ship functions
void Controller::ship_course(Ship *ship)
{
    double course = read_double();
    if (course < 0 || course >= 360) throw Error("Invalid heading entered!");
    ship->set_course_and_speed(course, read_speed());
}
void Controller::ship_position(Ship *ship)
{
    ship->set_destination_position_and_speed(Point(read_double(), read_double()), read_speed);
}
void Controller::ship_destination(Ship *ship)
{
    ship->set_destination_position_and_speed(read_island()->get_location(), read_speed());
}
void Controller::ship_load_at(Ship *ship)
{
    ship->set_load_destination(read_island());
}
void Controller::ship_unload_at(Ship *ship)
{
    ship->set_unload_destination(read_island());
}
void Controller::ship_dock_at(Ship *ship)
{
    ship->dock(read_island());
}
void Controller::ship_attack(Ship *ship)
{
    ship->attack(read_ship());
}
void Controller::ship_refuel(Ship *ship)
{
    ship->refuel();
}
void Controller::ship_stop(Ship *ship)
{
    ship->stop();
}
void Controller::ship_stop_attack(Ship *ship)
{
    ship->stop_attack();
}