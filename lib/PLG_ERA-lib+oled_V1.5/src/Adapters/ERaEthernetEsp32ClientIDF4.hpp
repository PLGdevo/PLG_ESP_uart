#ifndef INC_ERA_ETHERNET_ESP32_CLIENT_IDF_4_HPP_
#define INC_ERA_ETHERNET_ESP32_CLIENT_IDF_4_HPP_

#if !defined(ERA_NETWORK_TYPE)
    #define ERA_NETWORK_TYPE          "Ethernet"
#endif

#if !defined(ERA_AUTH_TOKEN)
    #error "Please specify your ERA_AUTH_TOKEN"
#endif

#include <ETH.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ERa/ERaProtocol.hpp>
#include <ERa/ERaApiEsp32.hpp>
#include <MQTT/ERaMqtt.hpp>
#include <Modbus/ERaModbusEsp32.hpp>
#include <Zigbee/ERaZigbeeEsp32.hpp>
#include <Storage/ERaFlashEsp32.hpp>

template <class Transport>
class ERaEthernet
    : public ERaProto<Transport, ERaFlash>
{
    const char* TAG = "Ethernet";
    friend class ERaProto<Transport, ERaFlash>;
    typedef ERaProto<Transport, ERaFlash> Base;

public:
    ERaEthernet(Transport& _transp, ERaFlash& _flash)
        : Base(_transp, _flash)
        , authToken(nullptr)
    {}
    ~ERaEthernet()
    {}

    bool connectNetwork(uint8_t phyAddr = ETH_PHY_ADDR,
                        int power = ETH_PHY_POWER,
                        int mdc = ETH_PHY_MDC,
                        int mdio = ETH_PHY_MDIO,
                        eth_phy_type_t type = ETH_PHY_LAN8720,
                        eth_clock_mode_t clkMode = ETH_CLOCK_GPIO0_IN) {
        ERaWatchdogFeed();

        ERA_LOG(TAG, ERA_PSTR("Connecting network..."));
        if (!ETH.begin(phyAddr, power,
                       mdc, mdio, type, clkMode)){
            ERA_LOG_ERROR(TAG, ERA_PSTR("Connect failed"));
            return false;
        }
        this->getTransp().setSSID(ERA_NETWORK_TYPE);

        ERaWatchdogFeed();

        ERaDelay(1000);
        IPAddress localIP = ETH.localIP();
        ERA_FORCE_UNUSED(localIP);
        ERA_LOG(TAG, ERA_PSTR("Connected to network"));
        ERA_LOG(TAG, ERA_PSTR("IP: %s"), localIP.toString().c_str());
        return true;
    }

    void config(const char* auth,
                const char* host = ERA_MQTT_HOST,
                uint16_t port = ERA_MQTT_PORT,
                const char* username = ERA_MQTT_USERNAME,
                const char* password = ERA_MQTT_PASSWORD) {
        Base::begin(auth);
        this->getTransp().config(host, port, username, password);
    }

    void config(IPAddress localIP,
                IPAddress gateway,
                IPAddress subnet,
                IPAddress dns1 = (uint32_t)0x00000000,
                IPAddress dns2 = (uint32_t)0x00000000) {
        ETH.config(localIP, gateway, subnet, dns1, dns2);
    }

    void begin(const char* auth,
                uint8_t phyAddr,
                int power,
                int mdc,
                int mdio,
                eth_phy_type_t type,
                eth_clock_mode_t clkMode,
                const char* host,
                uint16_t port,
                const char* username,
                const char* password) {
        Base::init();
        this->config(auth, host, port, username, password);
        this->connectNetwork(phyAddr, power, mdc, mdio, type, clkMode);
    }

    void begin(const char* auth,
                uint8_t phyAddr = ETH_PHY_ADDR,
                int power = ETH_PHY_POWER,
                int mdc = ETH_PHY_MDC,
                int mdio = ETH_PHY_MDIO,
                eth_phy_type_t type = ETH_PHY_LAN8720,
                eth_clock_mode_t clkMode = ETH_CLOCK_GPIO0_IN,
                const char* host = ERA_MQTT_HOST,
                uint16_t port = ERA_MQTT_PORT) {
        this->begin(auth, phyAddr, power,
                    mdc, mdio, type, clkMode,
                    host, port, auth, auth);
    }

    void begin(uint8_t phyAddr = ETH_PHY_ADDR,
                int power = ETH_PHY_POWER,
                int mdc = ETH_PHY_MDC,
                int mdio = ETH_PHY_MDIO,
                eth_phy_type_t type = ETH_PHY_LAN8720,
                eth_clock_mode_t clkMode = ETH_CLOCK_GPIO0_IN) {
        this->begin(ERA_AUTHENTICATION_TOKEN,
                    phyAddr, power,
                    mdc, mdio,
                    type, clkMode,
                    ERA_MQTT_HOST, ERA_MQTT_PORT,
                    ERA_MQTT_USERNAME, ERA_MQTT_PASSWORD);
    }

    void run() {
        switch (ERaState::get()) {
            case StateT::STATE_CONNECTING_CLOUD:
                if (Base::connect()) {
                    ERaOptConnected(this);
                    ERaState::set(StateT::STATE_CONNECTED);
                }
                else {
                    ERaState::set(StateT::STATE_CONNECTING_NETWORK);
                }
                break;
            case StateT::STATE_CONNECTED:
                ERaState::set(StateT::STATE_RUNNING);
                break;
            case StateT::STATE_RUNNING:
                Base::run();
                break;
            case StateT::STATE_CONNECTING_NEW_NETWORK:
                Base::connectNewNetworkResult();
                break;
            case StateT::STATE_REQUEST_LIST_WIFI:
                Base::responseListWiFi();
                break;
            default:
                ERaState::set(StateT::STATE_CONNECTING_CLOUD);
                break;
        }
    }

protected:
private:
    const char* authToken;
};

