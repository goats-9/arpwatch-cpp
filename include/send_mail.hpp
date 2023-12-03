#pragma once

#include <iostream>
#include <string>
#include <curl/curl.h>

/**
 * @brief Send email to recipient with a subject and body.
 * @param to Recipient email address.
 * @param subject Subject of the email.
 * @param body Body of the email.
 * @return `void`
*/
void send_email(const std::string &to, const std::string &subject, const std::string &body);