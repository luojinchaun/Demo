#include "MainWindow.h"

#include <QTabWidget>
#include <QCloseEvent>
#include <QMessageBox>

#include "pages/ProductPage.h"
#include "pages/CartPage.h"
#include "pages/OrdersPage.h"
#include "pages/AccountPage.h"
#include "utils/DataStore.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	setupUi();
	loadData();
	wireSignals();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi() {
	m_tabWidget = new QTabWidget(this);
	setCentralWidget(m_tabWidget);

	m_productPage = new ProductPage(m_products, this);
	m_cartPage = new CartPage(m_products, m_orders, m_user, this);
	m_ordersPage = new OrdersPage(m_orders, this);
	m_accountPage = new AccountPage(m_user, this);

	m_tabWidget->addTab(m_productPage, tr("商品管理"));
	m_tabWidget->addTab(m_cartPage, tr("购物车"));
	m_tabWidget->addTab(m_ordersPage, tr("订单"));
	m_tabWidget->addTab(m_accountPage, tr("个人中心"));
}

void MainWindow::wireSignals() {
	connect(m_productPage, &ProductPage::requestAddToCart,
			m_cartPage, &CartPage::addToCart);

	connect(m_cartPage, &CartPage::productsChanged,
			m_productPage, &ProductPage::reload);

	connect(m_cartPage, &CartPage::checkoutCompleted, m_ordersPage, &OrdersPage::reload);
}

void MainWindow::closeEvent(QCloseEvent *event) {
	try {
		saveData();
		event->accept();
	} catch (...) {
		QMessageBox::warning(this, tr("保存失败"), tr("保存数据失败，可能导致数据丢失。"));
		event->accept();
	}
}

void MainWindow::loadData() {
	if (!DataStore::loadAll(m_products, m_orders, m_user)) {
		// Initialize default user if not present
		if (m_user.username.isEmpty()) {
			m_user.username = tr("游客");
			m_user.email = "";
			m_user.balance = 0.0;
		}
	}

	m_productPage->reload();
	m_ordersPage->reload();
	m_accountPage->reload();
}

void MainWindow::saveData() {
	if (!DataStore::saveAll(m_products, m_orders, m_user)) {
		throw std::runtime_error("Failed to save data.json");
	}
}