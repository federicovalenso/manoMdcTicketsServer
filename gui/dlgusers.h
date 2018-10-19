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
    explicit dlgUsers(QWidget* parent = nullptr);
    ~dlgUsers() override;

protected:
    virtual void resizeEvent(QResizeEvent* event) override;

private slots:
    void on_twUsers_doubleClicked(const QModelIndex& index);
    void on_btnAdd_clicked();
    void on_btnEdit_clicked();

private:
    void updateListView();
    void editUser(const QModelIndex& index);

    Ui::dlgUsers* ui;
    UserModel model_;
};

#endif // DLGUSERS_H
