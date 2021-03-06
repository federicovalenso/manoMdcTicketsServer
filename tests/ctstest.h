#ifndef CTSTEST_H
#define CTSTEST_H

#include <QNetworkAccessManager>
#include <QTest>
#include <QThread>
#include <array>

class CtsTest : public QObject {
  Q_OBJECT
 public:
  explicit CtsTest(QObject* app = nullptr);

 signals:
  void TestsFinished();

 private slots:
  void init();
  void TestAuthorization();
  void TestTicketAdd();
  void TestTicketUpdate();
  void TestStatistics();

 private:
  const QString kLocalHost = "localhost";
  int port_;
  QNetworkAccessManager* network_access_manager_;
  void processRequest(std::function<void(QNetworkReply* reply)>&& func,
                      QEventLoop& loop);
};

#endif  // CTSTEST_H
