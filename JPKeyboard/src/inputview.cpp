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

#include <inputview.h>
#include <QKeyEvent>
#include <QFontDatabase>
#include <QApplication>
#include <KeyboardGlobal.h>

namespace QtVirtualKeyboard {

	QString toRGB(QColor clr){
		return QString("rgb(%1,%2,%3)").arg(clr.red()).arg(clr.green()).arg(clr.blue());
	}
	//////////////////////////////////////////////////////////////////////////
	InputDialogView::InputDialogView(QObject* main, QWidget *parent) :
	QWidget(parent)
	{
//		m_backgroundClr = QColor(0, 0, 0);           // 主窗体背景颜色
//		m_btnbackgroundClr = QColor(56, 53, 51);     // 按钮背景颜色
//		m_btnHoverClr = QColor(50, 30, 24);          // 按钮hover颜色
//		m_btnPressedClr = QColor(30, 27, 24);        // 按钮按钮颜色
//		m_btnTxtClr = QColor(255, 255, 255);         // 按钮字体颜色
//		m_HanziClr = QColor(30, 27, 24);             // 汉字背景颜色
//		m_HanziHoverClr = QColor(0, 255, 0);         // 汉字hover颜色
//		m_HanziPressedClr = QColor(255, 255, 0);     // 汉字按下颜色
//		m_HanziTxtClr = QColor(90, 137, 46);         // 汉字字体颜色
//		m_HanziPressTxtClr = QColor(255, 0, 0);      // 汉字字体颜色
		m_backgroundClr = QColor(160, 160, 160);           // 主窗体背景颜色
		m_btnbackgroundClr = QColor(204, 204, 204);     // 按钮背景颜色
		m_btnHoverClr = QColor(203, 219, 239);          // 按钮hover颜色
		m_btnPressedClr = QColor(156, 179, 226);        // 按钮按钮颜色
		m_btnTxtClr = QColor(0, 0, 0);				  // 按钮字体颜色
		m_HanziClr = QColor(255, 255, 255);             // 汉字背景颜色
		m_HanziHoverClr = QColor(203, 219, 239);         // 汉字hover颜色
		m_HanziPressedClr = QColor(156, 179, 226);     // 汉字按下颜色
		m_HanziTxtClr = QColor(0, 0, 0);				 // 汉字字体颜色
		m_HanziPressTxtClr = QColor(255, 255, 240);      // 汉字字体颜色


    main_object = main;
    m_ViewMode = ViewABC;
    setWindowModality(Qt::WindowModal);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowDoesNotAcceptFocus);
    setWindowOpacity(1);
    setAttribute(Qt::WA_TranslucentBackground);

    connect(this, SIGNAL(virtualKeyPress(Qt::Key, const QString &, Qt::KeyboardModifiers, bool)),
        main_object->parent(), SLOT(virtualKeyPress(Qt::Key, const QString &, Qt::KeyboardModifiers, bool)));
    connect(this, SIGNAL(virtualKeyRelease(Qt::Key, const QString &, Qt::KeyboardModifiers)),
        main_object->parent(), SLOT(virtualKeyRelease(Qt::Key, const QString &, Qt::KeyboardModifiers)));

		QString str = QString("QWidget{background:%1; border:1px solid #353535; border-radius:2px;}\
							  QListWidget{background:%2; border: 0px; margin-left:1px; margin-right:1px;margin-top:1px; padding-left:0 px ;padding-right:0 px;}\
							  QListWidget::item{background:%3; color:%4; padding-left: 5%; padding-right: 5%;}\
							  QListWidget::Item:hover{background:%5; }\
							  QListWidget::item:selected{background:%6; color:%7; }\
							  QListWidget::item:selected:!active{border-width:0px; background:%8; }\
							  QPushButton{background:%9; color:%10; border:1px solid #353535; border-radius:3px; padding:10% 10% 10% 10%;} \
							  QPushButton:hover{ background:%11; }	 \
							  QPushButton:pressed{ background:%12;}")
							  .arg(toRGB(m_backgroundClr))
							  .arg(toRGB(m_HanziClr))
							  .arg(toRGB(m_HanziClr))
							  .arg(toRGB(m_HanziTxtClr))
							  .arg(toRGB(m_HanziHoverClr))
							  .arg(toRGB(m_HanziPressedClr))
							  .arg(toRGB(m_HanziPressTxtClr))
							  .arg(toRGB(m_HanziPressedClr))
							  .arg(toRGB(m_btnbackgroundClr))
							  .arg(toRGB(m_btnTxtClr))
							  .arg(toRGB(m_btnHoverClr))
							  .arg(toRGB(m_btnPressedClr));
		setStyleSheet(str);

    InitControl();
}

void InputDialogView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    emit sizeChanged();
    Layout();
}

InputDialogView::~InputDialogView()
{
//    delete m_mainLayout;
}

