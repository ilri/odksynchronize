#ifndef ANDROIDINTERFACE_H
#define ANDROIDINTERFACE_H

#include <QObject>

class androidInterface : public QObject
{
    Q_OBJECT
public:
    explicit androidInterface(QObject *parent = 0);
    QString getExternalStorageDirectory();
    QString getDataDirectory();
};

#endif // ANDROIDINTERFACE_H
