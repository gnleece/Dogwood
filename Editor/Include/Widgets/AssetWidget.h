#pragma once

#include <QtWidgets>
#include <string>

class AssetDatabaseModel;
class MainEditorWindow;

using std::string;

namespace Ui
{
    class AssetWidget;
}

class AssetWidget : public QWidget
{
    Q_OBJECT

public:
    enum eAssetType { ASSET_MESH, ASSET_TEXTURE, ASSET_SHADER, ASSET_SCRIPT, NUM_ASSET_TYPES };

    AssetWidget(MainEditorWindow* window, QWidget* parent = 0);

    void Init();

private:
    void MapHookup(QSignalMapper* signalMapper, QObject* sender, eAssetType assetType);

    Ui::AssetWidget*        m_ui;
    MainEditorWindow*       m_window;

    AssetDatabaseModel*     m_sourceModel;
    QSortFilterProxyModel*  m_proxyModel;

    QSignalMapper*          m_signalMapper;
    QPushButton*            m_filterButtons[NUM_ASSET_TYPES];
    string                  m_filterNames[NUM_ASSET_TYPES];

private slots:
    void ImportAssetClicked();
    void ChangeTypeFilter(int filter);

    void AddButtonClicked();
};