// 键盘布局[中文]
const int line1Value[InputDialogView::ViewMask][11] = {
    {Qt::Key_Q, Qt::Key_W, Qt::Key_E, Qt::Key_R, Qt::Key_T, Qt::Key_Y, Qt::Key_U, Qt::Key_I, Qt::Key_O, Qt::Key_P, Qt::Key_Backspace},
    {Qt::Key_1, Qt::Key_2, Qt::Key_3, Qt::Key_4, Qt::Key_5, Qt::Key_6, Qt::Key_7, Qt::Key_8, Qt::Key_9, Qt::Key_0, Qt::Key_Backspace},
    {Qt::Key_1, Qt::Key_2, Qt::Key_3, Qt::Key_4, Qt::Key_5, Qt::Key_6, Qt::Key_7, Qt::Key_8, Qt::Key_9, Qt::Key_0, Qt::Key_Backspace}
};
const int line2Value[InputDialogView::ViewMask][10] = {
    {Qt::Key_A, Qt::Key_S, Qt::Key_D, Qt::Key_F, Qt::Key_G, Qt::Key_H, Qt::Key_J, Qt::Key_K, Qt::Key_L, Qt::Key_Enter},
    {Qt::Key_Plus, Qt::Key_Minus, Qt::Key_division, Qt::Key_multiply, Qt::Key_Asterisk, Qt::Key_Equal, Qt::Key_Underscore, Qt::Key_Semicolon, Qt::Key_Colon, Qt::Key_Enter},
    {Qt::Key_QuoteLeft, Qt::Key_AsciiTilde, Qt::Key_Exclam, Qt::Key_At, Qt::Key_NumberSign, Qt::Key_Dollar, Qt::Key_Percent, Qt::Key_AsciiCircum, Qt::Key_Ampersand, Qt::Key_Enter}
};
const int line3Value[InputDialogView::ViewMask][11] = {
    {Qt::Key_Shift, Qt::Key_Z, Qt::Key_X, Qt::Key_C, Qt::Key_V, Qt::Key_B, Qt::Key_N, Qt::Key_M, Qt::Key_Comma, Qt::Key_Period, Qt::Key_Shift},
    {Key_Pager, Qt::Key_Apostrophe, Key_ApostropheRight, Qt::Key_QuoteDbl, Key_QuoteDblRight, Qt::Key_Comma, 	Qt::Key_Period, Qt::Key_Slash, Qt::Key_Bar, Qt::Key_Backslash, Key_Pager},
    {Key_Pager, Qt::Key_ParenLeft, Qt::Key_ParenRight, Qt::Key_BracketLeft, Qt::Key_BracketRight, Qt::Key_BraceLeft, Qt::Key_BraceRight, Qt::Key_Less, Qt::Key_Greater, Qt::Key_Question, Key_Pager}
};
const int line4Value[InputDialogView::ViewMask][6] = {
    {Key_SwitchNumber, Key_SwitchLanguage, Qt::Key_Space, Qt::Key_Question, Key_Other, Key_Exit},
    {Key_SwitchABC, Key_SwitchLanguage, Qt::Key_Space, Qt::Key_Period, Key_Other, Key_Exit},
    {Key_SwitchABC, Key_SwitchLanguage, Qt::Key_Space, Qt::Key_Period, Key_Other, Key_Exit}
};

void InputDialogView::InitControl()
{
    m_AllbtnList.clear();
    QFont deffont("Sans");
    deffont.setPixelSize(15);
    //////////////////////////////////////////////////////////////////////////
    m_backgroud = new QWidget(this);

    // 提示器
    m_ListWidget = new QListWidget(m_backgroud);
    m_ListWidget->setViewMode(QListView::ListMode);
    m_ListWidget->setFlow(QListView::LeftToRight);
    m_ListWidget->setObjectName(QStringLiteral("listWidget"));
    m_ListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ListWidget->setFont(deffont);
    connect(m_ListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(OnListWidgetItemClick(QListWidgetItem*)));
    connect(m_ListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(OnListWidgetItemClick(QListWidgetItem*)));

    // 第一行
    for (int i = 0; i < 11; i++)
    {
        m_btnLine1[i] = new QPushButton(m_backgroud);
        m_btnLine1[i]->setUserData(Qt::UserRole, new KeyBoardData(line1Value[m_ViewMode][i]));
        m_btnLine1[i]->setFont(deffont);
        connect(m_btnLine1[i], SIGNAL(pressed()), this, SLOT(press()));
        connect(m_btnLine1[i], SIGNAL(released()), this,  SLOT(release()));
        m_AllbtnList.append(m_btnLine1[i]);
    }
    // 第二行
    for (int i = 0; i < 10; i++)
    {
        m_btnLine2[i] = new QPushButton(m_backgroud);
        m_btnLine2[i]->setUserData(Qt::UserRole, new KeyBoardData(line2Value[m_ViewMode][i]));
        m_btnLine2[i]->setFont(deffont);
        connect(m_btnLine2[i], SIGNAL(pressed()), this, SLOT(press()));
        connect(m_btnLine2[i], SIGNAL(released()), this,  SLOT(release()));
        m_AllbtnList.append(m_btnLine2[i]);
    }

    // 第三行
    for (int i = 0; i < 11; i++)
    {
        m_btnLine3[i] = new QPushButton(m_backgroud);
        m_btnLine3[i]->setUserData(Qt::UserRole, new KeyBoardData(line3Value[m_ViewMode][i]));
        m_btnLine3[i]->setFont(deffont);
        connect(m_btnLine3[i], SIGNAL(pressed()), this, SLOT(press()));
        connect(m_btnLine3[i], SIGNAL(released()), this,  SLOT(release()));
        m_AllbtnList.append(m_btnLine3[i]);
    }

    // 第四行
    for (int i = 0; i < 6; i++)
    {
        m_btnLine4[i] = new QPushButton(m_backgroud);
        m_btnLine4[i]->setUserData(Qt::UserRole, new KeyBoardData(line4Value[m_ViewMode][i]));
        m_btnLine4[i]->setFont(deffont);
        connect(m_btnLine4[i], SIGNAL(pressed()), this, SLOT(press()));
        connect(m_btnLine4[i], SIGNAL(released()), this,  SLOT(release()));
        m_AllbtnList.append(m_btnLine4[i]);
    }

    ResetButtonStyle();
    InitTextAndIcon();
    Layout();
}


