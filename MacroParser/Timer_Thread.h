#pragma once

#include <stdio.h>
#include <windows.h>

#define TIMER_INTERVAL 1000

// 전역 변수
volatile BOOL timerRunning = FALSE;  // 타이머가 실행 중인지 여부
volatile DWORD elapsedTime = 0;      // 경과된 시간 (단위: 초)

// 뮤텍스 생성
HANDLE hMutex;

// 타이머 함수
DWORD WINAPI TimerThread(LPVOID lpParam) {
	while (1) {
		if (timerRunning) {
			Sleep(TIMER_INTERVAL);  // 1초 대기

			// 타이머가 실행 중일 때만 경과 시간을 증가시킴
			WaitForSingleObject(hMutex, INFINITE); // 뮤텍스 획득
			elapsedTime++;                      // 경과 시간 증가
			ReleaseMutex(hMutex);               // 뮤텍스 반환
		}
		else {
			Sleep(100);  // 타이머가 멈췄을 때는 대기만 함
		}
	}
	return 0;
}

int Timer_Ctrl() {
	HANDLE hThread;

	// 뮤텍스 초기화
	hMutex = CreateMutex(NULL, FALSE, NULL);
	if (hMutex == NULL) {
		printf("뮤텍스 생성 실패\n");
		return 1;
	}

	// 타이머 스레드 생성
	hThread = CreateThread(
		NULL,             // 기본 보안 속성
		0,                // 기본 스택 크기
		TimerThread,      // 스레드 함수
		NULL,             // 매개변수
		0,                // 실행 옵션 (0: 즉시 실행)
		NULL              // 스레드 ID (NULL이면 시스템이 자동으로 관리)
	);

	if (hThread == NULL) {
		printf("스레드 생성 실패\n");
		return 1;
	}

	int input;

	//timerRunning = True;		타이머 시작
	//timerRunning = False;		타이머 멈춤
	//elapsedTime = 0;			타이머의 현재 값(전역변수)

	//elapsedTime을 Read or Write하려면 반드시
	//WaitForSingleObject(hMutex, INFINITE);
	//elapsedTime = 0;
	//ReleaseMutex(hMutex); 
	//이런식으로 사용해줘야 함

	int Trgt_Pos = 200;
	int Trgt_Spd = 5;
	int Cur_Pos = 0;

	int Time = ((Trgt_Pos - Cur_Pos) / Trgt_Spd) * 100;

	int Min_Dac = 0;
	int Max_Dac = 100;



	// 사용자의 입력을 처리
	while (1) {
		printf("타이머를 시작하려면 1을 입력하고, 멈추려면 2를 입력하세요, 현재 시간을 확인하려면 3을 입력하세요 (종료하려면 0을 입력): ");
		scanf_s("%d", &input);


		if (input == 1) {
			// 타이머 시작
			timerRunning = TRUE;
			printf("타이머가 시작되었습니다.\n");
		}
		else if (input == 2) {
			// 타이머 멈춤
			timerRunning = FALSE;

			// 현재 경과된 시간 읽기
			WaitForSingleObject(hMutex, INFINITE); // 뮤텍스 획득
			printf("타이머가 멈췄습니다. 경과된 시간: %ld 초\n", elapsedTime);
			elapsedTime = 0;  // 타이머를 멈추면 경과 시간 초기화
			ReleaseMutex(hMutex); // 뮤텍스 반환
		}
		else if (input == 3) {
			// 타이머의 현재 경과 시간을 읽어오기
			WaitForSingleObject(hMutex, INFINITE);  // 뮤텍스 획득
			printf("현재 타이머 경과 시간: %ld 초\n", elapsedTime); // 타이머 값 읽기
			ReleaseMutex(hMutex); // 뮤텍스 반환
		}
		else if (input == 0) {
			// 프로그램 종료
			printf("프로그램을 종료합니다.\n");
			break;
		}
		else {
			printf("잘못된 입력입니다.\n");
		}
	}

	// 스레드 종료 대기
	WaitForSingleObject(hThread, INFINITE);

	// 스레드 핸들 닫기
	CloseHandle(hThread);
	CloseHandle(hMutex);  // 뮤텍스 핸들 닫기

	return 0;
}