#pragma once

#include <memory>
#include <string_view>

#include <plg/config.hpp>

namespace pcf {
	class Config;

	std::unique_ptr<Config> ReadJsonConfig(std::string_view path);
	std::unique_ptr<Config> ReadJsoncConfig(std::string_view path);
#if PLUGIFY_HAS_CXX23
	std::unique_ptr<Config> ReadYamlConfig(std::string_view path);
	std::unique_ptr<Config> ReadTomlConfig(std::string_view path);
#endif
}// namespace pcf
