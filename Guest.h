#include <iostream>
#include <thread>
#include <vector>
#include <sstream>
#include <semaphore.h>

#ifndef GUEST
#define GUEST

class Guest
{
    private:
        int room;
        static int next_id;
        int id; // Guest id (1-10)
        std::thread t;

        void checkin(){
            //can transfer via a public static variable since only one transaction happens at a time
            //better and easier than piping, pipes are only for seperate processes
            
            sem_wait(&Globals::s_rooms);

            //cout is threadsafe, but multistep operations can interleave mid line, prevent that by building the string before outputting it
            std::stringstream stream;
            stream << "Guest " << id << " waits for check in" << std::endl;
            std::cout << stream.str();
            stream.str("");

            sem_wait(&Globals::s_checkin);
            
            //Post id of current guest in checkin
            Globals::guestidin = id;

            sem_post(&Globals::s_checkin_guest_id);

            stream << "Guest " << id << " goes to check in reservationist" << std::endl;
            std::cout << stream.str();
            stream.str("");

            sem_wait(&Globals::s_checkin_assign_room);

            //Get room of current guest in checkin
            room = Globals::roomidin;           

            stream << "Guest " << id << " recieves room " << room << std::endl;
            std::cout << stream.str();
            stream.str("");

            enjoystay();
        }
        void enjoystay() {
            //results in even distribution of 0-3 integers
            int rand = std::rand() % 4;
            std::stringstream stream;
            switch (rand)
            {
                case 0:
                    stream << "Activity: Guest " << id << " goes to pool" << std::endl;
                    std::cout << stream.str();
                    stream.str("");
                    poolcount++;
                    break;
                case 1:
                    stream << "Activity: Guest " << id << " goes to restraunt" << std::endl;
                    std::cout << stream.str();
                    stream.str("");
                    restrauntcount++;
                    break;
                case 2:
                    stream << "Activity: Guest " << id << " goes to fitness center" << std::endl;
                    std::cout << stream.str();
                    stream.str("");
                    fitnesscount++;
                    break;
                case 3:
                    stream << "Activity: Guest " << id << " goes to do business" << std::endl;
                    std::cout << stream.str();
                    stream.str("");
                    businesscount++;
                    break;
            }
            //Each thread waits either 1, 2, or 3 seconds
            rand = (std::rand() % 3) + 1;
            std::this_thread::sleep_for(std::chrono::seconds(rand));

            checkout();

        }
        void checkout() {

            sem_wait(&Globals::s_checkout);

            std::stringstream stream;
            stream << "Guest " << id << " goes to the check out reservationist and returns room " << room << std::endl;
            std::cout << stream.str();
            stream.str("");

            //Post room and id of current guest in checkout
            Globals::guestidout = id;
            Globals::roomidout = room;

            sem_post(&Globals::s_checkout_idroom);

            sem_wait(&Globals::s_checkout_balance);

            stream << "Guest " << id << " recieves the total balance of $??? "<< std::endl;
            std::cout << stream.str();
            stream.str("");

            stream << "Guest " << id << " makes a payment " << std::endl;
            std::cout << stream.str();
            stream.str("");

        }
    public:
        //counters to keep track of each activity
        static int totalguests;
        static int poolcount;
        static int restrauntcount;
        static int fitnesscount;
        static int businesscount;
        Guest();
        ~Guest();
        int getid() {
            return id;
        }   
        void joinline() {
            t = std::thread(&Guest::checkin, this);
            totalguests++;
        }   
        void waitthread() {
            t.join();
        }   
};

Guest::Guest() 
{ 
    id = next_id;
    next_id = ++next_id;
    std::stringstream stream;
    stream << "Guest " << id << " was created" << std::endl; 
    std::cout << stream.str();
}
Guest::~Guest() 
{ 
    std::stringstream stream;
    stream << "Guest " << id << " was destroyed" << std::endl; 
    std::cout << stream.str();
}

#endif
