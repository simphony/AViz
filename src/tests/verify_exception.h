// The file contains code copied from https://github.com/qtproject/qtbase/blob/df757a2e62456a919dfe2e983f003123c3eb151c/src/testlib/qtestcase.h

/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtTest module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

# define QVERIFY_EXCEPTION_THROWN(expression, exceptiontype) \
    do {\
        QT_TRY {\
            QT_TRY {\
                expression;\
             QTest::qFail("Expected exception of type " #exceptiontype " to be thrown" \
             " but no exception caught", __FILE__, __LINE__);\
             return;\
            } QT_CATCH (const exceptiontype &) {\
        }\
    } QT_CATCH (const std::exception &e) {\
        QByteArray msg = QByteArray() + "Expected exception of type " #exceptiontype \
        " to be thrown but std::exception caught with message: " + e.what(); \
        QTest::qFail(msg.constData(), __FILE__, __LINE__);\
        return;\
    } QT_CATCH (...) {\
        QTest::qFail("Expected exception of type " #exceptiontype " to be thrown" \
        " but unknown exception caught", __FILE__, __LINE__);\
        return;\
    }\
    } while (0)
