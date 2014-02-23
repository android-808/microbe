#include "SettingsDialog.h"

#include "LanguageSelector.h"

#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QMaemo5ListPickSelector>
#include <QMaemo5ValueButton>
#include <QPushButton>
#include <QStandardItemModel>

#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent) :
    AbstractDialog(parent),
    mButtonBox(new QDialogButtonBox(Qt::Vertical, this)),
    mSaveButton(mButtonBox->addButton(tr("Save"), QDialogButtonBox::ActionRole)),
    mMainLayout(new QVBoxLayout()), // Not sure here
    mGrid(new QGridLayout(this))
{
    // Window Setup
    this->setWindowTitle(tr("Settings"));
    mSettings = Settings::instance();

    // Add settings
    mMainLayout->addWidget(new QLabel(tr("Browser"), this), 0, Qt::AlignHCenter);
    
    mLanguageSelector = new LanguageSelector(tr("Language"), true, this);
    mMainLayout->addWidget(mLanguageSelector);
    
    QMaemo5ListPickSelector *selector;
    QStandardItemModel *model;
    
    memoryCacheSizeButton = new QMaemo5ValueButton(tr("Memory cache size"));
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
    mMainLayout->addWidget(memoryCacheSizeButton);

    screenOrientationButton = new QMaemo5ValueButton(tr("Screen orientation"));
    selector = new QMaemo5ListPickSelector;
    model = new QStandardItemModel(0, 1, selector);
    model->appendRow(new QStandardItem(tr("Automatic")));
    model->appendRow(new QStandardItem(tr("Landscape")));
    model->appendRow(new QStandardItem(tr("Portrait")));
    selector->setModel(model);
    screenOrientationButton->setPickSelector(selector);
    mMainLayout->addWidget(screenOrientationButton);
    
    searchEngineButton = new QMaemo5ValueButton(tr("Selected search engine"));
    selector = new QMaemo5ListPickSelector;
    model = new QStandardItemModel(0, 1, selector);
    model->appendRow(new QStandardItem(tr("Bing")));
    model->appendRow(new QStandardItem(tr("Google")));
    model->appendRow(new QStandardItem(tr("Yahoo")));
    selector->setModel(model);
    searchEngineButton->setPickSelector(selector);
    mMainLayout->addWidget(searchEngineButton);

    this->loadSettings();
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

void SettingsDialog::loadSettings()
{
    mLanguageSelector->setValue(mSettings->language());
    //memoryCacheSizeButton->setValue(mSettings->memoryCacheSize());
    //screenOrientationButton->setValue(mSettings->screenOrientation());
       
    searchEngineButton->setValueText(mSettings->searchEngine());
}
