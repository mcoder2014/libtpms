#ifndef RANGEPROJECTDIALOG_H
#define RANGEPROJECTDIALOG_H

#include <QDialog>

#include "ui_rangeprojectdialog.h"

class RangeProjectDialog : public QDialog, public Ui::RangeProjectDialog
{
    Q_OBJECT

public:
    explicit RangeProjectDialog(QWidget *parent = nullptr);
    ~RangeProjectDialog();
};

#endif // RANGEPROJECTDIALOG_H
