#include "Settings.h"
#include <QDir>
#include <QDesktopServices>
#include <QDebug>
#include <QMetaEnum>
#include <QSettings>

Settings* settingsInstance = 0;

Settings::Settings(QObject *parent) :
    QObject(parent)
{
    if (!settingsInstance) {
        settingsInstance = this;
    }

    MozContext = QMozContext::GetInstance();
    connect(MozContext, SIGNAL(recvObserve(QString,QVariant)), this, SLOT(onRecvObserve(QString,QVariant)));
}

Settings* Settings::instance() {
    return settingsInstance;
}

void Settings::setScreenOrientation()
{
    Q_EMIT screenOrientationChanged(screenOrientation());
}

void Settings::setSearchEngine()
{
    QVariant searchEngine = QSettings().value("Browser/SearchEngine", QVariant(QString("Google")));
    QMozContext::GetInstance()->setPref(QString("browser.search.defaultenginename"), searchEngine);
    Q_EMIT searchEngineChanged(searchEngine.toString());
}

void Settings::onRecvObserve(const QString message, const QVariant data)
{
    QVariantMap dataMap = data.toMap(); // Convert the Json data to a map
    qDebug() << "==========================================================";
    qDebug() << dataMap;
    qDebug() << "==========================================================";
//    abort();
    if (message == "embed:prefs")
    {
        //TODO what now?
    }
    else if (message == "embed:search")
    {
        QString msg = dataMap["msg"].toString();
        if (msg == "init")
        {
            if (!dataMap.value("defaultEngine").isValid())
            {
                QMozContext *mozContext = QMozContext::GetInstance();
                QVariantMap loadsearch;

                // load opensearch descriptions
                qDebug("Adding default search plugins");
                loadsearch.insert(QString("msg"), QVariant(QString("loadxml")));
                loadsearch.insert(QString("uri"), QVariant(QString("chrome://embedlite/content/google.xml")));
                loadsearch.insert(QString("confirm"), QVariant(false));
                mozContext->sendObserve("embedui:search", QVariant(loadsearch));
                loadsearch.insert(QString("uri"), QVariant(QString("chrome://embedlite/content/bing.xml")));
                mozContext->sendObserve("embedui:search", QVariant(loadsearch));
                loadsearch.insert(QString("uri"), QVariant(QString("chrome://embedlite/content/yahoo.xml")));
                mozContext->sendObserve("embedui:search", QVariant(loadsearch));
            }
        }
        else if (msg == "pluginslist")
        {
            // TODO what now?
        }

    }
}

