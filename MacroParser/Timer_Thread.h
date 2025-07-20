#pragma once

#pragma once

#include <windows.h>
#include <stdio.h>
#include <Math.h>
#include <stdlib.h>

#define TICK 100

#define TIMER_INTERVAL TICK // Ÿ�̸� ���� (ms)  0.01sec

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
			elapsedTime += TICK;
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
}           // ���ؽ� �ڵ�

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

int Mov_Mt_Pos(double Cur_Pos, double Trg_Pos, double Trg_Spd, HANDLE& hThread, int Cur_Dac) {
	static int Cur_Tm = 0;
	const static int Trg_Tm = (Trg_Pos - Cur_Pos) / Trg_Spd * 1000;
	static long lastPrintTime = -1;

	WaitForSingleObject(hMutex, INFINITE);
	Cur_Tm = elapsedTime;
	ReleaseMutex(hMutex);

	if (Cur_Tm != lastPrintTime) {
		//printf("Time: %ld, DAC: %d, Pos: %.3f\n", Cur_Tm, Cur_Dac, Cur_Pos);
		Cur_Pos = Cur_Pos + Trg_Spd * (TICK / 1000.0);
		lastPrintTime = Cur_Tm;
	}

	return Cur_Pos;
}

int Incre_Dac() {
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

	const double Trg_Spd = 50.000;
	int DbgDac = 0;

	double TrgtPosStr = 128.000;
	double TrgtPosEnd = 500.000;
	double TrgtPosMid = (TrgtPosEnd - TrgtPosStr) / 2 + TrgtPosStr;
	double CurPos = TrgtPosStr;
	UINT16 DacMin = 0;
	UINT16 DacMax = 0;
	UINT16 CurDac = DacMin;
	double DacUpCal = 0.000;
	double DacDnCal = 0.000;

	printf("�Է��ϼ���\n");
	printf("Dac Min : ");
	scanf_s("%hu", &DacMin);
	printf("Dac Max : ");
	scanf_s("%hu", &DacMax);

	while (1) {
		timerRunning = TRUE;
		CurPos = Mov_Mt_Pos(CurPos, TrgtPosEnd, Trg_Spd, hThread, CurDac);

		if (CurDac != DbgDac) {
			printf("Cur_Pos: %.3f, DAC: %hu, Trg_Pos: %.3f\n", CurPos, CurDac, TrgtPosEnd);  // ������ ���
			DbgDac = CurDac;
		}

		if (CurPos <= TrgtPosMid) {
			//printf("CurPos: %.3f, TrgPosMid: %.3f\n", CurPos, TrgtPosMid); 
			DacUpCal = (CurPos - TrgtPosStr) / (TrgtPosMid - TrgtPosStr);
			if (DacUpCal > 1.0) DacUpCal = 1.0;
			if (DacUpCal < 0.0) DacUpCal = 0.0;

			//���
			double ConsSpd = DacUpCal;
			//J��
			double TypeJ = DacUpCal * DacUpCal;
			//r��
			double TypeR = (-DacUpCal * DacUpCal) + (2 * DacUpCal);

			//���
			//CurDac = (int)(DacMin + (DacMax - DacMin) * ConsSpd);
			//J��
			CurDac = (int)(DacMin + (DacMax - DacMin) * TypeJ);
			//R��
			//CurDac = (int)(DacMin + (DacMax - DacMin) * TypeR);

		}
		else if (CurPos <= TrgtPosEnd) {
			//printf("CurPos: %.3f, TrgPosEnd: %.3f\n", CurPos, TrgtPosEnd);
			DacDnCal = (CurPos - TrgtPosMid) / (TrgtPosEnd - TrgtPosMid);

			if (DacDnCal > 1.0) DacDnCal = 1.0;
			if (DacDnCal < 0.0) DacDnCal = 0.0;

			//���
			double ConsSpd = DacDnCal;
			//R��
			double TypeR = DacDnCal * DacDnCal;
			//J��
			double TypeJ = (-DacDnCal * DacDnCal) + (2 * DacDnCal);

			//���
			//CurDac = (int)(DacMax - (DacMax - DacMin) * ConsSpd);
			//J��
			//CurDac = (int)(DacMax - (DacMax - DacMin) * TypeJ);
			//R��
			CurDac = (int)(DacMax - (DacMax - DacMin) * TypeR);
		}

		if (CurPos > TrgtPosEnd) break;
	}

	//Ÿ�̸� ����
	WaitForSingleObject(hMutex, INFINITE);
	timerRunning = FALSE;
	timerExit = TRUE;
	ReleaseMutex(hMutex);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hMutex);

	return 0;
}

