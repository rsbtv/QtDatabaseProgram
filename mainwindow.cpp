#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) // Конструктор
{
    ui->setupUi(this);
    db = nullptr;

    // >подключение к бд
    if(db != nullptr)
    {
        db->close();
        QMessageBox::information(this, "Ошибка", "База данных не окрыта", QMessageBox::Ok);
    }
    else
    {
        db = new QSqlDatabase(QSqlDatabase::addDatabase("QPSQL"));
        QString dbName("postgres");//наша база данных без расширения
        QString host("127.0.0.1");//адресс сервера
        QString usr("postgres");//логин
        QString pwd("12345678");//пароль
        db->setDatabaseName(dbName);
        db->setHostName(host);
        db->setUserName(usr);
        db->setPassword(pwd);
        db->setPort(5432);

        // скрываем кнопки
        ui->pushButton_Login->hide();
        ui->pushButton_Register->hide();
        ui->lineEdit_Name->hide();
        ui->lineEdit_Email->hide();
        ui->lineEdit_Password->hide();

        ui->label_Name->hide();
        ui->label_Email->hide();
        ui->label_Password->hide();

        ui->tableView->hide();
        ui->pushButton_Exit->hide();
        ui->comboBox_Tables->hide();

        hideEditButtons();

        // tablemodel
        model = new QSqlRelationalTableModel();
        model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    }
    if (!db->open())
    {
        QMessageBox::warning(this, "Error", db->lastError().text());
        delete db;
        db = nullptr;
    }
}

MainWindow::~MainWindow() // Деструктор
{
    delete ui;
}

void MainWindow::on_pushButton_Register_clicked() // Регистрация пользователя
{
    QString name = ui->lineEdit_Name->text();
    QString email = ui->lineEdit_Email->text();
    QString password = ui->lineEdit_Password->text();
    if (!name.isEmpty() && !email.isEmpty() && !password.isEmpty())
    {
        if (db->isOpen())
        {
                QString qwText;
                qwText = "SELECT COUNT(*) FROM users WHERE email='" + email + "';"; // проверка на существование пользователя
                qw = db->exec(qwText);
                while (qw.next())
                {
                    if (qw.value(0) > 0) // если пользователь уже есть
                        QMessageBox::critical(this, "Ошибка!", "Пользователь с такой почтой уже зарегистрирован!");

                    else {
                        qwText = "INSERT INTO users (name, email, password) VALUES ('" + name + "','" + email + "','" + password + "');";
                        db->exec(qwText);
                        QMessageBox::information(this, "Успешно!", "Вы зарегистрировали учетную запись!");
                        qwText = "SELECT id FROM users WHERE email='" + email + "';";
                        qw = db->exec(qwText);

                        while (qw.next())
                        {
                            currentUserID = qw.value(0).toString();
                            qDebug() << currentUserID;
                        }

                        ui->lineEdit_Name->hide();
                        ui->lineEdit_Email->hide();
                        ui->lineEdit_Password->hide();
                        ui->label_Name->hide();
                        ui->label_Email->hide();
                        ui->label_Password->hide();
                        ui->pushButton_Exit->show();
                        ui->pushButton_Register->hide();
                        ui->tableView->show();
                        ui->groupBox->hide();
                        ui->comboBox_Tables->show();
                        ui->comboBox_Tables->clear();
                        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

                        hideEditButtons();

                        ui->comboBox_Tables->addItem("Треки"); // 0
                        ui->comboBox_Tables->addItem("Мои треки"); // 1
                        ui->comboBox_Tables->addItem("Артисты"); // 2
                        ui->comboBox_Tables->addItem("Любимые артисты"); // 3
                        ui->comboBox_Tables->addItem("Альбомы"); // 4
                        ui->comboBox_Tables->addItem("Любимые альбомы"); // 5
                        ui->comboBox_Tables->addItem("Мои плейлисты"); // 6

                        ui->radioButton_Login->setAutoExclusive(false);
                        ui->radioButton_Register->setAutoExclusive(false);
                        ui->radioButton_Login->setChecked(false);
                        ui->radioButton_Register->setChecked(false);
                        ui->radioButton_Login->setAutoExclusive(true);
                        ui->radioButton_Register->setAutoExclusive(true);

//                        on_comboBox_Tables_currentIndexChanged(0);
                    }
                }
        }
    }
    else {
        QMessageBox::critical(this, "Ошибка!", "Введите данные!", QMessageBox::Ok);
    }
}

