#include <Windows.h>
#include <string>
#include <iostream>
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")

using namespace std;

wstring wsWebhook; // Declare the wstring here
string title = "Title"; string desc = "Description"; bool forever = true;


void initializeWebhook(const string& webhook) {
    wsWebhook = wstring(webhook.begin(), webhook.end());
}

int spammer() {
    while (forever) {
        HINTERNET hSession = WinHttpOpen(L"WebhookSpam/1.0",
            WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
            WINHTTP_NO_PROXY_NAME,
            WINHTTP_NO_PROXY_BYPASS,
            0);

        if (!hSession) {
            wcerr << L"Failed to open WinHTTP session." << endl;
            return 1;
        }

        HINTERNET hConnect = WinHttpConnect(hSession,
            L"discord.com",
            INTERNET_DEFAULT_HTTPS_PORT,
            0);

        if (!hConnect) {
            wcerr << L"Failed to connect." << endl;
            WinHttpCloseHandle(hSession);
            return 1;
        }

        HINTERNET hRequest = WinHttpOpenRequest(hConnect,
            L"POST",
            wsWebhook.c_str(), // Use c_str() to get LPCWSTR
            NULL,
            WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            WINHTTP_FLAG_SECURE);

        if (!hRequest) {
            wcerr << L"Failed to open request." << endl;
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            return 1;
        }


        string color = "16711680"; // Decimal color
        string request_body = "{\"username\": \"github.com/Fontesie\",\"content\": \"@everyone https://github.com/Freakish-Z\",\"embeds\": [{\"title\": \"" + title + "\",\"description\": \"" + desc + "\",\"footer\": {\"text\": \"Get Spammed\"},\"color\": " + color + " }], \"attachments\": []}";

        BOOL bResults = WinHttpSendRequest(hRequest,
            L"Content-Type: application/json\r\n",
            (DWORD)-1L,
            (LPVOID)request_body.c_str(),
            (DWORD)request_body.length(),
            (DWORD)request_body.length(),
            0);

        if (bResults) {
            bResults = WinHttpReceiveResponse(hRequest, NULL);
            if (!bResults) {
                wcerr << L"Failed to receive response." << endl;
            }
        }
        else {
            wcerr << L"Failed to send request." << endl;
        }

        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
    }

    return 0;
}

int main() {
    string webhook;
    cout << "Enter the webhook (ONLY THE PART INCLUDING AND AFTER /api)!: "; cin >> webhook;
    initializeWebhook(webhook);

    cout << "Choose the title of the spammed embed (Only one word, or add - in between words)!: ";
    cin >> title; cout << "\n"; cout << "\n"; 
    
    cout << "Choose the description for the spammed embed(Only one word, or add - in between words)!: "; cin >> desc; cout << "\n"; cout << "\n";

    cout << "Spamming! Close app to stop.";

    spammer();

    return 0;
}