template <class Proto, class Flash>
inline
void ERaApi<Proto, Flash>::addInfo(cJSON* root) {
    cJSON_AddNumberToObject(root, INFO_PLUG_AND_PLAY, 0);
    cJSON_AddStringToObject(root, INFO_NETWORK_PROTOCOL, ERA_NETWORK_TYPE);
    cJSON_AddStringToObject(root, INFO_SSID, ERA_NETWORK_TYPE);
    cJSON_AddStringToObject(root, INFO_BSSID, ERA_NETWORK_TYPE);
    cJSON_AddNumberToObject(root, INFO_RSSI, ETH.linkSpeed());
    cJSON_AddNumberToObject(root, INFO_SIGNAL_STRENGTH, 100);
    cJSON_AddStringToObject(root, INFO_MAC, ETH.macAddress().c_str());
    cJSON_AddStringToObject(root, INFO_LOCAL_IP, ETH.localIP().toString().c_str());
    cJSON_AddNumberToObject(root, INFO_SSL, ERaInfoSSL());

    /* Override info */
    ERaInfo(root);
}

template <class Proto, class Flash>
inline
void ERaApi<Proto, Flash>::addSelfInfo(cJSON* root) {
    cJSON_AddNumberToObject(root, SELF_CHIP_TEMPERATURE, static_cast<uint16_t>(temperatureRead() * 100.0f));
    cJSON_AddNumberToObject(root, SELF_SIGNAL_STRENGTH, 100);

    /* Override self info */
    ERaSelfInfo(root);
}

#if defined(ERA_MODBUS)
    template <class Proto, class Flash>
    inline
    void ERaApi<Proto, Flash>::addModbusInfo(cJSON* root) {
        cJSON_AddNumberToObject(root, INFO_MB_CHIP_TEMPERATURE, static_cast<uint16_t>(temperatureRead() * 100.0f));
        cJSON_AddNumberToObject(root, INFO_MB_RSSI, ETH.linkSpeed());
        cJSON_AddNumberToObject(root, INFO_MB_SIGNAL_STRENGTH, 100);
        cJSON_AddStringToObject(root, INFO_MB_WIFI_USING, ERA_NETWORK_TYPE);

        /* Override modbus info */
        ERaModbusInfo(root);
    }
#endif

#endif /* INC_ERA_ETHERNET_ESP32_CLIENT_IDF_4_HPP_ */
