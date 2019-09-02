#include "ticketcounter.h"
#include <QCoreApplication>
#include <QSettings>
#include "serversettings.h"

const QByteArray TicketCounter::kClassName = "TicketCounter";
const QString TicketCounter::kGroup = "counter";
const QString TicketCounter::kCounter = "current_count";
const QString TicketCounter::kPrevCount = "prev_count";
const QString TicketCounter::kLastTimeClear = "last_clear_time";

TicketCounter &TicketCounter::instance() {
  return *qApp->findChild<TicketCounter *>();
}

TicketCounter::TicketCounter(QCoreApplication *parent) : QObject(parent) {
  auto settings_file = parent->property(Settings::kSettingsFile).toString();
  QSettings counter_settings(settings_file, QSettings::IniFormat);
  counter_settings.beginGroup(kGroup);
  counter_ = counter_settings.value(kCounter, 0).toUInt();
  prev_count_ = counter_settings.value(kPrevCount, 0).toUInt();
  last_clear_time_ = counter_settings.value(kLastTimeClear).toDateTime();
}

TicketCounter::~TicketCounter() {
  auto settings_file = parent()->property(Settings::kSettingsFile).toString();
  QSettings counter_settings(settings_file, QSettings::IniFormat);
  counter_settings.beginGroup(kGroup);
  counter_settings.setValue(kCounter, counter_.load());
  counter_settings.setValue(kPrevCount, prev_count_);
  counter_settings.setValue(kLastTimeClear, last_clear_time_);
}

TicketCounter &TicketCounter::operator++() {
  ++counter_;
  return *this;
}

void TicketCounter::clear() {
  prev_count_ = counter_;
  counter_ = 0;
  last_clear_time_ = QDateTime::currentDateTime();
}
