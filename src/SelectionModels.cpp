#include "SelectionModels.h"
#include <QFile>
#include <QVariantMap>
#include <qjson/parser.h>

SelectionModel::SelectionModel(QObject *parent) :
    QStandardItemModel(parent),
    m_textAlignment(Qt::AlignCenter)
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "name";
    roles[Qt::UserRole + 1] = "value";
    this->setRoleNames(roles);
}

QString SelectionModel::name(int row) const {
    return this->data(this->index(row, 0), Qt::DisplayRole).toString();
}

QVariant SelectionModel::value(int row) const {
    return this->data(this->index(row, 0), Qt::UserRole + 1);
}

QVariantMap SelectionModel::get(int row) const {
    QVariantMap item;
    item.insert("name", this->name(row));
    item.insert("value", this->value(row));

    return item;
}

void SelectionModel::addItem(const QString &name, const QVariant &value) {
    QStandardItem *item = new QStandardItem(name);
    item->setData(value);
    item->setTextAlignment(this->textAlignment());
    item->setEditable(false);
    this->appendRow(item);
}

LanguageModel::LanguageModel(QObject *parent) :
    SelectionModel(parent)
{
    this->addItem(tr("All languages"), "all");
    this->addItem(tr("English"), "en");
    
    Q_EMIT countChanged(this->rowCount());
}

MemoryCacheSizeModel::MemoryCacheSizeModel(QObject *parent) :
    SelectionModel(parent)
{
    this->addItem(tr("Automatic"), -1);
    this->addItem(tr("40 MB"), 40);
    this->addItem(tr("20 MB"), 20);
    this->addItem(tr("10 MB"), 10);
    this->addItem(tr("5 MB"), 5);
    this->addItem(tr("Off"), 0);

    Q_EMIT countChanged(this->rowCount());
}

VolumeKeyActionModel::VolumeKeyActionModel(QObject *parent) :
    SelectionModel(parent)
{
    this->addItem(tr("Zooming"), 0);
    this->addItem(tr("Volume"), 1);

    Q_EMIT countChanged(this->rowCount());
}

ScreenOrientationModel::ScreenOrientationModel(QObject *parent) :
    SelectionModel(parent)
{
    this->addItem(tr("Automatic"), ScreenOrientation::Automatic);
    this->addItem(tr("Portrait"), ScreenOrientation::LockPortrait);
    this->addItem(tr("Landscape"), ScreenOrientation::LockLandscape);

    Q_EMIT countChanged(this->rowCount());
}

PopUpActionModel::PopUpActionModel(QObject *parent) :
    SelectionModel(parent)
{
    this->addItem(tr("Always"), 0);
    this->addItem(tr("Only requested"), 1);
    this->addItem(tr("Never"), 2);

    Q_EMIT countChanged(this->rowCount());
}

CookieActionModel::CookieActionModel(QObject *parent) :
    SelectionModel(parent)
{
    this->addItem(tr("Always"), 0);
    this->addItem(tr("Ask first"), 1);
    this->addItem(tr("Never"), 2);

    Q_EMIT countChanged(this->rowCount());
}

SearchEngineModel::SearchEngineModel(QObject *parent) :
    SelectionModel(parent)
{
    QFile pluginlist("~/.mozilla/mozembed/search.json");
    pluginlist.open(QIODevice::ReadOnly | QIODevice::Text);
    
    QByteArray json = pluginlist.readAll();
    
    QJson::Parser parser;
    bool ok;
    //QVariantMap result = parser.parse(json, &ok);
    
    //foreach (QVariant plugin, result["engines"].toList())
    //{
    //    QVariantMap engine = plugin.toMap();
    //    qDebug() << "\t-" << plugin.toString();
    //}
    
    //this->addItem(tr("Always"), 0);

    Q_EMIT countChanged(this->rowCount());
}