// 内容设置 图标设置
void InputDialogView::InitTextAndIcon()
{
    InputEngine::InputMode mode = InputEngine::Latin;
    bool shift = false;
    bool capsLock = false;

    if (InputContextBase)
    {
        mode = InputEngineBase->inputMode();
        shift = InputContextBase->shift();
        capsLock = InputContextBase->capsLock();
    }

    for (int i = 0; i < m_AllbtnList.size(); i++)
    {
        QPushButton* btn = m_AllbtnList.at(i);
        if (btn && btn->userData(Qt::UserRole))
        {
            KeyBoardData* key = (KeyBoardData*)btn->userData(Qt::UserRole);
            btn->setText(GetShowText(key->value, shift, capsLock, mode));
            switch (key->value)
            {
				case Qt::Key_Shift:
					if (capsLock)
						btn->setStyleSheet("QPushButton{background:rgb(90, 137, 46); \
										   image: url(:/QtQuick/VirtualKeyboard/content/styles/default/images/shift-c5d6b6.svg);}");		// Shift
					else if (shift)
						btn->setStyleSheet(QString("QPushButton{background:%1;\
												    image: url(:/QtQuick/VirtualKeyboard/content/styles/default/images/shift-80c342.svg);} \
													QPushButton:hover{ background:%2; } \
													QPushButton:pressed{ background:%3;}")
													.arg(toRGB(m_btnbackgroundClr))
													.arg(toRGB(m_btnHoverClr))
													.arg(toRGB(m_btnbackgroundClr)));		// Shift
					else
						btn->setStyleSheet(QString("QPushButton{background:%1;\
												    image: url(:/QtQuick/VirtualKeyboard/content/styles/default/images/shift-868482.svg);}\
												   QPushButton:hover{ background:%2; } \
												   QPushButton:pressed{ background:%3;}")
												   .arg(toRGB(m_btnbackgroundClr))
												   .arg(toRGB(m_btnHoverClr))
												   .arg(toRGB(m_btnbackgroundClr)));		// Shift
					break;
            }
        }
    }
}

// 设置按钮样式
void InputDialogView::ResetButtonStyle()
{
    for (int i = 0; i < m_AllbtnList.size(); i++)
    {
        QPushButton* btn = m_AllbtnList.at(i);
        if (btn && btn->userData(Qt::UserRole))
        {
            KeyBoardData* key = (KeyBoardData*)btn->userData(Qt::UserRole);
            switch (key->value)
            {
            case Qt::Key_Backspace:
                btn->setStyleSheet("QPushButton{image: url(:/QtQuick/VirtualKeyboard/content/styles/default/images/backspace-868482.svg);}");
                break;
            case Qt::Key_Enter:
                btn->setStyleSheet("QPushButton{image: url(:/QtQuick/VirtualKeyboard/content/styles/default/images/enter-868482.svg);}");
                break;
            case Key_Exit:
                btn->setStyleSheet("QPushButton{image: url(:/QtQuick/VirtualKeyboard/content/styles/default/images/hidekeyboard-868482.svg);}");
                break;
            case Key_SwitchLanguage:
                btn->setStyleSheet("QPushButton{image: url(:/QtQuick/VirtualKeyboard/content/styles/default/images/globe-868482.svg);}");
                break;
				case Key_Pager:
					btn->setStyleSheet(QString("QPushButton{background:%1;}\
											   QPushButton:hover{ background:%2; } \
											   QPushButton:pressed{ background:%3;}")
											   .arg(toRGB(m_btnbackgroundClr))
											   .arg(toRGB(m_btnHoverClr))
											   .arg(toRGB(m_btnbackgroundClr)));
					break;
            }
        }
    }
}