FILE* File_Write() {
	FILE* pFile = NULL;

	fopen_s(&pFile, "..\\Excel_Json_File\\DacGraph.json", "w");

	return pFile;
}

void No_Timer_Dac() {

	FILE* pFile = File_Write();

	const double Trg_Spd = 50.000;
	int DbgDac = 0;

	double TrgtPosStr = 128.000;
	double TrgtPosEnd = 500.000;
	double TrgtPosMid = (TrgtPosEnd - TrgtPosStr) / 2 + TrgtPosStr;
	double CurPos = TrgtPosStr;
	UINT16 DacMin = 0;
	UINT16 DacMax = 0;
	UINT16 CurDac = DacMin;
	double DacUpCal = 0.000;
	double DacDnCal = 0.000;


	printf("�Է��ϼ���\n");
	printf("Dac Min : ");
	scanf_s("%hu", &DacMin);
	printf("Dac Max : ");
	scanf_s("%hu", &DacMax);

	fprintf(pFile, "{\"TrgtStr\" : %.3f, \"TrgtEnd\" : %.3f, \"DacStr\" : %hu, \"DacEnd\" : %hu, \n \"DacGraph\":[", TrgtPosStr, TrgtPosEnd, CurDac, DacMax);
	while (1) {
		fprintf(pFile, "{\"CurPos\":%.3f, \"CurDac\":%hu}", CurPos, CurDac);
		if (CurPos <= TrgtPosMid) {
			DacUpCal = (CurPos - TrgtPosStr) / (TrgtPosMid - TrgtPosStr);
			if (DacUpCal > 1.0) DacUpCal = 1.0;
			if (DacUpCal < 0.0) DacUpCal = 0.0;

			//���
			double ConsSpd = DacUpCal;
			//J��
			double TypeJ = DacUpCal * DacUpCal;
			//r��
			double TypeR = (-DacUpCal * DacUpCal) + (2 * DacUpCal);

			//���
			//CurDac = (int)(DacMin + (DacMax - DacMin) * ConsSpd);
			//J��
			//CurDac = (int)(DacMin + (DacMax - DacMin) * TypeJ);
			//R��
			CurDac = (int)(DacMin + (DacMax - DacMin) * TypeR);

		}
		else if (CurPos <= TrgtPosEnd) {
			//printf("CurPos: %.3f, TrgPosEnd: %.3f\n", CurPos, TrgtPosEnd);
			DacDnCal = (CurPos - TrgtPosMid) / (TrgtPosEnd - TrgtPosMid);

			if (DacDnCal > 1.0) DacDnCal = 1.0;
			if (DacDnCal < 0.0) DacDnCal = 0.0;

			//���
			double ConsSpd = DacDnCal;
			//R��
			double TypeR = DacDnCal * DacDnCal;
			//J��
			double TypeJ = (-DacDnCal * DacDnCal) + (2 * DacDnCal);

			//���
			//CurDac = (int)(DacMax - (DacMax - DacMin) * ConsSpd);
			//J��
			//CurDac = (int)(DacMax - (DacMax - DacMin) * TypeJ);
			//R��
			CurDac = (int)(DacMax - (DacMax - DacMin) * TypeR);
		}

		if (CurPos > TrgtPosEnd) break;
		CurPos += 5;
		fprintf(pFile, ",\n");
		printf("Cur_Pos: %.3f, DAC: %hu, Trg_Pos: %.3f\n", CurPos, CurDac, TrgtPosEnd);  // ������ ���
	}
	fprintf(pFile, "]}\n");
	fclose(pFile);
}