void MainWindow::on_pushButton_Login_clicked() // ЛОГИН
{
    QString email = ui->lineEdit_Email->text();
    QString password = ui->lineEdit_Password->text();
    if (!email.isEmpty() && !password.isEmpty())
    {
        if (db->isOpen())
        {
            if (email == "admin@mail.ru" and password == "admin") // ЛОГИН ДЛЯ АДМИНА
            {
                ui->comboBox_Tables->clear();
                QMessageBox::information(this, "Успешно!", "Вы вошли как администратор");
                admin = true;
                ui->lineEdit_Name->hide();
                ui->lineEdit_Email->hide();
                ui->lineEdit_Password->hide();
                ui->label_Name->hide();
                ui->label_Email->hide();
                ui->label_Password->hide();
                ui->pushButton_Exit->show();
                ui->pushButton_Login->hide();
                ui->tableView->show();
                ui->groupBox->hide();
                ui->comboBox_Tables->addItem("Жанры"); // 0
//                ui->comboBox_Tables->addItem("Типы плейлистов"); // 1
                ui->comboBox_Tables->addItem("Плейлисты"); // 1
                ui->comboBox_Tables->addItem("Артисты"); // 2
                ui->comboBox_Tables->addItem("Альбомы"); // 3
                ui->comboBox_Tables->addItem("Треки"); // 4
                ui->comboBox_Tables->addItem("Страны"); // 5
                ui->comboBox_Tables->show();

                hideEditButtons();
                ui->radioButton_Login->setAutoExclusive(false);
                ui->radioButton_Register->setAutoExclusive(false);
                ui->radioButton_Login->setChecked(false);
                ui->radioButton_Register->setChecked(false);
                ui->radioButton_Login->setAutoExclusive(true);
                ui->radioButton_Register->setAutoExclusive(true);

                on_comboBox_Tables_currentIndexChanged(0);
            }
            else // КАК ОБЫЧНЫЙ ПОЛЬЗОВАТЕЛЬ
            {
            QString qwText;
            qwText = "SELECT COUNT(*) FROM users WHERE email='" + email + "' and password='" + password + "';";
            qw = db->exec(qwText);
            while (qw.next())
            {
                if (qw.value(0) > 0) {
                    ui->comboBox_Tables->clear();
                    QMessageBox::information(this, "Успешно!", "Вы вошли в учётную запись!");
                    qwText = "SELECT id FROM users WHERE email='" + email + "';";
                    qw = db->exec(qwText);
                    while (qw.next())
                    {
                        currentUserID = qw.value(0).toString();
                        qDebug() << "current user ID is: " << currentUserID;
                    }
                    ui->lineEdit_Name->hide();
                    ui->lineEdit_Email->hide();
                    ui->lineEdit_Password->hide();
                    ui->label_Name->hide();
                    ui->label_Email->hide();
                    ui->label_Password->hide();
                    ui->pushButton_Exit->show();
                    ui->pushButton_Login->hide();
                    ui->tableView->show();
                    ui->groupBox->hide();
                    ui->comboBox_Tables->show();
                    ui->comboBox_Tables->clear();

                    hideEditButtons();
                    ui->radioButton_Login->setAutoExclusive(false);
                    ui->radioButton_Register->setAutoExclusive(false);
                    ui->radioButton_Login->setChecked(false);
                    ui->radioButton_Register->setChecked(false);
                    ui->radioButton_Login->setAutoExclusive(true);
                    ui->radioButton_Register->setAutoExclusive(true);


                    ui->comboBox_Tables->addItem("Треки"); // 0
                    ui->comboBox_Tables->addItem("Мои треки"); // 1
                    ui->comboBox_Tables->addItem("Артисты"); // 2
                    ui->comboBox_Tables->addItem("Любимые артисты"); // 3
                    ui->comboBox_Tables->addItem("Альбомы"); // 4
                    ui->comboBox_Tables->addItem("Любимые альбомы"); // 5
                    ui->comboBox_Tables->addItem("Мои плейлисты"); // 6


                    admin = false;

                }
                else {
                    QMessageBox::critical(this, "Ошибка!", "Неверно введены данные!");
                }
            }
            }

    }
    else
        QMessageBox::critical(this, "Ошибка!", "Введите данные!", QMessageBox::Ok);
}
    else {
        QMessageBox::critical(this, "Ошибка!", "Введите данные!", QMessageBox::Ok);
    }
}