// 显示文字
QString InputDialogView::GetShowText(int key, bool bshift, bool bcaplock, InputEngine::InputMode mode)
{
    bool bUpper = (bshift || bcaplock);
    bool bEn = (mode == InputEngine::Latin);

    switch (key)
    {
    case Qt::Key_Space:
        switch (mode )
        {
        case InputEngine::Latin:	return QString::fromLocal8Bit("English");break;// 切换语言键
        case InputEngine::Pinyin:	return QString::fromLocal8Bit("简体中文");break;// 切换语言键
        }
        break;
    case Qt::Key_Ampersand :	return QString::fromLocal8Bit("&&");break;
    case Qt::Key_Apostrophe :	return QString::fromLocal8Bit("‘");break;
    case Key_ApostropheRight :	return QString::fromLocal8Bit("’");break;
    case Qt::Key_QuoteDbl :		return QString::fromLocal8Bit("“");break;
    case Key_QuoteDblRight :	return QString::fromLocal8Bit("”");break;
    default:
        return GetValueText(key, bshift, bUpper, mode);	break;
    }

    return QString("");
}

// 数据文字
QString InputDialogView::GetValueText(int key, bool bshift, bool bcaplock, InputEngine::InputMode mode)
{
    bool bUpper = (bshift || bcaplock);
    bool bEn = (mode == InputEngine::Latin);
    switch (key)
    {
    case Qt::Key_1:	return QString("1");break;
    case Qt::Key_2:	return QString("2");break;
    case Qt::Key_3:	return QString("3");break;
    case Qt::Key_4:	return QString("4");break;
    case Qt::Key_5:	return QString("5");break;
    case Qt::Key_6:	return QString("6");break;
    case Qt::Key_7:	return QString("7");break;
    case Qt::Key_8:	return QString("8");break;
    case Qt::Key_9:	return QString("9");break;
    case Qt::Key_0:	return QString("0");break;
    case Qt::Key_A:	return bUpper ? QString("A") : QString("a");break;
    case Qt::Key_B:	return bUpper ? QString("B") : QString("b");break;
    case Qt::Key_C:	return bUpper ? QString("C") : QString("c");break;
    case Qt::Key_D:	return bUpper ? QString("D") : QString("d");break;
    case Qt::Key_E:	return bUpper ? QString("E") : QString("e");break;
    case Qt::Key_F:	return bUpper ? QString("F") : QString("f");break;
    case Qt::Key_G:	return bUpper ? QString("G") : QString("g");break;
    case Qt::Key_H:	return bUpper ? QString("H") : QString("h");break;
    case Qt::Key_I:	return bUpper ? QString("I") : QString("i");break;
    case Qt::Key_J:	return bUpper ? QString("J") : QString("j");break;
    case Qt::Key_K:	return bUpper ? QString("K") : QString("k");break;
    case Qt::Key_L:	return bUpper ? QString("L") : QString("l");break;
    case Qt::Key_M:	return bUpper ? QString("M") : QString("m");break;
    case Qt::Key_N:	return bUpper ? QString("N") : QString("n");break;
    case Qt::Key_O:	return bUpper ? QString("O") : QString("o");break;
    case Qt::Key_P:	return bUpper ? QString("P") : QString("p");break;
    case Qt::Key_Q:	return bUpper ? QString("Q") : QString("q");break;
    case Qt::Key_R:	return bUpper ? QString("R") : QString("r");break;
    case Qt::Key_S:	return bUpper ? QString("S") : QString("s");break;
    case Qt::Key_T:	return bUpper ? QString("T") : QString("t");break;
    case Qt::Key_U:	return bUpper ? QString("U") : QString("u");break;
    case Qt::Key_V:	return bUpper ? QString("V") : QString("v");break;
    case Qt::Key_W:	return bUpper ? QString("W") : QString("w");break;
    case Qt::Key_X:	return bUpper ? QString("X") : QString("x");break;
    case Qt::Key_Y:	return bUpper ? QString("Y") : QString("y");break;
    case Qt::Key_Z:	return bUpper ? QString("Z") : QString("z");break;
    //////////////////////////////////////////////////////////////////////////
    case Qt::Key_QuoteLeft :return bEn ? QString::fromLocal8Bit("·") : QString::fromLocal8Bit("、");break;
    case Qt::Key_AsciiTilde:return bEn ? QString::fromLocal8Bit("~") : QString::fromLocal8Bit("～");break;
    case Qt::Key_Exclam		:return bEn ? QString::fromLocal8Bit("!") : QString::fromLocal8Bit("！");break;
    case Qt::Key_At :return bEn ? QString::fromLocal8Bit("@") : QString::fromLocal8Bit("@");break;
    case Qt::Key_NumberSign :return bEn ? QString::fromLocal8Bit("#") : QString::fromLocal8Bit("#");break;
    case Qt::Key_Dollar :return bEn ? QString::fromLocal8Bit("$") : QString::fromLocal8Bit("￥");break;
    case Qt::Key_Percent  :return bEn ? QString::fromLocal8Bit("%") : QString::fromLocal8Bit("%");break;
    case Qt::Key_AsciiCircum :return bEn ? QString::fromLocal8Bit("^") : QString::fromLocal8Bit("…");break;
    case Qt::Key_Ampersand :return bEn ? QString::fromLocal8Bit("&") : QString::fromLocal8Bit("&");break;
    case Qt::Key_Asterisk :return bEn ? QString::fromLocal8Bit("*") : QString::fromLocal8Bit("*");break;
    case Qt::Key_Equal :return bEn ? QString::fromLocal8Bit("=") : QString::fromLocal8Bit("=");break;
    case Qt::Key_Underscore :return bEn ? QString::fromLocal8Bit("_") : QString::fromLocal8Bit("—");break;

    case Qt::Key_Semicolon :return bEn ? QString::fromLocal8Bit(";") : QString::fromLocal8Bit("；");break;
    case Qt::Key_Colon :return bEn ? QString::fromLocal8Bit(":") : QString::fromLocal8Bit("：");break;
    case Qt::Key_Comma :return bEn ? QString::fromLocal8Bit(",") : QString::fromLocal8Bit("，");break;	// 逗号
    case Qt::Key_Period  :return bEn ? QString::fromLocal8Bit(".") : QString::fromLocal8Bit("。");break;	// 句号
    case Qt::Key_Slash :return bEn ? QString::fromLocal8Bit("/") : QString::fromLocal8Bit("/");break;
    case Qt::Key_Bar :return bEn ? QString::fromLocal8Bit("|") : QString::fromLocal8Bit("|");break;
    case Qt::Key_Backslash :return bEn ? QString::fromLocal8Bit("\\") : QString::fromLocal8Bit("、");
    case Qt::Key_Question :return bEn ? QString::fromLocal8Bit("?") : QString::fromLocal8Bit("？");break;	// 问号
    // 加减乘除键
    case Qt::Key_Plus :return bEn ? QString::fromLocal8Bit("+") : QString::fromLocal8Bit("+");break;
    case Qt::Key_Minus :return bEn ? QString::fromLocal8Bit("-") : QString::fromLocal8Bit("-");break;
    case Qt::Key_division: return bEn ? QString::fromLocal8Bit("÷") : QString::fromLocal8Bit("÷");break;
    case Qt::Key_multiply: return bEn ? QString::fromLocal8Bit("×") : QString::fromLocal8Bit("×");break;
    //一对键
    case Qt::Key_ParenLeft  :return bEn ? QString::fromLocal8Bit("(") : QString::fromLocal8Bit("（");break;
    case Qt::Key_ParenRight :return bEn ? QString::fromLocal8Bit(")") : QString::fromLocal8Bit("）");break;
    case Qt::Key_BracketLeft  :return bEn ? QString::fromLocal8Bit("[") : QString::fromLocal8Bit("【");break;
    case Qt::Key_BracketRight :return bEn ? QString::fromLocal8Bit("]") : QString::fromLocal8Bit("】");break;
    case Qt::Key_BraceLeft :return bEn ? QString::fromLocal8Bit("{") : QString::fromLocal8Bit("『");break;
    case Qt::Key_BraceRight :return bEn ? QString::fromLocal8Bit("}") : QString::fromLocal8Bit("』");break;
    case Qt::Key_Less :return bEn ? QString::fromLocal8Bit("<") : QString::fromLocal8Bit("《");break;
    case Qt::Key_Greater  :return bEn ? QString::fromLocal8Bit(">") : QString::fromLocal8Bit("》");break;
    case Qt::Key_Apostrophe :return bEn ? QString::fromLocal8Bit("'") : QString::fromLocal8Bit("‘");break;
    case Key_ApostropheRight :return bEn ? QString::fromLocal8Bit("'") : QString::fromLocal8Bit("’");break;
    case Qt::Key_QuoteDbl :return bEn ? QString::fromLocal8Bit("\"") : QString::fromLocal8Bit("“");break;
    case Key_QuoteDblRight :return bEn ? QString::fromLocal8Bit("\"") : QString::fromLocal8Bit("”");break;
    //////////////////////////////////////////////////////////////////////////
    // 控制键
    case Key_Pager:
        switch (m_ViewMode)
        {
        case ViewABC: break;
			case ViewNum0:return bEn ? QString::fromLocal8Bit("↓") : QString::fromLocal8Bit("↓");break;	// 下一页
			case ViewNum1:return bEn ? QString::fromLocal8Bit("↑") : QString::fromLocal8Bit("↑");break;	// 下一页
			}
        break;
    case Qt::Key_Backspace:	return QString::fromLocal8Bit("");break;						// 删除
    case Qt::Key_Enter:		return QString::fromLocal8Bit("");break;						// Enter
    case Qt::Key_Shift:		return QString::fromLocal8Bit("");break;						// Shift
    case Qt::Key_Space:		return QString::fromLocal8Bit(" ");break;						// 切换语言键
    case Key_SwitchNumber:	return QString::fromLocal8Bit("&&123");break;					// 切换数字键
    case Key_SwitchABC:		return QString::fromLocal8Bit("&&ABC");break;					// 切换字母键
    case Key_SwitchLanguage:return QString::fromLocal8Bit("");break;						// 切换语言键
    case Key_Other:			return QString::fromLocal8Bit(":-)");break;						//
    case Key_Exit:			return QString::fromLocal8Bit("");break;						// 退出键盘按键
    }

    return QString("");
}

