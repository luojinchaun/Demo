set(SRC_FILES
	main.cpp
	MainWindow.cpp

	model/Product.h
	model/Order.h
	model/User.h

	utils/DataStore.h
	utils/DataStore.cpp

	pages/ProductPage.h
	pages/ProductPage.cpp
	pages/CartPage.h
	pages/CartPage.cpp
	pages/OrdersPage.h
	pages/OrdersPage.cpp
	pages/AccountPage.h
	pages/AccountPage.cpp

	dialogs/ProductDialog.h
	dialogs/ProductDialog.cpp
)

add_executable(ShopMallQt ${SRC_FILES})

target_include_directories(ShopMallQt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ShopMallQt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)