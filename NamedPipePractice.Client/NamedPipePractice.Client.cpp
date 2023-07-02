// NamedPipePractice.Client.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#include <windows.h>
#include <iostream>
#include <tchar.h>

int main()
{
    HANDLE hPipe;
    char buffer[1024];
    DWORD dwRead;

    // 名前付きパイプに接続
    std::cout << "Connecting to server..." << std::endl;
    hPipe = CreateFile(_T("\\\\192.168.100.100\\pipe\\MyPipe"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (hPipe == INVALID_HANDLE_VALUE)
    {
        DWORD errorId = GetLastError();
        std::cout << "Failed to connect to server. Error Code: " << errorId << std::endl;
        std::cout << "hPipe: " << hPipe << std::endl;
        std::cin.get();
        return 1;
    }

    std::cout << "Connected to server." << std::endl;

    // メッセージの送信と受信
    while (true)
    {
        // メッセージの送信
        std::cout << "Enter message to send (type 'exit' to quit): ";
        std::cin.getline(buffer, sizeof(buffer));
        DWORD dwWritten;
        if (!WriteFile(hPipe, buffer, strlen(buffer), &dwWritten, NULL))
        {
            std::cout << "Failed to send message to server." << std::endl;
            break;
        }

        // 終了条件の判定
        if (strcmp(buffer, "exit") == 0)
            break;

        // メッセージの受信
        if (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL))
        {
            buffer[dwRead] = '\0';
            std::cout << "Received message from server: " << buffer << std::endl;
        }
        else
        {
            std::cout << "Failed to receive message from server." << std::endl;
            break;
        }
    }

    // 名前付きパイプのクローズ
    CloseHandle(hPipe);

    return 0;
}