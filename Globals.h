#include <iostream>
#include <thread>
#include <vector>
#include <pthread.h>
#include <sstream> 
#include <semaphore.h>

#ifndef GLOBALS
#define GLOBALS

class Globals
{
    public:
        //Static global variables to be accessed from any class
        static sem_t s_checkin;  
        static sem_t s_checkin_assign_room;  
        static sem_t s_checkin_guest_id;  
        static sem_t s_checkout;
        static sem_t s_checkout_balance;
        static sem_t s_checkout_idroom;
        static sem_t s_rooms;  
        static int roomidin; //1-5 rooms
        static int guestidin;
        static int roomidout; //1-5 rooms
        static int guestidout;
        Globals();
        ~Globals();
};

Globals::Globals() 
{ 
    std::stringstream stream;
    stream << "Globals was created" << std::endl; 
    std::cout << stream.str();
}
Globals::~Globals() 
{
    std::stringstream stream;
    stream << "Globals was destroyed" << std::endl; 
    std::cout << stream.str();
}

#endif
