#pragma once

#include <QString>
#include <QJsonObject>

struct Product {
	int id{0};
	QString name;
	QString description;
	double price{0.0};
	int stock{0};
	bool onShelf{true};

	QJsonObject toJson() const {
		QJsonObject obj;
		obj["id"] = id;
		obj["name"] = name;
		obj["description"] = description;
		obj["price"] = price;
		obj["stock"] = stock;
		obj["onShelf"] = onShelf;
		return obj;
	}

	static Product fromJson(const QJsonObject &obj) {
		Product p;
		p.id = obj.value("id").toInt();
		p.name = obj.value("name").toString();
		p.description = obj.value("description").toString();
		p.price = obj.value("price").toDouble();
		p.stock = obj.value("stock").toInt();
		p.onShelf = obj.value("onShelf").toBool(true);
		return p;
	}
};