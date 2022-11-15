#include <QFileDialog>

#include "mainwindow.h"
#include "about.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->actionSave->setDisabled(true);
    ui->actionSave_As->setDisabled(true);
    ui->radioButtonINES->setDisabled(true);
    ui->radioButtonNES2->setDisabled(true);
    ui->spinMapper->setDisabled(true);
    ui->spinSubmapper->setDisabled(true);
    ui->checkBattery->setDisabled(true);
    ui->checkTrainer->setDisabled(true);
    ui->spinPRGROM->setDisabled(true);
    ui->spinPRGRAM->setDisabled(true);
    ui->spinPRGNVRAM->setDisabled(true);
    ui->spinCHRROM->setDisabled(true);
    ui->spinCHRRAM->setDisabled(true);
    ui->spinCHRNVRAM->setDisabled(true);
    ui->radioButtonH->setDisabled(true);
    ui->radioButtonV->setDisabled(true);
    ui->radioButton4->setDisabled(true);
    ui->radioButtonConsole->setDisabled(true);
    ui->radioButtonVS->setDisabled(true);
    ui->radioButtonPC10->setDisabled(true);
    ui->radioButtonExtsys->setDisabled(true);
    ui->radioButtonNTSC->setDisabled(true);
    ui->radioButtonPAL->setDisabled(true);
    ui->radioButtonMulti->setDisabled(true);
    ui->radioButtonDendy->setDisabled(true);
    ui->comboBoxVSPPU->setDisabled(true);
    ui->comboBoxVSHardware->setDisabled(true);
    ui->comboBoxExtsys->setDisabled(true);
    ui->comboBoxExpdev->setDisabled(true);
    ui->spinMiscROMs->setDisabled(true);
}


MainWindow::~MainWindow()
{
    if (rom) {
        delete rom;
    }
    delete ui;
}


void MainWindow::open(std::string filename) {
    rom = new nesrom(filename);

    if (!rom->load()) {
        delete rom;
    }

    deactivate_fields();
    activate_fields();
    populate_fields();
}


void MainWindow::activate_fields() {
    ui->actionSave->setEnabled(true);
    ui->actionSave_As->setEnabled(true);
    ui->radioButtonINES->setEnabled(true);
    ui->radioButtonNES2->setEnabled(true);
    ui->spinMapper->setEnabled(true);
    ui->checkBattery->setEnabled(true);
    ui->checkTrainer->setEnabled(true);
    ui->spinPRGROM->setEnabled(true);
    ui->spinCHRROM->setEnabled(true);
    ui->radioButtonH->setEnabled(true);
    ui->radioButtonV->setEnabled(true);
    ui->radioButton4->setEnabled(true);
    ui->radioButtonConsole->setEnabled(true);
    ui->radioButtonVS->setEnabled(true);
    ui->radioButtonPC10->setEnabled(true);
    ui->radioButtonNTSC->setEnabled(true);
    ui->radioButtonPAL->setEnabled(true);

    if (rom->get_header().get_version() == 2) {
        ui->spinSubmapper->setEnabled(true);
        ui->spinPRGRAM->setEnabled(true);
        ui->spinPRGNVRAM->setEnabled(true);
        ui->spinCHRRAM->setEnabled(true);
        ui->spinCHRNVRAM->setEnabled(true);
        ui->radioButtonExtsys->setEnabled(true);
        ui->radioButtonMulti->setEnabled(true);
        ui->radioButtonDendy->setEnabled(true);
        ui->comboBoxVSPPU->setEnabled(true);
        ui->comboBoxVSHardware->setEnabled(true);
        ui->comboBoxExtsys->setEnabled(true);
        ui->comboBoxExpdev->setEnabled(true);
        ui->spinMiscROMs->setEnabled(true);
    }
}


