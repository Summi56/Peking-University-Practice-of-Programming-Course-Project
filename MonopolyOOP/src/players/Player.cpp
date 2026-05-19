#include "players/Player.h"
#include "tiles/PropertyTile.h"
#include <QMap>

Player::Player(const QString& name, QObject* parent)
    : QObject(parent), m_name(name), m_funds(0),
      m_position(0), m_bankrupt(false), m_skipped(false) {
}

void Player::addFunds(int amount) {
    m_funds += amount;
    emit fundsChanged(m_funds);
}

bool Player::deductFunds(int amount) {
    if (m_funds >= amount) {
        m_funds -= amount;
        emit fundsChanged(m_funds);
        return true;
    }
    return false; // 资金不足
}

void Player::addProperty(PropertyTile* property) {
    if (!m_properties.contains(property)) {
        m_properties.append(property);
        emit propertyAcquired(property);
    }
}

void Player::removeProperty(PropertyTile* property) {
    m_properties.removeAll(property);
    emit propertyLost(property);
}

void Player::clearProperties() {
    m_properties.clear();
}

int Player::getTotalPropertyValue() const {
    int total = 0;
    for (PropertyTile* prop : m_properties) {
        total += prop->getTotalValue();
    }
    return total;
}

void Player::addItem(int itemId) {
    m_items[itemId] = m_items.value(itemId, 0) + 1;
    emit itemAdded(itemId);
}

void Player::removeItem(int itemId) {
    if (m_items.contains(itemId) && m_items[itemId] > 0) {
        m_items[itemId]--;
        if (m_items[itemId] <= 0) {
            m_items.remove(itemId);
        }
        emit itemUsed(itemId);
    }
}

bool Player::hasItem(int itemId) const {
    return m_items.contains(itemId) && m_items[itemId] > 0;
}

int Player::getItemCount(int itemId) const {
    return m_items.value(itemId, 0);
}
