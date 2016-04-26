#include <iostream>
#include <thread>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <sstream>

#ifndef CHECKOUTEMPLOYEE
#define CHECKOUTEMPLOYEE

class CheckoutEmployee
{
    private:
        //Store thread reference upon creation
        std::thread t;
        //Changes to false when thread should end
        bool cont;

        void checkout() {
            while (cont) {
                sem_wait(&Globals::s_checkout_idroom);
                
                //Get room and id of current guest in checkout
                int id = Globals::guestidout;
                int room = Globals::roomidout;

                //cout is threadsafe, but multi-output operations can interleave mid line, prevent that by building the string before outputting it
                std::stringstream stream;
                stream << "The check out reservationist greets guest " << id << " and recieves key from room " << room << std::endl;
                std::cout << stream.str();
                stream.str("");

                stream << "Calculate the balance for guest " << id << std::endl;
                std::cout << stream.str();
                stream.str("");

                sem_post(&Globals::s_checkout_balance);

                stream << "Recieve $??? from Guest " << id << " and complete the check out" << std::endl;
                std::cout << stream.str();
                stream.str("");

                sem_post(&Globals::s_checkout);
                sem_post(&Globals::s_rooms);
            }
        }
    public:
        CheckoutEmployee();
        ~CheckoutEmployee();
        void serveCustomers() {
            t = std::thread(&CheckoutEmployee::checkout, this);
        }
        void waitthread() {
            t.join();
        }   
        void end() {
            cont = false;
            //std::terminate(t);
        }   
};

CheckoutEmployee::CheckoutEmployee() 
{ 
    std::stringstream stream;
    stream << "CheckoutEmployee was created" << std::endl; 
    std::cout << stream.str();
    cont = true;
}
CheckoutEmployee::~CheckoutEmployee() 
{ 
    std::stringstream stream;
    stream << "CheckoutEmployee was destroyed" << std::endl; 
    std::cout << stream.str();
}

#endif
