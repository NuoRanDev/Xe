#include "MmapFstream.h"

#ifndef _
#include <Windows.h>
int main() {
    HANDLE hFileMapping;
    LPVOID pFileView;

    hFileMapping = OpenFileMapping(FILE_MAP_READ, FALSE, L"ShareFile");
    if (hFileMapping == NULL) {
        std::cout << "OpenFileMapping Failed:" << GetLastError() << std::endl;

        return -1;
    }

    pFileView = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);
    if (pFileView == NULL) {
        CloseHandle(hFileMapping);

        return -1;
    }

    //�ӹ����ڴ��ж�ȡ����
    while (true) {
        Sleep(1000 * 3);

        std::cout << "Received data in WriteMapping Process:" << (char*)pFileView << std::endl;
    }

    //���ӳ��͹ر��ļ�ӳ�����
    UnmapViewOfFile(pFileView);
    CloseHandle(hFileMapping);
}
#endif // !_
