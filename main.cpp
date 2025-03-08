#include "survey.h"
#include "user.h"
#include "ui.h"
#include <iostream>

int main() {
    UI::title();

    int userType;
    std::cout << "Are you an admin (1) or a client (2)? ";
    std::cin >> userType;
    std::cin.ignore();

    if (userType == 1) {
        // Admin flow
        if (!User::validateAdminLogin()) {
            std::cout << "Exiting..." << std::endl;
            return 0;
        }

        int adminChoice;
        UI::clearScreen();
        do {
            // Admin menu implementation
            // ...existing code...
        } while (adminChoice != 9);
    }
    else if (userType == 2) {
        // Client flow
        // ...existing code...
    }
    else {
        UI::clearScreen();
        std::cout << "Invalid user type. Exiting..." << std::endl;
    }

    return 0;
}
