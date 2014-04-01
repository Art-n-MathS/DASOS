/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created: Tue Apr 1 15:51:13 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/MainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      26,   11,   11,   11, 0x0a,
      41,   11,   11,   11, 0x0a,
      54,   11,   11,   11, 0x0a,
      78,   66,   11,   11, 0x0a,
      99,   11,   11,   11, 0x0a,
     107,   11,   11,   11, 0x0a,
     120,   11,   11,   11, 0x0a,
     144,   11,   11,   11, 0x0a,
     166,   11,   11,   11, 0x0a,
     185,  178,   11,   11, 0x0a,
     205,  178,   11,   11, 0x0a,
     237,  228,   11,   11, 0x0a,
     265,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0loadLASfile()\0createObject()\0"
    "polygonise()\0exportOBJ()\0i_threshold\0"
    "setThreshold(double)\0close()\0createHist()\0"
    "loadHyperspectraldata()\0updateHyperspectral()\0"
    "createMap()\0i_type\0changeDataType(int)\0"
    "changeShaderType(bool)\0i_reveal\0"
    "hideOrRevealIGMButton(bool)\0loadIGM()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->loadLASfile(); break;
        case 1: _t->createObject(); break;
        case 2: _t->polygonise(); break;
        case 3: _t->exportOBJ(); break;
        case 4: _t->setThreshold((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->close(); break;
        case 6: _t->createHist(); break;
        case 7: _t->loadHyperspectraldata(); break;
        case 8: _t->updateHyperspectral(); break;
        case 9: _t->createMap(); break;
        case 10: _t->changeDataType((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->changeShaderType((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->hideOrRevealIGMButton((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->loadIGM(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