// 修正布局
void InputDialogView::Layout()
{
    if (m_backgroud == NULL)
        return;
    //////////////////////////////////////////////////////////////////////////
    QFont deffont("Sans");
    deffont.setPixelSize(width()/40);

    m_backgroud->setGeometry(0, 0, width(), height());
    m_backgroud->setFixedSize(width(), height());
    m_backgroud->setFont(deffont);

    double hei = height();
    double wid = width();
    double margin = wid * 3 / 70;	// 左右边缘
		double margin_top = margin*1.5;	// 上边缘
		double padding = hei * 1 / 25;	// 间隔
    double margin_bottom = padding;// 下边缘

    const double btnHeight = (hei - margin_top - margin_bottom - 3 * padding)/4;
    const double btnWidth = (wid - margin*2 - padding*10)/11;

    m_ListWidget->setGeometry(0, 0, width(), margin_top - padding);
    m_ListWidget->setFont(deffont);
    // 第一行
    for (int i = 0; i < 11; i++)
    {
        int ax = margin + i * (btnWidth+padding);
        int ay = margin_top;
        m_btnLine1[i]->setGeometry(ax, ay, btnWidth, btnHeight);
        m_btnLine1[i]->setFont(deffont);
    }

    // 第二行
    for (int i = 0; i < 10; i++)
    {
        int ax = margin + i * (btnWidth+padding) + btnWidth / 2;
        int ay = margin_top + btnHeight + padding;
        switch (i)
        {
        case 9:	m_btnLine2[i]->setGeometry(ax, ay, btnWidth * 3 / 2 + padding, btnHeight);break;
        default:m_btnLine2[i]->setGeometry(ax, ay, btnWidth, btnHeight);break;
        }
        m_btnLine2[i]->setFont(deffont);
    }

    // 第三行
    for (int i = 0; i < 11; i++)
    {
        int ax = margin + i * (btnWidth+padding);
        int ay = margin_top + (btnHeight + padding)*2;
        m_btnLine3[i]->setGeometry(ax, ay, btnWidth, btnHeight);
        m_btnLine3[i]->setFont(deffont);
    }
    // 第四行
    int ax = margin;
    int ay = margin_top + (btnHeight + padding)*3;

    for (int i = 0; i < 6; i++)
    {
        int aw = 0;
        switch (i)
        {
        case 0:	aw = btnWidth * 1.5;	break;
        case 1:	aw = btnWidth;			break;
        case 2:	aw = btnWidth * 5.5;	break;
        case 3:	aw = btnWidth;			break;
        case 4:	aw = btnWidth;			break;
        case 5:	aw = wid - margin - ax;break;
        }
        m_btnLine4[i]->setGeometry(ax, ay, aw, btnHeight);
        ax += aw + padding;
        m_btnLine4[i]->setFont(deffont);
    }
}
// 切换语言输入法
bool InputDialogView::SwitchInputMode(InputEngine::InputMode mode)
{
	if (InputEngineBase == NULL || PinyinInputMethodBase == NULL || InputContextBase == NULL || PlainInputMethodBase == NULL)
	{
		return false;
	}

	if (mode == InputEngine::Latin)			// en_GB
	{
		if (!PinyinInputMethodBase->inputModes("zh_CN").contains(InputEngine::Pinyin))
		{
			return false;
		}
		InputContextBase->setLocale("zh_CN");
		InputEngineBase->setInputMethod(PinyinInputMethodBase);
		InputEngineBase->setInputMode(InputEngine::Pinyin);
	}
	else if (mode == InputEngine::Pinyin)	// zh_CN
	{
		if (!PlainInputMethodBase->inputModes("en_GB").contains(InputEngine::Latin))
		{
			return false;
		}
		InputContextBase->setLocale("en_GB");
		InputEngineBase->setInputMethod(PlainInputMethodBase);
		InputEngineBase->setInputMode(InputEngine::Latin);
	}
	InputContextBase->setShift(false);
	InputContextBase->setCapsLock(false);
		
	UpdateWordCandidate();
	InitTextAndIcon();

	return true;
}

