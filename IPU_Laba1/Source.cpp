#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <stdio.h>
#include <Windows.h>
#include <setupapi.h>
#include <locale.h>
#include <vector>
#include <devguid.h>
#include <iostream>
#include <cctype>
#include <cstring>
#include <string>
#include <sstream>
#include <iomanip>
#pragma comment(lib, "setupapi.lib")
using namespace std;

void deviceInfoDataSize(SP_DEVINFO_DATA& DeviceInfoData) {
	 DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
}
void findInFile(string VEN, string DEV);

HDEVINFO getDeviceInfoSet() {
	HDEVINFO deviceInfoSet;
	GUID *guidDev = (GUID*)&GUID_DEVCLASS_USB;
	deviceInfoSet = SetupDiGetClassDevs(guidDev, "PCI", NULL, DIGCF_ALLCLASSES);
	if (deviceInfoSet == INVALID_HANDLE_VALUE) {
		return 0;
	}
	else return deviceInfoSet;
}

void getInfoPCI(SP_DEVINFO_DATA DeviceInfoData, HDEVINFO deviceInfoSet) {
	TCHAR buffer[4000];
	for (int i = 0; SetupDiEnumDeviceInfo(deviceInfoSet, i++, &DeviceInfoData);) {
		SP_DEVINFO_DATA deviceInfoData;
		ZeroMemory(&deviceInfoData, sizeof(SP_DEVINFO_DATA));
		deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

		if (SetupDiEnumDeviceInfo(deviceInfoSet, i, &deviceInfoData) == FALSE) {
			if (GetLastError() == ERROR_NO_MORE_ITEMS) {
				break;
			}
		}
		DWORD nSize = 0;
		SetupDiGetDeviceInstanceId(deviceInfoSet, &deviceInfoData, buffer, sizeof(buffer), &nSize);

		char tempDev[5];
		for (int i = 4; i < 8; i++) tempDev[i - 4] = (char)tolower(std::strstr(buffer, "DEV")[i]);
		tempDev[4] = '\0';
		char tempVEN[5];
		for (int i = 4; i < 8; i++) tempVEN[i - 4] = (char)tolower(std::strstr(buffer, "VEN")[i]);
		tempVEN[4] = '\0';
		string DEV = tempDev;
		string VEN = tempVEN;
		std::cout << std::endl << "#" << i << " " << endl;
		findInFile(DEV, VEN);
		}
	
}

void findInFile(string DEV, string VEN) {
	ifstream file("pci_ids.txt");
	while (file) {

		string str;
		getline(file, str);
		if ((int)str.find(VEN) != -1) {
			cout << str << endl;
			while (file) {
				getline(file, str);
				if ((int)str.find(DEV) != -1) {
					cout << str << endl;
					break;
				}
			}
			break;
		}

	}
}

int main() {

	SP_DEVINFO_DATA DeviceInfoData;
	deviceInfoDataSize(DeviceInfoData);
	HDEVINFO deviceInfoSet = getDeviceInfoSet();
	
	getInfoPCI(DeviceInfoData, deviceInfoSet);

	if (!SetupDiDestroyDeviceInfoList(deviceInfoSet)) {
		std::cout << GetLastError();
	}

	getchar();
	return 0;
}
