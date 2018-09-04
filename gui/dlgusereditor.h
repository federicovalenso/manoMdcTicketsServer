#ifndef DLGUSEREDITOR_H
#define DLGUSEREDITOR_H

#include <QDialog>
#include <QAbstractItemModel>
#include <QDataWidgetMapper>


namespace Ui {
class dlgUserEditor;
}

class dlgUserEditor : public QDialog
{
    Q_OBJECT

public:
    explicit dlgUserEditor(QWidget *parent = 0);
    ~dlgUserEditor();

    void setModel(QAbstractItemModel*);
    void setCurrentModelIndex(const QModelIndex&);

public slots:
    virtual void done(int) override;

private slots:
    void on_cbChangePass_stateChanged(int state);

private:
    Ui::dlgUserEditor *ui;
    QDataWidgetMapper* mapper;
};

#endif // DLGUSEREDITOR_H
