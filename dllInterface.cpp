#include "stdafx.h"
#include "dllInterface.h"

void _stdcall DLL_CreateRenderer(HWND handle) {

}

void _stdcall DLL_ResizeRenderer(int w, int h) {

}

void _stdcall DLL_SetCallbackDeCarga(FNCallbackDeCarga cbk) {

}

void _stdcall DLL_LoadVolume(const char* pathToFile) {

}

void _stdcall DLL_SetThickness(int t) {

}

void _stdcall DLL_SetFuncao(int idFuncao) {

}

void _stdcall DLL_Reset() {

}

void _stdcall DLL_SetOperacaoDoMouse(int qualBotao, int qualOperacao) {

}

int _stdcall DLL_MouseMove(HWND wnd, UINT nFlags, int X, int Y) {
	return 0;
}

int _stdcall DLL_LMouseDown(HWND wnd, UINT nFlags, int X, int Y) {
	return 0;
}

int _stdcall DLL_LMouseUp(HWND wnd, UINT nFlags, int X, int Y) {
	return 0;
}

int _stdcall DLL_MMouseDown(HWND wnd, UINT nFlags, int X, int Y) {
	return 0;
}

int _stdcall DLL_MMouseUp(HWND wnd, UINT nFlags, int X, int Y) {
	return 0;
}

int _stdcall DLL_RMouseDown(HWND wnd, UINT nFlags, int X, int Y) {
	return 0;
}

int _stdcall DLL_RMouseUp(HWND wnd, UINT nFlags, int X, int Y) {
	return 0;
}