#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LINE 1024
#define MAX_HISTORY 100

#define SHELL_NAME "PrabuddhaShell"
#define SHELL_VERSION "0.1.0"
#define MOM_NUMBER "xxxxxxxxx"
#define P_W 20
#define P_H 20

void paint() {
    char canvas[P_H][P_W];
    int x = 0, y = 0;

    // init canvas
    for (int i = 0; i < P_H; i++)
        for (int j = 0; j < P_W; j++)
            canvas[i][j] = '.';

    while (1) {
        system("cls");

        printf("=== PAINT MODE ===\n");
        printf("WASD = move | SPACE = draw | E = erase | C = clear | Q = quit\n\n");

        // draw canvas
        for (int i = 0; i < P_H; i++) {
            for (int j = 0; j < P_W; j++) {

                if (i == y && j == x)
                    printf("@ ");   // cursor
                else
                    printf("%c ", canvas[i][j]);
            }
            printf("\n");
        }

        char ch = getchar();
        getchar(); // consume newline

        if (ch == 'q' || ch == 'Q')
            break;

        else if (ch == 'w' && y > 0)
            y--;

        else if (ch == 's' && y < P_H - 1)
            y++;

        else if (ch == 'a' && x > 0)
            x--;

        else if (ch == 'd' && x < P_W - 1)
            x++;

        else if (ch == ' ')
            canvas[y][x] = '#';

        else if (ch == 'e')
            canvas[y][x] = '.';

        else if (ch == 'c') {
            for (int i = 0; i < P_H; i++)
                for (int j = 0; j < P_W; j++)
                    canvas[i][j] = '.';
        }
    }
}
void clearScreen() {
    system("cls");
}

void dirCommand() {
    system("dir");
}

void echoCommand(char *command) {
    printf("%s\n", command + 5);
}

void versionCommand() {
    printf("%s Version %s\n", SHELL_NAME, SHELL_VERSION);
}

void timeAndDateCommand() {
    time_t t;
    time(&t);
    printf("%s", ctime(&t));
}

void delFileName(char *filename) {
    if (remove(filename) == 0)
        printf("Deleted: %s\n", filename);
    else
        printf("Error deleting file\n");
}

void executePipe(char *cmd1, char *cmd2) {
    SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };

    HANDLE hRead, hWrite;
    CreatePipe(&hRead, &hWrite, &sa, 0);
    SetHandleInformation(hRead, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFO si1 = {0}, si2 = {0};
    PROCESS_INFORMATION pi1 = {0}, pi2 = {0};

    si1.cb = sizeof(si1);
    si2.cb = sizeof(si2);

    si1.dwFlags = STARTF_USESTDHANDLES;
    si1.hStdOutput = hWrite;

    char cmdA[512];
    sprintf(cmdA, "cmd.exe /c %s", cmd1);
    CreateProcess(NULL, cmdA, NULL, NULL, TRUE, 0, NULL, NULL, &si1, &pi1);

    si2.dwFlags = STARTF_USESTDHANDLES;
    si2.hStdInput = hRead;

    char cmdB[512];
    sprintf(cmdB, "cmd.exe /c %s", cmd2);
    CreateProcess(NULL, cmdB, NULL, NULL, TRUE, 0, NULL, NULL, &si2, &pi2);

    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);

    CloseHandle(hWrite);
    CloseHandle(hRead);

    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);
}

