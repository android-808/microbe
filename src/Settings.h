#ifndef SETTINGS_H
#define SETTINGS_H

#include "Enums.h"
#include <QStringList>
#include <QSet>
#include <QSettings>
#include <qplatformdefs.h>
#include "qtmozembed/qmozcontext.h"

class QNetworkProxy;
class QAuthenticator;

class Settings : public QObject
{
    Q_OBJECT

public:
    explicit Settings(QObject *parent = 0);
    
    inline ScreenOrientation::Orientation screenOrientation() const { return mOrientation; }
    //QString searchEngine() const {return mSearchEngine;}


    static Settings* instance();
public Q_SLOTS:
    void reload();
    void onRecvObserve(const QString message, const QVariant data);
    
    
private:

private Q_SLOTS:

Q_SIGNALS:
    void screenOrientationChanged(ScreenOrientation::Orientation orientation);
    void searchEngineChanged(const QString &engine);

protected:
    ScreenOrientation::Orientation mOrientation;
    QMozContext * MozContext;

private:
};

#endif // SETTINGS_H
