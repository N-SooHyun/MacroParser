#pragma once

#include <windows.h>

#define TIMER_INTERVAL 1000 // 타이머 간격 (ms)

// 전역 변수
static volatile BOOL timerRunning = FALSE;       // 타이머 상태
static volatile DWORD elapsedTime = 0;           // 경과 시간 (초)
static volatile BOOL timerExit = FALSE;          // 타이머 종료 플래그
static HANDLE hMutex = NULL;                     // 뮤텍스 핸들

// 타이머 스레드 함수
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

// 타이머 컨트롤 함수
int Timer_Ctrl() {
    HANDLE hThread = NULL;

    // 뮤텍스 생성
    hMutex = CreateMutex(NULL, FALSE, NULL);
    if (hMutex == NULL) {
        printf("뮤텍스 생성 실패: %lu\n", GetLastError());
        return 1;
    }

    // 타이머 스레드 생성
    hThread = CreateThread(
        NULL, 0, TimerThread, NULL, 0, NULL
    );

    if (hThread == NULL) {
        printf("스레드 생성 실패: %lu\n", GetLastError());
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

    // 사용자 입력
    printf("1. 등속기울기,  2. J자형,  3. r자형,  4. 종료 : ");
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
            printf("잘못된 입력입니다. 다시 입력하세요.\n");
            continue;
        }

        // 목표 도달 시 DAC 및 타이머 정지
        if (Cur_Dac >= Trg_Dac || curTm >= Trg_Tm) {
            Cur_Dac = Trg_Dac;         // 정확히 고정
            timerRunning = FALSE;     // 타이머 스레드 종료 요청
            printf("도달 완료: Time: %ld, DAC: %d, Pos: %d\n", curTm, Cur_Dac, Cur_Pos);
            goto END;
            break;
        }

        // 시간 변동 있을 때만 출력
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