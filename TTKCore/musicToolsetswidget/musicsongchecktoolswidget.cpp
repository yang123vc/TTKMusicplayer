#include "musicsongchecktoolswidget.h"
#include "ui_musicsongchecktoolswidget.h"
#include "musictoolsetsuiobject.h"
#include "musicuiobject.h"
#include "musicsongchecktoolscore.h"
#include "musicconnectionpool.h"
#include "musicsongssummariziedwidget.h"
#include "musicsongchecktoolsitemselecteddialog.h"

MusicSongCheckToolsWidget::MusicSongCheckToolsWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicSongCheckToolsWidget)
{
    Q_UNUSED(qRegisterMetaType<SongCheckToolsRenames>("SongCheckToolsRenames"));
    Q_UNUSED(qRegisterMetaType<SongCheckToolsDuplicates>("SongCheckToolsDuplicates"));
    Q_UNUSED(qRegisterMetaType<SongCheckToolsQualitys>("SongCheckToolsQualitys"));
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->modifiedItemButton, SIGNAL(clicked()), SLOT(modifiedItemButtonClicked()));

    renameWidgetInit();
    qualityWidgetInit();
    duplicateWidgetInit();

    switchToSelectedItemStyle(0);

    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicSongsSummariziedWidget::getClassName());
}

MusicSongCheckToolsWidget::~MusicSongCheckToolsWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName());
    delete m_renameCore;
    delete m_duplicateCore;
    delete m_qualityCore;
    delete m_ui;
}

QString MusicSongCheckToolsWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongCheckToolsWidget::modifiedItemButtonClicked()
{
    MusicSongItems songs;
    emit getMusicLists(songs);

    MusicSongCheckToolsItemSelectedDialog dialog;
    dialog.createAllItems(&songs);
    dialog.exec();
}

void MusicSongCheckToolsWidget::renameButtonClicked()
{
    switchToSelectedItemStyle(0);
}

void MusicSongCheckToolsWidget::renameButtonCheckClicked()
{
    if(m_ui->renameCheckButton->text() == tr("StartCheck"))
    {
        renameReCheckButtonClicked();
    }
    else if(m_ui->renameCheckButton->text() == tr("StopCheck"))
    {
        m_ui->renameLoadingLabel->stop();
        m_ui->renameLoadingLabel->hide();
        m_ui->renameCheckButton->setText(tr("StartCheck"));
        m_renameCore->stopAndQuitThread();
    }
    else if(m_ui->renameCheckButton->text() == tr("ApplayCheck"))
    {
        m_ui->renameLoadingLabel->stop();
        m_ui->renameLoadingLabel->hide();
        m_ui->renameReCheckButton->show();
    }
}

void MusicSongCheckToolsWidget::renameReCheckButtonClicked()
{
    m_ui->renameReCheckButton->hide();
    m_ui->renameLoadingLabel->start();
    m_ui->renameLoadingLabel->show();
    m_ui->renameCheckButton->setText(tr("StopCheck"));
    m_ui->renameSelectAllButton->setChecked(false);

    m_ui->renameTableWidget->clear();
    m_renameCore->stopAndQuitThread();
    getSelectedSongItems();
    m_renameCore->setRenameSongs(&m_localSongs);
    m_renameCore->start();
}

void MusicSongCheckToolsWidget::renameCheckFinished(const SongCheckToolsRenames &items)
{
    m_ui->renameLoadingLabel->stop();
    m_ui->renameLoadingLabel->hide();
    m_ui->renameCheckButton->setText(tr("ApplayCheck"));
    m_ui->renameReCheckButton->show();
    m_ui->renameSelectAllButton->setEnabled(!items.isEmpty());

    m_ui->renameTableWidget->clear();
    m_ui->renameTableWidget->createAllItems(items);
}

void MusicSongCheckToolsWidget::qualityButtonClicked()
{
    switchToSelectedItemStyle(2);
}

