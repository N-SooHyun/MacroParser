#pragma once

#include <windows.h>

#define TIMER_INTERVAL 1000 // Ÿ�̸� ���� (ms)

// ���� ����
static volatile BOOL timerRunning = FALSE;       // Ÿ�̸� ����
static volatile DWORD elapsedTime = 0;           // ��� �ð� (��)
static volatile BOOL timerExit = FALSE;          // Ÿ�̸� ���� �÷���
static HANDLE hMutex = NULL;                     // ���ؽ� �ڵ�

// Ÿ�̸� ������ �Լ�
DWORD WINAPI TimerThread(LPVOID lpParam) {
    while (1) {
        if (timerRunning) {
            Sleep(TIMER_INTERVAL);

            WaitForSingleObject(hMutex, INFINITE);
            elapsedTime++;
            ReleaseMutex(hMutex);
        }
        else if (timerExit) {
            break;
        }
        else {
            Sleep(100);
        }
    }
    return 0;
}

// Ÿ�̸� ��Ʈ�� �Լ�
int Timer_Ctrl() {
    HANDLE hThread = NULL;

    // ���ؽ� ����
    hMutex = CreateMutex(NULL, FALSE, NULL);
    if (hMutex == NULL) {
        printf("���ؽ� ���� ����: %lu\n", GetLastError());
        return 1;
    }

    // Ÿ�̸� ������ ����
    hThread = CreateThread(
        NULL, 0, TimerThread, NULL, 0, NULL
    );

    if (hThread == NULL) {
        printf("������ ���� ����: %lu\n", GetLastError());
        CloseHandle(hMutex);
        return 1;
    }

    int input = 0;
    const int Trg_Pos = 200;
    int Cur_Pos = 0;
    const int Trg_Spd = 10;
    const int Trg_Tm = (Trg_Pos - Cur_Pos) / Trg_Spd;

    const int Trg_Dac = 100;
    int Cur_Dac = 0;
    long curTm = 0;

    // ����� �Է�
    printf("1. ��ӱ���,  2. J����,  3. r����,  4. ���� : ");
    scanf_s("%d", &input);
    timerRunning = TRUE;
    long lastPrintTime = -1;
    while (1) {
        WaitForSingleObject(hMutex, INFINITE);
        curTm = elapsedTime;
        ReleaseMutex(hMutex);

        float t = (float)curTm / Trg_Tm;
        if (t > 1.0f) t = 1.0f;

        switch (input) {
        case 1:
            Cur_Dac = (int)(Trg_Dac * t);
            break;
        case 2:
            Cur_Dac = (int)(Trg_Dac * t * t);
            break;
        case 3:
            Cur_Dac = (int)(Trg_Dac * (-t * t + 2 * t));
            break;
        case 4:
            timerRunning = FALSE;
            goto END;
        default:
            printf("�߸��� �Է��Դϴ�. �ٽ� �Է��ϼ���.\n");
            continue;
        }

        // ��ǥ ���� �� DAC �� Ÿ�̸� ����
        if (Cur_Dac >= Trg_Dac || curTm >= Trg_Tm) {
            Cur_Dac = Trg_Dac;         // ��Ȯ�� ����
            timerRunning = FALSE;     // Ÿ�̸� ������ ���� ��û
            printf("���� �Ϸ�: Time: %ld, DAC: %d, Pos: %d\n", curTm, Cur_Dac, Cur_Pos);
            goto END;
            break;
        }

        // �ð� ���� ���� ���� ���
        if (curTm != lastPrintTime) {
            printf("Time: %ld, DAC: %d, Pos: %d\n", curTm, Cur_Dac, Cur_Pos);
			Cur_Pos = Cur_Pos + Trg_Spd;
            lastPrintTime = curTm;
        }
    }

END:
    WaitForSingleObject(hMutex, INFINITE);
    timerExit = TRUE;
    ReleaseMutex(hMutex);

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    CloseHandle(hMutex);

    return 0;
}