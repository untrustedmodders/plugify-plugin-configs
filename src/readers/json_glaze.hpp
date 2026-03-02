#pragma once

#include <memory>
#include <string_view>

namespace pcf {
	class Config;

	std::unique_ptr<Config> ReadJsonConfig(std::string_view path);
	std::unique_ptr<Config> ReadJsoncConfig(std::string_view path);
#if __has_include(<glaze/yaml.hpp>)
	std::unique_ptr<Config> ReadYamlConfig(std::string_view path);
	std::unique_ptr<Config> ReadTomlConfig(std::string_view path);
#endif
}// namespace pcf