void MusicSongCheckToolsWidget::qualityButtonCheckClicked()
{
    if(m_ui->qualityCheckButton->text() == tr("StartCheck"))
    {
        qualityReCheckButtonClicked();
    }
    else if(m_ui->qualityCheckButton->text() == tr("StopCheck"))
    {
        m_ui->qualityLoadingLabel->stop();
        m_ui->qualityLoadingLabel->hide();
        m_ui->qualityCheckButton->setText(tr("StartCheck"));
        m_qualityCore->stopAndQuitThread();
    }
    else if(m_ui->qualityCheckButton->text() == tr("ApplayCheck"))
    {
        m_ui->qualityLoadingLabel->stop();
        m_ui->qualityLoadingLabel->hide();
        m_ui->qualityReCheckButton->show();
    }
}

void MusicSongCheckToolsWidget::qualityReCheckButtonClicked()
{
    m_ui->qualityReCheckButton->hide();
    m_ui->qualityLoadingLabel->start();
    m_ui->qualityLoadingLabel->show();
    m_ui->qualityCheckButton->setText(tr("StopCheck"));

    m_qualityCore->stopAndQuitThread();
    getSelectedSongItems();
    m_qualityCore->setQualitySongs(&m_localSongs);
    m_qualityCore->start();
}

void MusicSongCheckToolsWidget::qualityCheckFinished(const SongCheckToolsQualitys &items)
{
    m_ui->qualityLoadingLabel->stop();
    m_ui->qualityLoadingLabel->hide();
    m_ui->qualityCheckButton->setText(tr("ApplayCheck"));
    m_ui->qualityReCheckButton->show();

    m_ui->qualityTableWidget->clear();
    m_ui->qualityTableWidget->createAllItems(items);
}

void MusicSongCheckToolsWidget::duplicateButtonClicked()
{
    switchToSelectedItemStyle(1);
}

void MusicSongCheckToolsWidget::duplicateButtonCheckClicked()
{
    if(m_ui->duplicateCheckButton->text() == tr("StartCheck"))
    {
        duplicateReCheckButtonClicked();
    }
    else if(m_ui->duplicateCheckButton->text() == tr("StopCheck"))
    {
        m_ui->duplicateLoadingLabel->stop();
        m_ui->duplicateLoadingLabel->hide();
        m_ui->duplicateCheckButton->setText(tr("StartCheck"));
        m_duplicateCore->stopAndQuitThread();
    }
    else if(m_ui->duplicateCheckButton->text() == tr("ApplayCheck"))
    {
        m_ui->duplicateLoadingLabel->stop();
        m_ui->duplicateLoadingLabel->hide();
        m_ui->duplicateReCheckButton->show();
    }
}

void MusicSongCheckToolsWidget::duplicateReCheckButtonClicked()
{
    m_ui->duplicateReCheckButton->hide();
    m_ui->duplicateLoadingLabel->start();
    m_ui->duplicateLoadingLabel->show();
    m_ui->duplicateCheckButton->setText(tr("StopCheck"));
    m_ui->duplicateSelectAllButton->setChecked(false);

    m_qualityCore->stopAndQuitThread();
    getSelectedSongItems();
    m_duplicateCore->setDuplicateSongs(&m_localSongs);
    m_duplicateCore->start();
}

void MusicSongCheckToolsWidget::duplicateCheckFinished(const SongCheckToolsDuplicates &items)
{
    m_ui->duplicateLoadingLabel->stop();
    m_ui->duplicateLoadingLabel->hide();
    m_ui->duplicateCheckButton->setText(tr("ApplayCheck"));
    m_ui->duplicateReCheckButton->show();
    m_ui->duplicateSelectAllButton->setEnabled(!items.isEmpty());

    m_ui->duplicateTableWidget->clear();
    m_ui->duplicateTableWidget->createAllItems(items);
}

void MusicSongCheckToolsWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}

void MusicSongCheckToolsWidget::getSelectedSongItems()
{
    m_localSongs.clear();

    MusicSongItems songs;
    emit getMusicLists(songs);

    foreach(const MusicSongItem &item, songs)
    {
        if(item.m_itemIndex != MUSIC_LOVEST_LIST && item.m_itemIndex != MUSIC_NETWORK_LIST &&
           item.m_itemIndex != MUSIC_RECENT_LIST )
        {
            m_localSongs << item.m_songs;
        }
    }
}

