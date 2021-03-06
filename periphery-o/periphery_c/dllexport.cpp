// Dll1.cpp : 定义 DLL 应用程序的导出函数。
//
#include "pch.h"
#include <iostream>
#include <exception>

#include "stdafx.h"
#include "dllexport.h"


using namespace std;

int gp_main(int argc, char* argv[]);



 


 DWORD WINAPI gp_thr(LPVOID lpParameter)
{
	 int ret = 1;
	 int argc = 8;
	 char* argv[15] = { 0 };
	 char argv_0[32] = "gpatc.exe";
	 char argv_1[32] = "-run";
	 char argv_2[32] = "-wait";
	 char argv_3[32] = "-enc";
	 char argv_4[32] = "-script=gp.script";
	 char argv_5[32] = "-sconfig=gps.conf";
	 char argv_6[32] = "-uconfig=gpu.conf";
	 char argv_7[32] = "-role=";

	 try
	 {
		 char *role = (char *)lpParameter;
		 argv[0] = argv_0;
		 argv[1] = argv_1;
		 argv[2] = argv_2;
		 argv[3] = argv_3;
		 argv[4] = argv_4;
		 argv[5] = argv_5;
		 argv[6] = argv_6;
		 strcat_s(argv_7, role);
		 argv[7] = argv_7;
		 argv[8] = 0;
		 ret = gp_main(argc, argv);
	 }
	 catch (exception& e)
	 {
		 cout << "main error" << e.what() << endl;
	 }
	return ret;
}

 DWORD WINAPI gp_thr_quick(LPVOID lpParameter)
 {
	 int ret = 1;
	 int argc = 10;
	 char* argv[15] = { 0 };
	 char argv_0[32] = "gpatc.exe";
	 char argv_1[32] = "-run";
	 char argv_2[32] = "-wait";
	 char argv_3[32] = "-enc";
	 char argv_4[32] = "-script=gp.script";
	 char argv_5[32] = "-sconfig=gps.conf";
	 char argv_6[32] = "-uconfig=gpu.conf";
	 char argv_7[32] = "-role=";
	 char argv_8[32] = "-direct";
	 char argv_9[32] = "-step=6";
	 try
	 {
		 char *role = (char *)lpParameter;
		 argv[0] = argv_0;
		 argv[1] = argv_1;
		 argv[2] = argv_2;
		 argv[3] = argv_3;
		 argv[4] = argv_4;
		 argv[5] = argv_5;
		 argv[6] = argv_6;
		 strcat_s(argv_7, role);
		 argv[7] = argv_7;
		 argv[8] = argv_8;
		 argv[9] = argv_9;
		 argv[10] = 0;
		 ret = gp_main(argc, argv);
	 }
	 catch (exception& e)
	 {
		 cout << "quick main error" <<  e.what() << endl;
	 }	 return ret;
 }

 DWORD WINAPI gp_thr_no_enc(LPVOID lpParameter)
 {
	 int ret = 1;
	 int argc = 8;
	 char* argv[15] = { 0 };
	 char argv_0[32] = "gpatc.exe";
	 char argv_1[32] = "-run";
	 char argv_2[32] = "-wait";
	 char argv_3[32] = "-script=gp.script";
	 char argv_4[32] = "-sconfig=gps.conf";
	 char argv_5[32] = "-uconfig=gpu.conf";
	 char argv_6[32] = "-role=";

	 try
	 {
		 char* role = (char*)lpParameter;
		 argv[0] = argv_0;
		 argv[1] = argv_1;
		 argv[2] = argv_2;
		 argv[3] = argv_3;
		 argv[4] = argv_4;
		 argv[5] = argv_5;
		 strcat_s(argv_6, role);
		 argv[6] = argv_6;
		 argv[7] = 0;
		 ret = gp_main(argc, argv);
	 }
	 catch (exception & e)
	 {
		 cout << "no enc main error" << e.what() << endl;
	 }
	 return ret;
 }