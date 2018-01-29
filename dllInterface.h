#pragma once
#include "stdafx.h"

#define DLL_INTERFACE __declspec(dllexport)

extern "C" {
	DLL_INTERFACE void _stdcall DLL_CreateRenderer(HWND handle);
	DLL_INTERFACE void _stdcall DLL_ResizeRenderer(int w, int h);
	DLL_INTERFACE void _stdcall DLL_SetCallbackDeCarga(FNCallbackDeCarga cbk);
	DLL_INTERFACE void _stdcall DLL_LoadVolume(const char* pathToFile);
	DLL_INTERFACE void _stdcall DLL_SetThickness(double t);
	DLL_INTERFACE void _stdcall DLL_SetFuncao(int idFuncao);

	DLL_INTERFACE void _stdcall DLL_Reset();

	DLL_INTERFACE void _stdcall DLL_SetOperacaoDoMouse(int qualBotao, int qualOperacao);

	DLL_INTERFACE int _stdcall DLL_MouseMove(HWND wnd, UINT nFlags, int X, int Y);
	DLL_INTERFACE int _stdcall DLL_LMouseDown(HWND wnd, UINT nFlags, int X, int Y);
	DLL_INTERFACE int _stdcall DLL_LMouseUp(HWND wnd, UINT nFlags, int X, int Y);
	DLL_INTERFACE int _stdcall DLL_MMouseDown(HWND wnd, UINT nFlags, int X, int Y);
	DLL_INTERFACE int _stdcall DLL_MMouseUp(HWND wnd, UINT nFlags, int X, int Y);
	DLL_INTERFACE int _stdcall DLL_RMouseDown(HWND wnd, UINT nFlags, int X, int Y);
	DLL_INTERFACE int _stdcall DLL_RMouseUp(HWND wnd, UINT nFlags, int X, int Y);
}