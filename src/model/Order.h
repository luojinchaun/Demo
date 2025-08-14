#pragma once

#include <QDateTime>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>

struct OrderItem {
	int productId{0};
	QString productName;
	double unitPrice{0.0};
	int quantity{0};

	QJsonObject toJson() const {
		QJsonObject obj;
		obj["productId"] = productId;
		obj["productName"] = productName;
		obj["unitPrice"] = unitPrice;
		obj["quantity"] = quantity;
		return obj;
	}

	static OrderItem fromJson(const QJsonObject &obj) {
		OrderItem it;
		it.productId = obj.value("productId").toInt();
		it.productName = obj.value("productName").toString();
		it.unitPrice = obj.value("unitPrice").toDouble();
		it.quantity = obj.value("quantity").toInt();
		return it;
	}
};

struct Order {
	int id{0};
	QDateTime createdAt{QDateTime::currentDateTime()};
	QVector<OrderItem> items;
	double totalAmount{0.0};

	QJsonObject toJson() const {
		QJsonObject obj;
		obj["id"] = id;
		obj["createdAt"] = createdAt.toString(Qt::ISODate);
		obj["totalAmount"] = totalAmount;
		QJsonArray arr;
		for (const auto &it : items) arr.push_back(it.toJson());
		obj["items"] = arr;
		return obj;
	}

	static Order fromJson(const QJsonObject &obj) {
		Order o;
		o.id = obj.value("id").toInt();
		o.createdAt = QDateTime::fromString(obj.value("createdAt").toString(), Qt::ISODate);
		o.totalAmount = obj.value("totalAmount").toDouble();
		const auto arr = obj.value("items").toArray();
		for (const auto &v : arr) {
			o.items.push_back(OrderItem::fromJson(v.toObject()));
		}
		return o;
	}
};