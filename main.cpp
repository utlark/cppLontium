#include <iostream>

#include "GpioDevice.h"
#include "LontiumConfig.h"
#include "LontiumDevice.h"

#include "cxxopts.hpp"

struct AppConfig {
    int resetPin = 154;
    std::string edid = "1920x1080";
    std::vector<std::string> devices;

    LontiumConfig lontiumConfig;
};

const std::map<std::string, std::vector<uint8_t>> edidMap = {
        {"1920x1080", {
                              0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x05, 0xD7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0xFF, 0x23, 0x01, 0x03, 0x80, 0x46, 0x14, 0x78, 0x07, 0xEE, 0x95, 0xA3, 0x54, 0x4C, 0x99, 0x26,
                              0x0F, 0x50, 0x54, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                              0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C,
                              0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x17, 0xF0, 0x0F,
                              0xFF, 0x0F, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x48,
                              0x44, 0x54, 0x56, 0x20, 0x31, 0x30, 0x38, 0x30, 0x70, 0x0A, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xD1,
                              0x02, 0x03, 0x16, 0x00, 0x41, 0x90, 0x67, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x67, 0xD8,
                              0x5D, 0xC4, 0x01, 0x1E, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81
                      }},
        {"1024x768",  {
                              0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x05, 0xD7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0xFF, 0x23, 0x01, 0x03, 0x80, 0x18, 0x12, 0x78, 0x07, 0xEE, 0x95, 0xA3, 0x54, 0x4C, 0x99, 0x26,
                              0x0F, 0x50, 0x54, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                              0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x64, 0x19, 0x00, 0x40, 0x41, 0x00, 0x26, 0x30, 0x18, 0x88,
                              0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x17, 0xF0, 0x0F,
                              0xFF, 0x07, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x31,
                              0x30, 0x32, 0x34, 0x78, 0x37, 0x36, 0x38, 0x20, 0x58, 0x47, 0x41, 0x0A, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x83,
                              0x02, 0x03, 0x14, 0x00, 0x67, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x67, 0xD8, 0x5D, 0xC4,
                              0x01, 0x0E, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x74
                      }}
};

