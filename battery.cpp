#include "battery.h"
#include "QDebug"
#include "QPainter"
#include "QTimer"

Battery::Battery(QWidget *parent) : QWidget{parent} {
    minValue = 0;
    maxValue = 100;
    value = 0;
    alarmValue = 30;
    step = 5.0;
  timer = new QTimer(this);
  timer->setInterval(10);
  connect(timer, SIGNAL(timeout()), this, SLOT(updateValue()));
}

Battery::~Battery() {
  if (timer->isActive()) {
    timer->stop();
  }
}

void Battery::paintEvent(QPaintEvent *) {
  // Draw preparations, enable anti-aliasing
  QPainter painter(this);
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

  // Draw border
  drawBorder(&painter);
  // Draw background
  drawBg(&painter);
  // Draw head
  drawHead(&painter);
}

void Battery::drawBorder(QPainter *painter) {
  painter->save();

  int borderWidth = 5;
  int borderRadius = 8;
  int bgRadius = 5;
  int headRadius = 3;

  QColor borderColorStart = QColor(100, 100, 100);

  double headWidth = width() / 15;
  double batteryWidth = width() - headWidth;

  // Draw the battery border
  QPointF topLeft(borderWidth, borderWidth);
  QPointF bottomRight(batteryWidth, height() - borderWidth);
  batteryRect = QRectF(topLeft, bottomRight);

  painter->setPen(QPen(borderColorStart, borderWidth));
  painter->setBrush(Qt::NoBrush);
  painter->drawRoundedRect(batteryRect, borderRadius, borderRadius);

  painter->restore();
}

void Battery::drawBg(QPainter *painter) {
  if (battery_component.getCurrentPercentage() == minValue) {
    return;
  }

  painter->save();
  QColor alarmColorStart = QColor(250, 118, 113);
  QColor alarmColorEnd = QColor(204, 38, 38);
  QColor normalColorStart = QColor(50, 205, 51);
  QColor normalColorEnd = QColor(60, 179, 133);
  int borderWidth = 5;
  int bgRadius = 5;

  QLinearGradient batteryGradient(QPointF(0, 0), QPointF(0, height()));
  if (currentValue <= alarmValue) {
    batteryGradient.setColorAt(0.0, alarmColorStart);
    batteryGradient.setColorAt(1.0, alarmColorEnd);
  } else {
    batteryGradient.setColorAt(0.0, normalColorStart);
    batteryGradient.setColorAt(1.0, normalColorEnd);
  }

  int margin = qMin(width(), height()) / 20;
  double unit = (batteryRect.width() - (margin * 2)) / 100;
  double width = currentValue * unit;
  QPointF topLeft(batteryRect.topLeft().x() + margin,
                  batteryRect.topLeft().y() + margin);
  QPointF bottomRight(width + margin + borderWidth,
                      batteryRect.bottomRight().y() - margin);
  QRectF rect(topLeft, bottomRight);

  painter->setPen(Qt::NoPen);
  painter->setBrush(batteryGradient);
  painter->drawRoundedRect(rect, bgRadius, bgRadius);

  painter->restore();
}

void Battery::drawHead(QPainter *painter) {
  painter->save();

  QColor borderColorStart = QColor(100, 100, 100);
  QColor borderColorEnd = QColor(80, 80, 80);
  int headRadius = 3;

  QPointF headRectTopLeft(batteryRect.topRight().x(), height() / 3);
  QPointF headRectBottomRight(width(), height() - height() / 3);
  QRectF headRect(headRectTopLeft, headRectBottomRight);

  QLinearGradient headRectGradient(headRect.topLeft(), headRect.bottomLeft());
  headRectGradient.setColorAt(0.0, borderColorStart);
  headRectGradient.setColorAt(1.0, borderColorEnd);

  painter->setPen(Qt::NoPen);
  painter->setBrush(headRectGradient);
  painter->drawRoundedRect(headRect, headRadius, headRadius);

  painter->restore();
}

void Battery::updateValue() {
  if (isForward) {
    currentValue -= step;
    if (currentValue <= value) {
      timer->stop();
    }
  } else {
    currentValue += step;
    if (currentValue >= value) {
      timer->stop();
    }
  }

  this->update();
}

void Battery::setValue(float change) {
  float value = this->value + (step*change);
  // If the value is consistent with the current value, there is no need to deal
  // with it
  if (value == this->value) {
    return;
  }

  // If the value is less than the minimum value, take the minimum value, and if
  // the value is greater than the maximum value, take the maximum value
  if (value < minValue) {
    value = minValue;
  } else if (value > maxValue) {
    value = maxValue;
  }

  if (value > currentValue) {
    isForward = false;
  } else if (value < currentValue) {
    isForward = true;
  } else {
    this->value = value;
    this->update();
    return;
  }

  this->value = value;
  this->update();
  emit valueChanged(value);
  timer->stop();
  timer->start();
}
