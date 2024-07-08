#ifndef DIALOG_CREATEPLAYLIST_H
#define DIALOG_CREATEPLAYLIST_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <QtDebug>

namespace Ui {
class Dialog_createPlaylist;
}

class Dialog_createPlaylist : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_createPlaylist(QWidget *parent = nullptr);
    ~Dialog_createPlaylist();

    QString getPlaylistTitle();

private slots:
    void on_pushButton_Create_clicked();

private:
    Ui::Dialog_createPlaylist *ui;

    void setPlaylistTitle(QString title);

    QString playlistTitle;
};

#endif // DIALOG_CREATEPLAYLIST_H