AppConfig ParseArgs(int argc, char **argv) {
    cxxopts::Options options("lontium-config", "Lontium configurator utility\n");

    options.add_options()
            ("reset-pin", "GPIO pin for reset", cxxopts::value<int>()->default_value("154"))
            ("edid", "EDID name (1920x1080/1024x768)", cxxopts::value<std::string>()->default_value("1920x1080"))
            ("dev", "I2C device path, can repeat", cxxopts::value<std::vector<std::string>>())
            ("mapping", "LVDS mapping (vesa/jeida)", cxxopts::value<std::string>()->default_value("vesa"))
            ("color-depth", "Color depth (6/8)", cxxopts::value<int>()->default_value("8"))
            ("output", "LVDS output enable (on/off)", cxxopts::value<std::string>()->default_value("on"))
            ("cp-mode", "Conversion mode (sdtv/sdpc/hdtv/hdpc)", cxxopts::value<std::string>()->default_value("hdtv"))
            ("channel-width", "LVDS channel width (single/dual)", cxxopts::value<std::string>()->default_value("dual"))
            ("sync", "Sync mode (de/sync)", cxxopts::value<std::string>()->default_value("sync"))
            ("cd-swap", "Channel-port swap (on/off)", cxxopts::value<std::string>()->default_value("off"))
            ("rb-swap", "Red-blue swap (on/off)", cxxopts::value<std::string>()->default_value("off"))
            ("help", "Print help");

    auto result = options.parse(argc, argv);
    if (result.count("help")) {
        std::cout << options.help() << "\n";
        std::exit(0);
    }

    AppConfig appConfig;
    appConfig.resetPin = result["reset-pin"].as<int>();
    appConfig.edid = result["edid"].as<std::string>();
    if (result.count("dev"))
        appConfig.devices = result["dev"].as<std::vector<std::string>>();

    auto strToMap = [](const std::string &s) {
        if (s == "vesa") return LVDS_Map::VESA;
        if (s == "jeida") return LVDS_Map::JEIDA;
        throw std::invalid_argument("Invalid mapping: " + s + ". Allowed: vesa, jeida");
    };

    auto strToOutput = [](const std::string &s) {
        if (s == "on") return LVDS_Output::Enabled;
        if (s == "off") return LVDS_Output::Disabled;
        throw std::invalid_argument("Invalid output: " + s + ". Allowed: on, off");
    };

    auto strToCP = [](const std::string &s) {
        if (s == "sdtv") return CP_Convert_Mode::SDTV;
        if (s == "sdpc") return CP_Convert_Mode::SDPC;
        if (s == "hdtv") return CP_Convert_Mode::HDTV;
        if (s == "hdpc") return CP_Convert_Mode::HDPC;
        throw std::invalid_argument("Invalid cp-mode: " + s + ". Allowed: sdtv, sdpc, hdtv, hdpc");
    };

    auto strToChannel = [](const std::string &s) {
        if (s == "single") return ChannelWidth::Single;
        if (s == "dual") return ChannelWidth::Dual;
        throw std::invalid_argument("Invalid channel-width: " + s + ". Allowed: single, dual");
    };

    auto strToSync = [](const std::string &s) {
        if (s == "de") return SyncMode::DE_Mode;
        if (s == "sync") return SyncMode::Sync_Mode;
        throw std::invalid_argument("Invalid sync: " + s + ". Allowed: de, sync");
    };

    auto strToCD = [](const std::string &s) {
        if (s == "on") return C_D_PortSwap::Enabled;
        if (s == "off") return C_D_PortSwap::Disabled;
        throw std::invalid_argument("Invalid cd-swap: " + s + ". Allowed: on, off");
    };

    auto strToRB = [](const std::string &s) {
        if (s == "on") return R_B_ColorSwap::Enabled;
        if (s == "off") return R_B_ColorSwap::Disabled;
        throw std::invalid_argument("Invalid rb-swap: " + s + ". Allowed: on, off");
    };

    appConfig.lontiumConfig.lvdsMap = strToMap(result["mapping"].as<std::string>());
    appConfig.lontiumConfig.lvdsOutput = strToOutput(result["output"].as<std::string>());
    appConfig.lontiumConfig.cpConvertMode = strToCP(result["cp-mode"].as<std::string>());
    appConfig.lontiumConfig.channelWidth = strToChannel(result["channel-width"].as<std::string>());
    appConfig.lontiumConfig.syncMode = strToSync(result["sync"].as<std::string>());
    appConfig.lontiumConfig.cDPortSwap = strToCD(result["cd-swap"].as<std::string>());
    appConfig.lontiumConfig.rBColorSwap = strToRB(result["rb-swap"].as<std::string>());

    int depth = result["color-depth"].as<int>();
    if (depth == 6)
        appConfig.lontiumConfig.colorDepth = ColorDepth::Bit_6;
    else if (depth == 8)
        appConfig.lontiumConfig.colorDepth = ColorDepth::Bit_8;
    else
        throw std::invalid_argument("Invalid color-depth: " + std::to_string(depth) + ". Allowed: 6, 8");

    return appConfig;
}

void InitLontium(LontiumDevice &dev, const std::vector<uint8_t> &edid) {
    if (!dev.CheckChipId())
        throw std::runtime_error("Chip ID mismatch");

    dev.SetHPD(LontiumDevice::Value::OFF);
    dev.SetEDID(edid);
    dev.SetHPD(LontiumDevice::Value::ON);

    dev.RxReset();
    dev.LvdsInit();
    dev.LockDetect();
    dev.LvdsSoftReset();
    dev.ColorConfig();
}

int main(int argc, char **argv) {
    try {
        AppConfig appConfig = ParseArgs(argc, argv);

        if (edidMap.find(appConfig.edid) == edidMap.end())
            throw std::runtime_error("Unknown EDID: " + appConfig.edid);
        auto &edid = edidMap.at(appConfig.edid);

        if (appConfig.devices.empty())
            throw std::runtime_error("No devices specified (use --dev)");

        GpioDevice resetGpio(appConfig.resetPin);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        resetGpio.SetValue(GpioDevice::Value::HIGH);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        for (auto &devPath: appConfig.devices) {
            std::cout << "Init device: " << devPath << " with EDID=" << appConfig.edid << "\n";
            LontiumDevice dev(devPath, appConfig.lontiumConfig);
            InitLontium(dev, edid);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
