#include "CartPage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <algorithm>

CartPage::CartPage(QVector<Product> &productsRef, QVector<Order> &ordersRef, User &userRef, QWidget *parent)
	: QWidget(parent), m_products(productsRef), m_orders(ordersRef), m_user(userRef) {
	setupUi();
	reload();
}

void CartPage::setupUi() {
	auto *layout = new QVBoxLayout(this);
	m_table = new QTableWidget(this);
	m_table->setColumnCount(5);
	QStringList headers{tr("ID"), tr("名称"), tr("单价"), tr("数量"), tr("小计")};
	m_table->setHorizontalHeaderLabels(headers);
	m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	layout->addWidget(m_table);

	auto *btnBar = new QHBoxLayout();
	m_btnRemove = new QPushButton(tr("移除"), this);
	m_btnClear = new QPushButton(tr("清空"), this);
	m_btnCheckout = new QPushButton(tr("下单结算"), this);
	btnBar->addWidget(m_btnRemove);
	btnBar->addWidget(m_btnClear);
	btnBar->addWidget(m_btnCheckout);
	btnBar->addStretch();
	layout->addLayout(btnBar);

	connect(m_btnRemove, &QPushButton::clicked, this, &CartPage::onRemove);
	connect(m_btnClear, &QPushButton::clicked, this, &CartPage::onClear);
	connect(m_btnCheckout, &QPushButton::clicked, this, &CartPage::onCheckout);
}

void CartPage::reload() {
	m_table->setRowCount(m_cart.size());
	int row = 0;
	double total = 0.0;
	for (auto it = m_cart.begin(); it != m_cart.end(); ++it, ++row) {
		int productId = it.key();
		int qty = it.value();
		const Product *p = nullptr;
		for (const auto &pr : m_products) if (pr.id == productId) { p = &pr; break; }
		if (!p) continue;
		double sub = p->price * qty;
		total += sub;
		m_table->setItem(row, 0, new QTableWidgetItem(QString::number(p->id)));
		m_table->setItem(row, 1, new QTableWidgetItem(p->name));
		m_table->setItem(row, 2, new QTableWidgetItem(QString::number(p->price, 'f', 2)));
		m_table->setItem(row, 3, new QTableWidgetItem(QString::number(qty)));
		m_table->setItem(row, 4, new QTableWidgetItem(QString::number(sub, 'f', 2)));
	}
	// Show total in window title
	setWindowTitle(tr("购物车 - 合计: %1").arg(QString::number(total, 'f', 2)));
}

double CartPage::cartTotal() const {
	double total = 0.0;
	for (auto it = m_cart.begin(); it != m_cart.end(); ++it) {
		int productId = it.key();
		int qty = it.value();
		for (const auto &p : m_products) if (p.id == productId) { total += p.price * qty; break; }
	}
	return total;
}

void CartPage::addToCart(int productId, int quantity) {
	if (quantity <= 0) return;
	// validate product
	for (auto &p : m_products) {
		if (p.id == productId) {
			if (!p.onShelf || p.stock < quantity) {
				QMessageBox::warning(this, tr("失败"), tr("商品不可用或库存不足"));
				return;
			}
			m_cart[productId] += quantity;
			reload();
			return;
		}
	}
}

void CartPage::onRemove() {
	auto items = m_table->selectedItems();
	if (items.isEmpty()) return;
	int row = items.first()->row();
	int productId = m_table->item(row, 0)->text().toInt();
	m_cart.remove(productId);
	reload();
}

void CartPage::onClear() {
	m_cart.clear();
	reload();
}

void CartPage::onCheckout() {
	if (m_cart.isEmpty()) return;
	// verify stock
	for (auto it = m_cart.begin(); it != m_cart.end(); ++it) {
		int productId = it.key();
		int qty = it.value();
		bool found = false;
		for (const auto &p : m_products) {
			if (p.id == productId) {
				found = true;
				if (p.stock < qty || !p.onShelf) {
					QMessageBox::warning(this, tr("失败"), tr("库存不足或商品已下架"));
					return;
				}
			}
		}
		if (!found) {
			QMessageBox::warning(this, tr("失败"), tr("找不到商品"));
			return;
		}
	}

	double total = cartTotal();
	// create order
	Order order;
	int maxId = 0;
	for (const auto &o : m_orders) maxId = std::max(maxId, o.id);
	order.id = maxId + 1;
	for (auto it = m_cart.begin(); it != m_cart.end(); ++it) {
		int productId = it.key();
		int qty = it.value();
		for (auto &p : m_products) {
			if (p.id == productId) {
				OrderItem item;
				item.productId = p.id;
				item.productName = p.name;
				item.unitPrice = p.price;
				item.quantity = qty;
				order.items.push_back(item);
				p.stock -= qty;
				break;
			}
		}
	}
	order.totalAmount = total;
	m_orders.push_back(order);
	m_cart.clear();
	reload();
	emit productsChanged();
	emit checkoutCompleted();
	QMessageBox::information(this, tr("成功"), tr("下单成功，总额: %1").arg(QString::number(total, 'f', 2)));
}