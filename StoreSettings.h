#ifndef STORESETTINGS_H
#define STORESETTINGS_H

#if defined(STORESETTINGS_LIBRARY)
#  define STORESETTINGS_EXPORT Q_DECL_EXPORT
#else
#  define STORESETTINGS_EXPORT Q_DECL_IMPORT
#endif


#include <QObject>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

class STORESETTINGS_EXPORT StoreSettings : public QObject
{
Q_OBJECT
public:
    explicit StoreSettings(QObject *parent = nullptr);
    ~StoreSettings();
    QString getPassword();
    void setPassword(const QString password);
    void saveFormGeometry(const QSize formSize);
    QSize getFormGeometry() const;
    void saveFormPosition(const QPoint formPos);
    QPoint getFormPosition() const;
    QByteArray getByteArray(const QString paramName) const;
    void setByteArray(const QString paramName, const QByteArray value);
    bool paramIsEnabled(const QString paramName) const;
    void setParamEnabled(const QString paramName, const bool value);

private:
    QSettings *m_settings;
    QString groupName;
    unsigned long long cryptoKey {Q_UINT64_C(0x0000000000000000)};
};

#endif // STORESETTINGS_H
