/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Virtual Keyboard module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef INPUTVIEW_H
#define INPUTVIEW_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QListWidget>
#include <inputengine.h>
#include <jpkeyboard_global.h>
#include <QColor>

namespace QtVirtualKeyboard {

class JPKEYBOARDSHARED_EXPORT KeyBoardData : public QObjectUserData{
public:
	KeyBoardData(){ value = 0;	}
	KeyBoardData(int v){ value = v;	}
public:
	int value;
};

class JPKEYBOARDSHARED_EXPORT InputDialogView : public QWidget
{
	Q_OBJECT
public:
	explicit InputDialogView(QObject* main, QWidget *parent = 0);
	~InputDialogView();
public:
	enum ViewMode
	{
		ViewABC = 0,
		ViewNum0 = 1,
		ViewNum1 = 2,
		ViewMask = 3
	};
private:
	void InitControl();
	void InitControl2();
	void InitTextAndIcon();
	bool SwitchInputMode(InputEngine::InputMode mode);
	void SwitchShiftMode();
	void DoQuitView();
	void UpdateWordCandidate();		// 显示候选字
	void ResetShiftStyle(bool bshift, bool bcaplock, InputEngine::InputMode mode);			// Shift状态恢复
	QString GetShowText(int key, bool bshift, bool bcaplock, InputEngine::InputMode mode);
	QString GetValueText(int key, bool bshift, bool bcaplock, InputEngine::InputMode mode);
	void Layout();
	void ResetButtonStyle();
public:
	QList<QPushButton*> GetButtonList(int key);
	InputDialogView::ViewMode GetViewMode(){	return m_ViewMode;	}
	void SwitchViewMode(InputDialogView::ViewMode mode);
private:
	QWidget*	 m_backgroud;	// 背景
	QPushButton* m_btnLine1[11];// 第一行按钮组
	QPushButton* m_btnLine2[10];// 第一行按钮组
	QPushButton* m_btnLine3[11];// 第一行按钮组
	QPushButton* m_btnLine4[6];	// 第一行按钮组
	QList<QPushButton*>	m_AllbtnList;	// 全部按钮集合
	QListWidget* m_ListWidget;	// 提示器
	InputDialogView::ViewMode m_ViewMode;

    QColor m_backgroundClr;     // 主窗体背景颜色
    QColor m_btnbackgroundClr;  // 按钮背景颜色
    QColor m_btnHoverClr;       // 按钮hover颜色
    QColor m_btnPressedClr;     // 按钮按下颜色
    QColor m_btnTxtClr;         // 按钮字体颜色

    QColor m_HanziClr;          // 汉字背景颜色
    QColor m_HanziHoverClr;     // 汉字hover颜色
    QColor m_HanziPressedClr;   // 汉字按下颜色
    QColor m_HanziTxtClr;       // 汉字字体颜色
    QColor m_HanziPressTxtClr;  // 汉字字体颜色
signals:
	void sendKeyToFocusItem(const QString &keyText);
	bool virtualKeyPress(Qt::Key key, const QString &text, Qt::KeyboardModifiers modifiers, bool isAutoRepeat);
	bool virtualKeyRelease(Qt::Key key, const QString &text, Qt::KeyboardModifiers modifiers);
public slots:
	void press();
	void release();
	void OnListWidgetItemClick(QListWidgetItem * item);
signals:
	void sizeChanged();
protected:
	void resizeEvent(QResizeEvent *event);
public:
	QObject* main_object;
};

} // namespace QtVirtualKeyboard

#endif // INPUTVIEW_H
