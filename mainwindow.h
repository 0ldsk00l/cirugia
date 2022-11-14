#pragma once

#include <iostream>
#include <QMainWindow>

#include "nesrom.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void open(std::string filename);

private slots:
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionQuit_triggered();
    void on_actionAbout_triggered();
    void on_radioButtonINES_toggled(bool checked);
    void on_radioButtonNES2_toggled(bool checked);
    void on_spinMapper_valueChanged(int arg1);
    void on_spinSubmapper_valueChanged(int arg1);
    void on_checkBattery_toggled(bool checked);
    void on_checkTrainer_toggled(bool checked);
    void on_spinPRGROM_valueChanged(int arg1);
    void on_spinPRGRAM_valueChanged(int arg1);
    void on_spinPRGNVRAM_valueChanged(int arg1);
    void on_spinCHRROM_valueChanged(int arg1);
    void on_spinCHRRAM_valueChanged(int arg1);
    void on_spinCHRNVRAM_valueChanged(int arg1);
    void on_radioButtonH_toggled(bool checked);
    void on_radioButtonV_toggled(bool checked);
    void on_radioButton4_toggled(bool checked);
    void on_radioButtonConsole_toggled(bool checked);
    void on_radioButtonVS_toggled(bool checked);
    void on_radioButtonPC10_toggled(bool checked);
    void on_radioButtonExtsys_toggled(bool checked);
    void on_radioButtonNTSC_toggled(bool checked);
    void on_radioButtonPAL_toggled(bool checked);
    void on_radioButtonMulti_toggled(bool checked);
    void on_radioButtonDendy_toggled(bool checked);
    void on_comboBoxVSPPU_currentIndexChanged(int index);
    void on_comboBoxVSHardware_currentIndexChanged(int index);
    void on_comboBoxExtsys_currentIndexChanged(int index);
    void on_comboBoxExpdev_currentIndexChanged(int index);
    void on_spinMiscROMs_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    nesrom *rom;

    void activate_fields();
    void deactivate_fields();
    void populate_fields();
};
