#include "http.h"
#include <curl/curl.h>
#include <iostream>
#include <fstream>

namespace duck::utils {

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

static size_t FileWriteCallback(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

std::optional<std::string> HttpClient::post(const std::string& url, const std::string& body) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return std::nullopt;
        }
        return readBuffer;
    }
    return std::nullopt;
}

bool HttpClient::download(const std::string& url, const std::string& outputPath) {
    CURL* curl;
    FILE* fp;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        fp = fopen(outputPath.c_str(), "wb");
        if (!fp) return false;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FileWriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        res = curl_easy_perform(curl);
        
        long response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

        curl_easy_cleanup(curl);
        fclose(fp);

        if (res != CURLE_OK) {
            std::cerr << "Download failed: " << curl_easy_strerror(res) << std::endl;
            return false;
        }
        
        if (response_code >= 400) {
            std::cerr << "HTTP Error: " << response_code << " for " << url << std::endl;
            return false;
        }
        
        return true;
    }
    return false;
}

} // namespace duck::utils