void MainWindow::on_radioButton_Register_clicked() // выбран режим регистрация
{
    ui->lineEdit_Name->show();
    ui->lineEdit_Email->show();
    ui->lineEdit_Password->show();
    ui->label_Name->show();
    ui->label_Email->show();
    ui->label_Password->show();
    ui->pushButton_Register->show();

    ui->pushButton_Login->hide();
    ui->pushButton_Exit->hide();
}

void MainWindow::on_radioButton_Login_clicked() // выбран режим авторизация
{
    ui->lineEdit_Name->hide();
    ui->pushButton_Register->hide();
    ui->pushButton_Exit->hide();
    ui->label_Name->hide();

    ui->label_Email->show();
    ui->label_Password->show();

    ui->lineEdit_Email->show();
    ui->lineEdit_Password->show();
    ui->pushButton_Login->show();
}

void MainWindow::on_pushButton_Exit_clicked() // выйти из аккаунта
{
    ui->groupBox->show();
    ui->tableView->hide();
    ui->comboBox_Tables->hide();
    ui->pushButton_Exit->hide();

    hideEditButtons();

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    admin = false;

    ui->lineEdit_Name->clear();
    ui->lineEdit_Email->clear();
    ui->lineEdit_Password->clear();
}

void MainWindow::on_comboBox_Tables_currentIndexChanged(int index) // ВЫБОР ТАБЛИЦЫ
{
    if (admin) // ДЛЯ АДМИНА
    {
        switch (index) {
        case 0:
            model->setTable("genres");  // ЖАНРЫ
            model->select();
            ui->tableView->setModel(model);
            ui->tableView->setEditTriggers(QAbstractItemView::EditTrigger(true));
            ui->tableView->hideColumn(0);

            // разблочить кнопки добавления сохранения и ресета
            hideEditButtons();
            showEditButtons();
            break;
        case 1:
            model->setTable("playlists"); // ВСЕ ПЛЕЙЛИСТЫ
            model->select();
            ui->tableView->setModel(model);
            ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->tableView->hideColumn(0);
            model->setRelation(model->fieldIndex("creator"), QSqlRelation("users", "id", "name"));
//            model->setRelation(model->fieldIndex("type"), QSqlRelation("playlist_types", "id", "Тип"));
            ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));

            // разблочить кнопки добавления сохранения и ресета
            hideEditButtons();
            ui->pushButton_OpenPlaylist->show();
            ui->tableView->clearSelection();
            break;
        case 2:
            model->setTable("artists"); // ВСЕ АРТИСТЫ
            model->select();
            ui->tableView->setModel(model);
            ui->tableView->setEditTriggers(QAbstractItemView::EditTrigger(true));
            model->setRelation(model->fieldIndex("country"), QSqlRelation("countries", "id", "Страна"));
            ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
            ui->tableView->hideColumn(0);

            // разблочить кнопки добавления сохранения и ресета
            hideEditButtons();
            showEditButtons();
            break;
        case 3:
            model->setTable("albums"); // ВСЕ АЛЬБОМЫ
            model->select();
            ui->tableView->setModel(model);
            ui->tableView->setEditTriggers(QAbstractItemView::EditTrigger(true));
            model->setRelation(model->fieldIndex("artist"), QSqlRelation("artists", "id", "Артист"));
            ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
            ui->tableView->hideColumn(0);

            // разблочить кнопки добавления сохранения и ресета
            hideEditButtons();
            showEditButtons();
            ui->pushButton_OpenAlbum->show();
            break;
        case 4:
            model->setTable("tracks"); // ВСЕ ТРЕКИ
            model->select();
            ui->tableView->setModel(model);
            ui->tableView->setEditTriggers(QAbstractItemView::EditTrigger(true));
            model->setRelation(model->fieldIndex("artist"), QSqlRelation("artists", "id", "Артист"));
            model->setRelation(model->fieldIndex("genre"), QSqlRelation("genres", "id", "Жанр"));
            model->setRelation(model->fieldIndex("album"), QSqlRelation("albums", "id", "Альбом"));
            ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
            ui->tableView->hideColumn(0);

            // разблочить кнопки добавления сохранения и ресета
            hideEditButtons();
            showEditButtons();
            break;
        case 5:
            model->setTable("countries"); // ВСЕ СТРАНЫ
            model->select();
            ui->tableView->setModel(model);
            ui->tableView->setEditTriggers(QAbstractItemView::EditTrigger(true));
            ui->tableView->hideColumn(0);

            // разблочить кнопки добавления сохранения и ресета
            hideEditButtons();
            showEditButtons();
            break;
        default:
            break;

        }
        model->sort(0, Qt::AscendingOrder);
    }
    else {
        switch (index) {
        case 0:
            model->setTable("tracks"); // ВСЕ ТРЕКИ
            model->select();

            ui->tableView->setModel(model);
            ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
            model->setRelation(model->fieldIndex("artist"), QSqlRelation("artists", "id", "Артист"));
            model->setRelation(model->fieldIndex("genre"), QSqlRelation("genres", "id", "Жанр"));
            model->setRelation(model->fieldIndex("album"), QSqlRelation("albums", "id", "Альбом"));
//            ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
            ui->tableView->hideColumn(0);

            // разблочить кнопки добавления сохранения и ресета
            hideEditButtons();

            // Показать кнопку добавления трека
            ui->pushButton_AddTrack->show();
            break;
        case 1:
            model->setTable("my_tracks"); // МОИ ТРЕКИ
            model->select();
            model->setFilter("user_id = " + currentUserID);
            ui->tableView->setModel(model);
            ui->tableView->hideColumn(0);
            ui->tableView->hideColumn(1);
            ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
            model->setRelation(model->fieldIndex("track"), QSqlRelation("tracks", "id", "Название"));
            ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));

            // разблочить кнопки добавления сохранения и ресета

            hideEditButtons();

            ui->pushButton_DelTrack->show();

            break;
        case 2:
            model->setTable("artists"); // ВСЕ АРТИСТЫ
            model->select();

            ui->tableView->setModel(model);
            ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
            model->setRelation(model->fieldIndex("country"), QSqlRelation("countries", "id", "Страна"));
            ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
            ui->tableView->hideColumn(0);

            // разблочить кнопки добавления сохранения и ресета
            hideEditButtons();

            // Показать кнопку добавления трека
            ui->pushButton_AddArtist->show();
            break;
        case 3:
            model->setTable("fav_artists"); // МОИ АРТИСТЫ
            model->select();
            model->setFilter("user_id = " + currentUserID);
            ui->tableView->setModel(model);
            ui->tableView->hideColumn(0);
            ui->tableView->hideColumn(1);
            ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
            model->setRelation(model->fieldIndex("artist"), QSqlRelation("artists", "id", "Артист"));

            hideEditButtons();
            ui->pushButton_DelArtist->show();

            break;
        case 4:
            model->setTable("albums"); // ВСЕ АЛЬБОМЫ
            model->select();
            ui->tableView->setModel(model);
            ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
            model->setRelation(model->fieldIndex("artist"), QSqlRelation("artists", "id", "Артист"));
