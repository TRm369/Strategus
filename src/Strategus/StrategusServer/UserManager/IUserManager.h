#pragma once
#include "pch.h"

//TODO: Everything

/// Interface for user-managing subsystem
class IUserManager {
public:
	/// ID for unknown user
	const ID_t unknownUserID = 0;

	/// Converts username to ID, for unknown username returns unknownUserID
	virtual ID_t usernameToID(std::string& username) = 0;
};