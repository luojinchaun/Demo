#include "DataStore.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <QDir>

namespace DataStore {

static QString ensureDataDir() {
	QString base = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	if (base.isEmpty()) {
		base = QDir::homePath() + "/.shop_mall_qt";
	}
	QDir().mkpath(base);
	return base;
}

QString dataFilePath() {
	return ensureDataDir() + "/data.json";
}

bool loadAll(QVector<Product> &products, QVector<Order> &orders, User &user) {
	products.clear();
	orders.clear();

	QFile f(dataFilePath());
	if (!f.exists()) return false;
	if (!f.open(QIODevice::ReadOnly)) return false;
	const auto doc = QJsonDocument::fromJson(f.readAll());
	if (!doc.isObject()) return false;
	const auto root = doc.object();

	const auto prodArr = root.value("products").toArray();
	for (const auto &v : prodArr) products.push_back(Product::fromJson(v.toObject()));

	const auto orderArr = root.value("orders").toArray();
	for (const auto &v : orderArr) orders.push_back(Order::fromJson(v.toObject()));

	user = User::fromJson(root.value("user").toObject());
	return true;
}

bool saveAll(const QVector<Product> &products, const QVector<Order> &orders, const User &user) {
	QJsonObject root;
	QJsonArray prodArr;
	for (const auto &p : products) prodArr.push_back(p.toJson());
	root["products"] = prodArr;

	QJsonArray orderArr;
	for (const auto &o : orders) orderArr.push_back(o.toJson());
	root["orders"] = orderArr;

	root["user"] = user.toJson();

	QFile f(dataFilePath());
	if (!f.open(QIODevice::WriteOnly)) return false;
	QJsonDocument doc(root);
	f.write(doc.toJson(QJsonDocument::Indented));
	return true;
}

}