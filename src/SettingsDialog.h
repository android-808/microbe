#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "dialogues/AbstractDialog.h"
#include "Settings.h"

class LanguageSelector;

class QDialogButtonBox;
class QGridLayout;
class QPushButton;
class QVBoxLayout;
class QMaemo5ValueButton;

class SettingsDialog : public AbstractDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);

private:
    void setLandscapeLayout();
    void setPortraitLayout();
    void loadSettings();

protected:
    QDialogButtonBox* mButtonBox;
    QPushButton* mSaveButton;
    QVBoxLayout* mMainLayout;
    QGridLayout* mGrid;
    Settings* mSettings;
    
    LanguageSelector* mLanguageSelector;
    MemoryCacheSizeSelector* mMemoryCacheSizeSelector;
    ScreenOrientationSelector* mOrientationSelector;
   
    QMaemo5ValueButton* searchEngineButton;
    

};

#endif // SETTINGSDIALOG_H
