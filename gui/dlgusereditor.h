#ifndef DLGUSEREDITOR_H
#define DLGUSEREDITOR_H

#include <QDialog>
#include <QAbstractItemModel>
#include <QDataWidgetMapper>


namespace Ui {
class UserEditDialog;
}

class UserEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserEditDialog(QWidget *parent = nullptr);
    ~UserEditDialog() override;

    void setModel(QAbstractItemModel*);
    void setCurrentModelIndex(const QModelIndex&);

public slots:
    virtual void done(int) override;

private slots:
    void on_cbChangePass_stateChanged(int state);
    void on_buttonBox_accepted();

private:
    Ui::UserEditDialog *ui;
    QDataWidgetMapper* mapper;
};

#endif // DLGUSEREDITOR_H