// 切换shift
void InputDialogView::SwitchShiftMode()
{
    if (InputContextBase && InputContextBase->shiftHandler())
    {
        InputContextBase->shiftHandler()->toggleShift();
        InitTextAndIcon();
    }
}

// 切换显示模式
void InputDialogView::SwitchViewMode(InputDialogView::ViewMode mode)
{
    m_ViewMode = mode;
    // 第一行
    for (int i = 0; i < 11; i++)
    {
        if (m_btnLine1[i])
            m_btnLine1[i]->setUserData(Qt::UserRole, new KeyBoardData(line1Value[m_ViewMode][i]));
    }
    // 第二行
    for (int i = 0; i < 10; i++)
    {
        if (m_btnLine2[i])
            m_btnLine2[i]->setUserData(Qt::UserRole, new KeyBoardData(line2Value[m_ViewMode][i]));
    }

    // 第三行
    for (int i = 0; i < 11; i++)
    {
        if (m_btnLine3[i])
            m_btnLine3[i]->setUserData(Qt::UserRole, new KeyBoardData(line3Value[m_ViewMode][i]));
    }

    // 第四行
    for (int i = 0; i < 6; i++)
    {
        if (m_btnLine4[i])
            m_btnLine4[i]->setUserData(Qt::UserRole, new KeyBoardData(line4Value[m_ViewMode][i]));
    }

    ResetButtonStyle();
    InputContextBase->setShift(false);
    InputContextBase->setCapsLock(false);
    UpdateWordCandidate();
    InitTextAndIcon();
}

