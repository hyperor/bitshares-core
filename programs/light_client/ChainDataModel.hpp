#pragma once
#pragma GCC diagnostic ignored "-Wunknown-pragmas"

#include <fc/network/http/websocket.hpp>
#include <graphene/app/api.hpp>

#include "BoostMultiIndex.hpp"
#include "Asset.hpp"
#include "Account.hpp"

#include <QObject>

using graphene::chain::by_id;

namespace fc {
class thread;
}

struct by_symbol_name;
typedef multi_index_container<
   Asset*,
   indexed_by<
      hashed_unique< tag<by_id>, const_mem_fun<GrapheneObject, ObjectId, &GrapheneObject::id > >,
      ordered_unique< tag<by_symbol_name>, const_mem_fun<Asset, QString, &Asset::symbol> >
   >
> asset_multi_index_type;

struct by_account_name;
typedef multi_index_container<
   Account*,
   indexed_by<
      hashed_unique< tag<by_id>, const_mem_fun<GrapheneObject, ObjectId, &GrapheneObject::id > >,
      ordered_unique< tag<by_account_name>, const_mem_fun<Account, QString, &Account::name> >
   >
> account_multi_index_type;

class ChainDataModel : public QObject {
   Q_OBJECT

   void processUpdatedObject(const fc::variant& update);

   void getAssetImpl(QString assetIdentifier, Asset* const * assetInContainer);
   void getAccountImpl(QString accountIdentifier, Account* const * accountInContainer);

public:
   Q_INVOKABLE Account* getAccount(ObjectId id);
   Q_INVOKABLE Account* getAccount(QString name);
   Q_INVOKABLE Asset* getAsset(ObjectId id);
   Q_INVOKABLE Asset* getAsset(QString symbol);

   ChainDataModel(){}
   ChainDataModel(fc::thread& t, QObject* parent = nullptr);

   void setDatabaseAPI(fc::api<graphene::app::database_api> dbapi);

   const graphene::chain::global_property_object& global_properties() const { return m_global_properties; }

Q_SIGNALS:
   void queueExecute(const std::function<void()>&);
   void exceptionThrown(QString message);

private:
   fc::thread* m_rpc_thread = nullptr;
   std::string m_api_url;
   fc::api<graphene::app::database_api> m_db_api;

   graphene::chain::global_property_object m_global_properties;

   ObjectId m_account_query_num = -1;
   account_multi_index_type m_accounts;
   asset_multi_index_type m_assets;
};

