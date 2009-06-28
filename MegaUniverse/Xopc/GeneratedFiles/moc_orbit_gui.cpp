/****************************************************************************
** Meta object code from reading C++ file 'orbit_gui.h'
**
** Created: Sun 28. Jun 22:19:54 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../orbit_gui.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'orbit_gui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_OrbitEarth[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

       0        // eod
};

static const char qt_meta_stringdata_OrbitEarth[] = {
    "OrbitEarth\0"
};

const QMetaObject OrbitEarth::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_OrbitEarth,
      qt_meta_data_OrbitEarth, 0 }
};

const QMetaObject *OrbitEarth::metaObject() const
{
    return &staticMetaObject;
}

void *OrbitEarth::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OrbitEarth))
        return static_cast<void*>(const_cast< OrbitEarth*>(this));
    if (!strcmp(_clname, "QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< OrbitEarth*>(this));
    return QObject::qt_metacast(_clname);
}

int OrbitEarth::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_OrbitShip[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

       0        // eod
};

static const char qt_meta_stringdata_OrbitShip[] = {
    "OrbitShip\0"
};

const QMetaObject OrbitShip::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_OrbitShip,
      qt_meta_data_OrbitShip, 0 }
};

const QMetaObject *OrbitShip::metaObject() const
{
    return &staticMetaObject;
}

void *OrbitShip::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OrbitShip))
        return static_cast<void*>(const_cast< OrbitShip*>(this));
    if (!strcmp(_clname, "QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< OrbitShip*>(this));
    return QObject::qt_metacast(_clname);
}

int OrbitShip::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
