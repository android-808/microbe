#include "SettingsDialog.h"
#include "Settings.h"

#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QMaemo5ListPickSelector>
#include <QMaemo5ValueButton>
#include <QPushButton>
#include <QStandardItemModel>
#include <QSettings>

#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent) :
    AbstractDialog(parent),
    mButtonBox(new QDialogButtonBox(QDialogButtonBox::Save, Qt::Vertical, this)),
    mMainLayout(new QVBoxLayout()), // Not sure here
    mGrid(new QGridLayout(this))
{
    // Window Setup
    this->setWindowTitle(tr("Settings"));

    // Add settings
    mMainLayout->addWidget(new QLabel(tr("Browser"), this), 0, Qt::AlignHCenter);
    
    QMaemo5ListPickSelector *selector;
    QStandardItemModel *model;
    
    /*memoryCacheSizeButton = new QMaemo5ValueButton(tr("Memory cache size"));
    selector = new QMaemo5ListPickSelector;
    model = new QStandardItemModel(0, 1, selector);
    model->appendRow(new QStandardItem(tr("Automatic")));
    model->appendRow(new QStandardItem(tr("40 MB")));
    model->appendRow(new QStandardItem(tr("20 MB")));
    model->appendRow(new QStandardItem(tr("10 MB")));
    model->appendRow(new QStandardItem(tr("5 MB")));
    model->appendRow(new QStandardItem(tr("Off")));
    selector->setModel(model);
    memoryCacheSizeButton->setPickSelector(selector);
    mMainLayout->addWidget(memoryCacheSizeButton);*/

    screenOrientationButton = new QMaemo5ValueButton(tr("Screen orientation"));
    selector = new QMaemo5ListPickSelector;
    model = new QStandardItemModel(0, 1, selector);
    model->appendRow(new QStandardItem(tr("Automatic")));
    model->appendRow(new QStandardItem(tr("Landscape")));
    model->appendRow(new QStandardItem(tr("Portrait")));
    selector->setModel(model);
    selector->setCurrentIndex(QSettings().value("Browser/ScreenOrientation").toString() == "Automatic" ? 0 :
                              QSettings().value("Browser/ScreenOrientation").toString() == "Landscape" ? 1 :
                              QSettings().value("Browser/ScreenOrientation").toString() == "Portrait" ? 2 : 0);
    screenOrientationButton->setPickSelector(selector);
    mMainLayout->addWidget(screenOrientationButton);
    
    // TODO: It is possible to add icon next to items.
    searchEngineButton = new QMaemo5ValueButton(tr("Selected search engine"));
    selector = new QMaemo5ListPickSelector;
    model = new QStandardItemModel(0, 1, selector);
    model->appendRow(new QStandardItem(tr("Bing")));
    model->appendRow(new QStandardItem(tr("Google")));
    model->appendRow(new QStandardItem(tr("Yahoo")));
    selector->setModel(model);
    selector->setCurrentIndex(QSettings().value("Browser/SearchEngine").toString() == "Bing" ? 0 :
                              QSettings().value("Browser/SearchEngine").toString() == "Google" ? 1 :
                              QSettings().value("Browser/SearchEngine").toString() == "Yahoo" ? 2 : 1);
    searchEngineButton->setPickSelector(selector);
    mMainLayout->addWidget(searchEngineButton);
    
    connect(mButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
    
    this->onOrientationChanged();
}

void SettingsDialog::setLandscapeLayout() {
    this->setMaximumHeight(260);
    mButtonBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    if (mGrid->isEmpty()) {
        mGrid->addLayout(mMainLayout, 0, 0);
        mGrid->addWidget(mButtonBox, 0, 1, Qt::AlignBottom);
    }
    else {
        mGrid->removeWidget(mButtonBox);
        mGrid->addWidget(mButtonBox, 0, 1, Qt::AlignBottom);
    }
}

void SettingsDialog::setPortraitLayout() {
    this->setMaximumHeight(400);
    mButtonBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    if (mGrid->isEmpty()) {
        mGrid->addLayout(mMainLayout, 0, 0);
        mGrid->addWidget(mButtonBox, 1, 0);
    }
    else {
        mGrid->removeWidget(mButtonBox);
        mGrid->addWidget(mButtonBox, 1, 0);
    }
}

void SettingsDialog::accept()
{
    switch (static_cast<QMaemo5ListPickSelector*>(screenOrientationButton->pickSelector())->currentIndex()) {
        case 0: QSettings().setValue("Browser/ScreenOrientation", "Automatic"); break;
        case 1: QSettings().setValue("Browser/ScreenOrientation", "Landscape"); break;
        case 2: QSettings().setValue("Browser/ScreenOrientation", "Portrait"); break;
    }
    Settings::instance()->setScreenOrientation();

    switch (static_cast<QMaemo5ListPickSelector*>(searchEngineButton->pickSelector())->currentIndex()) {
        case 0: QSettings().setValue("Browser/SearchEngine", "Bing"); break;
        case 1: QSettings().setValue("Browser/SearchEngine", "Google"); break;
        case 2: QSettings().setValue("Browser/SearchEngine", "Yahoo"); break;
    }
    Settings::instance()->setSearchEngine();
    
    QDialog::accept();
}
