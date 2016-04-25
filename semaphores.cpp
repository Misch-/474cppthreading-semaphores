#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <semaphore.h>

#include "Globals.h"
#include "Guest.h"
#include "CheckinEmployee.h"
#include "CheckoutEmployee.h"

int Guest::next_id = 1;
sem_t Globals::s_rooms;
sem_t Globals::s_checkin;
sem_t Globals::s_checkin_assign_room;
sem_t Globals::s_checkin_guest_id;
int Globals::guestidin;
int Globals::roomidin;
int Globals::guestidout;
int Globals::roomidout;
sem_t Globals::s_checkout;
sem_t Globals::s_checkout_idroom;
sem_t Globals::s_checkout_balance;
int Guest::currentguest;
int Guest::poolcount;
int Guest::restrauntcount;
int Guest::fitnesscount;
int Guest::businesscount;


int main (int argc, char *argv[])
{
    sem_init(&Globals::s_rooms, 0, 5);
    sem_init(&Globals::s_checkin, 0, 1);
    sem_init(&Globals::s_checkin_assign_room, 0, 0);
    sem_init(&Globals::s_checkin_guest_id, 0, 0);
    sem_init(&Globals::s_checkout, 0, 1);

    CheckinEmployee ciemp;
    ciemp.serveCustomers();
    CheckoutEmployee coemp;
    coemp.serveCustomers();

    Guest::poolcount = 0;
    Guest::restrauntcount = 0;
    Guest::fitnesscount = 0;
    Guest::businesscount = 0;

    Guest guest[10];
    for (auto &g : guest) {
        g.joinline();
    }
    for (auto &g : guest) {
        g.waitthread();
    }
    //ciemp.end();
    //coemp.end();
    return 1;
}
