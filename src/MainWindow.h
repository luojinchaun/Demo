#pragma once

#include <QMainWindow>
#include <QVector>
#include "model/Product.h"
#include "model/Order.h"
#include "model/User.h"

class QTabWidget;
class ProductPage;
class CartPage;
class OrdersPage;
class AccountPage;

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow() override;

protected:
	void closeEvent(QCloseEvent *event) override;

private:
	void setupUi();
	void wireSignals();
	void loadData();
	void saveData();

private:
	QTabWidget *m_tabWidget{nullptr};

	// Shared application state
	QVector<Product> m_products;
	QVector<Order> m_orders;
	User m_user;

	// Pages
	ProductPage *m_productPage{nullptr};
	CartPage *m_cartPage{nullptr};
	OrdersPage *m_ordersPage{nullptr};
	AccountPage *m_accountPage{nullptr};
};