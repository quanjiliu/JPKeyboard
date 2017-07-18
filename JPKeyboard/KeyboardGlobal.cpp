#include "KeyboardGlobal.h"

QVirtualKeyboardPlugin*	QVirtualKeyboardPluginBase = NULL;	// 插件类
PlatformInputContext*	PlatformInputContextBase = NULL;		// 服务类
InputContext*			InputContextBase = NULL;				// 文本管理类
InputEngine*			InputEngineBase = NULL;				// 输入法引擎
ShiftHandler*			ShiftHandlerBase = NULL;				//
PlainInputMethod*		PlainInputMethodBase = NULL;			// 英文输入法
PinyinInputMethod*		PinyinInputMethodBase = NULL;			// 中文输入法

JPKEYBOARDSHARED_EXPORT void GlobalInit()
{
    if (QVirtualKeyboardPluginBase == NULL)
    {
        QVirtualKeyboardPluginBase = new QVirtualKeyboardPlugin();
        PlatformInputContextBase = static_cast<PlatformInputContext*>(QVirtualKeyboardPluginBase->create(QLatin1String("qtvirtualkeyboard"), QStringList()));
        // 文本管理类
        InputContextBase = new InputContext(PlatformInputContextBase);
        // 英文输入法
        PlainInputMethodBase = new PlainInputMethod();
        // 中文输入法
        PinyinInputMethodBase = new PinyinInputMethod();
        // 输入法引擎
        InputEngineBase = InputContextBase->inputEngine();
        InputEngineBase->setInputMethod(PlainInputMethodBase);
        InputEngineBase->setInputMode(InputEngine::Latin);
        ShiftHandlerBase = InputContextBase->shiftHandler();
        PlatformInputContextBase->update(Qt::ImEnabled);
    }
}
