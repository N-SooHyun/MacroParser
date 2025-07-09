#pragma once

#include <stdio.h>
#include <windows.h>

#define TIMER_INTERVAL 1000

// ���� ����
volatile BOOL timerRunning = FALSE;  // Ÿ�̸Ӱ� ���� ������ ����
volatile DWORD elapsedTime = 0;      // ����� �ð� (����: ��)

// ���ؽ� ����
HANDLE hMutex;

// Ÿ�̸� �Լ�
DWORD WINAPI TimerThread(LPVOID lpParam) {
	while (1) {
		if (timerRunning) {
			Sleep(TIMER_INTERVAL);  // 1�� ���

			// Ÿ�̸Ӱ� ���� ���� ���� ��� �ð��� ������Ŵ
			WaitForSingleObject(hMutex, INFINITE); // ���ؽ� ȹ��
			elapsedTime++;                      // ��� �ð� ����
			ReleaseMutex(hMutex);               // ���ؽ� ��ȯ
		}
		else {
			Sleep(100);  // Ÿ�̸Ӱ� ������ ���� ��⸸ ��
		}
	}
	return 0;
}

int Timer_Ctrl() {
	HANDLE hThread;

	// ���ؽ� �ʱ�ȭ
	hMutex = CreateMutex(NULL, FALSE, NULL);
	if (hMutex == NULL) {
		printf("���ؽ� ���� ����\n");
		return 1;
	}

	// Ÿ�̸� ������ ����
	hThread = CreateThread(
		NULL,             // �⺻ ���� �Ӽ�
		0,                // �⺻ ���� ũ��
		TimerThread,      // ������ �Լ�
		NULL,             // �Ű�����
		0,                // ���� �ɼ� (0: ��� ����)
		NULL              // ������ ID (NULL�̸� �ý����� �ڵ����� ����)
	);

	if (hThread == NULL) {
		printf("������ ���� ����\n");
		return 1;
	}

	int input;

	//timerRunning = True;		Ÿ�̸� ����
	//timerRunning = False;		Ÿ�̸� ����
	//elapsedTime = 0;			Ÿ�̸��� ���� ��(��������)

	//elapsedTime�� Read or Write�Ϸ��� �ݵ��
	//WaitForSingleObject(hMutex, INFINITE);
	//elapsedTime = 0;
	//ReleaseMutex(hMutex); 
	//�̷������� �������� ��

	int Trgt_Pos = 200;
	int Trgt_Spd = 5;
	int Cur_Pos = 0;

	int Time = ((Trgt_Pos - Cur_Pos) / Trgt_Spd) * 100;

	int Min_Dac = 0;
	int Max_Dac = 100;



	// ������� �Է��� ó��
	while (1) {
		printf("Ÿ�̸Ӹ� �����Ϸ��� 1�� �Է��ϰ�, ���߷��� 2�� �Է��ϼ���, ���� �ð��� Ȯ���Ϸ��� 3�� �Է��ϼ��� (�����Ϸ��� 0�� �Է�): ");
		scanf_s("%d", &input);


		if (input == 1) {
			// Ÿ�̸� ����
			timerRunning = TRUE;
			printf("Ÿ�̸Ӱ� ���۵Ǿ����ϴ�.\n");
		}
		else if (input == 2) {
			// Ÿ�̸� ����
			timerRunning = FALSE;

			// ���� ����� �ð� �б�
			WaitForSingleObject(hMutex, INFINITE); // ���ؽ� ȹ��
			printf("Ÿ�̸Ӱ� ������ϴ�. ����� �ð�: %ld ��\n", elapsedTime);
			elapsedTime = 0;  // Ÿ�̸Ӹ� ���߸� ��� �ð� �ʱ�ȭ
			ReleaseMutex(hMutex); // ���ؽ� ��ȯ
		}
		else if (input == 3) {
			// Ÿ�̸��� ���� ��� �ð��� �о����
			WaitForSingleObject(hMutex, INFINITE);  // ���ؽ� ȹ��
			printf("���� Ÿ�̸� ��� �ð�: %ld ��\n", elapsedTime); // Ÿ�̸� �� �б�
			ReleaseMutex(hMutex); // ���ؽ� ��ȯ
		}
		else if (input == 0) {
			// ���α׷� ����
			printf("���α׷��� �����մϴ�.\n");
			break;
		}
		else {
			printf("�߸��� �Է��Դϴ�.\n");
		}
	}

	// ������ ���� ���
	WaitForSingleObject(hThread, INFINITE);

	// ������ �ڵ� �ݱ�
	CloseHandle(hThread);
	CloseHandle(hMutex);  // ���ؽ� �ڵ� �ݱ�

	return 0;
}