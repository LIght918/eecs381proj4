#include "Warship.h"
#include "Ship.h"
#include <string>
#include <iostream>

using namespace std;

// initialize, then output constructor message
Warship::Warship(const string& name_, Point position_, double fuel_capacity_,
        double maximum_speed_, double fuel_consumption_, int resistance_,
        int firepower_, double maximum_range_) :
        Ship(name_, position_, fuel_capacity_, maximum_speed_, fuel_consumption_, resistance_),
        firepower(firepower_), max_range(maximum_range_), warship_state(State_warship::NOT_ATTACKING), target(nullptr)
{
    cout << "Warship " << get_name() << " constructed" << endl;
}

// a pure virtual function to mark this as an abstract class,
// but defined anyway to output destructor message
Warship::~Warship()
{
    cout << "Warship " << get_name() << " destructed" << endl;
}

// perform warship-specific behavior
void Warship::update() override
{
    Ship::update();
    if (warship_state == State_warship::ATTACKING)
    {
        if (!is_afloat() || !target->is_afloat())
        {
            stop_attack();
            return;
        }
        cout << get_name() << " is attacking" << endl;
    }
}

// Warships will act on an attack and stop_attack command

// will	throw Error("Cannot attack!") if not Afloat
// will throw Error("Warship may not attack itself!")
// if supplied target is the same as this Warship
void Warship::attack(Ship* target_ptr_) override
{
    if (!is_afloat()) throw Error("Cannot attack!");
    if (target_ptr_ == this) throw Error("Warship may not attack itself!");
    if (target_ptr_ == target) throw Error("Already attacking this target!");
    target = target_ptr_;
    warship_state = State_warship::ATTACKING;
    cout << get_name() << " will attack " << target->get_name() << endl;
}

// will throw Error("Was not attacking!") if not Attacking
void Warship::stop_attack() override
{
    if (warship_state != State_warship::ATTACKING) throw Error("Was not attacking!");
    warship_state = State_warship::NOT_ATTACKING;
    target = nullptr;
}

void Warship::describe() const override
{
    Ship::describe();
    if (warship_state == State_warship::ATTACKING)
    {
        cout << "Attacking " << target->get_name() << endl;
    }
}

// fire at the current target
void Warship::fire_at_target()
{
    cout << get_name() << " fires" << endl;
    target->receive_hit(firepower, target);
}