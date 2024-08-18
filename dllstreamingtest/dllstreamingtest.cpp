#include <stdio.h>
#include <string>
#include <Windows.h>
#include <Wininet.h>
#pragma comment(lib, "wininet.lib")

std::string replace(std::string subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}

std::string download(const std::string& URL) {
    HINTERNET head = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    std::string rtrn;

    if (head) {
        HINTERNET urlFile = InternetOpenUrlA(head, URL.c_str(), NULL, 0, 0, 0);
        if (urlFile) {
            char buffer[2000];
            DWORD bytesRead;
            do {
                InternetReadFile(urlFile, buffer, sizeof(buffer), &bytesRead);
                rtrn.append(buffer, bytesRead);
            } while (bytesRead > 0);

            InternetCloseHandle(urlFile);
        }
        InternetCloseHandle(head);
    }

    return replace(rtrn, "|n", "\r\n");
}

int main() {
    std::string content = download("https://pastebin.com/raw/K5ZvsHnk");
    printf("%s\n", content.c_str());
    getchar();
}