void MainWindow::deactivate_fields() {
    ui->radioButtonINES->setDisabled(true);
    ui->radioButtonNES2->setDisabled(true);
    ui->spinMapper->setDisabled(true);
    ui->spinSubmapper->setDisabled(true);
    ui->checkTrainer->setDisabled(true);
    ui->spinPRGROM->setDisabled(true);
    ui->spinPRGRAM->setDisabled(true);
    ui->spinPRGNVRAM->setDisabled(true);
    ui->spinCHRROM->setDisabled(true);
    ui->spinCHRRAM->setDisabled(true);
    ui->spinCHRNVRAM->setDisabled(true);
    ui->radioButtonH->setDisabled(true);
    ui->radioButtonV->setDisabled(true);
    ui->radioButton4->setDisabled(true);
    ui->radioButtonConsole->setDisabled(true);
    ui->radioButtonVS->setDisabled(true);
    ui->radioButtonPC10->setDisabled(true);
    ui->radioButtonExtsys->setDisabled(true);
    ui->radioButtonNTSC->setDisabled(true);
    ui->radioButtonPAL->setDisabled(true);
    ui->radioButtonMulti->setDisabled(true);
    ui->radioButtonDendy->setDisabled(true);
    ui->comboBoxVSPPU->setDisabled(true);
    ui->comboBoxVSHardware->setDisabled(true);
    ui->comboBoxExtsys->setDisabled(true);
    ui->comboBoxExpdev->setDisabled(true);
    ui->spinMiscROMs->setDisabled(true);
}


void MainWindow::populate_fields() {
    int version = rom->get_header().get_version();

    if (version == 2) {
        ui->radioButtonNES2->toggle();
    }
    else {
        ui->radioButtonINES->toggle();
    }

    // Mapper/Submapper
    ui->spinMapper->setValue(rom->get_header().get_mapper());
    ui->spinSubmapper->setValue(rom->get_header().get_submapper());

    // Flags
    ui->checkBattery->setCheckState(rom->get_header().get_battery() ? Qt::Checked : Qt::Unchecked);
    ui->checkTrainer->setCheckState(rom->get_header().get_trainer() ? Qt::Checked : Qt::Unchecked);

    // PRG
    ui->spinPRGROM->setValue(rom->get_header().get_prgsize());
    ui->spinPRGRAM->setValue(rom->get_header().get_prgramsize());
    ui->spinPRGNVRAM->setValue(rom->get_header().get_prgnvramsize());

    // CHR
    ui->spinCHRROM->setValue(rom->get_header().get_chrsize());
    ui->spinCHRRAM->setValue(rom->get_header().get_chrramsize());
    ui->spinCHRNVRAM->setValue(rom->get_header().get_chrnvramsize());

    // Mirroring
    switch (rom->get_header().get_mirroring()) {
        case 0: ui->radioButtonH->toggle(); break;
        case 1: ui->radioButtonV->toggle(); break;
        case 2: ui->radioButton4->toggle(); break;
    }

    // System Type
    switch (rom->get_header().get_systype()) {
        case 0: ui->radioButtonConsole->toggle(); break;
        case 1: ui->radioButtonVS->toggle(); break;
        case 2: ui->radioButtonPC10->toggle(); break;
        case 3: ui->radioButtonExtsys->toggle(); break;
    }

    // CPU/PPU Timing
    if (version == 2) {
        switch (rom->get_header().get_timing()) {
            case 0: ui->radioButtonNTSC->toggle(); break;
            case 1: ui->radioButtonPAL->toggle(); break;
            case 2: ui->radioButtonMulti->toggle(); break;
            case 3: ui->radioButtonDendy->toggle(); break;
        }
    }
    else {
        switch (rom->get_header().get_tvsystem()) {
            case 0: ui->radioButtonNTSC->toggle(); break;
            case 1: ui->radioButtonPAL->toggle(); break;
        }
    }

    // VS PPU/Hardware
    ui->comboBoxVSPPU->setCurrentIndex(rom->get_header().get_vsppu());
    ui->comboBoxVSHardware->setCurrentIndex(rom->get_header().get_vshardware());

    // Extended System
    ui->comboBoxExtsys->setCurrentIndex(rom->get_header().get_extsys());

    // Expansion Device
    ui->comboBoxExpdev->setCurrentIndex(rom->get_header().get_expdev());

    // Misc ROMs
    ui->spinMiscROMs->setValue(rom->get_header().get_miscroms());

    // Checksums
    ui->lineEditSHA1->setText(QString::fromStdString(rom->get_sha1()));
    ui->lineEditCRC->setText(QString::number(rom->get_crc(), 16).toUpper());
    ui->lineEditCRCPRG->setText(QString::number(rom->get_prgcrc(), 16).toUpper());
    if (rom->get_chr().size()) {
        ui->lineEditCRCCHR->setText(QString::number(rom->get_chrcrc(), 16).toUpper());
    }
    else {
        ui->lineEditCRCCHR->setText("");
    }
}


void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open a ROM");

    if (filename.isEmpty()) {
        return;
    }

    if (rom == nullptr) {
        delete rom;
    }

    open(filename.toStdString());
}


void MainWindow::on_actionSave_triggered()
{
    rom->save();
}