//            ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
            ui->tableView->hideColumn(0);

            // разблочить кнопки добавления сохранения и ресета
            hideEditButtons();
            ui->pushButton_AddAlbum->show();
            ui->pushButton_OpenAlbum->show();
            break;
        case 5:
            model->setTable("my_albums"); // МОИ АЛЬБОМЫ
            model->select();
            model->setFilter("user_id = " + currentUserID);
            ui->tableView->setModel(model);
            ui->tableView->hideColumn(0);
            ui->tableView->hideColumn(1);
            ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
            model->setRelation(model->fieldIndex("album"), QSqlRelation("albums", "id", "Альбом"));

            hideEditButtons();

//            ui->pushButton_Del->show();
//            ui->pushButton_Submit->show();
//            ui->pushButton_Reset->show();
            ui->pushButton_DelAlbum->show();
            ui->pushButton_OpenAlbum->show();
            break;
        case 6:
            model->setTable("playlists");// МОИ ПЛЕЙЛИСТЫ
            model->select();
            model->setFilter("creator = " + currentUserID);
            ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
//            model->setRelation(model->fieldIndex("creator"), QSqlRelation("users", "id", "name"));
            ui->tableView->hideColumn(0);
            ui->tableView->hideColumn(2);
            hideEditButtons();
            ui->pushButton_CreatePlaylist->show();
            ui->pushButton_OpenPlaylist->show();
            ui->pushButton_DelPlaylist->show();
            ui->pushButton_OpenTracksForAdding->show();
