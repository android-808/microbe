#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "dialogues/AbstractDialog.h"

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
    void accept();

protected:
    QDialogButtonBox* mButtonBox;
    QVBoxLayout* mMainLayout;
    QGridLayout* mGrid;
    
    //QMaemo5ValueButton* memoryCacheSizeButton;
    QMaemo5ValueButton* screenOrientationButton;
    QMaemo5ValueButton* searchEngineButton;
};

#endif // SETTINGSDIALOG_H
