#pragma once
#pragma GCC diagnostic ignored "-Wunknown-pragmas"

#include <fc/thread/thread.hpp>

#include <boost/signals2.hpp>

#include <QObject>

namespace fc { namespace http {
class websocket_client;
}}

class ChainDataModel;
class OperationBuilder;
class GrapheneApplication : public QObject {
   Q_OBJECT

   Q_PROPERTY(ChainDataModel* model READ model CONSTANT)
   Q_PROPERTY(OperationBuilder* operationBuilder READ operationBuilder CONSTANT)
   Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)


   fc::thread m_thread;
   ChainDataModel* m_model = nullptr;
   OperationBuilder* m_operationBuilder = nullptr;
   bool m_isConnected = false;

   boost::signals2::scoped_connection m_connectionClosed;

   std::shared_ptr<fc::http::websocket_client> m_client;
   fc::future<void> m_done;

   void setIsConnected(bool v);

protected Q_SLOTS:
   void execute(const std::function<void()>&)const;

public:
   GrapheneApplication(QObject* parent = nullptr);
   ~GrapheneApplication();

   ChainDataModel* model() const {
      return m_model;
   }
   OperationBuilder* operationBuilder() const {
      return m_operationBuilder;
   }

   Q_INVOKABLE void start(QString apiUrl,
                          QString user,
                          QString pass);

   bool isConnected() const
   {
      return m_isConnected;
   }

Q_SIGNALS:
   void exceptionThrown(QString message);
   void loginFailed();
   void isConnectedChanged(bool isConnected);
   void queueExecute(const std::function<void()>&);
};