//            ui->tableView->clearSelection();
            break;
        default:
            break;
        }
        model->sort(0, Qt::AscendingOrder);
    }
}

void MainWindow::on_pushButton_Submit_clicked() // НАЖАТА КНОПКА СОХРАНИТЬ
{
    if (model->submitAll())
        QMessageBox::information(this, "Успешно!", "Изменения сохранены!", QMessageBox::Ok);
    else
    {
        QMessageBox::critical(this, "Ошибка!", "Изменения не сохранены!", QMessageBox::Ok);
        qDebug() << db->lastError().text();
    }
}

void MainWindow::on_pushButton_Reset_clicked() // сброс изменений
{
    model->revertAll();
    QMessageBox::information(this, "Успешно!", "Изменения отменены", QMessageBox::Ok);
}

void MainWindow::on_pushButton_Add_clicked() // добавить строку
{
    model->insertRow(model->rowCount());
}

void MainWindow::on_pushButton_Del_clicked() // удалить строку
{
    int selectedRow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(selectedRow, 0);
    QString selectedItemID = model->data(index).toString();
    qDebug() << "selected item ID is: " << selectedItemID;

    if (selectedRow >= 0)
        model->removeRow(selectedRow);
    else
        QMessageBox::critical(this, "Ошибка!", "Строка не выбрана!", QMessageBox::Ok);

}

void MainWindow::on_pushButton_AddTrack_clicked() // добавить трек в медиатеку
{
    int selectedRow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(selectedRow, 0);
    QString selectedTrackID = model->data(index).toString();
    qDebug() << "selected track ID is: " << selectedTrackID;

    if (selectedTrackID.isEmpty())
    {
        QMessageBox::critical(this, "Ошибка!", "Ни один трек не выбран!", QMessageBox::Ok);
    }
    else {
        QString qwText;
        qwText = "SELECT COUNT(*) FROM my_tracks WHERE user_id=" + currentUserID + " and track=" + selectedTrackID  + ";";
        qw = db->exec(qwText);
        while (qw.next())
        {
            if (qw.value(0) > 0){
                QMessageBox::critical(this, "Ошибка!", "Данный трек уже добавлен!", QMessageBox::Ok);
            }
            else {
                qwText = "INSERT INTO my_tracks(user_id,track) VALUES (" + currentUserID + "," + selectedTrackID + ");";
                qDebug() << qwText;
                db->exec(qwText);
                QMessageBox::information(this, "Успешно!", "Трек добавлен в медиатеку!", QMessageBox::Ok);
            }
        }
    }
}

void MainWindow::on_pushButton_AddArtist_clicked() // добавить артиста в любимые
{
    int selectedRow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(selectedRow, 0);
    QString selectedArtistID = model->data(index).toString();
    qDebug() << "selected artist ID is: " << selectedArtistID;

    if (selectedArtistID.isEmpty())
    {
        QMessageBox::critical(this, "Ошибка!", "Ни один артист не выбран!", QMessageBox::Ok);
    }
    else {
        QString qwText;
        qwText = "SELECT COUNT(*) FROM fav_artists WHERE user_id=" + currentUserID + " and artist=" + selectedArtistID  + ";";
        qw = db->exec(qwText);
        while (qw.next())
        {
            if (qw.value(0) > 0){
                QMessageBox::critical(this, "Ошибка!", "Данный артист уже добавлен!", QMessageBox::Ok);
            }
            else {
                qwText = "INSERT INTO fav_artists(user_id,artist) VALUES (" + currentUserID + "," + selectedArtistID + ");";
                qDebug() << qwText;
                db->exec(qwText);
            }
        }
    }
}

