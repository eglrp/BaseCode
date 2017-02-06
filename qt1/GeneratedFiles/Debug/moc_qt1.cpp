/****************************************************************************
** Meta object code from reading C++ file 'qt1.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qt1.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qt1.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainFrame_t {
    QByteArrayData data[18];
    char stringdata0[274];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainFrame_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainFrame_t qt_meta_stringdata_MainFrame = {
    {
QT_MOC_LITERAL(0, 0, 9), // "MainFrame"
QT_MOC_LITERAL(1, 10, 21), // "on_action_5_triggered"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 21), // "on_action_2_triggered"
QT_MOC_LITERAL(4, 55, 21), // "on_action_3_triggered"
QT_MOC_LITERAL(5, 77, 21), // "on_action_4_triggered"
QT_MOC_LITERAL(6, 99, 21), // "on_action_6_triggered"
QT_MOC_LITERAL(7, 121, 21), // "on_action_7_triggered"
QT_MOC_LITERAL(8, 143, 8), // "onRunICP"
QT_MOC_LITERAL(9, 152, 11), // "onDenoising"
QT_MOC_LITERAL(10, 164, 13), // "onIsWireframe"
QT_MOC_LITERAL(11, 178, 10), // "onLockView"
QT_MOC_LITERAL(12, 189, 19), // "onPointCloudFitting"
QT_MOC_LITERAL(13, 209, 15), // "onClearSelected"
QT_MOC_LITERAL(14, 225, 11), // "onShapeLine"
QT_MOC_LITERAL(15, 237, 12), // "onShapePlane"
QT_MOC_LITERAL(16, 250, 11), // "onOpenFileM"
QT_MOC_LITERAL(17, 262, 11) // "onOpenFileT"

    },
    "MainFrame\0on_action_5_triggered\0\0"
    "on_action_2_triggered\0on_action_3_triggered\0"
    "on_action_4_triggered\0on_action_6_triggered\0"
    "on_action_7_triggered\0onRunICP\0"
    "onDenoising\0onIsWireframe\0onLockView\0"
    "onPointCloudFitting\0onClearSelected\0"
    "onShapeLine\0onShapePlane\0onOpenFileM\0"
    "onOpenFileT"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainFrame[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x08 /* Private */,
       3,    0,   95,    2, 0x08 /* Private */,
       4,    0,   96,    2, 0x08 /* Private */,
       5,    0,   97,    2, 0x08 /* Private */,
       6,    0,   98,    2, 0x08 /* Private */,
       7,    0,   99,    2, 0x08 /* Private */,
       8,    0,  100,    2, 0x08 /* Private */,
       9,    0,  101,    2, 0x08 /* Private */,
      10,    0,  102,    2, 0x08 /* Private */,
      11,    0,  103,    2, 0x08 /* Private */,
      12,    0,  104,    2, 0x08 /* Private */,
      13,    0,  105,    2, 0x08 /* Private */,
      14,    0,  106,    2, 0x08 /* Private */,
      15,    0,  107,    2, 0x08 /* Private */,
      16,    0,  108,    2, 0x08 /* Private */,
      17,    0,  109,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainFrame *_t = static_cast<MainFrame *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_action_5_triggered(); break;
        case 1: _t->on_action_2_triggered(); break;
        case 2: _t->on_action_3_triggered(); break;
        case 3: _t->on_action_4_triggered(); break;
        case 4: _t->on_action_6_triggered(); break;
        case 5: _t->on_action_7_triggered(); break;
        case 6: _t->onRunICP(); break;
        case 7: _t->onDenoising(); break;
        case 8: _t->onIsWireframe(); break;
        case 9: _t->onLockView(); break;
        case 10: _t->onPointCloudFitting(); break;
        case 11: _t->onClearSelected(); break;
        case 12: _t->onShapeLine(); break;
        case 13: _t->onShapePlane(); break;
        case 14: _t->onOpenFileM(); break;
        case 15: _t->onOpenFileT(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MainFrame::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainFrame.data,
      qt_meta_data_MainFrame,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainFrame.stringdata0))
        return static_cast<void*>(const_cast< MainFrame*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
