#include "dialog_createplaylist.h"
#include "ui_dialog_createplaylist.h"

Dialog_createPlaylist::Dialog_createPlaylist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_createPlaylist)
{
    ui->setupUi(this);

}

Dialog_createPlaylist::~Dialog_createPlaylist()
{
    delete ui;
}

QString Dialog_createPlaylist::getPlaylistTitle()
{
    return playlistTitle;
}

void Dialog_createPlaylist::setPlaylistTitle(QString title)
{
    playlistTitle = title;
}

void Dialog_createPlaylist::on_pushButton_Create_clicked()
{
    setPlaylistTitle(ui->lineEdit_Title->text());
    close();
}