void MainWindow::hideEditButtons() // скрыть кнопки редактирования
{
    ui->pushButton_Submit->hide();
    ui->pushButton_Reset->hide();
    ui->pushButton_Add->hide();
    ui->pushButton_Del->hide();
    ui->pushButton_AddTrack->hide();
    ui->pushButton_AddArtist->hide();
    ui->pushButton_DelArtist->hide();
    ui->pushButton_AddAlbum->hide();
    ui->pushButton_DelAlbum->hide();
    ui->pushButton_OpenAlbum->hide();
    ui->pushButton_CreatePlaylist->hide();
    ui->pushButton_OpenPlaylist->hide();
    ui->pushButton_DelPlaylist->hide();
    ui->pushButton_OpenTracksForAdding->hide();
    ui->pushButton_AddTrackToPlaylist->hide();
    ui->pushButton_DelTrackFromPlaylist->hide();
    ui->pushButton_DelTrack->hide();
    ui->pushButton_Back->hide();
}

void MainWindow::showEditButtons() // показать кнопки редактирования
{
    ui->pushButton_Submit->show();
    ui->pushButton_Reset->show();
    ui->pushButton_Add->show();
    ui->pushButton_Del->show();
}




void MainWindow::on_pushButton_DelArtist_clicked() // удалить артисты из любимых
{
    int selectedRow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(selectedRow, 0);
    QString selectedItemID = model->data(index).toString();
    qDebug() << "selected item ID is: " << selectedItemID;

    if (selectedItemID.isEmpty())
    {
        QMessageBox::critical(this, "Ошибка!", "Ни один артист не выбран!", QMessageBox::Ok);
    }
    else {
        QMessageBox::StandardButton answer = QMessageBox::question(this, "Удаление", "Вы уверены, что хотите удалить данного артиста?", QMessageBox::Yes|QMessageBox::No);
        if (answer == QMessageBox::Yes)
        {
            QString qwText;
            qwText = "DELETE FROM fav_artists WHERE id=" + selectedItemID  + ";";
            qw = db->exec(qwText);
            model->select();
            QMessageBox::information(this, "Успешно!", "Артист удалён из списка любимых!", QMessageBox::Ok);
        }
    }


}

void MainWindow::on_pushButton_AddAlbum_clicked() // добавить альбом в любимые
{
    int selectedRow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(selectedRow, 0);
    QString selectedAlbumID = model->data(index).toString();
    qDebug() << "selected album ID is: " << selectedAlbumID;

    if (selectedAlbumID.isEmpty())
    {
        QMessageBox::critical(this, "Ошибка!", "Ни один альбом не выбран!", QMessageBox::Ok);
    }
    else {
        QString qwText;
        qwText = "SELECT COUNT(*) FROM my_albums WHERE user_id=" + currentUserID + " and album=" + selectedAlbumID  + ";";
        qw = db->exec(qwText);
        while (qw.next())
        {
            if (qw.value(0) > 0){
                QMessageBox::critical(this, "Ошибка!", "Данный альбом уже добавлен!", QMessageBox::Ok);
            }
            else {
                qwText = "INSERT INTO my_albums(user_id,album) VALUES (" + currentUserID + "," + selectedAlbumID + ");";
                qDebug() << qwText;
                db->exec(qwText);
            }
        }
    }
}

void MainWindow::on_pushButton_OpenAlbum_clicked() // открыть содержимое альбома
{
    int selectedRow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(selectedRow, 0);
    QString selectedAlbumID = model->data(index).toString();
    qDebug() << "selected Album ID is: " << selectedAlbumID;

    if (selectedAlbumID.isEmpty())
    {
        QMessageBox::critical(this, "Ошибка!", "Ни один альбом не выбран!", QMessageBox::Ok);
    }
    else {
        if (admin) {
            on_comboBox_Tables_currentIndexChanged(4);
//            ui->comboBox_Tables->setCurrentIndex(4);
            hideEditButtons();
        }
        else
        {
            on_comboBox_Tables_currentIndexChanged(0);
            ui->comboBox_Tables->setCurrentIndex(0);
        }
        model->setFilter("album =" + selectedAlbumID);
        ui->pushButton_Back->show();
    }
}

