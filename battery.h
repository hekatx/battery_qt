#ifndef BATTERY_H
#define BATTERY_H

#include "battery_base.h"
#include <QObject>
#include <QWidget>

class Battery : public QWidget {
  Q_OBJECT
public:
  explicit Battery(QWidget *parent = nullptr);
  ~Battery();

protected:
  void paintEvent(QPaintEvent *);
  void drawBorder(QPainter *painter);
  void drawBg(QPainter *painter);
  void drawHead(QPainter *painter);

private:
  QRectF batteryRect;
  double minValue;     // Minimum
  double maxValue;     // Maximum
  double alarmValue;   // Battery level warning value
  bool isForward;      // Whether to move forward
  double currentValue; // Current battery
  double step;
  double value;  // Target battery
  QTimer *timer; // Draw timer

  BatteryBase battery_component;

private slots:
  void updateValue();

public slots:
  // Set the battery level value
  void setValue(float value);

signals:
  void valueChanged(double value);
};

#endif // BATTERY_H
