#pragma once
#include <string>

class User {
public:
    static bool validateAdminLogin();
    static void clientSignup();
    static bool clientLogin();
};
