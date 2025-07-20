import QtQuick 2.12
import QtQuick.Controls 2.5

ApplicationWindow {
    visible: true
    width: 360
    height: 640
    title: qsTr("NieS Mobile")

    ListModel {
        id: productModel
    }

    Component.onCompleted: restClient.fetchProducts()

    Connections {
        target: restClient
        onProductsReceived: {
            productModel.clear()
            for (var i = 0; i < products.length; ++i)
                productModel.append(products[i])
        }
        onSaleFinished: function(success, err) {
            if (success)
                console.log("Sale recorded")
            else
                console.log("Sale error: " + err)
        }
    }

    ListView {
        anchors.fill: parent
        model: productModel
        delegate: Item {
            width: parent.width
            height: 50
            Row {
                anchors.verticalCenter: parent.verticalCenter
                spacing: 10
                Text { text: name; width: 150 }
                Text { text: "$" + price; width: 60 }
                Button {
                    text: qsTr("Sell")
                    onClicked: restClient.postSale(id, 1)
                }
            }
        }
    }
}
