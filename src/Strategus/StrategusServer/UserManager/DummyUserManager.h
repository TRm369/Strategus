#pragma once
#include "pch.h"
#include "IUserManager.h"

class DummyUserManager : public IUserManager {
	ID_t usernameToID(std::string& username) { return unknownUserID; }
};