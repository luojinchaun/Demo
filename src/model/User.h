#pragma once

#include <QString>
#include <QJsonObject>

struct User {
	QString username;
	QString email;
	double balance{0.0};

	QJsonObject toJson() const {
		QJsonObject obj;
		obj["username"] = username;
		obj["email"] = email;
		obj["balance"] = balance;
		return obj;
	}

	static User fromJson(const QJsonObject &obj) {
		User u;
		u.username = obj.value("username").toString();
		u.email = obj.value("email").toString();
		u.balance = obj.value("balance").toDouble();
		return u;
	}
};