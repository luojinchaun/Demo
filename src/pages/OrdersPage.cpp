#include "OrdersPage.h"

#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>

OrdersPage::OrdersPage(QVector<Order> &ordersRef, QWidget *parent)
	: QWidget(parent), m_orders(ordersRef) {
	setupUi();
	reload();
}

void OrdersPage::setupUi() {
	auto *layout = new QVBoxLayout(this);
	m_table = new QTableWidget(this);
	m_table->setColumnCount(4);
	QStringList headers{tr("订单ID"), tr("时间"), tr("条目数"), tr("总额")};
	m_table->setHorizontalHeaderLabels(headers);
	m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	layout->addWidget(m_table);
}

void OrdersPage::reload() {
	m_table->setRowCount(m_orders.size());
	for (int row = 0; row < m_orders.size(); ++row) {
		const Order &o = m_orders[row];
		m_table->setItem(row, 0, new QTableWidgetItem(QString::number(o.id)));
		m_table->setItem(row, 1, new QTableWidgetItem(o.createdAt.toString("yyyy-MM-dd hh:mm:ss")));
		m_table->setItem(row, 2, new QTableWidgetItem(QString::number(o.items.size())));
		m_table->setItem(row, 3, new QTableWidgetItem(QString::number(o.totalAmount, 'f', 2)));
	}
}