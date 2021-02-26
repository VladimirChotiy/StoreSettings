#include "StoreSettings.h"
#include "simplecrypt.h"
#include <QStandardPaths>
#include <QDir>
#include <QRandomGenerator>
#include <QSettings>
#include <QSize>
#include <QPoint>
#include <QDebug>

StoreSettings::StoreSettings(QObject *parent) :
    QObject(parent),
    groupName(parent->objectName())
{
    bool createResult {true};
    QString fileLocation {QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)};
    QString fullPath {fileLocation + "/" + "settings.cfg"};
    QDir confDir {fileLocation};
    if(!confDir.exists()){
        createResult = confDir.mkpath(fileLocation);
    }

    if(createResult){
        m_settings = new QSettings(fullPath, QSettings::IniFormat);
        m_settings->beginGroup(groupName);
        if (m_settings->contains("KEY")){
            cryptoKey = m_settings->value("KEY", Q_UINT64_C(0x0000000000000000)).toLongLong();
        }else {
            cryptoKey = QRandomGenerator::global()->generate64() & std::numeric_limits<qint64>::max();
            m_settings->setValue("KEY", cryptoKey);
         }
    }else {
        qDebug() << "ERROR Storage!";
    }
}

StoreSettings::~StoreSettings()
{
    m_settings->endGroup();
    delete m_settings;
}

QString StoreSettings::getPassword()
{
    QString result = m_settings->value("Password", "").toString();
    if(!result.isEmpty()){
        SimpleCrypt crypto {cryptoKey};
        return crypto.decryptToString(result);
    }else {
        return "";
    }
}

void StoreSettings::setPassword(const QString password)
{
    if(!password.isEmpty()){
        SimpleCrypt crypto {cryptoKey};
        m_settings->setValue("Password", crypto.encryptToString(password));
    }
}

void StoreSettings::saveFormGeometry(const QSize formSize)
{
    m_settings->setValue("Size", formSize);
}

QSize StoreSettings::getFormGeometry() const
{
    return m_settings->value("Size", QSize(400, 200)).toSize();
}

void StoreSettings::saveFormPosition(const QPoint formPos)
{
    m_settings->setValue("Position", formPos);
}

QPoint StoreSettings::getFormPosition() const
{
    return m_settings->value("Position", QPoint(200, 200)).toPoint();
}

QByteArray StoreSettings::getByteArray(const QString paramName) const
{
    return m_settings->value(paramName).toByteArray();
}

void StoreSettings::setByteArray(const QString paramName, const QByteArray value)
{
    m_settings->setValue(paramName, value);
}

bool StoreSettings::paramIsEnabled(const QString paramName) const
{
    return m_settings->value(paramName, false).toBool();
}

void StoreSettings::setParamEnabled(const QString paramName, const bool value)
{
    m_settings->setValue(paramName, value);
}
