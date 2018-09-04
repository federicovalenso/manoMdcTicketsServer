#ifndef DLGUSERS_H
#define DLGUSERS_H

#include <QDialog>
#include <QList>
#include <QJsonObject>

#include "models/usermodel.h"

namespace Ui {
class dlgUsers;
}

class dlgUsers : public QDialog
{
    Q_OBJECT

public:
    explicit dlgUsers(QWidget *parent = 0);
    ~dlgUsers();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_twUsers_doubleClicked(const QModelIndex &index);

    void on_btnAdd_clicked();

private:
    void updateListView();

    Ui::dlgUsers *ui;
    UserModel model;
};

#endif // DLGUSERS_H