void MainWindow::on_pushButton_CreatePlaylist_clicked() // создать плейлист
{
    Dialog_createPlaylist dialog;
    dialog.exec();
    QString playlistTitle = dialog.getPlaylistTitle();
    qDebug() << playlistTitle;

    QString qwText = "SELECT COUNT(*) FROM playlists WHERE title='" + playlistTitle + "';";
    qw = db->exec(qwText);
    while (qw.next())
    {
        if (qw.value(0) > 0){
            QMessageBox::critical(this, "Ошибка!", "Плейлист с таким названием уже существует!", QMessageBox::Ok);
        }
        else {
            qwText = "INSERT INTO playlists(title,creator) VALUES ('"+ playlistTitle + "'," + currentUserID + ");";
            qDebug() << qwText;
            db->exec(qwText);
        }
    }
    model->select();
}

void MainWindow::on_pushButton_DelPlaylist_clicked() // удалить плейлист
{
    int selectedRow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(selectedRow, 0);
    QString selectedItemID = model->data(index).toString();
    qDebug() << "selected item ID is: " << selectedItemID;

    if (selectedItemID.isEmpty())
    {
        QMessageBox::critical(this, "Ошибка!", "Ни один плейлист не выбран!", QMessageBox::Ok);
    }
    else {
        QString qwText;
        QMessageBox::StandardButton answer = QMessageBox::question(this, "Удаление", "Вы уверены, что хотите удалить данный плейлист?\nПеред удалением удостоверьтесь, что плейлист пуст.", QMessageBox::Yes|QMessageBox::No);
        if (answer == QMessageBox::Yes)
        {
            qwText = "DELETE FROM playlists WHERE id=" + selectedItemID  + ";";
            qw = db->exec(qwText);
            qDebug() << db->lastError();
            if (!db->lastError().text().isEmpty())
            {
                QMessageBox::critical(this, "Ошибка", db->lastError().text(), QMessageBox::Ok);
            }
        }
    }
    model->select();
}

void MainWindow::on_pushButton_OpenTracksForAdding_clicked() // открыть список трека для добавления в плейлист
{
    int selectedRow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(selectedRow, 0);
    currentPlaylistID = model->data(index).toString();



//    model->setTable("tracks");
//    model->select();
//    model->setRelation(model->fieldIndex("artist"), QSqlRelation("artists", "id", "Артист"));
//    model->setRelation(model->fieldIndex("genre"), QSqlRelation("genres", "id", "Жанр"));
//    model->setRelation(model->fieldIndex("album"), QSqlRelation("albums", "id", "Альбом"));

//    ui->tableView->setModel(model);
    if (!admin) {
        on_comboBox_Tables_currentIndexChanged(0);
        hideEditButtons();
    }
//    ui->tableView->hideColumn(0);

    ui->pushButton_AddTrackToPlaylist->show();
    ui->pushButton_Back->show();
}

void MainWindow::on_pushButton_AddTrackToPlaylist_clicked() // добавить трек в плейлист
{
//    ui->tableView->clearSelection();
    int selectedRow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(selectedRow, 0);
    QString selectedTrackID = model->data(index).toString();

    if (selectedTrackID.isEmpty())
        QMessageBox::critical(this, "Ошибка!", "Ни один трек не выбран!", QMessageBox::Ok);
    else {
        QString qwText;
        qwText = "SELECT COUNT(*) FROM playlist_content WHERE playlist=" + currentPlaylistID + " and track=" + selectedTrackID  + ";";
        qw = db->exec(qwText);
        while (qw.next())
        {
            if (qw.value(0) > 0)
                QMessageBox::critical(this, "Ошибка!", "Данный трек уже добавлен!", QMessageBox::Ok);
            else {
                qwText = "INSERT INTO playlist_content(playlist,track) VALUES (" + currentPlaylistID + "," + selectedTrackID + ");";
                qDebug() << qwText;
                db->exec(qwText);
                QMessageBox::information(this, "Успешно!", "Трек добавлен в плейлист!", QMessageBox::Ok);
            }
        }
    }

}

