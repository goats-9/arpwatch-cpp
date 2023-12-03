#include "send_mail.hpp"

struct upload_status {
    int lines_read;
};

std::string payload_text;

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp) {
    struct upload_status *upload_ctx = (struct upload_status *)userp;
    const char *data;

    if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
        return 0;
    }
    
    data = payload_text.c_str();

    if (data[upload_ctx->lines_read]) {
        size_t len = strlen(data);
        memcpy(ptr, data + upload_ctx->lines_read, len);
        upload_ctx->lines_read += len;
        return len;
    }

    return 0;
}

int send_mail(std::string &sub, std::string &body) {
    CURL *curl;
    CURLcode res = CURLE_OK;
    struct upload_status upload_ctx;

    upload_ctx.lines_read = 0;
    payload_text = "To: " + std::string(ADMIN_EMAIL) 
                    + "\r\nFrom: " + std::string(ADMIN_EMAIL) 
                    + "\r\nSubject: " + sub + "\r\n\r\n" + body 
                    + "\r\n";

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_USERNAME, ADMIN_EMAIL);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, PASSWORD);
        curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");
        curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, ADMIN_EMAIL);
        struct curl_slist *recipients = NULL;
        recipients = curl_slist_append(recipients, ADMIN_EMAIL);
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }

    return (int)res;
}
