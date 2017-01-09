/****************************************************************************
** Meta object code from reading C++ file 'audiorecorder.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../audiorecorder.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'audiorecorder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AudioTrapRecorder_t {
    QByteArrayData data[30];
    char stringdata[365];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AudioTrapRecorder_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AudioTrapRecorder_t qt_meta_stringdata_AudioTrapRecorder = {
    {
QT_MOC_LITERAL(0, 0, 17),
QT_MOC_LITERAL(1, 18, 9),
QT_MOC_LITERAL(2, 28, 0),
QT_MOC_LITERAL(3, 29, 11),
QT_MOC_LITERAL(4, 41, 9),
QT_MOC_LITERAL(5, 51, 7),
QT_MOC_LITERAL(6, 59, 8),
QT_MOC_LITERAL(7, 68, 10),
QT_MOC_LITERAL(8, 79, 13),
QT_MOC_LITERAL(9, 93, 12),
QT_MOC_LITERAL(10, 106, 11),
QT_MOC_LITERAL(11, 118, 31),
QT_MOC_LITERAL(12, 150, 5),
QT_MOC_LITERAL(13, 156, 6),
QT_MOC_LITERAL(14, 163, 4),
QT_MOC_LITERAL(15, 168, 17),
QT_MOC_LITERAL(16, 186, 4),
QT_MOC_LITERAL(17, 191, 12),
QT_MOC_LITERAL(18, 204, 21),
QT_MOC_LITERAL(19, 226, 5),
QT_MOC_LITERAL(20, 232, 13),
QT_MOC_LITERAL(21, 246, 22),
QT_MOC_LITERAL(22, 269, 6),
QT_MOC_LITERAL(23, 276, 19),
QT_MOC_LITERAL(24, 296, 9),
QT_MOC_LITERAL(25, 306, 21),
QT_MOC_LITERAL(26, 328, 3),
QT_MOC_LITERAL(27, 332, 5),
QT_MOC_LITERAL(28, 338, 21),
QT_MOC_LITERAL(29, 360, 4)
    },
    "AudioTrapRecorder\0activated\0\0deactivated\0"
    "recording\0stopped\0activate\0deactivate\0"
    "nullRecording\0newRecording\0levelChange\0"
    "AudioLevelMeter::ThresholdState\0level\0"
    "update\0read\0audioInputChanged\0name\0"
    "stateChanged\0QMediaRecorder::State\0"
    "state\0statusChanged\0QMediaRecorder::Status\0"
    "status\0availabilityChanged\0available\0"
    "actualLocationChanged\0url\0error\0"
    "QMediaRecorder::Error\0quit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AudioTrapRecorder[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x06 /* Public */,
       3,    0,  100,    2, 0x06 /* Public */,
       4,    0,  101,    2, 0x06 /* Public */,
       5,    0,  102,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,  103,    2, 0x0a /* Public */,
       7,    0,  104,    2, 0x0a /* Public */,
       8,    0,  105,    2, 0x0a /* Public */,
       9,    0,  106,    2, 0x0a /* Public */,
      10,    1,  107,    2, 0x0a /* Public */,
      13,    1,  110,    2, 0x0a /* Public */,
      15,    1,  113,    2, 0x0a /* Public */,
      17,    1,  116,    2, 0x0a /* Public */,
      20,    1,  119,    2, 0x0a /* Public */,
      23,    1,  122,    2, 0x0a /* Public */,
      25,    1,  125,    2, 0x0a /* Public */,
      27,    1,  128,    2, 0x0a /* Public */,
      29,    0,  131,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void, QMetaType::Bool,   24,
    QMetaType::Void, QMetaType::QUrl,   26,
    QMetaType::Void, 0x80000000 | 28,   27,
    QMetaType::Void,

       0        // eod
};

void AudioTrapRecorder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AudioTrapRecorder *_t = static_cast<AudioTrapRecorder *>(_o);
        switch (_id) {
        case 0: _t->activated(); break;
        case 1: _t->deactivated(); break;
        case 2: _t->recording(); break;
        case 3: _t->stopped(); break;
        case 4: _t->activate(); break;
        case 5: _t->deactivate(); break;
        case 6: _t->nullRecording(); break;
        case 7: _t->newRecording(); break;
        case 8: _t->levelChange((*reinterpret_cast< AudioLevelMeter::ThresholdState(*)>(_a[1]))); break;
        case 9: _t->update((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->audioInputChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->stateChanged((*reinterpret_cast< QMediaRecorder::State(*)>(_a[1]))); break;
        case 12: _t->statusChanged((*reinterpret_cast< QMediaRecorder::Status(*)>(_a[1]))); break;
        case 13: _t->availabilityChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->actualLocationChanged((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        case 15: _t->error((*reinterpret_cast< QMediaRecorder::Error(*)>(_a[1]))); break;
        case 16: _t->quit(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMediaRecorder::State >(); break;
            }
            break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMediaRecorder::Status >(); break;
            }
            break;
        case 15:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMediaRecorder::Error >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AudioTrapRecorder::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioTrapRecorder::activated)) {
                *result = 0;
            }
        }
        {
            typedef void (AudioTrapRecorder::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioTrapRecorder::deactivated)) {
                *result = 1;
            }
        }
        {
            typedef void (AudioTrapRecorder::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioTrapRecorder::recording)) {
                *result = 2;
            }
        }
        {
            typedef void (AudioTrapRecorder::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioTrapRecorder::stopped)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject AudioTrapRecorder::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AudioTrapRecorder.data,
      qt_meta_data_AudioTrapRecorder,  qt_static_metacall, 0, 0}
};


const QMetaObject *AudioTrapRecorder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AudioTrapRecorder::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AudioTrapRecorder.stringdata))
        return static_cast<void*>(const_cast< AudioTrapRecorder*>(this));
    return QObject::qt_metacast(_clname);
}

int AudioTrapRecorder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void AudioTrapRecorder::activated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void AudioTrapRecorder::deactivated()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void AudioTrapRecorder::recording()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void AudioTrapRecorder::stopped()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