int main() {
    char command[MAX_LINE];
    char history[MAX_HISTORY][MAX_LINE];
    int historyCount = 0;

    while (1) {

        char cwd[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, cwd);

        printf("%s> ", cwd);
        fflush(stdout);

        if (!fgets(command, sizeof(command), stdin))
            break;

        command[strcspn(command, "\n")] = '\0';

        if (strlen(command) == 0)
            continue;

        if (historyCount < MAX_HISTORY)
            strcpy(history[historyCount++], command);

        if (strcmp(command, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }
        if (strcmp(command, "paint") == 0) {
            paint();
            continue;
        }

        if (strcmp(command, "help") == 0) {
            printf("\n=== PrabuddhaShell v0.1 ===\n\n");

            printf("CORE COMMANDS:\n");
            printf("  help        - show this menu\n");
            printf("  exit        - exit shell\n");
            printf("  cd <dir>    - change directory\n");
            printf("  cls         - clear screen\n");
            printf("  dir         - list files\n");
            printf("  echo text   - print text\n");
            printf("  ver         - version\n");
            printf("  time        - show date/time\n");
            printf("  del <file>  - delete file\n");
            printf("  history     - show command history\n\n");

            printf("TOOLS:\n");
            printf("  ping <host>     - ping network host\n");
            printf("  curl <url>      - HTTP request\n");
            printf("  weather <city>  - weather report\n");
            printf("  cat <file>      - read file\n\n");

            printf("FEATURES:\n");
            printf("  pipe (cmd1 | cmd2)\n");
            printf("  paint           - mini drawing tool\n");
            printf("  call mom        - WhatsApp contact\n\n");

            printf("APPS:\n");
            printf("  brave, git, dis, vs, tele\n\n");

            printf("WEB:\n");
            printf("  youtube, github, leetcode\n\n");

            printf("===========================\n\n");
            continue;
        }

        if (strcmp(command, "history") == 0) {
            for (int i = 0; i < historyCount; i++)
                printf("%d. %s\n", i + 1, history[i]);
            continue;
        }

        if (strcmp(command, "cls") == 0) {
            clearScreen();
            continue;
        }

        if (strcmp(command, "dir") == 0) {
            dirCommand();
            continue;
        }

        if (strncmp(command, "echo ", 5) == 0) {
            echoCommand(command);
            continue;
        }

        if (strcmp(command, "ver") == 0) {
            versionCommand();
            continue;
        }

        if (strcmp(command, "time") == 0) {
            timeAndDateCommand();
            continue;
        }

        if (strncmp(command, "del ", 4) == 0) {
            delFileName(command + 4);
            continue;
        }

        if (strncmp(command, "cd ", 3) == 0) {
            SetCurrentDirectory(command + 3);
            continue;
        }

        if (strcmp(command, "brave") == 0) {
            system("ADDRESS OF THE APPLICATION");
            continue;
        }

        if (strcmp(command, "git") == 0) {
            system("ADDRESS OF THE APPLICATION");
            continue;
        }

        if (strcmp(command, "dis") == 0) {
            system("ADDRESS OF THE APPLICATION");
            continue;
        }

        if (strcmp(command, "vs") == 0) {
            system("ADDRESS OF THE APPLICATION");
            continue;
        }

        if (strcmp(command, "tele") == 0) {
            system("ADDRESS OF THE APPLICATION");
            continue;
        }

        if (strncmp(command, "ping ", 5) == 0) {
            char cmd[256];
            sprintf(cmd, "ping %s", command + 5);
            system(cmd);
            continue;
        }

        if (strncmp(command, "curl ", 5) == 0) {
            char cmd[256];
            sprintf(cmd, "curl %s", command + 5);
            system(cmd);
            continue;
        }

        if (strncmp(command, "weather ", 8) == 0) {
            char cmd[512];
            sprintf(cmd, "curl wttr.in/%s", command + 8);
            system(cmd);
            continue;
        }

        if (strncmp(command, "call ", 5) == 0) {
            char *name = command + 5;

            printf("Calling %s...\n", name);

            if (strcmp(name, "mom") == 0) {
                char url[256];
                sprintf(url, "start https://wa.me/%s", MOM_NUMBER);
                system(url);
            } else {
                printf("Contact not found\n");
            }
            continue;
        }

        if (strncmp(command, "cat ", 4) == 0) {
            FILE *f = fopen(command + 4, "r");
            if (!f) {
                printf("File not found\n");
                continue;
            }

            char c;
            while ((c = fgetc(f)) != EOF)
                putchar(c);

            fclose(f);
            continue;
        }

        char *pipePos = strchr(command, '|');

        if (pipePos) {
            char left[512], right[512];

            strncpy(left, command, pipePos - command);
            left[pipePos - command] = '\0';
            strcpy(right, pipePos + 1);

            executePipe(left, right);
            continue;
        }

        if (strcmp(command, "youtube") == 0) {
            system("start https://youtube.com");
            continue;
        }

        if (strcmp(command, "github") == 0) {
            system("start https://github.com");
            continue;
        }

        if (strcmp(command, "leetcode") == 0) {
            system("start https://leetcode.com");
            continue;
        }

        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        ZeroMemory(&pi, sizeof(pi));
        si.cb = sizeof(si);

        char fullCommand[2048];
        sprintf(fullCommand, "cmd.exe /c %s", command);

        if (!CreateProcess(NULL, fullCommand, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            printf("Unknown command\n");
            continue;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    return 0;
}