// 键盘退出
void InputDialogView::DoQuitView()
{
    if (PlatformInputContextBase)
        PlatformInputContextBase->hideInputPanel();
}

// 显示候选字
void InputDialogView::UpdateWordCandidate()
{
    m_ListWidget->clear();
    if (InputEngineBase && InputEngineBase->inputMethod() && InputEngineBase->inputMode() == InputEngine::Pinyin)
    {
        int nCount = InputEngineBase->inputMethod()->selectionListItemCount(SelectionListModel::WordCandidateList);
        for (int i = 0; i < nCount; i++)
        {
            QVariant value = InputEngineBase->inputMethod()->selectionListData(SelectionListModel::WordCandidateList,
                i, SelectionListModel::DisplayRole);
            QString strValue = value.toString();
            m_ListWidget->addItem(strValue);
        }
    }
}

// Shift状态恢复
void InputDialogView::ResetShiftStyle(bool bshift, bool bcaplock, InputEngine::InputMode mode)
{
    if (bshift && !bcaplock)
    {
        InputContextBase->setShift(false);
        InitTextAndIcon();
    }
}

// 根据键值获取按钮对象
QList<QPushButton*> InputDialogView::GetButtonList(int key)
{
    QList<QPushButton*> btnList;
    for (int i = 0; i < m_AllbtnList.size(); i++)
    {
        QPushButton* btn = m_AllbtnList.at(i);
        if (btn && btn->userData(Qt::UserRole))
        {
            KeyBoardData* keyData = (KeyBoardData*)btn->userData(Qt::UserRole);
            if (keyData->value == key)
            {
                btnList.append(btn);
            }
        }
    }

    return btnList;
}

// 扩展子点击事件
void InputDialogView::OnListWidgetItemClick(QListWidgetItem * item)
{
    if (m_ListWidget && item && InputEngineBase && InputEngineBase->wordCandidateListModel())
    {
        InputEngineBase->wordCandidateListModel()->selectItem(m_ListWidget->currentRow());
        UpdateWordCandidate();
    }
}

void InputDialogView::press()
{
    if (InputContextBase == NULL || InputEngineBase == NULL)
    {
        return;
    }
    InputEngine::InputMode mode = InputEngineBase->inputMode();
    bool shift = InputContextBase->shift();
    bool capsLock = InputContextBase->capsLock();
    QString strText = "";
    QPushButton* btn = (QPushButton*)sender();

    if (btn && btn->userData(Qt::UserRole))
    {
        KeyBoardData* key = (KeyBoardData*)btn->userData(Qt::UserRole);
        strText = GetValueText(key->value, shift, capsLock, mode);
        switch (key->value)
        {
        // 0-9
        case Qt::Key_1:		case Qt::Key_2:		case Qt::Key_3:		case Qt::Key_4:		case Qt::Key_5:		case Qt::Key_6:		case Qt::Key_7:		case Qt::Key_8:
        case Qt::Key_9:		case Qt::Key_0:
        // A-Z
        case Qt::Key_A:		case Qt::Key_B:		case Qt::Key_C:		case Qt::Key_D:		case Qt::Key_E:		case Qt::Key_F:		case Qt::Key_G:		case Qt::Key_H:
        case Qt::Key_I:		case Qt::Key_J:		case Qt::Key_K:		case Qt::Key_L:		case Qt::Key_M:		case Qt::Key_N:		case Qt::Key_O:		case Qt::Key_P:
        case Qt::Key_Q:		case Qt::Key_R:		case Qt::Key_S:		case Qt::Key_T:		case Qt::Key_U:		case Qt::Key_V:		case Qt::Key_W:		case Qt::Key_X:
        case Qt::Key_Y:		case Qt::Key_Z:		case Qt::Key_Space:	case Qt::Key_Backspace:		case Qt::Key_Enter: case Key_Other:
        // 符号
        case Qt::Key_QuoteLeft: case Qt::Key_AsciiTilde:	case Qt::Key_Exclam:	case Qt::Key_At:	case Qt::Key_NumberSign :	case Qt::Key_Dollar :
        case Qt::Key_Percent :	case Qt::Key_AsciiCircum :	case Qt::Key_Ampersand :case Qt::Key_Asterisk :	case Qt::Key_Equal :	case Qt::Key_Underscore :
        case Qt::Key_Semicolon :case Qt::Key_Colon :		case Qt::Key_Comma :	case Qt::Key_Period :	case Qt::Key_Slash :	case Qt::Key_Bar :
        case Qt::Key_Backslash :case Qt::Key_Question :
        // 加减乘除
        case Qt::Key_Plus :		case Qt::Key_Minus:			case Qt::Key_division:	case Qt::Key_multiply:
        // 一对
        case Qt::Key_ParenLeft:	case Qt::Key_ParenRight:	case Qt::Key_BracketLeft:	case Qt::Key_BracketRight:	case Qt::Key_BraceLeft:	case Qt::Key_BraceRight:
        case Qt::Key_Less:		case Qt::Key_Greater:		case Qt::Key_Apostrophe:	case Key_ApostropheRight:	case Qt::Key_QuoteDbl:	case Key_QuoteDblRight:
            emit virtualKeyPress((Qt::Key)key->value, strText, Qt::NoModifier, false);
            break;
        default:
            break;
        }
    }
}