void MusicSongCheckToolsWidget::renameWidgetInit()
{
    m_ui->renameSelectAllButton->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->renameCheckButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    connect(m_ui->renameButton, SIGNAL(clicked()), SLOT(renameButtonClicked()));
    connect(m_ui->renameCheckButton, SIGNAL(clicked()), SLOT(renameButtonCheckClicked()));
    connect(m_ui->renameReCheckButton, SIGNAL(clicked()), SLOT(renameReCheckButtonClicked()));
    connect(m_ui->renameSelectAllButton, SIGNAL(clicked(bool)), m_ui->renameTableWidget, SLOT(selectedAllItems(bool)));

    m_ui->renameSelectAllButton->setEnabled(false);
    m_ui->renameLoadingLabel->setType(MusicGifLabelWidget::Gif_Cicle_Blue);
    m_ui->renameLoadingLabel->hide();
    m_ui->renameReCheckButton->hide();

    m_renameCore = new MusicSongCheckToolsRenameCore(this);
    connect(m_renameCore, SIGNAL(finished(SongCheckToolsRenames)), SLOT(renameCheckFinished(SongCheckToolsRenames)));
}

void MusicSongCheckToolsWidget::qualityWidgetInit()
{
    m_ui->qualityCheckButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    connect(m_ui->qualityButton, SIGNAL(clicked()), SLOT(qualityButtonClicked()));
    connect(m_ui->qualityCheckButton, SIGNAL(clicked()), SLOT(qualityButtonCheckClicked()));
    connect(m_ui->qualityReCheckButton, SIGNAL(clicked()), SLOT(qualityReCheckButtonClicked()));

    m_ui->qualityLoadingLabel->setType(MusicGifLabelWidget::Gif_Cicle_Blue);
    m_ui->qualityLoadingLabel->hide();
    m_ui->qualityReCheckButton->hide();

    m_qualityCore = new MusicSongCheckToolsQualityCore(this);
    connect(m_qualityCore, SIGNAL(finished(SongCheckToolsQualitys)), SLOT(qualityCheckFinished(SongCheckToolsQualitys)));
}

void MusicSongCheckToolsWidget::duplicateWidgetInit()
{
    m_ui->duplicateSelectAllButton->setStyleSheet(MusicUIObject::MCheckBoxStyle01);
    m_ui->duplicateCheckButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    connect(m_ui->duplicateButton, SIGNAL(clicked()), SLOT(duplicateButtonClicked()));
    connect(m_ui->duplicateCheckButton, SIGNAL(clicked()), SLOT(duplicateButtonCheckClicked()));
    connect(m_ui->duplicateReCheckButton, SIGNAL(clicked()), SLOT(duplicateReCheckButtonClicked()));
    connect(m_ui->duplicateSelectAllButton, SIGNAL(clicked(bool)), m_ui->duplicateTableWidget, SLOT(selectedAllItems(bool)));

    m_ui->duplicateSelectAllButton->setEnabled(false);
    m_ui->duplicateLoadingLabel->setType(MusicGifLabelWidget::Gif_Cicle_Blue);
    m_ui->duplicateLoadingLabel->hide();
    m_ui->duplicateReCheckButton->hide();

    m_duplicateCore = new MusicSongCheckToolsDuplicateCore(this);
    connect(m_duplicateCore, SIGNAL(finished(SongCheckToolsDuplicates)), SLOT(duplicateCheckFinished(SongCheckToolsDuplicates)));
}

void MusicSongCheckToolsWidget::switchToSelectedItemStyle(int index)
{
    m_ui->renameButton->setStyleSheet(MusicUIObject::MKGCheckTestRename);
    m_ui->qualityButton->setStyleSheet(MusicUIObject::MKGCheckTestQuality);
    m_ui->duplicateButton->setStyleSheet(MusicUIObject::MKGCheckTestDuplicate);

    m_ui->stackedWidget->setCurrentIndex(index);
    switch(index)
    {
        case 0: m_ui->renameButton->setStyleSheet(MusicUIObject::MKGCheckTestRenameClicked); break;
        case 1: m_ui->duplicateButton->setStyleSheet(MusicUIObject::MKGCheckTestDuplicateClicked); break;
        case 2: m_ui->qualityButton->setStyleSheet(MusicUIObject::MKGCheckTestQualityClicked); break;
        default: break;
    }
}