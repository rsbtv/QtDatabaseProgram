#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRelationalTableModel>
#include <QtSql/QSqlRelationalDelegate>
#include <QMessageBox>
#include <QtDebug>
#include <dialog_createplaylist.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Register_clicked();

    void on_pushButton_Login_clicked();

    void on_radioButton_Register_clicked();

    void on_radioButton_Login_clicked();

    void on_pushButton_Exit_clicked();

    void on_comboBox_Tables_currentIndexChanged(int index);

    void on_pushButton_Submit_clicked();

    void on_pushButton_Reset_clicked();

    void on_pushButton_Add_clicked();

    void on_pushButton_Del_clicked();

    void on_pushButton_AddTrack_clicked();

    void hideEditButtons();

    void showEditButtons();

    void on_pushButton_AddArtist_clicked();

    void on_pushButton_DelArtist_clicked();

    void on_pushButton_AddAlbum_clicked();

    void on_pushButton_OpenAlbum_clicked();

    void on_pushButton_CreatePlaylist_clicked();

    void on_pushButton_DelPlaylist_clicked();

    void on_pushButton_OpenTracksForAdding_clicked();

    void on_pushButton_AddTrackToPlaylist_clicked();

    void on_pushButton_OpenPlaylist_clicked();

    void on_pushButton_DelTrackFromPlaylist_clicked();

    void on_pushButton_DelTrack_clicked();

    void on_pushButton_DelAlbum_clicked();

    void on_pushButton_Back_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase *db;
    QSqlQuery qw;
    QSqlRelationalTableModel *model;

    QString currentUserID;
    QString currentPlaylistID;
    bool admin = false;
};

#endif // MAINWINDOW_H
