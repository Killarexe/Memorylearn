#include "bluetooth_manager.hpp"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

void bluetooth_start(MemoryLearn* memory_learn) {
  BLEDevice::init(DEVICE_NAME);
  memory_learn->ble_server = BLEDevice::createServer();
  memory_learn->ble_service = memory_learn->ble_server->createService(SERVICE_UUID);
  memory_learn->best_score = memory_learn->ble_service->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  memory_learn->best_score->setValue("Hello, world! From MemoryLearn via Bluetooth!");
  memory_learn->ble_service->start();
  memory_learn->ble_advertising = BLEDevice::getAdvertising();
  memory_learn->ble_advertising->addServiceUUID(SERVICE_UUID);
  memory_learn->ble_advertising->setScanResponse(true);
  memory_learn->ble_advertising->setMinPreferred(0x06);
  memory_learn->ble_advertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}
