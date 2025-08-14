#pragma once

#include <QWidget>
#include <QVector>
#include <QMap>
#include "model/Product.h"
#include "model/Order.h"
#include "model/User.h"

class QTableWidget;
class QPushButton;

class CartPage : public QWidget {
	Q_OBJECT
public:
	explicit CartPage(QVector<Product> &productsRef, QVector<Order> &ordersRef, User &userRef, QWidget *parent = nullptr);

signals:
	void productsChanged();
	void checkoutCompleted();

public slots:
	void addToCart(int productId, int quantity);

private slots:
	void onRemove();
	void onClear();
	void onCheckout();

private:
	void setupUi();
	void reload();
	double cartTotal() const;

private:
	QVector<Product> &m_products;
	QVector<Order> &m_orders;
	User &m_user;
	QMap<int, int> m_cart; // productId -> quantity
	QTableWidget *m_table{nullptr};
	QPushButton *m_btnRemove{nullptr};
	QPushButton *m_btnClear{nullptr};
	QPushButton *m_btnCheckout{nullptr};
};