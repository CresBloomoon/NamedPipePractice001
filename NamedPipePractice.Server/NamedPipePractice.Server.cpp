#include <Windows.h>
#include <iostream>
#include <tchar.h>

int main()
{
    HANDLE hPipe;
    char buffer[1024];
    DWORD dwRead;

    // 名前付きパイプの作成
    hPipe = CreateNamedPipe(
        _T("\\\\.\\pipe\\MyPipe"),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        1024,
        1024,
        0,
        NULL);

    if (hPipe == INVALID_HANDLE_VALUE)
    {
        DWORD errorId = GetLastError();
        std::cout << "Failed to create named pipe. Error code: " << errorId << std::endl;
        std::cin.get(); // ユーザのキー入力を待機
        return 1;
    }

    std::cout << "Named pipe created. Waiting for clients..." << std::endl;

    // クライアントからの接続を待機
    if (!ConnectNamedPipe(hPipe, NULL))
    {
        std::cout << "Failed to connect to client." << std::endl;
        std::cin.get(); // ユーザのキー入力を待機
        CloseHandle(hPipe);
        return 1;
    }

    std::cout << "Connected to client." << std::endl;

    // メッセージの送信と受信
    while (true)
    {
        // メッセージの受信
        if (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL))
        {
            buffer[dwRead] = '\0';
            std::cout << "Received message from client: " << buffer << std::endl;
        }
        else
        {
            std::cout << "Failed to receive message from client." << std::endl;
            break;
        }

        // 終了条件の判定
        if (strcmp(buffer, "exit") == 0)
            break;

        // メッセージの送信
        std::cout << "Enter message to send (type 'exit' to quit): ";
        std::cin.getline(buffer, sizeof(buffer));
        DWORD dwWritten;
        if (!WriteFile(hPipe, buffer, strlen(buffer), &dwWritten, NULL))
        {
            std::cout << "Failed to send message to client." << std::endl;
            break;
        }
    }

    // 名前付きパイプのクローズ
    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);

    std::cin.get(); // ユーザのキー入力を待機
    return 0;
}
