// WindowSocket.cpp : This file contains the 'main' function. Program execution begins and ends there.
//



#include <stdio.h>
#include<string.h>
#include <WinSock2.h>
#include <Windows.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

int main()
{
    char desIp[20] = "127.0.0.1";
    unsigned short desPort = 8000u;

    printf("Enter your destination IP:");
    scanf("%s", &desIp);
    printf("Enter your destination Port:");
    scanf("%hu", &desPort);

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(desIp);
    addr.sin_port = htons(desPort);

    system("pause");

    int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR)
    {
        ret = WSAGetLastError();
        printf("Ket noi khong thanh cong - %d\n", ret);
        return 1;
    }

    // Initital data to send
    char allMyNameString[200], allMyLocalDriverString[200], sectorAndByte[200], tmpString[200];
    const char* myNameString = "\nMy computer name is: ";
    const char* myLocalDriver = "\nMy local driver is: ";
    const char* sectorString = "\nSector per cluster in disk C is: ";
    const char* byteSectorString = "\nNumber of byte per sector in disk C is:";

    char myComputerName[256] = "";
    DWORD sizeOfName = sizeof(myComputerName);
    GetComputerNameA(myComputerName, &sizeOfName);

    char myListDriver[256];
    DWORD size = sizeof(&myListDriver);
    DWORD numberOfDriver = GetLogicalDriveStringsA(size, myListDriver);

    const char* rootPathName = "C\:";
    DWORD sectorPerCluster, bytePerSector, numberOfFreeCluster, totalNumberOfClusters;
    bool check = GetDiskFreeSpaceA(rootPathName, &sectorPerCluster, &bytePerSector, &numberOfFreeCluster, &totalNumberOfClusters);

    // convert DWORD to char
    char tmp3[10], tmp1[20], tmp2[20];
    sprintf(tmp3, "%d", numberOfDriver);
    sprintf(tmp1, "%d", sectorPerCluster);
    sprintf(tmp2, "%d", bytePerSector);

    // copy and concat all string
    // computer name
    strcpy(allMyNameString, myNameString);
    strcat(allMyNameString, myComputerName);

    // local driver
    strcpy(allMyLocalDriverString, myLocalDriver);
    strncat(allMyLocalDriverString, tmp3, sizeof(tmp3));

    // sector and byte
    strcpy(sectorAndByte, sectorString);
    strncat(sectorAndByte, tmp1, sizeof(tmp1));
    strcat(sectorAndByte, byteSectorString);
    strncat(sectorAndByte, tmp2, sizeof(tmp2));

    // Send data to server
    send(client, allMyNameString, strlen(allMyNameString), 0);
    send(client, allMyLocalDriverString, strlen(allMyLocalDriverString), 0);
    send(client, sectorAndByte, strlen(sectorAndByte), 0);

    printf("\nClient sends computer information successfully!!!");

    closesocket(client);
    WSACleanup();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
