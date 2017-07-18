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

#include <desktopinputpanel2.h>
#include <inputview.h>
#include <platforminputcontext.h>
#include <inputcontext.h>
#include <QGuiApplication>
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <virtualkeyboarddebug.h>
#if defined(QT_VIRTUALKEYBOARD_HAVE_XCB)
#include <xcb/xcb.h>
#include <xcb/xfixes.h>
#endif
#include <QtCore/private/qobject_p.h>
#include <QtCore/QLibraryInfo>
#include <qwindow.h>

namespace QtVirtualKeyboard {

class DesktopInputPanel2Private : public AppInputPanelPrivate
{
public:
    enum WindowingSystem {
        Windows,
        Xcb,
        Other,
    };

    DesktopInputPanel2Private() :
        AppInputPanelPrivate(),
        view(),
        keyboardRect(),
        previewRect(),
        previewVisible(false),
        previewBindingActive(false),
        windowingSystem(Other)
    {
        const QString platformName = QGuiApplication::platformName();
        if (platformName == QLatin1String("windows"))
            windowingSystem = Windows;
        else if (platformName == QLatin1String("xcb"))
            windowingSystem = Xcb;
    }

    QScopedPointer<InputDialogView> view;
    QRectF keyboardRect;
    QRectF previewRect;
    bool previewVisible;
    bool previewBindingActive;
    WindowingSystem windowingSystem;
};

/*!
    \class QtVirtualKeyboard::DesktopInputPanel2
    \internal
*/

DesktopInputPanel2::DesktopInputPanel2(QObject *parent) :
    AppInputPanel(*new DesktopInputPanel2Private(), parent)
{
    /*  Activate the alpha buffer for this application.
    */
    QScreen *screen = QGuiApplication::primaryScreen();
    connect(screen, SIGNAL(virtualGeometryChanged(QRect)), SLOT(repositionView(QRect)));
}

DesktopInputPanel2::~DesktopInputPanel2()
{
}

void DesktopInputPanel2::show(QObject* focusobj)
{
    AppInputPanel::show(focusobj);
    Q_D(DesktopInputPanel2);
    if (d->view) {
//      repositionView(QGuiApplication::primaryScreen()->availableGeometry());
//      QRect rc = QGuiApplication::screens().at(0)->availableGeometry();
//      repositionView(QRect(rc.x(), rc.height() - 400, rc.width(), 400));
        QWidget *w = qobject_cast<QWidget*>(focusobj);
        if (w)
        {
            QPoint pt = w->pos();
            QRect rc = w->rect();
            pt = w->mapToGlobal(QPoint(0, 0));
            QRect rcScreen = QApplication::desktop()->screenGeometry(pt);

            int x = pt.x();
            int y = pt.y() + rc.height() + 1;
            int hei = 240;
            int wid = 600;
            if (x + wid > rcScreen.right())
            {
                x = rcScreen.right() - wid;
            }
            if (x < rcScreen.left())
            {
                wid = wid + x - rcScreen.left();
                x = rcScreen.left();
            }
            if (y + hei > rcScreen.bottom())
            {
                y = pt.y() - hei - 1;
            }
            if (y < rcScreen.top())
            {
                hei = hei + y - rcScreen.top();
                y = rcScreen.top();
            }
            repositionView(QRect(x, y, wid, hei));
        }
        d->view->show();
    }
}

void DesktopInputPanel2::hide(QObject* focusobj)
{
    AppInputPanel::hide(focusobj);
    Q_D(DesktopInputPanel2);
    if (d->view)
        d->view->hide();
}

bool DesktopInputPanel2::isVisible() const
{
    return AppInputPanel::isVisible();
}

void DesktopInputPanel2::setInputRect(const QRect &inputRect)
{
    Q_D(DesktopInputPanel2);
    d->keyboardRect = inputRect;
    updateInputRegion();
}

void DesktopInputPanel2::createView()
{
    Q_D(DesktopInputPanel2);
    if (!d->view) {
        if (qGuiApp) {
            connect(qGuiApp, SIGNAL(focusWindowChanged(QWindow*)), SLOT(focusWindowChanged(QWindow*)));
            focusWindowChanged(qGuiApp->focusWindow());
        }
         d->view.reset(new InputDialogView(this));
         /*  Set appropriate WindowType for target environment.
            There seems to be no common type which would
            work in all environments. The purpose of this
            flag is to avoid the window from capturing focus,
            as well as hiding it from the task bar. */
        d->view->setWindowModality(Qt::NonModal);
        switch (d->windowingSystem) {
        case DesktopInputPanel2Private::Xcb:
            d->view->setWindowFlags(d->view->windowFlags() | Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowDoesNotAcceptFocus);
            break;
        default:
            d->view->setWindowFlags(d->view->windowFlags() | Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
            break;
        }

        // d->view->setcolor(QColor(Qt::transparent));
        // d->view->setSource(QUrl("qrc:///QtQuick/VirtualKeyboard/content/InputPanel.qml"));
        connect(qGuiApp, SIGNAL(aboutToQuit()), SLOT(destroyView()));
    }
}

void DesktopInputPanel2::destroyView()
{
    Q_D(DesktopInputPanel2);
    d->view.reset();
    d->previewBindingActive = false;
}

void DesktopInputPanel2::repositionView(const QRect &rect)
{
    Q_D(DesktopInputPanel2);
    VIRTUALKEYBOARD_DEBUG() << "DesktopInputPanel2::repositionView():" << rect;
    if (d->view && d->view->geometry() != rect) {
        InputContext *inputContext = qobject_cast<PlatformInputContext *>(parent())->inputContext();
        if (inputContext) {
            inputContext->setAnimating(true);
            if (!d->previewBindingActive) {
                connect(inputContext, SIGNAL(previewRectangleChanged()), SLOT(previewRectangleChanged()));
                connect(inputContext, SIGNAL(previewVisibleChanged()), SLOT(previewVisibleChanged()));
                d->previewBindingActive = true;
            }
        }
        //d->view->setResizeMode(QQuickView::SizeViewToRootObject);
        setInputRect(QRect());
        d->view->setGeometry(rect);
        //d->view->setResizeMode(QQuickView::SizeRootObjectToView);
        if (inputContext)
            inputContext->setAnimating(false);
    }
}

void DesktopInputPanel2::focusWindowChanged(QWindow *focusWindow)
{
    disconnect(this, SLOT(focusWindowVisibleChanged(bool)));
    if (focusWindow)
        connect(focusWindow, &QWindow::visibleChanged, this, &DesktopInputPanel2::focusWindowVisibleChanged);
}

void DesktopInputPanel2::focusWindowVisibleChanged(bool visible)
{
    if (!visible) {
        InputContext *inputContext = qobject_cast<PlatformInputContext *>(parent())->inputContext();
        if (inputContext)
            inputContext->hideInputPanel();
    }
}

void DesktopInputPanel2::previewRectangleChanged()
{
    Q_D(DesktopInputPanel2);
    InputContext *inputContext = qobject_cast<PlatformInputContext *>(parent())->inputContext();
    d->previewRect = inputContext->previewRectangle();
    if (d->previewVisible)
        updateInputRegion();
}

void DesktopInputPanel2::previewVisibleChanged()
{
    Q_D(DesktopInputPanel2);
    InputContext *inputContext = qobject_cast<PlatformInputContext *>(parent())->inputContext();
    d->previewVisible = inputContext->previewVisible();
    if (d->view->isVisible())
        updateInputRegion();
}

#if defined(QT_VIRTUALKEYBOARD_HAVE_XCB)
static inline xcb_rectangle_t qRectToXCBRectangle(const QRect &r)
{
    xcb_rectangle_t result;
    result.x = qMax(SHRT_MIN, r.x());
    result.y = qMax(SHRT_MIN, r.y());
    result.width = qMin((int)USHRT_MAX, r.width());
    result.height = qMin((int)USHRT_MAX, r.height());
    return result;
}
#endif

void DesktopInputPanel2::updateInputRegion()
{
    Q_D(DesktopInputPanel2);

    if (d->view.isNull() || d->keyboardRect.isEmpty())
        return;

    // Make sure the native window is created
    if (!d->view->windowHandle())
    {
        QWindow* wind = d->view->windowHandle();
    }
    switch (d->windowingSystem) {
    case DesktopInputPanel2Private::Xcb:
#if defined(QT_VIRTUALKEYBOARD_HAVE_XCB)
        {
            QVector<xcb_rectangle_t> rects;
            rects.push_back(qRectToXCBRectangle(d->keyboardRect.toRect()));
            if (d->previewVisible && !d->previewRect.isEmpty())
                rects.push_back(qRectToXCBRectangle(d->previewRect.toRect()));

            QWindow *window = d->view.data();
            QPlatformNativeInterface *platformNativeInterface = QGuiApplication::platformNativeInterface();
            xcb_connection_t *xbcConnection = static_cast<xcb_connection_t *>(platformNativeInterface->nativeResourceForWindow("connection", window));
            xcb_xfixes_region_t xbcRegion = xcb_generate_id(xbcConnection);
            xcb_xfixes_create_region(xbcConnection, xbcRegion, rects.size(), rects.constData());
            xcb_xfixes_set_window_shape_region(xbcConnection, window->winId(), XCB_SHAPE_SK_INPUT, 0, 0, xbcRegion);
            xcb_xfixes_destroy_region(xbcConnection, xbcRegion);
        }
#endif
        break;

    default:
        {
            QRegion inputRegion(d->keyboardRect.toRect());
            if (d->previewVisible && !d->previewRect.isEmpty())
                inputRegion += d->previewRect.toRect();

            d->view->setMask(inputRegion);
            break;
        }
    }
}

bool DesktopInputPanel2::eventFilter(QObject *object, QEvent *event)
{
    Q_D(DesktopInputPanel2);

    bool ret = false;
    if (d->view.isNull()/* || d->keyboardRect.isEmpty()*/)
        return false;

    QEvent::Type type = event->type();
    if (type == QEvent::KeyPress)
    {
        const QKeyEvent *keyEvent = static_cast<const QKeyEvent *>(event);
        QList<QPushButton*> list = d->view->GetButtonList(keyEvent->key());
        if (list.size() == 0)
        {
            return false;
        }

        foreach(QPushButton* btn, list)
        {
            if (btn)
            {

            }
        }

        return true;
    }
    else if (type == QEvent::KeyRelease)
    {
        const QKeyEvent *keyEvent = static_cast<const QKeyEvent *>(event);
        QList<QPushButton*> list = d->view->GetButtonList(keyEvent->key());
        if (list.size() == 0)
        {
            return false;
        }

        foreach(QPushButton* btn , list)
        {
            if (btn)
            {
                emit btn->pressed();
                emit btn->released();

                return true;
            }
        }

        return false;
    }

    return false;
}

} // namespace QtVirtualKeyboard
