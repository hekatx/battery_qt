#pragma once

class BatteryBase {
public:
  BatteryBase()
      : m_charge_percentage{ 100.0 }, m_step{ 1.0 } {};

  float getCurrentPercentage() { return m_charge_percentage; };

  float increaseCharge() {
    m_charge_percentage += m_step;
    return m_charge_percentage;
  };

  float decreaseCharge() {
    m_charge_percentage -= m_step;
    return m_charge_percentage;
  };

private:
  float m_charge_percentage;
  float m_step;
};
