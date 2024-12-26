#pragma once
#include <plugify_configs_export.h>
#include <string_view>
#include <memory>

namespace pcf
{
	class Config;
}

PLUGIFY_CONFIGS_API std::unique_ptr<pcf::Config> ReadConfig(std::string_view path);
PLUGIFY_CONFIGS_API std::unique_ptr<pcf::Config> MakeConfig();
