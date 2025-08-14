#pragma once

#include <QWidget>
#include <QVector>
#include "model/Order.h"

class QTableWidget;

class OrdersPage : public QWidget {
	Q_OBJECT
public:
	explicit OrdersPage(QVector<Order> &ordersRef, QWidget *parent = nullptr);

public slots:
	void reload();

private:
	void setupUi();

private:
	QVector<Order> &m_orders;
	QTableWidget *m_table{nullptr};
};