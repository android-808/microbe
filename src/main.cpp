#include "BrowserWindow.h"
#include "DBusAdaptor.h"
#include "Session.h"
#include "Settings.h"
#include "WindowCreator.h"
#include <QApplication>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDebug>
#include <QTimer>
#include <QTranslator>
#include "qtmozembed/qmozcontext.h"

#if defined(Q_WS_X11) && QT_VERSION < 0x040800
#include <X11/Xlib.h>
#endif

#define OBJECT_NAME "/"
#define SERVICE_NAME "org.mozilla.microbe"

int main(int argc, char *argv[])
{
    setenv("QML_BAD_GUI_RENDER_LOOP", "1", 1);
    setenv("USE_ASYNC", "1", 1);
    setenv("USE_TEXT_EVENTS", "1", 1);

    // Workaround for https://bugzilla.mozilla.org/show_bug.cgi?id=929879
    setenv("LC_NUMERIC", "C", 1);
    setlocale(LC_NUMERIC, "C");

#if QT_VERSION >= 0x040800
    QApplication::setAttribute(Qt::AA_X11InitThreads, true);
#else
    XInitThreads();
    QApplication::setAttribute(static_cast<Qt::ApplicationAttribute>(10), true);
#endif

    QApplication::setOrganizationName("Microbe");
    QApplication::setApplicationName("Microbe");
    QApplication::setApplicationVersion("0.1");

    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(true);

    new Session;

    if (!Settings::instance()->language().isEmpty()) {
        QTranslator translator;
        translator.load(QString("/opt/microbe/translations/microbe_%1").arg(Settings::instance()->language()));
        app.installTranslator(&translator);
    }
 
    QString urlstring("about:blank");

    QStringList arguments = app.arguments();
    for (int i = 0; i < arguments.count(); ++i) {
        QString parameter = arguments.at(i);
        if (parameter == "-url" || parameter == "--url") {
            if (i + 1 >= arguments.count())
                qFatal("-url requires an argument");
            urlstring = arguments.at(i + 1);
            i++;
        } else if (parameter == "-help"  || parameter == "--help") {
            qDebug() << "EMail application";
            qDebug() << "-url          - url to load";
            exit(0);
        } 
    }
    
    MozWindowCreator winCreator(0, true, false);
    BrowserWindow *view = winCreator.CreateNewWindow(urlstring);
    winCreator.mWindowStack.append(view);

    DBusAdaptor* adaptor = new DBusAdaptor();
    if (QDBusConnection::sessionBus().registerService(SERVICE_NAME) &&
        QDBusConnection::sessionBus().registerObject(OBJECT_NAME, adaptor, QDBusConnection::ExportScriptableSlots)) {
        qDebug() << "DBus service started!";
        QObject::connect(adaptor, SIGNAL(newWindowUrl(QString, unsigned, QNewWindowResponse*)),
                     &winCreator, SLOT(newWindowRequested(const QString&, const unsigned&, QNewWindowResponse*)));
        QObject::connect(adaptor, SIGNAL(bringToFront()),
                     &winCreator, SLOT(bringToFront()));
    }
    else {
        qDebug() << "Object already exists. Another instance running?";
        if ((urlstring.length() == 0) || (urlstring == QString("about:blank"))) {
            QDBusInterface("org.mozilla.mozembed", "/", "org.mozilla.mozembed",
                       QDBusConnection::sessionBus()).call("show");
        }
        else {
            QDBusInterface("org.mozilla.mozembed", "/", "org.mozilla.mozembed",
                       QDBusConnection::sessionBus()).call("newUrl", urlstring);
        }
        return 0;
    }

    view->show();

    qDebug() << "Starting Application!!!";

    QMozContext::GetInstance()->setViewCreator(&winCreator);

    QString componentPath(DEFAULT_COMPONENTS_PATH);
    qDebug() << "Load components from:" << componentPath + QString("/components") + QString("/EmbedLiteBinComponents.manifest");
    QMozContext::GetInstance()->addComponentManifest(componentPath + QString("/components") + QString("/EmbedLiteBinComponents.manifest"));
    qDebug() << "Load components from:" << componentPath + QString("/components") + QString("/EmbedLiteJSComponents.manifest");
    QMozContext::GetInstance()->addComponentManifest(componentPath + QString("/components") + QString("/EmbedLiteJSComponents.manifest"));
    qDebug() << "Load components from:" << componentPath + QString("/chrome") + QString("/EmbedLiteJSScripts.manifest");
    QMozContext::GetInstance()->addComponentManifest(componentPath + QString("/chrome") + QString("/EmbedLiteJSScripts.manifest"));
    qDebug() << "Load components from:" << componentPath + QString("/chrome") + QString("/EmbedLiteOverrides.manifest");
    QMozContext::GetInstance()->addComponentManifest(componentPath + QString("/chrome") + QString("/EmbedLiteOverrides.manifest"));

//    QMozContext::GetInstance()->addObserver("history:checkurivisited");
//    QMozContext::GetInstance()->addObserver("history:markurivisited");

    QObject::connect(&app, SIGNAL(lastWindowClosed()),
                     QMozContext::GetInstance(), SLOT(stopEmbedding()));
    QTimer::singleShot(0, QMozContext::GetInstance(), SLOT(runEmbedding()));

    QObject::connect(&app, SIGNAL(aboutToQuit()), Session::instance(), SLOT(deleteLater()));

    int retval = app.exec();
    qDebug() << "Exiting from Application!!!";
    QDBusConnection::sessionBus().unregisterObject(OBJECT_NAME);
    QDBusConnection::sessionBus().unregisterService(SERVICE_NAME);
    return retval;
}
