#pragma once
#include <plugify_configs_export.h>
#include <string_view>
#include <span>
#include <memory>

namespace pcf
{
	class Config;


	PLUGIFY_CONFIGS_API std::unique_ptr<pcf::Config> ReadConfig(std::string_view path);
	PLUGIFY_CONFIGS_API std::unique_ptr<pcf::Config> ReadConfigs(std::span<const std::string_view> paths);
	PLUGIFY_CONFIGS_API std::unique_ptr<pcf::Config> MakeConfig();
	PLUGIFY_CONFIGS_API void SetError(std::string_view error);
	PLUGIFY_CONFIGS_API std::string_view GetError();
}