void InputDialogView::release()
{
    if (InputContextBase == NULL)
    {
        return;
    }
    InputEngine::InputMode mode = InputEngineBase->inputMode();
    bool shift = InputContextBase->shift();
    bool capsLock = InputContextBase->capsLock();
    QString strText = "";
    QPushButton* btn = (QPushButton*)sender();

    if (btn && btn->userData(Qt::UserRole))
    {
        KeyBoardData* key = (KeyBoardData*)btn->userData(Qt::UserRole);
        strText = GetValueText(key->value, shift, capsLock, mode);

        switch (key->value)
        {
        case Qt::Key_Shift:
            SwitchShiftMode();
            break;
        case Key_SwitchLanguage:
            SwitchInputMode(mode);
            break;
        case Key_Exit:
            DoQuitView();
            break;
        case Key_SwitchABC:
            SwitchViewMode(ViewABC);
            break;
        case Key_SwitchNumber:
            SwitchViewMode(ViewNum0);
            break;
        case Key_Pager:
            switch (m_ViewMode)
            {
            case ViewNum0: SwitchViewMode(ViewNum1);break;
            case ViewNum1: SwitchViewMode(ViewNum0);break;
            }
            break;
        // 0-9
        case Qt::Key_1:		case Qt::Key_2:		case Qt::Key_3:		case Qt::Key_4:		case Qt::Key_5:		case Qt::Key_6:		case Qt::Key_7:		case Qt::Key_8:
        case Qt::Key_9:		case Qt::Key_0:
        // A-Z
        case Qt::Key_A:		case Qt::Key_B:		case Qt::Key_C:		case Qt::Key_D:		case Qt::Key_E:		case Qt::Key_F:		case Qt::Key_G:		case Qt::Key_H:
        case Qt::Key_I:		case Qt::Key_J:		case Qt::Key_K:		case Qt::Key_L:		case Qt::Key_M:		case Qt::Key_N:		case Qt::Key_O:		case Qt::Key_P:
        case Qt::Key_Q:		case Qt::Key_R:		case Qt::Key_S:		case Qt::Key_T:		case Qt::Key_U:		case Qt::Key_V:		case Qt::Key_W:		case Qt::Key_X:
        case Qt::Key_Y:		case Qt::Key_Z:		case Qt::Key_Space:	case Qt::Key_Backspace:	case Qt::Key_Enter:  case Key_Other:
        // 符号
        case Qt::Key_QuoteLeft: case Qt::Key_AsciiTilde:	case Qt::Key_Exclam:	case Qt::Key_At:	case Qt::Key_NumberSign :	case Qt::Key_Dollar :
        case Qt::Key_Percent :	case Qt::Key_AsciiCircum :	case Qt::Key_Ampersand :case Qt::Key_Asterisk :	case Qt::Key_Equal :	case Qt::Key_Underscore :
        case Qt::Key_Semicolon :case Qt::Key_Colon :		case Qt::Key_Comma :	case Qt::Key_Period :	case Qt::Key_Slash :	case Qt::Key_Bar :
        case Qt::Key_Backslash :case Qt::Key_Question :
        // 加减乘除
        case Qt::Key_Plus :		case Qt::Key_Minus:			case Qt::Key_division:	case Qt::Key_multiply:
        // 一对
        case Qt::Key_ParenLeft:	case Qt::Key_ParenRight:	case Qt::Key_BracketLeft:	case Qt::Key_BracketRight:	case Qt::Key_BraceLeft:	case Qt::Key_BraceRight:
        case Qt::Key_Less:		case Qt::Key_Greater:		case Qt::Key_Apostrophe:	case Key_ApostropheRight:	case Qt::Key_QuoteDbl:	case Key_QuoteDblRight:
            emit virtualKeyRelease((Qt::Key)key->value, strText, Qt::NoModifier);
            UpdateWordCandidate();
            ResetShiftStyle(shift, capsLock, mode);
            break;
        }
    }
}

} // namespace QtVirtualKeyboard