void MainWindow::on_actionSave_As_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save NES ROM...");

    if (filename.isEmpty()) {
        return;
    }

    rom->save(filename.toStdString());
}


void MainWindow::on_actionQuit_triggered()
{
    this->close();
}


void MainWindow::on_actionAbout_triggered()
{
    About aboutwin;
    aboutwin.setModal(true);
    aboutwin.exec();
}


void MainWindow::on_radioButtonINES_toggled(bool checked)
{
    if (checked) {
        deactivate_fields();
        rom->get_header().set_version(1);
        activate_fields();
    }
}


void MainWindow::on_radioButtonNES2_toggled(bool checked)
{
    if (checked) {
        rom->get_header().set_version(2);
        activate_fields();
    }
}


void MainWindow::on_spinMapper_valueChanged(int arg1)
{
    rom->get_header().set_mapper(arg1);
}


void MainWindow::on_spinSubmapper_valueChanged(int arg1)
{
    rom->get_header().set_submapper(arg1);
}


void MainWindow::on_checkBattery_toggled(bool checked)
{
    rom->get_header().set_battery(checked);
}


void MainWindow::on_checkTrainer_toggled(bool checked)
{
    rom->get_header().set_trainer(checked);
}


void MainWindow::on_spinPRGROM_valueChanged(int arg1)
{
    rom->get_header().set_prgsize(arg1);
}


void MainWindow::on_spinPRGRAM_valueChanged(int arg1)
{
    rom->get_header().set_prgramsize(arg1);
}


void MainWindow::on_spinPRGNVRAM_valueChanged(int arg1)
{
    rom->get_header().set_prgnvramsize(arg1);
}


void MainWindow::on_spinCHRROM_valueChanged(int arg1)
{
    rom->get_header().set_chrsize(arg1);
}


void MainWindow::on_spinCHRRAM_valueChanged(int arg1)
{
    rom->get_header().set_chrramsize(arg1);
}


void MainWindow::on_spinCHRNVRAM_valueChanged(int arg1)
{
    rom->get_header().set_chrnvramsize(arg1);
}


void MainWindow::on_radioButtonH_toggled(bool checked)
{
    if (checked) {
        rom->get_header().set_mirroring(0);
    }
}

void MainWindow::on_radioButtonV_toggled(bool checked)
{
    if (checked) {
        rom->get_header().set_mirroring(1);
    }
}


void MainWindow::on_radioButton4_toggled(bool checked)
{
    if (checked) {
        rom->get_header().set_mirroring(2);
    }
}


void MainWindow::on_radioButtonConsole_toggled(bool checked)
{
    if (checked) {
        rom->get_header().set_systype(0);
    }
}


void MainWindow::on_radioButtonVS_toggled(bool checked)
{
    if (checked) {
        rom->get_header().set_systype(1);
    }
}


void MainWindow::on_radioButtonPC10_toggled(bool checked)
{
    if (checked) {
        rom->get_header().set_systype(2);
    }
}


void MainWindow::on_radioButtonExtsys_toggled(bool checked)
{
    if (checked) {
        rom->get_header().set_systype(3);
    }
}


void MainWindow::on_radioButtonNTSC_toggled(bool checked)
{
    if (checked) {
        rom->get_header().set_timing(0);
        rom->get_header().set_tvsystem(0);
    }
}


void MainWindow::on_radioButtonPAL_toggled(bool checked)
{
    if (checked) {
        rom->get_header().set_timing(1);
        rom->get_header().set_tvsystem(1);
    }
}


void MainWindow::on_radioButtonMulti_toggled(bool checked)
{
    if (checked) {
        rom->get_header().set_timing(2);
    }
}


void MainWindow::on_radioButtonDendy_toggled(bool checked)
{
    if (checked) {
        rom->get_header().set_timing(3);
    }
}


void MainWindow::on_comboBoxVSPPU_currentIndexChanged(int index)
{
    rom->get_header().set_vsppu(index);
}


void MainWindow::on_comboBoxVSHardware_currentIndexChanged(int index)
{
    rom->get_header().set_vshardware(index);
}


void MainWindow::on_comboBoxExtsys_currentIndexChanged(int index)
{
    rom->get_header().set_extsys(index);
}


void MainWindow::on_comboBoxExpdev_currentIndexChanged(int index)
{
    rom->get_header().set_expdev(index);
}


void MainWindow::on_spinMiscROMs_valueChanged(int arg1)
{
    rom->get_header().set_miscroms(arg1);
}
