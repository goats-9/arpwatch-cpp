#pragma once

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <cstring>
#include "env.hpp"

/**
 * @brief Send email to recipient with a subject and body.
 * @param sub Subject of the email.
 * @param body Body of the email.
 * @return 0 if successful, nonzero otherwise.
*/
int send_email(const std::string &sub, const std::string &body);