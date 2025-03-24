#pragma once
#include <memory>
#include <string_view>

namespace pcf {
	class Config;

	std::unique_ptr<Config> ReadJsonConfig(std::string_view path);
	std::unique_ptr<Config> ReadJsoncConfig(std::string_view path);
}// namespace pcf
