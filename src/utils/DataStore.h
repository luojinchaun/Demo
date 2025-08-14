#pragma once

#include <QVector>
#include <QString>
#include "model/Product.h"
#include "model/Order.h"
#include "model/User.h"

namespace DataStore {
	bool loadAll(QVector<Product> &products, QVector<Order> &orders, User &user);
	bool saveAll(const QVector<Product> &products, const QVector<Order> &orders, const User &user);
	QString dataFilePath();
}