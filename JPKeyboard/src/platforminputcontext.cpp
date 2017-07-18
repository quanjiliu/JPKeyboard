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

#include <platforminputcontext.h>
#include <inputcontext.h>
#include <inputengine.h>
#include <abstractinputpanel.h>
#include <desktopinputpanel2.h>
#include <appinputpanel.h>
#include <virtualkeyboarddebug.h>
#include <QWindow>
#include <QGuiApplication>

namespace QtVirtualKeyboard {

/*!
    \class QtVirtualKeyboard::PlatformInputContext
    \internal
*/

PlatformInputContext::PlatformInputContext() :
    m_inputContext(0),
    m_inputPanel(0),
    m_focusObject(0),
    m_locale(),
    m_inputDirection(m_locale.textDirection()),
    m_filterEvent(0),
    m_visible(false)
{
}

PlatformInputContext::~PlatformInputContext()
{
}

bool PlatformInputContext::isValid() const
{
    return true;
}

void PlatformInputContext::reset()
{
    VIRTUALKEYBOARD_DEBUG() << "PlatformInputContext::reset()";
    if (m_inputContext)
        m_inputContext->reset();
}

void PlatformInputContext::commit()
{
    VIRTUALKEYBOARD_DEBUG() << "PlatformInputContext::commit()";
    if (m_inputContext)
        m_inputContext->externalCommit();
}

void PlatformInputContext::update(Qt::InputMethodQueries queries)
{
    VIRTUALKEYBOARD_DEBUG() << "PlatformInputContext::update():" << queries;
//  bool enabled = inputMethodQuery(Qt::ImEnabled).toBool();
	bool enabled = true;
//	if (enabled && !m_inputPanel) {
//		m_inputPanel = new DesktopInputPanel2(this);
//		m_inputPanel->createView();
//	}

    if (m_inputContext) {
        if (enabled) {
            m_inputContext->update(queries);
            if (m_visible)
                updateInputPanelVisible();
        } else {
            hideInputPanel();
        }
        m_inputContext->setFocus(enabled);
    }
}

QRectF PlatformInputContext::keyboardRect() const
{
    return m_inputContext ? m_inputContext->keyboardRectangle() : QRectF();
}

bool PlatformInputContext::isAnimating() const
{
    return m_inputContext ? m_inputContext->animating() : false;
}

void PlatformInputContext::showInputPanel()
{
    if (!m_visible) {
        VIRTUALKEYBOARD_DEBUG() << "PlatformInputContext::showInputPanel()";
        m_visible = true;
    }
    updateInputPanelVisible();
}

void PlatformInputContext::hideInputPanel()
{
    if (m_visible) {
        VIRTUALKEYBOARD_DEBUG() << "PlatformInputContext::hideInputPanel()";
        m_visible = false;
    }
    updateInputPanelVisible();
}

bool PlatformInputContext::isInputPanelVisible() const
{
    return m_inputPanel ? m_inputPanel->isVisible() : false;
}

QLocale PlatformInputContext::locale() const
{
    return m_locale;
}

void PlatformInputContext::setLocale(QLocale locale)
{
    if (m_locale != locale) {
        VIRTUALKEYBOARD_DEBUG() << "PlatformInputContext::setLocale():" << locale;
        m_locale = locale;
    }
}

Qt::LayoutDirection PlatformInputContext::inputDirection() const
{
    return m_inputDirection;
}

void PlatformInputContext::setInputDirection(Qt::LayoutDirection direction)
{
    if (m_inputDirection != direction) {
        VIRTUALKEYBOARD_DEBUG() << "PlatformInputContext::setInputDirection():" << direction;
        m_inputDirection = direction;
    }
}

QObject *PlatformInputContext::focusObject()
{
    return m_focusObject;
}

void PlatformInputContext::setFocusObject(QObject *object)
{
    VIRTUALKEYBOARD_DEBUG() << "PlatformInputContext::setFocusObject():" << object;
    if (m_focusObject != object) {
        if (m_focusObject)
            m_focusObject->removeEventFilter(this);
        m_focusObject = object;
        if (m_focusObject)
            m_focusObject->installEventFilter(this);
        emit focusObjectChanged();
    }
    update(Qt::ImQueryAll);
}

InputContext *PlatformInputContext::inputContext() const
{
    return m_inputContext;
}

bool PlatformInputContext::eventFilter(QObject *object, QEvent *event)
{
    if (event != m_filterEvent && object == m_focusObject && m_inputContext)
    {
		bool ret = m_inputContext->filterEvent(event);
		if (m_inputPanel)
		{
//			return m_inputPanel->eventFilter(object, event);
		}
		return ret;
	}

    return false;
}

void PlatformInputContext::sendEvent(QEvent *event)
{
    if (m_focusObject) {
        m_filterEvent = event;
        QGuiApplication::sendEvent(m_focusObject, event);
        m_filterEvent = 0;
    }
}

void PlatformInputContext::sendKeyEvent(QKeyEvent *event)
{
    const QGuiApplication *app = qApp;
    QWindow *focusWindow = app ? app->focusWindow() : 0;
    if (focusWindow) {
        m_filterEvent = event;
        QGuiApplication::sendEvent(focusWindow, event);
        m_filterEvent = 0;
    }
}

QVariant PlatformInputContext::inputMethodQuery(Qt::InputMethodQuery query)
{
    QInputMethodQueryEvent event(query);
    sendEvent(&event);
    return event.value(query);
}

void PlatformInputContext::setInputContext(InputContext *context)
{
    if (m_inputContext) {
        disconnect(this, SLOT(keyboardRectangleChanged()));
    }
    m_inputContext = context;
    if (m_inputContext) {
        if (!m_inputPanel)
	   {
            m_inputPanel = new DesktopInputPanel2(this);
            m_inputPanel->createView();
		}
        connect(m_inputContext, SIGNAL(keyboardRectangleChanged()), SLOT(keyboardRectangleChanged()));
    } else if (m_inputPanel) {
        m_inputPanel = 0;
    }
}

void PlatformInputContext::keyboardRectangleChanged()
{
    m_inputPanel->setInputRect(m_inputContext->keyboardRectangle().toRect());
}

void PlatformInputContext::updateInputPanelVisible()
{
    if (!m_inputPanel)
        return;

    if (m_visible && !m_inputPanel->isVisible()) {
        m_inputPanel->show(m_focusObject);
    } else if (!m_visible && m_inputPanel->isVisible()) {
        m_inputPanel->hide(m_focusObject);
    }
}

// 控件点击
bool PlatformInputContext::virtualKeyPress(Qt::Key key, const QString &text, 
	Qt::KeyboardModifiers modifiers, bool isAutoRepeat)
{
	if (m_inputContext && m_inputContext->inputEngine())
	{
		m_inputContext->inputEngine()->virtualKeyPress(key, text, modifiers, isAutoRepeat);
	}
	return false;
}

bool PlatformInputContext::virtualKeyRelease(Qt::Key key, const QString &text, 
	Qt::KeyboardModifiers modifiers)
{
	if (m_inputContext && m_inputContext->inputEngine())
	{
		m_inputContext->inputEngine()->virtualKeyRelease(key, text, modifiers);
	}
	return false;
}

void PlatformInputContext::FocusIn(QObject* obj)
{
    QObject* snd = obj;
    if (sender() != NULL)
    {
        snd = sender();
    }
    if (snd == NULL)
    {
        setFocusObject(NULL);
        hideInputPanel();
    }
    else
    {
        hideInputPanel();
        setFocusObject(snd);
        showInputPanel();
    }
}

void PlatformInputContext::FocusOut(QObject* obj)
{
    setFocusObject(NULL);
    hideInputPanel();
}

} // namespace QtVirtualKeyboard
