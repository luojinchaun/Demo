#pragma once

#include <QWidget>
#include <QVector>
#include "model/Product.h"

class QTableWidget;
class QPushButton;
class ProductDialog;

class ProductPage : public QWidget {
	Q_OBJECT
public:
	explicit ProductPage(QVector<Product> &productsRef, QWidget *parent = nullptr);

signals:
	void requestAddToCart(int productId, int quantity);

public slots:
	void reload();

private slots:
	void onAdd();
	void onEdit();
	void onDelete();
	void onToggleShelf();
	void onAddToCart();

private:
	void setupUi();
	int selectedProductRow() const;
	Product *productByRow(int row);

private:
	QVector<Product> &m_products;
	QTableWidget *m_table{nullptr};
	QPushButton *m_btnAdd{nullptr};
	QPushButton *m_btnEdit{nullptr};
	QPushButton *m_btnDelete{nullptr};
	QPushButton *m_btnToggleShelf{nullptr};
	QPushButton *m_btnAddToCart{nullptr};
};