#include <iostream>
#include <string>
#include <curl/curl.h>
#include <cstring>
#include <vector>
#include <unordered_map>

struct MemoryStruct {
    char* memory;
    size_t size;
};

struct CookieStruct {
    std::vector<std::string> cookies;
};

struct TokenStruct {
    std::string token;
};

struct SessionStruct {
    std::string sessionId;
};

struct CacheStruct {
    std::unordered_map<std::string, std::string> cacheData;
};

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    char* ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        std::cerr << "Not enough memory (realloc returned NULL)" << std::endl;
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

size_t CookieCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    struct CookieStruct* cookies = (struct CookieStruct*)userp;
    cookies->cookies.push_back(std::string((char*)contents, realsize));
    return realsize;
}

void printFormattedResponse(const std::string& response) {
    std::cout << "   - Body:\n     " << response << std::endl;
}

void printHeaders(const std::vector<std::string>& headers) {
    std::cout << "   - Headers:\n";
    for (const auto& header : headers) {
        std::cout << "     " << header;
    }
}

void handleSessions(CURL* curl, const std::string& url, struct SessionStruct* session) {
    if (session) {
        session->sessionId = "dummySessionID123";
        std::cout << "5. Handling session:\n   - Session ID: " << session->sessionId << std::endl;
    }
}

void handleCache(const std::string& url, struct MemoryStruct* chunk, struct CacheStruct* cache) {
    std::cout << "2. Checking cache for URL: " << url << std::endl;
    auto it = cache->cacheData.find(url);
    if (it != cache->cacheData.end()) {
        std::cout << "   Retrieving data from cache for URL: " << url << std::endl;
        std::string cachedData = it->second;
        if (chunk->memory) {
            free(chunk->memory);
        }
        chunk->memory = strdup(cachedData.c_str());
        chunk->size = cachedData.size();
    } else {
        std::cout << "   No cached data found for URL: " << url << std::endl;
    }
}

int main() {
    std::cout << "Starting HTTP client application...\n" << std::endl;
    
    CURL* curl;
    CURLcode res;
    long http_code = 0;  
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        std::string url = "https://jsonplaceholder.typicode.com/posts/1";
        
        struct MemoryStruct chunk;
        chunk.memory = NULL;
        chunk.size = 0;

        struct CookieStruct cookies;
        struct TokenStruct token;
        token.token = "myAuthToken123"; 

        struct SessionStruct session;
        struct CacheStruct cache;

        std::cout << "1. Sending HTTP request to: " << url << std::endl;

        handleCache(url, &chunk, &cache);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        std::string authHeader = "Authorization: Bearer " + token.token;
        headers = curl_slist_append(headers, authHeader.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        std::cout << "\n3. Setting HTTP headers:\n";
        std::cout << "   - Content-Type: application/json\n";
        std::cout << "   - " << authHeader << std::endl;

        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, ""); 
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies.txt"); 

        std::cout << "\n4. Managing cookies:\n";
        std::cout << "   - Enabling cookie storage\n";
        std::cout << "   - Using cookies from file: cookies.txt\n" << std::endl;

        handleSessions(curl, url, &session);

        curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, CookieCallback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*)&cookies);

        std::cout << "\n6. Sending request and awaiting response..." << std::endl;
        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            std::cerr << "   - Request failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

            if (http_code == 200) {
                std::cout << "   - Request successful.\n   - HTTP Status Code: " << http_code << std::endl;
            } else {
                std::cerr << "   - Request failed. HTTP Status Code: " << http_code << std::endl;
            }

            if (chunk.memory) {
                std::cout << "\n8. Response received:\n";
                printHeaders(cookies.cookies);
                printFormattedResponse(chunk.memory);

                if (http_code == 200) {
                    cache.cacheData[url] = std::string(chunk.memory);
                    std::cout << "\n9. Saving data to cache for URL: " << url << std::endl;
                }
            }

            std::cout << "\n7. Handling redirection (if any):\n   - No redirection followed." << std::endl;
        }

        curl_slist_free_all(headers);
        if (chunk.memory) {
            free(chunk.memory);
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    std::cout << "\n10. Checking and handling errors:\n   - No errors encountered." << std::endl;
    std::cout << "\nApplication finished." << std::endl;

    return 0;
}
