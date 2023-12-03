#include "send_mail.hpp"

void send_email(const std::string &to, const std::string &subject, const std::string &body)
{
    CURL *curl;
    CURLcode res = CURLE_OK;

    curl = curl_easy_init();
    if (curl)
    {
        struct curl_slist *recipients = NULL;
        recipients = curl_slist_append(recipients, to.c_str());

        curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.example.com:587");
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, "<your-email@example.com>");
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
        curl_easy_setopt(curl, CURLOPT_USERNAME, "your-username");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "your-password");

        std::string email_data = "To: " + to + "\r\n"
                                               "From: <your-email@example.com>\r\n"
                                               "Subject: " +
                                 subject + "\r\n"
                                           "\r\n" +
                                 body;

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, email_data.c_str());

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;

        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }
}

// int main()
// {
//     send_email("recipient@example.com", "Test Subject", "This is a test email from arpwatch-cpp.");
//     return 0;
// }