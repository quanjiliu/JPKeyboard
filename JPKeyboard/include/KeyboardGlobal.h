#ifndef __KEYBOARD_H_7DDFE17F_8AE4_41BB_89D8_D3A3FD80A124__
#define __KEYBOARD_H_7DDFE17F_8AE4_41BB_89D8_D3A3FD80A124__
#pragma once
#include <plugin.h>
#include <inputcontext.h>
#include <plaininputmethod.h>
#include <pinyininputmethod.h>
#include <shifthandler.h>
#include <inputengine.h>
#include <QtCore/qglobal.h>
#include <jpkeyboard_global.h>

using namespace QtVirtualKeyboard;
	
#define Key_Exit				Qt::Key_unknown + 1		// 退出键盘按键
#define Key_SwitchNumber		Qt::Key_unknown + 2		// 切换数字键
#define Key_SwitchLanguage		Qt::Key_unknown + 3		// 切换语言键
#define Key_Other				Qt::Key_unknown + 4		// 键
#define Key_Pager				Qt::Key_unknown + 10	// 上一页 下一页
#define Key_SwitchABC			Qt::Key_unknown + 11	// 切换字母
#define Key_dian				Qt::Key_unknown + 12	// 点
#define Key_duihao				Qt::Key_unknown + 13	// 对号

#define Key_QuoteDblRight		Qt::Key_unknown + 100	// “” ‘’ "" ''
#define Key_ApostropheRight		Qt::Key_unknown + 101	// “” ‘’ "" ''

//////////////////////////////////////////////////////////////////////////
#define QVirtualKeyboardPluginBase JPGlobalKeyBoard::QVirtualKeyboardPluginBase_    // 插件类
#define PlatformInputContextBase JPGlobalKeyBoard::PlatformInputContextBase_		// 服务类
#define InputContextBase JPGlobalKeyBoard::InputContextBase_	                    // 文本管理类
#define InputEngineBase JPGlobalKeyBoard::InputEngineBase_                          // 输入法引擎
#define ShiftHandlerBase JPGlobalKeyBoard::ShiftHandlerBase_
#define PlainInputMethodBase JPGlobalKeyBoard::PlainInputMethodBase_                // 英文输入法
#define PinyinInputMethodBase JPGlobalKeyBoard::PinyinInputMethodBase_              // 中文输入法

//////////////////////////////////////////////////////////////////////////
class JPKEYBOARDSHARED_EXPORT JPGlobalKeyBoard{
public:
    static QVirtualKeyboardPlugin*	QVirtualKeyboardPluginBase_;	// 插件类
    static PlatformInputContext*	PlatformInputContextBase_;		// 服务类
    static InputContext*			InputContextBase_;				// 文本管理类
    static InputEngine*             InputEngineBase_;                // 输入法引擎
    static ShiftHandler*			ShiftHandlerBase_;
    static PlainInputMethod*		PlainInputMethodBase_;			// 英文输入法
    static PinyinInputMethod*		PinyinInputMethodBase_;			// 中文输入法
};

JPKEYBOARDSHARED_EXPORT void GlobalInit();

#endif//__KEYBOARD_H_7DDFE17F_8AE4_41BB_89D8_D3A3FD80A124__
