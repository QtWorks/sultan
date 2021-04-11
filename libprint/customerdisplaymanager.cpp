/*
 * Copyright 2021, Apin <apin.klas@gmail.com>
 *
 * This file is part of Sultan.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "customerdisplaymanager.h"
#include <QDebug>
#include <QSerialPortInfo>

using namespace LibPrint;

static CustomerDisplayManager *sInstance = nullptr;

CustomerDisplayManager::CustomerDisplayManager(QObject *parent) : QObject(parent) {}

void CustomerDisplayManager::createInstance(QObject *parent) { sInstance = new CustomerDisplayManager(parent); }

CustomerDisplayManager *CustomerDisplayManager::instance() { return sInstance; }

CustomerDisplayManager::~CustomerDisplayManager() { mPort.close(); }

bool CustomerDisplayManager::openPort(int vendorId, int productId) {
    auto all = QSerialPortInfo::availablePorts();
    foreach (const auto &port, all) {
        if (port.vendorIdentifier() == vendorId && port.productIdentifier() == productId) {
            mPort.setPort(port);
            return mPort.open(QSerialPort::WriteOnly);
        }
    }
    return false;
}

void CustomerDisplayManager::close() { mPort.close(); }

bool CustomerDisplayManager::write(const QByteArray &data) {
    if (data.isEmpty())
        return false;
    if (!mPort.isOpen())
        return false;
    return mPort.write(data) > 0;
}

void CustomerDisplayManager::wait() { mPort.waitForBytesWritten(); }
