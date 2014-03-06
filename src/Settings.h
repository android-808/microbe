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
    
    inline ScreenOrientation::Orientation screenOrientation() const
    {
        QString orientation = QSettings().value("Browser/ScreenOrientation").toString();
        return (orientation == "Landscape" ? ScreenOrientation::LockLandscape :
                orientation == "Portrait"  ? ScreenOrientation::LockPortrait  :
                                             ScreenOrientation::Automatic);
    }
    //QString searchEngine() const {return mSearchEngine;}


    static Settings* instance();
public Q_SLOTS:
    void setScreenOrientation();
    void setSearchEngine();
    void onRecvObserve(const QString message, const QVariant data);
    
    
private:

private Q_SLOTS:

Q_SIGNALS:
    void screenOrientationChanged(ScreenOrientation::Orientation orientation);
    void searchEngineChanged(const QString &engine);

protected:
    QMozContext * MozContext;

private:
};

#endif // SETTINGS_H
