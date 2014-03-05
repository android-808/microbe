#include "Session.h"
#include "qtmozembed/qmozcontext.h"
#include "BookmarksModel.h"
#include "Settings.h"
//#include "notifications.h"
//#include "transfermanager.h"
//#include "thumbnailcache.h"
#ifdef Q_WS_MAEMO_5
//#include "windowstack.h"
#endif
#include <QDebug>
#include <QNetworkConfigurationManager>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QVariantMap>

Session* sessionInstance = 0;

Session::Session(QObject *parent) :
    QObject(parent),
    m_ncm(new QNetworkConfigurationManager(this)),
    m_nam(new QNetworkAccessManager(this)),
    m_settings(new Settings(this)),
    m_bookmarks(new BookmarksModel(this)),
    m_context(QMozContext::GetInstance())
//    m_notifications(new Notifications(this)),
//    m_transferManager(new TransferManager(this)),
//    m_cache(new ThumbnailCache)
  #ifdef Q_WS_MAEMO_5
//    m_stack(new WindowStack(this))
  #endif
{
    if (!sessionInstance) {
        sessionInstance = this;
    }

//    this->connect(m_transferManager, SIGNAL(alert(QString)), m_notifications, SLOT(onAlert(QString)));
//    m_transferManager->setNetworkAccessManager(this->networkAccessManager());
//    m_transferManager->restoreStoredDownloads();

    if (!QMozContext::GetInstance()->initialized()) {
        connect(QMozContext::GetInstance(), SIGNAL(onInitialized()), this, SLOT(onInitialized()));
    } else {
        QTimer::singleShot(0, this, SLOT(onInitialized()));
    }    
}

Session::~Session() {
//    delete m_cache;
//    m_cache = 0;
}

Session* Session::instance() {
    return sessionInstance;
}

void Session::onInitialized()
{
    qDebug("Session::onInitialized");
    
    // Infer and set Accept-Language header from the current system locale
    QString langs;
    QStringList locale = QLocale::system().name().split("_", QString::SkipEmptyParts);
    if (locale.size() > 1) {
        langs = QString("%1-%2,%3").arg(locale.at(0)).arg(locale.at(1)).arg(locale.at(0));
    } else {
        langs = locale.at(0);
    }
    m_context->setPref(QString("intl.accept_languages"), QVariant(langs));
    
    // these are magic numbers defining touch radius required to detect <image src=""> touch
    m_context->setPref("browser.ui.touch.left", 32);
    m_context->setPref("browser.ui.touch.right", 32);
    m_context->setPref("browser.ui.touch.top", 48);
    m_context->setPref("browser.ui.touch.bottom", 16);
    
    // Install embedlite handlers for guestures
    m_context->setPref("embedlite.azpc.handle.singletap", false);
    m_context->setPref("embedlite.azpc.json.singletap", true);
    m_context->setPref("embedlite.azpc.handle.longtap", false);
    m_context->setPref("embedlite.azpc.json.longtap", true);
    m_context->setPref("embedlite.azpc.json.viewport", true);
    
    // Without this pref placeholders get cleaned as soon as a character gets committed
    // by VKB and that happens only when Enter is pressed or comma/space/dot is entered.
    m_context->setPref(QString("dom.placeholder.show_on_focus"), QVariant(false));

    m_context->setPref(QString("security.alternate_certificate_error_page"), QString("certerror"));
    
    // Don't use autodownload, ask
    m_context->setPref(QString("browser.download.useDownloadDir"), QVariant(false));
    // see https://developer.mozilla.org/en-US/docs/Download_Manager_preferences
    // Use custom downloads location defined in browser.download.dir
    m_context->setPref(QString("browser.download.folderList"), QVariant(2));
    //mozContext->setPref(QString("browser.download.dir"), downloadDir());
    // Downloads should never be removed automatically
    m_context->setPref(QString("browser.download.manager.retention"), QVariant(2));
    // Downloads will be canceled on quit
    // TODO: this doesn't really work. Instead the incomplete downloads get restarted
    //       on browser launch.
    m_context->setPref(QString("browser.download.manager.quitBehavior"), QVariant(2));
    // TODO: this doesn't really work too
    m_context->setPref(QString("browser.helperApps.deleteTempFileOnExit"), QVariant(true));
    m_context->setPref(QString("geo.wifi.scan"), QVariant(false));
    m_context->setPref(QString("browser.enable_automatic_image_resizing"), QVariant(true));

    // Make long press timeout equal to the one in Qt
    m_context->setPref(QString("ui.click_hold_context_menus.delay"), QVariant(800));
    m_context->setPref(QString("apz.fling_stopped_threshold"), QString("0.13f"));

    // subscribe to gecko messages
    m_context->addObservers(QStringList()
                             << "clipboard:setdata"
                             << "media-decoder-info"
                             << "embed:download"
                             << "embed:search"
                             << "embedlite-before-first-paint"
                             << "embed:prefs"); // Microbe custom var

    // Enable internet search
    m_context->setPref(QString("keyword.enabled"), QVariant(true));

    // Scale up content size
    m_context->setPixelRatio(1.5);

    m_context->setPref(QString("embedlite.inputItemSize"), QVariant(38));
    m_context->setPref(QString("embedlite.zoomMargin"), QVariant(14));

    // OLD VARS
    //m_context->setPref("extensions.logging.enabled", true);
    //m_context->setPref("extensions.strictCompatibility",false);
    //m_context->setPref("dom.experimental_forms", true);
    //m_context->setPref("xpinstall.whitelist.add", "addons.mozilla.org");
    //m_context->setPref("xpinstall.whitelist.add.180", "marketplace.firefox.com");
    //m_context->setPref("embedlite.select.list.async", true);
    //m_context->setPref("browser.ui.touch.weight.visited", 120);
    // Params being tested:
    //m_context->setPref("layers.progressive-paint", false);
    
    //QStringList observers;
    //observers << "embed:download" << "embed:prefs" << "embed:allprefs" << "clipboard:setdata" << "embed:logger" << "embed:search";
    //m_context->addObservers(observers);
}

