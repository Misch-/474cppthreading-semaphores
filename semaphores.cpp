#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <semaphore.h>

#include "Globals.h"
#include "Guest.h"
#include "CheckinEmployee.h"
#include "CheckoutEmployee.h"

//Have to define static members of other classes before I can use them
sem_t Globals::s_rooms;
sem_t Globals::s_checkin;
sem_t Globals::s_checkin_assign_room;
sem_t Globals::s_checkin_guest_id;
sem_t Globals::s_checkout;
sem_t Globals::s_checkout_idroom;
sem_t Globals::s_checkout_balance;
//for communication between threads about guest # and room #
int Globals::guestidin;
int Globals::roomidin;
int Globals::guestidout;
int Globals::roomidout;
//for keeping count of things
int Guest::next_id = 1;
int Guest::totalguests = 0;
int Guest::poolcount;
int Guest::restrauntcount;
int Guest::fitnesscount;
int Guest::businesscount;

void printSummary()
{
    std::stringstream stream;
    stream << std::endl 
    << "Number of customers" << std::endl 
    << "Total Guests: " << Guest::totalguests << std::endl 
    << "Pool: " << Guest::poolcount << std::endl 
    << "Restraunt: " << Guest::restrauntcount << std::endl 
    << "Fitness Center: " << Guest::fitnesscount << std::endl 
    << "Business Center: " << Guest::businesscount << std::endl
    << std::endl;
    std::cout << stream.str();
    stream.str("");
}

int main (int argc, char *argv[])
{
    //initialize semaphores
    sem_init(&Globals::s_rooms, 0, 5);
    sem_init(&Globals::s_checkin, 0, 1);
    sem_init(&Globals::s_checkin_assign_room, 0, 0);
    sem_init(&Globals::s_checkin_guest_id, 0, 0);
    sem_init(&Globals::s_checkout, 0, 1);
    sem_init(&Globals::s_checkout_idroom, 0, 0);
    sem_init(&Globals::s_checkout_balance, 0, 0);

    //Create checkin checkout employees
    CheckinEmployee ciemp;
    ciemp.serveCustomers();
    CheckoutEmployee coemp;
    coemp.serveCustomers();

    Guest::poolcount = 0;
    Guest::restrauntcount = 0;
    Guest::fitnesscount = 0;
    Guest::businesscount = 0;

    //Create 10 guests and have them
    //get in line for the checkin employee
    Guest guest[10];
    for (auto &g : guest) {
        g.joinline();
    }
    //Wait for them all to checkout
    for (auto &g : guest) {
        g.waitthread();
    }
    //Couldn't quite get employee threads to end properly
    //ciemp.end();
    //coemp.end();

    //Print results
    printSummary();
    
    //Exit
    return 1;
}
