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
#include <QLoggingCategory>
#include <plugin.h>
#include <inputcontext.h>
#include <inputengine.h>
#include <shifthandler.h>
#include <plaininputmethod.h>
#ifdef HAVE_HUNSPELL
#include <hunspellinputmethod.h>
#endif
#ifdef HAVE_PINYIN
#include <pinyininputmethod.h>
#endif
#ifdef HAVE_TCIME
#include <tcinputmethod.h>
#endif
#ifdef HAVE_OPENWNN
#include <openwnninputmethod.h>
#endif
#ifdef HAVE_T9WRITE
#include <t9writeinputmethod.h>
#endif
#include <inputmethod.h>
#include <selectionlistmodel.h>
#include <trace.h>

using namespace QtVirtualKeyboard;

Q_LOGGING_CATEGORY(qlcVirtualKeyboard, "qt.virtualkeyboard")
static const char pluginName[] = "qtvirtualkeyboard";
static QPointer<PlatformInputContext> platformInputContext;

QtVirtualKeyboard::PlatformInputContext *QVirtualKeyboardPlugin::create(const QString &system, const QStringList &paramList)
{
    Q_UNUSED(paramList);
    if (system.compare(system, QLatin1String(pluginName), Qt::CaseInsensitive) == 0) {
        platformInputContext = new PlatformInputContext();
    }

    return platformInputContext;
}
