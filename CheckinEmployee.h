#include <iostream>
#include <thread>
#include <vector>
#include <pthread.h>
#include <sstream> 
#include <semaphore.h>

#ifndef CHECKINEMPLOYEE
#define CHECKINEMPLOYEE

class CheckinEmployee
{
    private:
        int availablerooms[5] = {1,1,1,1,1};
        std::thread t;
        bool cont;

        void checkin() {
                while (cont) {
                    sem_wait(&Globals::s_checkin_guest_id);
                    int id = Globals::guestidin; 

                    //cout is threadsafe, but multistep operations can interleave mid line, prevent that by building the string before outputting it
                    std::stringstream stream;
                    stream << "The check in reservationist greets guest " << id << std::endl;
                    std::cout << stream.str();
                    stream.str("");

                    //there will be at least one avaiable because it's required for the guest to be greeted 
                    int i;
                    for (i=0; i<5; i++) {
                        if (availablerooms[i] == 1) {
                            availablerooms[i] = 0;
                            Globals::roomidin = i + 1;
                            break;
                        }
                    }
                    sem_post(&Globals::s_checkin_assign_room);

                    stream << "Assigned room " << Globals::roomidin << " to guest " << id << std::endl;
                    std::cout << stream.str();
                    stream.str("");
                
                    sem_post(&Globals::s_checkin);
            }
       }
    public:
        CheckinEmployee();
        ~CheckinEmployee();
        void serveCustomers() {
            t = std::thread(&CheckinEmployee::checkin, this);
        }
        void waitthread() {
            t.join();
        }
        void end() {
            cont = false;
            //std::terminate(t);
        }
};

CheckinEmployee::CheckinEmployee() 
{ 
    std::stringstream stream;
    stream << "CheckinEmployee was created" << std::endl; 
    std::cout << stream.str();
    cont = true;
}
CheckinEmployee::~CheckinEmployee() 
{
    std::stringstream stream;
    stream << "CheckinEmployee was destroyed" << std::endl; 
    std::cout << stream.str();
}

#endif