void MainWindow::on_pushButton_OpenPlaylist_clicked() // открыть содержимое плейлиста
{
    hideEditButtons();

    int selectedRow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(selectedRow, 0);
    QString selectedPlaylistID = model->data(index).toString();
    currentPlaylistID = selectedPlaylistID;

    qDebug() << "currentPlaylistID is:hideEditButtons(); " << currentPlaylistID;
    qDebug() << "selectedPlaylistID is: " << selectedPlaylistID;

    if (selectedPlaylistID.isEmpty())
        QMessageBox::critical(this, "Ошибка!", "Ни один плейлист не выбран!", QMessageBox::Ok);
    else {
        model->setTable("playlist_content");
        model->select();
        ui->tableView->setModel(model);
        model->setRelation(model->fieldIndex("track"), QSqlRelation("tracks", "id", "Название"));
        ui->tableView->hideColumn(0);
        ui->tableView->hideColumn(1);
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

        if (!admin)
            ui->pushButton_DelTrackFromPlaylist->show();

        model->setFilter("playlist =" + selectedPlaylistID);
        ui->pushButton_Back->show();
    }
}




void MainWindow::on_pushButton_DelTrackFromPlaylist_clicked() // удалить трек из плейлиста
{
    int selectedRow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(selectedRow, 0);
    QString selectedItemID = model->data(index).toString();
    qDebug() << "selected item ID is: " << selectedItemID;
    qDebug() << "selected playlist is: " << currentPlaylistID;

    if (selectedItemID.isEmpty())
    {
        QMessageBox::critical(this, "Ошибка!", "Ни один трек не выбран!", QMessageBox::Ok);
    }
    else {
        QMessageBox::StandardButton answer = QMessageBox::question(this, "Удаление", "Вы уверены, что хотите удалить данный трек из плейлиста?", QMessageBox::Yes|QMessageBox::No);
        if (answer == QMessageBox::Yes)
        {
        QString qwText;
        qwText = "DELETE FROM playlist_content WHERE id=" + selectedItemID +  ";";
        qDebug() << qwText;
        qw = db->exec(qwText);
        }
    }

    model->select();
}

void MainWindow::on_pushButton_DelTrack_clicked() // удалить трек из медиатеки
{
    int selectedRow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(selectedRow, 0);
    QString selectedItemID = model->data(index).toString();

    if (selectedItemID.isEmpty())
    {
        QMessageBox::critical(this, "Ошибка!", "Ни один трек не выбран!", QMessageBox::Ok);
    }
    else {
        QMessageBox::StandardButton answer = QMessageBox::question(this, "Удаление", "Вы уверены, что хотите удалить данный трек?", QMessageBox::Yes|QMessageBox::No);
        if (answer == QMessageBox::Yes)
        {
            QString qwText = "DELETE FROM my_tracks WHERE id=" + selectedItemID + ";";
            qw = db->exec(qwText);
            model->select();
            QMessageBox::information(this, "Успешно!", "Трек удалён из медиатеки", QMessageBox::Ok);
        }
    }
}

void MainWindow::on_pushButton_DelAlbum_clicked() // удалить альбом из любимых
{

    int selectedRow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(selectedRow, 0);
    QString selectedItemID = model->data(index).toString();

    if (selectedItemID.isEmpty())
    {
        QMessageBox::critical(this, "Ошибка!", "Ни один альбом не выбран!", QMessageBox::Ok);
    }
    else {
        QMessageBox::StandardButton answer = QMessageBox::question(this, "Удаление", "Вы уверены, что хотите удалить данный альбом?", QMessageBox::Yes|QMessageBox::No);
        if (answer == QMessageBox::Yes)
        {
            QString qwText = "DELETE FROM my_albums WHERE id=" + selectedItemID + ";";
            qw = db->exec(qwText);
            model->select();
            QMessageBox::information(this, "Успешно!", "Альбом удалён из медиатеки", QMessageBox::Ok);
        }
    }
}

void MainWindow::on_pushButton_Back_clicked() // назад
{
    if (admin)
    {
        if (ui->comboBox_Tables->currentText() == "Плейлисты")
            on_comboBox_Tables_currentIndexChanged(1);
        else if (ui->comboBox_Tables->currentText() == "Альбомы")
            on_comboBox_Tables_currentIndexChanged(3);
    }
    else
    {
        if (ui->comboBox_Tables->currentText() == "Треки")
        {
            on_comboBox_Tables_currentIndexChanged(4);
        }
        else if (ui->comboBox_Tables->currentText() == "Мои плейлисты")
        {
            on_comboBox_Tables_currentIndexChanged(6);
        }

    }
}


