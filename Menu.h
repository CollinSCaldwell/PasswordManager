
#ifndef MENU
#define MENU

#include "Inherited.h"

int startUp();


void displayMainMenu();

bool logInMenu();

void wrongPasswordEntered();

void displayMainLoggedInMenu();

void addSite();

void deleteSite();

void pickSite();

bool listSites();

void listPasswordForSite(int);


bool setUpPassword();

bool resetPassword();

#endif 