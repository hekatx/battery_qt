#include "mainwindow.h"
#include "QKeyEvent"
#include "battery.h"
#include "QObject"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Battery");
  setFocusPolicy(Qt::StrongFocus); // Enabled the QMainWindow to get focus

  Battery *bat = new Battery();
  setCentralWidget(bat);
  QObject::connect(this, &MainWindow::decrease, bat, &Battery::setValue);
}

MainWindow::~MainWindow() {}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if(event->key() == Qt::Key_Up) {
    emit decrease(1.0);
  }
  if(event->key() == Qt::Key_Down) {
    emit decrease(-1.0);
  }
}
